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
#include <introvirt/windows/pe/types/IMAGE_EXPORT_DIRECTORY.hh>

namespace introvirt {
namespace windows {
namespace pe {

namespace structs {

struct _IMAGE_EXPORT_DIRECTORY {
    uint32_t Characteristics;
    uint32_t TimeDateStamp;
    uint16_t MajorVersion;
    uint16_t MinorVersion;
    uint32_t Name;
    uint32_t Base;
    uint32_t NumberOfFunctions;
    uint32_t NumberOfNames;
    uint32_t AddressOfFunctions;
    uint32_t AddressOfNames; // A relative offset to a table of 4 byte addresses, each pointing to
                             // an ascii string
    uint32_t AddressOfNameOrdinals;
};

} // namespace structs

class IMAGE_EXPORT_DIRECTORY_IMPL final : public IMAGE_EXPORT_DIRECTORY {
  public:
    uint32_t Characteristics() const override { return ptr_->Characteristics; }
    uint32_t TimeDateStamp() const override { return ptr_->TimeDateStamp; }
    uint16_t MajorVersion() const override { return ptr_->MajorVersion; }
    uint16_t MinorVersion() const override { return ptr_->MinorVersion; }

    const std::map<guest_ptr<void>, Export>& AddressToExportMap() const override {
        return AddressToExportMap_;
    }
    const std::unordered_map<std::string, Export>& NameToExportMap() const override {
        return NameToExportMap_;
    }

    const Export* find(const std::string& name) const override {
        auto iter = NameToExportMap_.find(name);
        if (iter != NameToExportMap_.end())
            return &(iter->second);
        return nullptr;
    }

    IMAGE_EXPORT_DIRECTORY_IMPL(const guest_ptr<void>& image_base, const guest_ptr<void>& dir_start,
                                uint32_t dir_size, const guest_ptr<void>& code_start,
                                uint32_t code_size)
        : image_base_(image_base), ptr_(dir_start) {

        // Get the tables
        guest_ptr<const uint32_t[]> names(image_base_ + ptr_->AddressOfNames, ptr_->NumberOfNames);
        guest_ptr<const uint16_t[]> name_ordinals(image_base_ + ptr_->AddressOfNameOrdinals,
                                                  ptr_->NumberOfNames);
        guest_ptr<const uint32_t[]> functions(image_base_ + ptr_->AddressOfFunctions,
                                              ptr_->NumberOfFunctions);

        const guest_ptr<void> dir_end = dir_start + dir_size;
        const guest_ptr<void> code_end = code_start + code_size;

        // Run through the AddressOfNames table
        for (size_t i = 0; i < ptr_->NumberOfNames; ++i) {
            guest_ptr<void> pExportName = image_base_ + names[i];

            auto mapping = map_guest_cstring(pExportName);
            std::string name(mapping.get(), mapping.length());

            uint16_t ordinal = name_ordinals[i];
            uint32_t functionOffset = functions[ordinal];
            guest_ptr<void> pfunction = image_base_ + functionOffset;
            ExportType export_type;

            if (pfunction >= dir_start && pfunction < dir_end) {
                // Forward Export
                export_type = EXPORT_TYPE_FORWARD;
            } else if (pfunction >= code_start && pfunction < code_end) {
                export_type = EXPORT_TYPE_CODE;
            } else {
                export_type = EXPORT_TYPE_DATA;
            }

            Export result;
            result.exportType = export_type;
            result.address = pfunction;
            result.name = name;

            NameToExportMap_[name] = result;
            AddressToExportMap_[pfunction] = result;
        }
    }

  private:
    guest_ptr<void> image_base_;
    guest_ptr<structs::_IMAGE_EXPORT_DIRECTORY> ptr_;

    std::map<guest_ptr<void>, Export> AddressToExportMap_;
    std::unordered_map<std::string, Export> NameToExportMap_;
};

} // namespace pe
} // namespace windows
} // namespace introvirt