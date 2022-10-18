/*
 * Copyright (c) 2022, the SerenityOS developers.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <LibCore/ArgsParser.h>
#include <LibCore/File.h>
#include <LibCore/System.h>
#include <stdio.h>
#include <unistd.h>

// https://uefi.org/sites/default/files/resources/ACPI_Spec_6_4_Jan22.pdf#page=193
struct [[gnu::packed]] SDTHeader {
    char sig[4];
    u32 length;
    u8 revision;
    u8 checksum;
    char oem_id[6];
    char oem_table_id[8];
    u32 oem_revision;
    u32 creator_id;
    u32 creator_revision;
};

constexpr u8 AliasOp = 0x06;
constexpr u8 NameOp  = 0x08;
constexpr u8 ScopeOp = 0x10;
constexpr u8 MethodOp = 0x14;
//constexpr u8 ScopeOp = 0x10;
//constexpr u8 ScopeOp = 0x10;
//constexpr u8 ScopeOp = 0x10;

constexpr u8 ExtOpPrefix = 0x5b;

bool isLeadNameChar(u8 data);
bool NamePath(const Vector<u8>& data,u32& pos);
void NameString(const Vector<u8>& data,u32& pos);

// https://uefi.org/sites/default/files/resources/ACPI_Spec_6_4_Jan22.pdf#page=1020
static void pkg_length(AK::Detail::ByteBuffer<32> const& data, u32& next_in_block, u32& next_block)
{
    u32 pos = next_block;

    outln("");
    out("data: ");
    if (pos > 3) {
        for (auto i = -3; i < 0; i++)
            out("{:#02x} ", data[pos + i]);
    } else
        out("               ");
    out("  ");
    for (auto i = 0; i < 5; i++)
        out("{:#02x} ", data[pos + i]);
    outln("");

    u32 length = 0;
    u32 delta = 0;
    int const size = data[pos + 1] >> 6;
    if (size == 0) {
        length = data[pos + 1] & 0x3f;
        delta = 1;
    } else if (size == 1) {
        length = data[pos + 1] & 0x0f;
        length = length | (data[pos + 2] << 4);
        delta = 2;
    } else if (size == 2) {
        length = data[pos + 1] & 0x0f;
        length = length | (data[pos + 2] << 4);
        length = length | (data[pos + 3] << 12);
        delta = 3;
    } else {
        length = data[pos + 1] & 0x0f;
        length = length | (data[pos + 2] << 4);
        length = length | (data[pos + 3] << 12);
        length = length | (data[pos + 4] << 20);
        delta = 4;
    }
    length -= (delta - 1);

    outln("pos: {} {} {}", delta, pos + delta + length, data.size());
    next_in_block = next_block + 1 + delta;
    next_block = next_block + delta + length;
}

bool isLeadNameChar(u8 data)
{
  return ((data >= 0x41 && data <= 0x5a) || (data==0x5f));
}

bool NamePath(const Vector<u8>& data,u32& pos)
{
  const u8 DualNamePrefix=0x2e;
  const u8 MultiNamePrefix=0x2f;
  const u8 NullName=0x00;

  if (data[pos]==DualNamePrefix) {
    outln("DualNamePrefix");
    out("  Name 1: ");
    for(auto i=0;i<4;i++) out("{:c}",data.at(pos+1+i));
    outln();
    out("  Name 2: ");
    for(auto i=4;i<8;i++) out("{:c}",data.at(pos+1+i));
    outln();
    pos+=1+8;
  } if (data[pos]==MultiNamePrefix) {
    outln("MultiNamePrefix");
    const auto num_names=data[pos+1];
    for (auto mnp=0; mnp<num_names; mnp++) {
      out("  Name {:03d}: ",mnp);
      for(auto i=0;i<4;i++) out("{:c}",data.at(pos+2+4*mnp+1));
      outln();
    }
    pos+=2+4*num_names+1;
  } if (data[pos]==NullName) {
    outln("NullName");
    pos+=1;
  } if (isLeadNameChar(data[pos])) {
    out("NameSeg: ");
    for(auto i=0;i<4;i++) out("{:c}",data.at(pos+i));
    outln("");
    pos+=4;
  } else {
    return false;
  }
  return true;
}

void NameString(const Vector<u8>& data,u32& pos)
{
  /*
  out("Name: ");
  for(auto i=-2;i<0;i++) out("{:#02x} ",data.at(pos+i));
  out("  ");
  for(auto i=0;i<10;i++) out("{:#02x} ",data.at(pos+i));
  outln("");
  */
  switch (data[pos]) {
  case 0x5c:  // <rootchar namepath>
    pos+=1;
    NamePath(data,pos);
    break;
  case 0x5e:  // <prefixpath namepath>
    pos+=1;
    NamePath(data,pos);
    break;
  default: 
    if (!NamePath(data,pos))
      outln("strange character @ {}",pos);

    break;
  }
}

ErrorOr<int> serenity_main(Main::Arguments arguments)
{
    TRY(Core::System::pledge("stdio rpath"));

    StringView dsdt_file_name;

    Core::ArgsParser args_parser;
    args_parser.add_positional_argument(dsdt_file_name, "Name of DSDT table", "DSDT table", Core::ArgsParser::Required::No);
    args_parser.parse(arguments);

    auto dsdt_file = TRY(Core::File::open_file_or_standard_stream(dsdt_file_name, Core::File::OpenMode::Read));

    TRY(Core::System::pledge("stdio"));

    Array<u8, sizeof(SDTHeader)> header_buffer;
    Bytes header_span = header_buffer.span();
    TRY(dsdt_file->read(header_span));
    if (header_span.size() != sizeof(SDTHeader)) {
        warnln("Failed to read SDTHeader from {}", dsdt_file_name);
        return EXIT_FAILURE;
    }

    SDTHeader const* const header = reinterpret_cast<SDTHeader*>(header_buffer.data());
    outln("data size: {}\n",header->length);

    auto data = TRY(dsdt_file->read_until_eof());

    if ((header->length - sizeof(SDTHeader)) != data.size()) {
        warnln("Bad data size, should be {} but is {}", header->length - sizeof(SDTHeader), data.size());
        return EXIT_FAILURE;
    }

    u32 next_in_block = 0;
    u32 next_block = 0;

    while (next_block < data.size()) {
      switch (data[next_block]) {
      case AliasOp: // 0x06
	outln("AliasOp");
	NameString(data,next_in_block);
	NameString(data,next_in_block);
	break;
      case NameOp: // 0x08
	outln("NameOp");
	NameString(data,next_in_block);
	break;
      case MethodOp: // 0x14
	outln("MethodOp");
	pkg_length(data, next_in_block, next_block);
	NameString(data,next_in_block);
	break;
      case ScopeOp: // 0x10
	outln("ScopeOp");
	pkg_length(data, next_in_block, next_block);
	NameString(data, next_in_block);
	break;
      case ExtOpPrefix: // 0x5b
	outln("ExtOpPrefix");
	switch (data[next_block+1]) {
	default:
	  outln("Unhandled ExtOp: {:#02x}",data[next_block+1]);
	  break;
	}
	break;
      default:
	outln("Unknown Opcode: {:#02x}",data[next_block]);

	out("around: ");
	for(auto i=-2;i<0;i++) out("{:#02x} ",data.at(next_block+i));
	out("  ");
	for(auto i=0;i<10;i++) out("{:#02x} ",data.at(next_block+i));
	outln("");

	return EXIT_FAILURE;
      }
    }
    return EXIT_SUCCESS;
}
