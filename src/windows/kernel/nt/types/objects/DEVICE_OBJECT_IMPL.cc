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
#include "DEVICE_OBJECT_IMPL.hh"
#include "DRIVER_OBJECT_IMPL.hh"

#include "windows/kernel/nt/NtKernelImpl.hh"

#include <introvirt/util/compiler.hh>
#include <introvirt/windows/exception/InvalidStructureException.hh>
#include <introvirt/windows/kernel/nt/NtKernel.hh>

namespace introvirt {
namespace windows {
namespace nt {

template <typename PtrType>
nt::DeviceType DEVICE_OBJECT_IMPL<PtrType>::DeviceType() const {
    return static_cast<nt::DeviceType>(offsets_->DeviceType.get<uint32_t>(buffer_));
}

template <typename PtrType>
std::string DEVICE_OBJECT_IMPL<PtrType>::DeviceName() const {
    return this->header().NameInfo().Name();
}

template <typename PtrType>
const DRIVER_OBJECT& DEVICE_OBJECT_IMPL<PtrType>::DriverObject() const {
    std::lock_guard lock(mtx_);
    if (!DriverObject_) {
        const PtrType pDriverObject = offsets_->DriverObject.get<PtrType>(buffer_);
        if (unlikely(pDriverObject == 0))
            throw InvalidStructureException("DEVICE_OBJECT::DriverObject was NULL");

        DriverObject_ =
            std::make_unique<DRIVER_OBJECT_IMPL<PtrType>>(kernel_, this->ptr_.clone(pDriverObject));
    }
    return *DriverObject_;
}

template <typename PtrType>
DEVICE_OBJECT_IMPL<PtrType>::DEVICE_OBJECT_IMPL(const NtKernelImpl<PtrType>& kernel,
                                                const guest_ptr<void>& ptr)
    : OBJECT_IMPL<PtrType, DEVICE_OBJECT>(kernel, ptr, ObjectType::Device), kernel_(kernel),
      offsets_(LoadOffsets<structs::DEVICE_OBJECT>(kernel)) {

    buffer_.reset(ptr, offsets_->size());
}

template <typename PtrType>
DEVICE_OBJECT_IMPL<PtrType>::DEVICE_OBJECT_IMPL(
    const NtKernelImpl<PtrType>& kernel,
    std::unique_ptr<OBJECT_HEADER_IMPL<PtrType>>&& object_header)
    : OBJECT_IMPL<PtrType, DEVICE_OBJECT>(kernel, std::move(object_header), ObjectType::Device),
      kernel_(kernel), offsets_(LoadOffsets<structs::DEVICE_OBJECT>(kernel)) {

    buffer_.reset(this->ptr_, offsets_->size());
}

template <typename PtrType>
DEVICE_OBJECT_IMPL<PtrType>::~DEVICE_OBJECT_IMPL() = default;

std::shared_ptr<DEVICE_OBJECT> DEVICE_OBJECT::make_shared(const NtKernel& kernel,
                                                          const guest_ptr<void>& ptr) {
    if (kernel.x64())
        return std::make_shared<DEVICE_OBJECT_IMPL<uint64_t>>(
            static_cast<const NtKernelImpl<uint64_t>&>(kernel), ptr);
    else
        return std::make_shared<DEVICE_OBJECT_IMPL<uint32_t>>(
            static_cast<const NtKernelImpl<uint32_t>&>(kernel), ptr);
}

std::shared_ptr<DEVICE_OBJECT>
DEVICE_OBJECT::make_shared(const NtKernel& kernel, std::unique_ptr<OBJECT_HEADER>&& object_header) {
    if (kernel.x64()) {
        std::unique_ptr<OBJECT_HEADER_IMPL<uint64_t>> object_header_impl;
        object_header_impl.reset(
            static_cast<OBJECT_HEADER_IMPL<uint64_t>*>(object_header.release()));
        return std::make_shared<DEVICE_OBJECT_IMPL<uint64_t>>(
            static_cast<const NtKernelImpl<uint64_t>&>(kernel), std::move(object_header_impl));
    } else {
        std::unique_ptr<OBJECT_HEADER_IMPL<uint32_t>> object_header_impl;
        object_header_impl.reset(
            static_cast<OBJECT_HEADER_IMPL<uint32_t>*>(object_header.release()));
        return std::make_shared<DEVICE_OBJECT_IMPL<uint32_t>>(
            static_cast<const NtKernelImpl<uint32_t>&>(kernel), std::move(object_header_impl));
    }
}

enum CHARACTERISTICS {
    FILE_REMOVABLE_MEDIA = 0x00000001,
    FILE_READ_ONLY_DEVICE = 0x00000002,
    FILE_FLOPPY_DISKETTE = 0x00000004,
    FILE_WRITE_ONCE_MEDIA = 0x00000008,
    FILE_REMOTE_DEVICE = 0x00000010,
    FILE_DEVICE_IS_MOUNTED = 0x00000020,
    FILE_VIRTUAL_VOLUME = 0x00000040,
    FILE_AUTOGENERATED_DEVICE_NAME = 0x00000080,
    FILE_DEVICE_SECURE_OPEN = 0x00000100,
    FILE_CHARACTERISTIC_PNP_DEVICE = 0x00000800,
    FILE_CHARACTERISTIC_TS_DEVICE = 0x00001000,
    FILE_CHARACTERISTIC_WEBDAV_DEVICE = 0x00002000,
    FILE_CHARACTERISTIC_CSV = 0x00010000,
    FILE_DEVICE_ALLOW_APPCONTAINER_TRAVERSAL = 0x00020000,
    FILE_PORTABLE_DEVICE = 0x00040000
};

bool DEVICE_OBJECT::DeviceCharacteristics::FILE_REMOVABLE_MEDIA() const {
    return value_ & CHARACTERISTICS::FILE_REMOVABLE_MEDIA;
}
bool DEVICE_OBJECT::DeviceCharacteristics::FILE_READ_ONLY_DEVICE() const {
    return value_ & CHARACTERISTICS::FILE_READ_ONLY_DEVICE;
}
bool DEVICE_OBJECT::DeviceCharacteristics::FILE_FLOPPY_DISKETTE() const {
    return value_ & CHARACTERISTICS::FILE_FLOPPY_DISKETTE;
}
bool DEVICE_OBJECT::DeviceCharacteristics::FILE_WRITE_ONCE_MEDIA() const {
    return value_ & CHARACTERISTICS::FILE_WRITE_ONCE_MEDIA;
}
bool DEVICE_OBJECT::DeviceCharacteristics::FILE_REMOTE_DEVICE() const {
    return value_ & CHARACTERISTICS::FILE_REMOTE_DEVICE;
}
bool DEVICE_OBJECT::DeviceCharacteristics::FILE_DEVICE_IS_MOUNTED() const {
    return value_ & CHARACTERISTICS::FILE_DEVICE_IS_MOUNTED;
}
bool DEVICE_OBJECT::DeviceCharacteristics::FILE_VIRTUAL_VOLUME() const {
    return value_ & CHARACTERISTICS::FILE_VIRTUAL_VOLUME;
}
bool DEVICE_OBJECT::DeviceCharacteristics::FILE_AUTOGENERATED_DEVICE_NAME() const {
    return value_ & CHARACTERISTICS::FILE_AUTOGENERATED_DEVICE_NAME;
}
bool DEVICE_OBJECT::DeviceCharacteristics::FILE_DEVICE_SECURE_OPEN() const {
    return value_ & CHARACTERISTICS::FILE_DEVICE_SECURE_OPEN;
}
bool DEVICE_OBJECT::DeviceCharacteristics::FILE_CHARACTERISTIC_PNP_DEVICE() const {
    return value_ & CHARACTERISTICS::FILE_CHARACTERISTIC_PNP_DEVICE;
}
bool DEVICE_OBJECT::DeviceCharacteristics::FILE_CHARACTERISTIC_TS_DEVICE() const {
    return value_ & CHARACTERISTICS::FILE_CHARACTERISTIC_TS_DEVICE;
}
bool DEVICE_OBJECT::DeviceCharacteristics::FILE_CHARACTERISTIC_WEBDAV_DEVICE() const {
    return value_ & CHARACTERISTICS::FILE_CHARACTERISTIC_WEBDAV_DEVICE;
}
bool DEVICE_OBJECT::DeviceCharacteristics::FILE_CHARACTERISTIC_CSV() const {
    return value_ & CHARACTERISTICS::FILE_CHARACTERISTIC_CSV;
}
bool DEVICE_OBJECT::DeviceCharacteristics::FILE_DEVICE_ALLOW_APPCONTAINER_TRAVERSAL() const {
    return value_ & CHARACTERISTICS::FILE_DEVICE_ALLOW_APPCONTAINER_TRAVERSAL;
}
bool DEVICE_OBJECT::DeviceCharacteristics::FILE_PORTABLE_DEVICE() const {
    return value_ & CHARACTERISTICS::FILE_PORTABLE_DEVICE;
}

template class DEVICE_OBJECT_IMPL<uint32_t>;
template class DEVICE_OBJECT_IMPL<uint64_t>;

} /* namespace nt */
} // namespace windows
} // namespace introvirt
