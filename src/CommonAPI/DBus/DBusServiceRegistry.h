/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */
#ifndef COMMONAPI_DBUS_DBUS_SERVICE_REGISTRY_H_
#define COMMONAPI_DBUS_DBUS_SERVICE_REGISTRY_H_


#include <CommonAPI/types.h>
#include <CommonAPI/Attribute.h>

#include "DBusConnection.h"

#include <unordered_map>
#include <map>
#include <unordered_set>
#include <string>
#include <vector>
#include <memory>
#include <list>
#include <mutex>

namespace CommonAPI {
namespace DBus {

typedef Event<std::string, std::string, std::string> NameOwnerChangedEvent;

typedef Event<std::string, std::string, std::string>::Subscription NameOwnerChangedEventSubscription;

typedef std::pair<std::string, std::string> DBusServiceInstanceId;

typedef std::unordered_map<std::string, int> PropertyDictStub;
typedef std::unordered_map<std::string, PropertyDictStub> InterfaceToPropertyDict;
typedef std::unordered_map<std::string, InterfaceToPropertyDict> DBusObjectToInterfaceDict;

class DBusConnection;
class DBusDaemonProxy;


class DBusServiceRegistry {
 public:
    static constexpr const char* getManagedObjectsDBusSignature_ = "a{oa{sa{sv}}}";

    DBusServiceRegistry() = delete;
    DBusServiceRegistry(const DBusServiceRegistry&) = delete;
    DBusServiceRegistry& operator=(const DBusServiceRegistry&) = delete;

    DBusServiceRegistry(std::shared_ptr<DBusConnection> connection);
    ~DBusServiceRegistry();

    std::vector<std::string> getAvailableServiceInstances(const std::string& interfaceName,
                                                          const std::string& domainName = "local");

    bool isServiceInstanceAlive(const std::string& instanceID,
                                const std::string& interfaceName,
                                const std::string& domainName = "local");

    bool isServiceInstanceAlive(const std::string& address);

    bool isReady();

    bool isReadyBlocking();

    void registerAvailabilityListener(const std::string& service, const std::function<void(bool)>& listener);

    std::future<bool>& getReadyFuture();

    DBusServiceStatusEvent& getServiceStatusEvent();

 private:
    void cacheAllServices();

    void removeProvidedServiceInstancesFromCache(const std::string& serviceBusName);
    void addProvidedServiceInstancesToCache(std::vector<std::string>& dbusNames);
    void addProvidedServiceInstancesToCache(const std::string& dbusNames);
    void addAllProvidedServiceInstancesToCache(const std::vector<std::string>& serviceBusNames);

    void getManagedObjects(const std::string& serviceBusName);

    void onDBusNameOwnerChangedEvent(const std::string& name, const std::string& oldOwner, const std::string& newOwner);

    bool isRemoteServiceVersionMatchingLocalVersion(const std::string& serviceBusName, const std::string& serviceInterfaceName);
    bool isServiceInstanceAliveHelper(const std::string& connectionName) const;

    DBusServiceInstanceId findInstanceIdMapping(const std::string& instanceId) const;
    std::string findInstanceIdMapping(const DBusServiceInstanceId& dbusInstanceId) const;

    void onManagedPaths(const CallStatus& status, DBusObjectToInterfaceDict replyMessage, std::string dbusWellKnownBusName);
    void onManagedPathsList(const CallStatus& status, DBusObjectToInterfaceDict managedObjects, std::list<std::string>::iterator iter, std::shared_ptr<std::list<std::string>> list);

    std::multimap<std::string, DBusServiceInstanceId> dbusCachedProvidersForInterfaces_;

    std::shared_ptr<DBusConnection> dbusConnection_;

    std::unordered_multimap<std::string, std::function<void(bool)>> availabilityCallbackList;

    bool ready;

    void onListNames(const CommonAPI::CallStatus&, std::vector<std::string>);

    NameOwnerChangedEvent::Subscription dbusNameOwnerChangedEventSubscription_;

    void updateListeners(const std::string& conName, const std::string& objName, const std::string& intName , bool available);

    mutable std::future<bool> readyFuture_;
    mutable std::promise<bool> readyPromise_;

    std::mutex readyMutex_;

    DBusServiceStatusEvent serviceStatusEvent_;
};


} // namespace DBus
} // namespace CommonAPI

#endif // COMMONAPI_DBUS_DBUS_SERVICE_REGISTRY_H_