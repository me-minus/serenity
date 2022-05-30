/*
 * Copyright (c) 2018-2021, Andreas Kling <kling@serenityos.org>
 * Copyright (c) 2022, Linus Groh <linusg@serenityos.org>
 * Copyright (c) 2022, the SerenityOS developers.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <Kernel/Arch/Processor.h>

namespace Kernel {

void Processor::wait_check()
{
    x86Processor::pause();
    if (x86Processor::is_smp_enabled())
      x86Processor::current().smp_process_pending_messages();
}

[[noreturn]] void Processor::halt()
{
  x86Processor::halt();
}

}
