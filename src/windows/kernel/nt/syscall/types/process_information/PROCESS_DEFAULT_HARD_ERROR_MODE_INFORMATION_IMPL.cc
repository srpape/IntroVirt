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
#include "PROCESS_DEFAULT_HARD_ERROR_MODE_INFORMATION_IMPL.hh"

#include <boost/io/ios_state.hpp>

namespace introvirt {
namespace windows {
namespace nt {

void PROCESS_DEFAULT_HARD_ERROR_MODE_INFORMATION_IMPL::write(std::ostream& os,
                                                             const std::string& linePrefix) const {
    PROCESS_DEFAULT_HARD_ERROR_MODE_INFORMATION_IMPL_BASE::write(os, linePrefix);

    boost::io::ios_flags_saver ifs(os);
    os << std::hex;
    os << linePrefix << "DefaultHardErrorMode: 0x" << DefaultHardErrorMode() << '\n';
}

Json::Value PROCESS_DEFAULT_HARD_ERROR_MODE_INFORMATION_IMPL::json() const {
    Json::Value result = PROCESS_DEFAULT_HARD_ERROR_MODE_INFORMATION_IMPL_BASE::json();
    result["DefaultHardErrorMode"] = DefaultHardErrorMode();
    return result;
}

} // namespace nt
} // namespace windows
} // namespace introvirt