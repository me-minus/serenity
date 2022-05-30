/*
 * Copyright (c) 2018-2021, James Mintram <me@jamesrm.com>
 * Copyright (c) 2018-2021, Andreas Kling <kling@serenityos.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <AK/Function.h>
#include <Kernel/Arch/DeferredCallEntry.h>

#if ARCH(X86_64) || ARCH(I386)
#    include <Kernel/Arch/x86/x86Processor.h>
#elif ARCH(AARCH64)
#    include <Kernel/Arch/aarch64/aarch64Processor.h>
#else
#    error "Unknown architecture"
#endif

namespace Kernel {

#if ARCH(X86_64) || ARCH(I386)
using ProcessorImpl = x86Processor;
#elif ARCH(AARCH64)
using ProcessorImpl = aarch64Processor;
using x86Processor = aarch64Processor;  // FIXME: remove this soon
#endif

namespace Memory {
class PageDirectory;
}

struct ProcessorMessageEntry;
struct ProcessorMessage {
    using CallbackFunction = Function<void()>;

    enum Type {
        FlushTlb,
        Callback,
    };
    Type type;
    Atomic<u32> refs;
    union {
        ProcessorMessage* next; // only valid while in the pool
        alignas(CallbackFunction) u8 callback_storage[sizeof(CallbackFunction)];
        struct {
            Memory::PageDirectory const* page_directory;
            u8* ptr;
            size_t page_count;
        } flush_tlb;
    };

    bool volatile async;

    ProcessorMessageEntry* per_proc_entries;

    CallbackFunction& callback_value()
    {
        return *bit_cast<CallbackFunction*>(&callback_storage);
    }

    void invoke_callback()
    {
        VERIFY(type == Type::Callback);
        callback_value()();
    }
};

struct ProcessorMessageEntry {
    ProcessorMessageEntry* next;
    ProcessorMessage* msg;
};

class Processor {
public:
    static void wait_check();

    [[noreturn]] static void halt();
};

template<typename T>
class ProcessorSpecific {
public:
    static void initialize()
    {
        ProcessorImpl::current().set_specific(T::processor_specific_data_id(), new T);
    }
    static T& get()
    {
        return *ProcessorImpl::current().get_specific<T>();
    }
};
}
