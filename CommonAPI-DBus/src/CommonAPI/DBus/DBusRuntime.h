/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */
#ifndef COMMONAPI_DBUS_DBUS_RUNTIME_H_
#define COMMONAPI_DBUS_DBUS_RUNTIME_H_

#include "CommonAPI/Runtime.h"

#include "DBusFactory.h"

namespace CommonAPI {
namespace DBus {

class DBusRuntime: public Runtime, public std::enable_shared_from_this<DBusRuntime> {
 public:
    static std::shared_ptr<Runtime> getInstance();

    std::shared_ptr<Factory> createFactory();

    static const MiddlewareInfo middlewareInfo_;
};

} // namespace DBus
} // namespace CommonAPI


extern "C" {

CommonAPI::MiddlewareInfo middlewareInfo = CommonAPI::DBus::DBusRuntime::middlewareInfo_;

}

#endif // COMMONAPI_DBUS_DBUS_RUNTIME_H_
