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

#include "FILE_INFO.hh"

#include <introvirt/windows/pe/fwd.hh>

#include <cstdint>

namespace introvirt {
namespace windows {
namespace pe {

/**
 * @brief
 *
 *
 * @see https://docs.microsoft.com/en-us/windows/win32/menurc/vs-versioninfo
 */
class VS_VERSIONINFO : public FILE_INFO {
  public:
    /*
     * Will return nullptr if wValueLength() is 0
     */
    virtual const VS_FIXEDFILEINFO& Value() const = 0;

    /**
     * Returns nullptr if non-existent
     */
    virtual const pe::StringFileInfo* StringFileInfo() const = 0;

    /**
     * Returns nullptr if non-existent
     */
    virtual const pe::VarFileInfo* VarFileInfo() const = 0;

    static std::unique_ptr<VS_VERSIONINFO> make_unique(const guest_ptr<void>& ptr);
};

} /* namespace pe */
} /* namespace windows */
} /* namespace introvirt */
