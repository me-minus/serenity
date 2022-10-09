/*
 * Copyright (c) 2021, Hunter Salyer <thefalsehonesty@gmail.com>
 * Copyright (c) 2022, Gregory Bertilson <zaggy1024@gmail.com>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include "Enums.h"
#include "MotionVector.h"
#include "Symbols.h"

namespace Video::VP9 {

static constexpr InterpolationFilter literal_to_type[4] = { EightTapSmooth, EightTap, EightTapSharp, Bilinear };
static constexpr TXSize tx_mode_to_biggest_tx_size[TX_MODES] = { TX_4x4, TX_8x8, TX_16x16, TX_32x32, TX_32x32 };
static constexpr u8 segmentation_feature_bits[SEG_LVL_MAX] = { 8, 6, 2, 0 };
static constexpr bool segmentation_feature_signed[SEG_LVL_MAX] = { true, true, false, false };
static constexpr u8 inv_map_table[MAX_PROB] = {
    7, 20, 33, 46, 59, 72, 85, 98, 111, 124, 137, 150, 163, 176, 189, 202, 215, 228, 241, 254,
    1, 2, 3, 4, 5, 6, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 21, 22, 23, 24, 25, 26, 27,
    28, 29, 30, 31, 32, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 47, 48, 49, 50, 51, 52,
    53, 54, 55, 56, 57, 58, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 73, 74, 75, 76, 77,
    78, 79, 80, 81, 82, 83, 84, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 99, 100, 101, 102,
    103, 104, 105, 106, 107, 108, 109, 110, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122,
    123, 125, 126, 127, 128, 129, 130, 131, 132, 133, 134, 135, 136, 138, 139, 140, 141, 142, 143,
    144, 145, 146, 147, 148, 149, 151, 152, 153, 154, 155, 156, 157, 158, 159, 160, 161, 162, 164,
    165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 175, 177, 178, 179, 180, 181, 182, 183, 184,
    185, 186, 187, 188, 190, 191, 192, 193, 194, 195, 196, 197, 198, 199, 200, 201, 203, 204, 205,
    206, 207, 208, 209, 210, 211, 212, 213, 214, 216, 217, 218, 219, 220, 221, 222, 223, 224, 225,
    226, 227, 229, 230, 231, 232, 233, 234, 235, 236, 237, 238, 239, 240, 242, 243, 244, 245, 246,
    247, 248, 249, 250, 251, 252, 253, 253
};
static constexpr BlockSubsize subsize_lookup[PARTITION_TYPES][BLOCK_SIZES] = {
    {
        // PARTITION_NONE
        Block_4x4,
        Block_4x8,
        Block_8x4,
        Block_8x8,
        Block_8x16,
        Block_16x8,
        Block_16x16,
        Block_16x32,
        Block_32x16,
        Block_32x32,
        Block_32x64,
        Block_64x32,
        Block_64x64,
    },
    {
        // PARTITION_HORZ
        Block_Invalid,
        Block_Invalid,
        Block_Invalid,
        Block_8x4,
        Block_Invalid,
        Block_Invalid,
        Block_16x8,
        Block_Invalid,
        Block_Invalid,
        Block_32x16,
        Block_Invalid,
        Block_Invalid,
        Block_64x32,
    },
    {
        // PARTITION_VERT
        Block_Invalid,
        Block_Invalid,
        Block_Invalid,
        Block_4x8,
        Block_Invalid,
        Block_Invalid,
        Block_8x16,
        Block_Invalid,
        Block_Invalid,
        Block_16x32,
        Block_Invalid,
        Block_Invalid,
        Block_32x64,
    },
    {
        // PARTITION_SPLIT
        Block_Invalid,
        Block_Invalid,
        Block_Invalid,
        Block_4x4,
        Block_Invalid,
        Block_Invalid,
        Block_8x8,
        Block_Invalid,
        Block_Invalid,
        Block_16x16,
        Block_Invalid,
        Block_Invalid,
        Block_32x32,
    }
};

static constexpr int partition_tree[6] = {
    -PartitionNone, 2,
    -PartitionHorizontal, 4,
    -PartitionVertical, -PartitionSplit
};
static constexpr int cols_partition_tree[2] = { -PartitionHorizontal, -PartitionSplit };
static constexpr int rows_partition_tree[2] = { -PartitionVertical, -PartitionSplit };
static constexpr int intra_mode_tree[18] = {
    -DcPred, 2,
    -TmPred, 4,
    -VPred, 6,
    8, 12,
    -HPred, 10,
    -D135Pred, -D117Pred,
    -D45Pred, 14,
    -D63Pred, 16,
    -D153Pred, -D207Pred
};
static constexpr int segment_tree[14] = {
    2, 4, 6, 8, 10, 12,
    0, -1, -2, -3, -4, -5, -6, -7
};
static constexpr int binary_tree[2] = { 0, -1 };
static constexpr int tx_size_32_tree[6] = {
    -TX_4x4, 2,
    -TX_8x8, 4,
    -TX_16x16, -TX_32x32
};
static constexpr int tx_size_16_tree[4] = {
    -TX_4x4, 2,
    -TX_8x8, -TX_16x16
};
static constexpr int tx_size_8_tree[2] = { -TX_4x4, -TX_8x8 };
static constexpr int inter_mode_tree[6] = {
    -(ZeroMv - NearestMv), 2,
    -(NearestMv - NearestMv), 4,
    -(NearMv - NearestMv), -(NewMv - NearestMv)
};
static constexpr int interp_filter_tree[4] = {
    -EightTap, 2,
    -EightTapSmooth, -EightTapSharp
};
static constexpr int mv_joint_tree[6] = {
    -MvJointZero, 2,
    -MvJointHnzvz, 4,
    -MvJointHzvnz, -MvJointHnzvnz
};
static constexpr int mv_class_tree[20] = {
    -MvClass0, 2,
    -MvClass1, 4,
    6, 8,
    -MvClass2, -MvClass3,
    10, 12,
    -MvClass4, -MvClass5,
    -MvClass6, 14,
    16, 18,
    -MvClass7, -MvClass8,
    -MvClass9, -MvClass10
};
static constexpr int mv_fr_tree[6] = {
    -0, 2,
    -1, 4,
    -2, -3
};
static constexpr int token_tree[20] = {
    -ZeroToken, 2,
    -OneToken, 4,
    6, 10,
    -TwoToken, 8,
    -ThreeToken, -FourToken,
    12, 14,
    -DctValCat1, -DctValCat2,
    16, 18,
    -DctValCat3, -DctValCat4,
    -DctValCat5, -DctValCat6
};

static constexpr u8 b_width_log2_lookup[BLOCK_SIZES] = { 0, 0, 1, 1, 1, 2, 2, 2, 3, 3, 3, 4, 4 };
static constexpr u8 b_height_log2_lookup[BLOCK_SIZES] = { 0, 1, 0, 1, 2, 1, 2, 3, 2, 3, 4, 3, 4 };
static constexpr u8 num_4x4_blocks_wide_lookup[BLOCK_SIZES] = { 1, 1, 2, 2, 2, 4, 4, 4, 8, 8, 8, 16, 16 };
static constexpr u8 num_4x4_blocks_high_lookup[BLOCK_SIZES] = { 1, 2, 1, 2, 4, 2, 4, 8, 4, 8, 16, 8, 16 };
static constexpr u8 mi_width_log2_lookup[BLOCK_SIZES] = { 0, 0, 0, 0, 0, 1, 1, 1, 2, 2, 2, 3, 3 };
static constexpr u8 num_8x8_blocks_wide_lookup[BLOCK_SIZES] = { 1, 1, 1, 1, 1, 2, 2, 2, 4, 4, 4, 8, 8 };
static constexpr u8 mi_height_log2_lookup[BLOCK_SIZES] = { 0, 0, 0, 0, 1, 0, 1, 2, 1, 2, 3, 2, 3 };
static constexpr u8 num_8x8_blocks_high_lookup[BLOCK_SIZES] = { 1, 1, 1, 1, 2, 1, 2, 4, 2, 4, 8, 4, 8 };
static constexpr u8 size_group_lookup[BLOCK_SIZES] = { 0, 0, 0, 1, 1, 1, 2, 2, 2, 3, 3, 3, 3 };

static constexpr TXSize max_txsize_lookup[BLOCK_SIZES] = {
    TX_4x4,
    TX_4x4,
    TX_4x4,
    TX_8x8,
    TX_8x8,
    TX_8x8,
    TX_16x16,
    TX_16x16,
    TX_16x16,
    TX_32x32,
    TX_32x32,
    TX_32x32,
    TX_32x32,
};

static constexpr BlockSubsize ss_size_lookup[BLOCK_SIZES][2][2] = {
    { { Block_4x4, Block_Invalid }, { Block_Invalid, Block_Invalid } },
    { { Block_4x8, Block_4x4 }, { Block_Invalid, Block_Invalid } },
    { { Block_8x4, Block_Invalid }, { Block_4x4, Block_Invalid } },
    { { Block_8x8, Block_8x4 }, { Block_4x8, Block_4x4 } },
    { { Block_8x16, Block_8x8 }, { Block_Invalid, Block_4x8 } },
    { { Block_16x8, Block_Invalid }, { Block_8x8, Block_8x4 } },
    { { Block_16x16, Block_16x8 }, { Block_8x16, Block_8x8 } },
    { { Block_16x32, Block_16x16 }, { Block_Invalid, Block_8x16 } },
    { { Block_32x16, Block_Invalid }, { Block_16x16, Block_16x8 } },
    { { Block_32x32, Block_32x16 }, { Block_16x32, Block_16x16 } },
    { { Block_32x64, Block_32x32 }, { Block_Invalid, Block_16x32 } },
    { { Block_64x32, Block_Invalid }, { Block_32x32, Block_32x16 } },
    { { Block_64x64, Block_64x32 }, { Block_32x64, Block_32x32 } },
};

static constexpr int small_token_tree[6] = {
    0, 0, // Unused
    -ZeroToken, 4,
    -OneToken, -TwoToken
};

static constexpr u32 default_scan_4x4[16] = { 0, 4, 1, 5, 8, 2, 12, 9, 3, 6, 13, 10, 7, 14, 11, 15 };

static constexpr u32 col_scan_4x4[16] = { 0, 4, 8, 1, 12, 5, 9, 2, 13, 6, 10, 3, 7, 14, 11, 15 };

static constexpr u32 row_scan_4x4[16] = { 0, 1, 4, 2, 5, 3, 6, 8, 9, 7, 12, 10, 13, 11, 14, 15 };

static constexpr u32 default_scan_8x8[64] = { 0, 8, 1, 16, 9, 2, 17, 24, 10, 3, 18, 25, 32, 11, 4, 26, 33, 19, 40, 12, 34, 27, 5, 41, 20, 48, 13, 35, 42, 28, 21, 6, 49, 56, 36, 43, 29, 7, 14, 50, 57, 44, 22, 37, 15, 51, 58, 30, 45, 23, 52, 59, 38, 31, 60, 53, 46, 39, 61, 54, 47, 62, 55, 63 };

static constexpr u32 col_scan_8x8[64] = { 0, 8, 16, 1, 24, 9, 32, 17, 2, 40, 25, 10, 33, 18, 48, 3, 26, 41, 11, 56, 19, 34, 4, 49, 27, 42, 12, 35, 20, 57, 50, 28, 5, 43, 13, 36, 58, 51, 21, 44, 6, 29, 59, 37, 14, 52, 22, 7, 45, 60, 30, 15, 38, 53, 23, 46, 31, 61, 39, 54, 47, 62, 55, 63 };

static constexpr u32 row_scan_8x8[64] = { 0, 1, 2, 8, 9, 3, 16, 10, 4, 17, 11, 24, 5, 18, 25, 12, 19, 26, 32, 6, 13, 20, 33, 27, 7, 34, 40, 21, 28, 41, 14, 35, 48, 42, 29, 36, 49, 22, 43, 15, 56, 37, 50, 44, 30, 57, 23, 51, 58, 45, 38, 52, 31, 59, 53, 46, 60, 39, 61, 47, 54, 55, 62, 63 };

static constexpr u32 default_scan_16x16[256] = { 0, 16, 1, 32, 17, 2, 48, 33, 18, 3, 64, 34, 49, 19, 65, 80, 50, 4, 35, 66, 20, 81, 96, 51, 5, 36, 82, 97, 67, 112, 21, 52, 98, 37, 83, 113, 6, 68, 128, 53, 22, 99, 114, 84, 7, 129, 38, 69, 100, 115, 144, 130, 85, 54, 23, 8, 145, 39, 70, 116, 101, 131, 160, 146, 55, 86, 24, 71, 132, 117, 161, 40, 9, 102, 147, 176, 162, 87, 56, 25, 133, 118, 177, 148, 72, 103, 41, 163, 10, 192, 178, 88, 57, 134, 149, 119, 26, 164, 73, 104, 193, 42, 179, 208, 11, 135, 89, 165, 120, 150, 58, 194, 180, 27, 74, 209, 105, 151, 136, 43, 90, 224, 166, 195, 181, 121, 210, 59, 12, 152, 106, 167, 196, 75, 137, 225, 211, 240, 182, 122, 91, 28, 197, 13, 226, 168, 183, 153, 44, 212, 138, 107, 241, 60, 29, 123, 198, 184, 227, 169, 242, 76, 213, 154, 45, 92, 14, 199, 139, 61, 228, 214, 170, 185, 243, 108, 77, 155, 30, 15, 200, 229, 124, 215, 244, 93, 46, 186, 171, 201, 109, 140, 230, 62, 216, 245, 31, 125, 78, 156, 231, 47, 187, 202, 217, 94, 246, 141, 63, 232, 172, 110, 247, 157, 79, 218, 203, 126, 233, 188, 248, 95, 173, 142, 219, 111, 249, 234, 158, 127, 189, 204, 250, 235, 143, 174, 220, 205, 159, 251, 190, 221, 175, 236, 237, 191, 206, 252, 222, 253, 207, 238, 223, 254, 239, 255 };

static constexpr u32 col_scan_16x16[256] = { 0, 16, 32, 48, 1, 64, 17, 80, 33, 96, 49, 2, 65, 112, 18, 81, 34, 128, 50, 97, 3, 66, 144, 19, 113, 35, 82, 160, 98, 51, 129, 4, 67, 176, 20, 114, 145, 83, 36, 99, 130, 52, 192, 5, 161, 68, 115, 21, 146, 84, 208, 177, 37, 131, 100, 53, 162, 224, 69, 6, 116, 193, 147, 85, 22, 240, 132, 38, 178, 101, 163, 54, 209, 117, 70, 7, 148, 194, 86, 179, 225, 23, 133, 39, 164, 8, 102, 210, 241, 55, 195, 118, 149, 71, 180, 24, 87, 226, 134, 165, 211, 40, 103, 56, 72, 150, 196, 242, 119, 9, 181, 227, 88, 166, 25, 135, 41, 104, 212, 57, 151, 197, 120, 73, 243, 182, 136, 167, 213, 89, 10, 228, 105, 152, 198, 26, 42, 121, 183, 244, 168, 58, 137, 229, 74, 214, 90, 153, 199, 184, 11, 106, 245, 27, 122, 230, 169, 43, 215, 59, 200, 138, 185, 246, 75, 12, 91, 154, 216, 231, 107, 28, 44, 201, 123, 170, 60, 247, 232, 76, 139, 13, 92, 217, 186, 248, 155, 108, 29, 124, 45, 202, 233, 171, 61, 14, 77, 140, 15, 249, 93, 30, 187, 156, 218, 46, 109, 125, 62, 172, 78, 203, 31, 141, 234, 94, 47, 188, 63, 157, 110, 250, 219, 79, 126, 204, 173, 142, 95, 189, 111, 235, 158, 220, 251, 127, 174, 143, 205, 236, 159, 190, 221, 252, 175, 206, 237, 191, 253, 222, 238, 207, 254, 223, 239, 255 };

static constexpr u32 row_scan_16x16[256] = { 0, 1, 2, 16, 3, 17, 4, 18, 32, 5, 33, 19, 6, 34, 48, 20, 49, 7, 35, 21, 50, 64, 8, 36, 65, 22, 51, 37, 80, 9, 66, 52, 23, 38, 81, 67, 10, 53, 24, 82, 68, 96, 39, 11, 54, 83, 97, 69, 25, 98, 84, 40, 112, 55, 12, 70, 99, 113, 85, 26, 41, 56, 114, 100, 13, 71, 128, 86, 27, 115, 101, 129, 42, 57, 72, 116, 14, 87, 130, 102, 144, 73, 131, 117, 28, 58, 15, 88, 43, 145, 103, 132, 146, 118, 74, 160, 89, 133, 104, 29, 59, 147, 119, 44, 161, 148, 90, 105, 134, 162, 120, 176, 75, 135, 149, 30, 60, 163, 177, 45, 121, 91, 106, 164, 178, 150, 192, 136, 165, 179, 31, 151, 193, 76, 122, 61, 137, 194, 107, 152, 180, 208, 46, 166, 167, 195, 92, 181, 138, 209, 123, 153, 224, 196, 77, 168, 210, 182, 240, 108, 197, 62, 154, 225, 183, 169, 211, 47, 139, 93, 184, 226, 212, 241, 198, 170, 124, 155, 199, 78, 213, 185, 109, 227, 200, 63, 228, 242, 140, 214, 171, 186, 156, 229, 243, 125, 94, 201, 244, 215, 216, 230, 141, 187, 202, 79, 172, 110, 157, 245, 217, 231, 95, 246, 232, 126, 203, 247, 233, 173, 218, 142, 111, 158, 188, 248, 127, 234, 219, 249, 189, 204, 143, 174, 159, 250, 235, 205, 220, 175, 190, 251, 221, 191, 206, 236, 207, 237, 252, 222, 253, 223, 238, 239, 254, 255 };

static constexpr u32 default_scan_32x32[1024] = { 0, 32, 1, 64, 33, 2, 96, 65, 34, 128, 3, 97, 66, 160, 129, 35, 98, 4, 67, 130, 161, 192, 36, 99, 224, 5, 162, 193, 68, 131, 37, 100, 225, 194, 256, 163, 69, 132, 6, 226, 257, 288, 195, 101, 164, 38, 258, 7, 227, 289, 133, 320, 70, 196, 165, 290, 259, 228, 39, 321, 102, 352, 8, 197, 71, 134, 322, 291, 260, 353, 384, 229, 166, 103, 40, 354, 323, 292, 135, 385, 198, 261, 72, 9, 416, 167, 386, 355, 230, 324, 104, 293, 41, 417, 199, 136, 262, 387, 448, 325, 356, 10, 73, 418, 231, 168, 449, 294, 388, 105, 419, 263, 42, 200, 357, 450, 137, 480, 74, 326, 232, 11, 389, 169, 295, 420, 106, 451, 481, 358, 264, 327, 201, 43, 138, 512, 482, 390, 296, 233, 170, 421, 75, 452, 359, 12, 513, 265, 483, 328, 107, 202, 514, 544, 422, 391, 453, 139, 44, 234, 484, 297, 360, 171, 76, 515, 545, 266, 329, 454, 13, 423, 203, 108, 546, 485, 576, 298, 235, 140, 361, 330, 172, 547, 45, 455, 267, 577, 486, 77, 204, 362, 608, 14, 299, 578, 109, 236, 487, 609, 331, 141, 579, 46, 15, 173, 610, 363, 78, 205, 16, 110, 237, 611, 142, 47, 174, 79, 206, 17, 111, 238, 48, 143, 80, 175, 112, 207, 49, 18, 239, 81, 113, 19, 50, 82, 114, 51, 83, 115, 640, 516, 392, 268, 144, 20, 672, 641, 548, 517, 424, 393, 300, 269, 176, 145, 52, 21, 704, 673, 642, 580, 549, 518, 456, 425, 394, 332, 301, 270, 208, 177, 146, 84, 53, 22, 736, 705, 674, 643, 612, 581, 550, 519, 488, 457, 426, 395, 364, 333, 302, 271, 240, 209, 178, 147, 116, 85, 54, 23, 737, 706, 675, 613, 582, 551, 489, 458, 427, 365, 334, 303, 241, 210, 179, 117, 86, 55, 738, 707, 614, 583, 490, 459, 366, 335, 242, 211, 118, 87, 739, 615, 491, 367, 243, 119, 768, 644, 520, 396, 272, 148, 24, 800, 769, 676, 645, 552, 521, 428, 397, 304, 273, 180, 149, 56, 25, 832, 801, 770, 708, 677, 646, 584, 553, 522, 460, 429, 398, 336, 305, 274, 212, 181, 150, 88, 57, 26, 864, 833, 802, 771, 740, 709, 678, 647, 616, 585, 554, 523, 492, 461, 430, 399, 368, 337, 306, 275, 244, 213, 182, 151, 120, 89, 58, 27, 865, 834, 803, 741, 710, 679, 617, 586, 555, 493, 462, 431, 369, 338, 307, 245, 214, 183, 121, 90, 59, 866, 835, 742, 711, 618, 587, 494, 463, 370, 339, 246, 215, 122, 91, 867, 743, 619, 495, 371, 247, 123, 896, 772, 648, 524, 400, 276, 152, 28, 928, 897, 804, 773, 680, 649, 556, 525, 432, 401, 308, 277, 184, 153, 60, 29, 960, 929, 898, 836, 805, 774, 712, 681, 650, 588, 557, 526, 464, 433, 402, 340, 309, 278, 216, 185, 154, 92, 61, 30, 992, 961, 930, 899, 868, 837, 806, 775, 744, 713, 682, 651, 620, 589, 558, 527, 496, 465, 434, 403, 372, 341, 310, 279, 248, 217, 186, 155, 124, 93, 62, 31, 993, 962, 931, 869, 838, 807, 745, 714, 683, 621, 590, 559, 497, 466, 435, 373, 342, 311, 249, 218, 187, 125, 94, 63, 994, 963, 870, 839, 746, 715, 622, 591, 498, 467, 374, 343, 250, 219, 126, 95, 995, 871, 747, 623, 499, 375, 251, 127, 900, 776, 652, 528, 404, 280, 156, 932, 901, 808, 777, 684, 653, 560, 529, 436, 405, 312, 281, 188, 157, 964, 933, 902, 840, 809, 778, 716, 685, 654, 592, 561, 530, 468, 437, 406, 344, 313, 282, 220, 189, 158, 996, 965, 934, 903, 872, 841, 810, 779, 748, 717, 686, 655, 624, 593, 562, 531, 500, 469, 438, 407, 376, 345, 314, 283, 252, 221, 190, 159, 997, 966, 935, 873, 842, 811, 749, 718, 687, 625, 594, 563, 501, 470, 439, 377, 346, 315, 253, 222, 191, 998, 967, 874, 843, 750, 719, 626, 595, 502, 471, 378, 347, 254, 223, 999, 875, 751, 627, 503, 379, 255, 904, 780, 656, 532, 408, 284, 936, 905, 812, 781, 688, 657, 564, 533, 440, 409, 316, 285, 968, 937, 906, 844, 813, 782, 720, 689, 658, 596, 565, 534, 472, 441, 410, 348, 317, 286, 1000, 969, 938, 907, 876, 845, 814, 783, 752, 721, 690, 659, 628, 597, 566, 535, 504, 473, 442, 411, 380, 349, 318, 287, 1001, 970, 939, 877, 846, 815, 753, 722, 691, 629, 598, 567, 505, 474, 443, 381, 350, 319, 1002, 971, 878, 847, 754, 723, 630, 599, 506, 475, 382, 351, 1003, 879, 755, 631, 507, 383, 908, 784, 660, 536, 412, 940, 909, 816, 785, 692, 661, 568, 537, 444, 413, 972, 941, 910, 848, 817, 786, 724, 693, 662, 600, 569, 538, 476, 445, 414, 1004, 973, 942, 911, 880, 849, 818, 787, 756, 725, 694, 663, 632, 601, 570, 539, 508, 477, 446, 415, 1005, 974, 943, 881, 850, 819, 757, 726, 695, 633, 602, 571, 509, 478, 447, 1006, 975, 882, 851, 758, 727, 634, 603, 510, 479, 1007, 883, 759, 635, 511, 912, 788, 664, 540, 944, 913, 820, 789, 696, 665, 572, 541, 976, 945, 914, 852, 821, 790, 728, 697, 666, 604, 573, 542, 1008, 977, 946, 915, 884, 853, 822, 791, 760, 729, 698, 667, 636, 605, 574, 543, 1009, 978, 947, 885, 854, 823, 761, 730, 699, 637, 606, 575, 1010, 979, 886, 855, 762, 731, 638, 607, 1011, 887, 763, 639, 916, 792, 668, 948, 917, 824, 793, 700, 669, 980, 949, 918, 856, 825, 794, 732, 701, 670, 1012, 981, 950, 919, 888, 857, 826, 795, 764, 733, 702, 671, 1013, 982, 951, 889, 858, 827, 765, 734, 703, 1014, 983, 890, 859, 766, 735, 1015, 891, 767, 920, 796, 952, 921, 828, 797, 984, 953, 922, 860, 829, 798, 1016, 985, 954, 923, 892, 861, 830, 799, 1017, 986, 955, 893, 862, 831, 1018, 987, 894, 863, 1019, 895, 924, 956, 925, 988, 957, 926, 1020, 989, 958, 927, 1021, 990, 959, 1022, 991, 1023 };

static constexpr u8 mode_to_txfm_map[MB_MODE_COUNT] = {
    DCT_DCT,   // DC
    ADST_DCT,  // V
    DCT_ADST,  // H
    DCT_DCT,   // D45
    ADST_ADST, // D135
    ADST_DCT,  // D117
    DCT_ADST,  // D153
    DCT_ADST,  // D207
    ADST_DCT,  // D63
    ADST_ADST, // TM
    DCT_DCT,   // NEARESTMV
    DCT_DCT,   // NEARMV
    DCT_DCT,   // ZEROMV
    DCT_DCT    // NEWMV
};

static constexpr u8 coefband_4x4[16] = { 0, 1, 1, 2, 2, 2, 3, 3, 3, 3, 4, 4, 4, 5, 5, 5 };

static constexpr u8 coefband_8x8plus[1024] = { 0, 1, 1, 2, 2, 2, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5 };

static constexpr u8 energy_class[12] = { 0, 1, 2, 3, 3, 4, 4, 5, 5, 5, 5, 5 };

static constexpr u8 extra_bits[11][3] = {
    { 0, 0, 0 },
    { 0, 0, 1 },
    { 0, 0, 2 },
    { 0, 0, 3 },
    { 0, 0, 4 },
    { 1, 1, 5 },
    { 2, 2, 7 },
    { 3, 3, 11 },
    { 4, 4, 19 },
    { 5, 5, 35 },
    { 6, 14, 67 }
};

static constexpr u8 cat_probs[7][14] = {
    { 0 },
    { 159 },
    { 165, 145 },
    { 173, 148, 140 },
    { 176, 155, 140, 135 },
    { 180, 157, 141, 134, 130 },
    { 254, 254, 254, 252, 249, 243, 230, 196, 177, 153, 140, 133, 130, 129 }
};

static constexpr MotionVector mv_ref_blocks[BLOCK_SIZES][MVREF_NEIGHBOURS] = {
    { { -1, 0 }, { 0, -1 }, { -1, -1 }, { -2, 0 }, { 0, -2 }, { -2, -1 }, { -1, -2 }, { -2, -2 } },
    { { -1, 0 }, { 0, -1 }, { -1, -1 }, { -2, 0 }, { 0, -2 }, { -2, -1 }, { -1, -2 }, { -2, -2 } },
    { { -1, 0 }, { 0, -1 }, { -1, -1 }, { -2, 0 }, { 0, -2 }, { -2, -1 }, { -1, -2 }, { -2, -2 } },
    { { -1, 0 }, { 0, -1 }, { -1, -1 }, { -2, 0 }, { 0, -2 }, { -2, -1 }, { -1, -2 }, { -2, -2 } },
    { { 0, -1 }, { -1, 0 }, { 1, -1 }, { -1, -1 }, { 0, -2 }, { -2, 0 }, { -2, -1 }, { -1, -2 } },
    { { -1, 0 }, { 0, -1 }, { -1, 1 }, { -1, -1 }, { -2, 0 }, { 0, -2 }, { -1, -2 }, { -2, -1 } },
    { { -1, 0 }, { 0, -1 }, { -1, 1 }, { 1, -1 }, { -1, -1 }, { -3, 0 }, { 0, -3 }, { -3, -3 } },
    { { 0, -1 }, { -1, 0 }, { 2, -1 }, { -1, -1 }, { -1, 1 }, { 0, -3 }, { -3, 0 }, { -3, -3 } },
    { { -1, 0 }, { 0, -1 }, { -1, 2 }, { -1, -1 }, { 1, -1 }, { -3, 0 }, { 0, -3 }, { -3, -3 } },
    { { -1, 1 }, { 1, -1 }, { -1, 2 }, { 2, -1 }, { -1, -1 }, { -3, 0 }, { 0, -3 }, { -3, -3 } },
    { { 0, -1 }, { -1, 0 }, { 4, -1 }, { -1, 2 }, { -1, -1 }, { 0, -3 }, { -3, 0 }, { 2, -1 } },
    { { -1, 0 }, { 0, -1 }, { -1, 4 }, { 2, -1 }, { -1, -1 }, { -3, 0 }, { 0, -3 }, { -1, 2 } },
    { { -1, 3 }, { 3, -1 }, { -1, 4 }, { 4, -1 }, { -1, -1 }, { -1, 0 }, { 0, -1 }, { -1, 6 } }
};

static constexpr u8 mode_2_counter[MB_MODE_COUNT] = { 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 0, 0, 3, 1 };

static constexpr u8 counter_to_context[19] = {
    BOTH_PREDICTED,
    NEW_PLUS_NON_INTRA,
    BOTH_NEW,
    ZERO_PLUS_PREDICTED,
    NEW_PLUS_NON_INTRA,
    INVALID_CASE,
    BOTH_ZERO,
    INVALID_CASE,
    INVALID_CASE,
    INTRA_PLUS_NON_INTRA,
    INTRA_PLUS_NON_INTRA,
    INVALID_CASE,
    INTRA_PLUS_NON_INTRA,
    INVALID_CASE,
    INVALID_CASE,
    INVALID_CASE,
    INVALID_CASE,
    INVALID_CASE,
    BOTH_INTRA
};

}
