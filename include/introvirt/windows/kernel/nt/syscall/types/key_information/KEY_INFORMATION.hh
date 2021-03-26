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

#include "KEY_INFORMATION_CLASS.hh"

#include <introvirt/core/memory/guest_ptr.hh>
#include <introvirt/util/json/json.hh>
#include <introvirt/windows/kernel/nt/fwd.hh>

#include <cstdint>
#include <string>

namespace introvirt {
namespace windows {
namespace nt {

/**
 * @brief An abstract class reprseneting a more specific key information buffer.
 *
 */
class KEY_INFORMATION {
  public:
    /**
     * @brief Get the type of key information held in the buffer
     *
     * @return KEY_INFORMATION_CLASS
     */
    virtual KEY_INFORMATION_CLASS KeyInformationClass() const = 0;

    /**
     * @brief Get the address that the buffer is at
     *
     * @return guest_ptr<void>
     */
    virtual guest_ptr<void> ptr() const = 0;

    /**
     * @brief Get the size of the buffer
     *
     * @return uint32_t
     */
    virtual uint32_t buffer_size() const = 0;

    /**
     * @brief Write a description to the ostream
     * @param os The output stream to write to
     * @param linePrefix The prefix written before each line
     */
    virtual void write(std::ostream& os, const std::string& linePrefix = "") const = 0;

    /**
     * @brief Return the file system information class as a JSON representation
     *
     * @return Json::Value
     */
    virtual Json::Value json() const = 0;

    /**
     * @brief Create a KEY_VALUE_INFORMATION parser instance.
     *
     * @param kernel The kernel of the guest
     * @param information_class The type of information contained in the buffer
     * @param ptr The address of the information in the guest
     * @param buffer_size The size of the information buffer
     */
    static std::unique_ptr<KEY_INFORMATION> make_unique(const NtKernel& kernel,
                                                        KEY_INFORMATION_CLASS information_class,
                                                        const guest_ptr<void>& ptr,
                                                        uint32_t buffer_size);

    virtual ~KEY_INFORMATION() = default;
};

} // namespace nt
} // namespace windows
} // namespace introvirt
