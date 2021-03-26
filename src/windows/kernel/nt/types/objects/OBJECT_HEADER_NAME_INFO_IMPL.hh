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
#pragma once

#include "windows/kernel/nt/structs/structs.hh"
#include "windows/kernel/nt/types/UNICODE_STRING_IMPL.hh"

#include <introvirt/core/memory/guest_ptr.hh>
#include <introvirt/fwd.hh>
#include <introvirt/windows/kernel/nt/types/objects/OBJECT_HEADER_NAME_INFO.hh>

#include <optional>

namespace introvirt {
namespace windows {
namespace nt {

template <typename PtrType>
class OBJECT_HEADER_NAME_INFO_IMPL final : public OBJECT_HEADER_NAME_INFO {
  public:
    const std::string& Name() const override;

    OBJECT_HEADER_NAME_INFO_IMPL(const NtKernelImpl<PtrType>& kernel, const guest_ptr<void>& ptr);

  private:
    const structs::OBJECT_HEADER_NAME_INFO* offsets_;
    guest_ptr<char[]> buffer_;
    std::optional<nt::UNICODE_STRING_IMPL<PtrType>> Name_;
};

} // namespace nt
} // namespace windows
} // namespace introvirt