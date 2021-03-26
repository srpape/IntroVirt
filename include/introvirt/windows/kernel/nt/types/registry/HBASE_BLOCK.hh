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

#include <introvirt/core/memory/guest_ptr.hh>
#include <introvirt/windows/kernel/nt/fwd.hh>

#include <cstdint>
#include <memory>
#include <string>

namespace introvirt {
namespace windows {
namespace nt {

class UNICODE_STRING;

class HBASE_BLOCK {
  public:
    virtual const int64_t TimeStamp() const = 0;
    virtual const std::string& FileName() const = 0;
    virtual uint32_t RootCell() const = 0;
    virtual uint32_t Length() const = 0;
    virtual guest_ptr<void> ptr() const = 0;

    virtual ~HBASE_BLOCK() = default;
};

} /* namespace nt */
} /* namespace windows */
} /* namespace introvirt */
