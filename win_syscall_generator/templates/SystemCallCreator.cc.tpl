/*
 * Copyright 2021 Assured Information Security, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *         http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/* This file is automatically generated. Do not edit. */
#include "SystemCallCreator.hh"
#include "windows/kernel/nt/syscall/syscall.hh"
#include "windows/kernel/win32k/syscall/syscall.hh"

#include <introvirt/windows/event/WindowsEvent.hh>
#include <introvirt/windows/kernel/nt/syscall/syscall.hh>
#include <introvirt/windows/kernel/win32k/syscall/syscall.hh>

namespace introvirt {
namespace windows {

template<typename PtrType>
std::unique_ptr<WindowsSystemCall> make_unique_impl(WindowsEvent& event) {
    switch (event.syscall().index()) {
    /*
     * NT system calls
     */
{%- for name in ntdata %}
{%- if not ntdata[name].get('helper_base') %}
    case SystemCallIndex::{{name}}:
        return std::make_unique<nt::{{name}}Impl<PtrType>>(event);
{%- endif %}
{%- endfor %}


    /*
     * Win32k system calls
     */
{%- for name in win32kdata %}
{%- if not win32kdata[name].get('helper_base') %}
    case SystemCallIndex::{{name}}:
        return std::make_unique<win32k::{{name}}Impl<PtrType>>(event);
{%- endif %}
{%- endfor %}


    /*
     *  No available handler
     */
    default:
        if (event.syscall().raw_index() < 0x1000) {
            // Lower number calls are from ntll
            return std::make_unique<nt::NtSystemCallImpl<PtrType>>(event, false);
        } else {
            // This is a win32k system call
            return std::make_unique<win32k::Win32kSystemCallImpl<PtrType>>(event, false);
        }
    }

}

std::unique_ptr<WindowsSystemCall> SystemCallCreator::make_unique(WindowsEvent& event) {
    if(event.guest().x64()) {
        return make_unique_impl<uint64_t>(event);
    } else {
        return make_unique_impl<uint32_t>(event);   
    }
}

} /* namespace windows */
} /* namespace introvirt */