//
//  JsbFirebaseConfigInfo.cpp
//  ee_x
//
//  Created by eps on 10/16/19
//

#include "ee/jsb/firebase/remote_config/JsbFirebaseConfigInfo.hpp"

#include <ee/nlohmann/json.hpp>

#include <ee/firebase/remote_config/FirebaseRemoteConfigBridge.hpp>
#include <ee/jsb/JsbCore.hpp>

using Self = ee::firebase::remote_config::ConfigInfo;

namespace ee {
namespace core {
template <>
void set_value(se::Value& value, Self& input) {
   auto item = nlohmann::json::object();
   item["fetchTime"] = input.fetchTime;
   item["lastFetchStatus"] = input.lastFetchStatus;
   item["lastFetchFailureReason"] = input.lastFetchFailureReason;
   item["throttledEndTime"] = input.throttledEndTime;
   auto&& obj = se::Object::createJSONObject(item.dump());
   value.setObject(obj);
}
} // namespace core

namespace firebase {
namespace remote_config {
/*
Not used since ConfigInfo returns JSON object.

namespace {
se::Class* clazz = nullptr;

// clang-format off
constexpr auto getFetchTime              = &core::makeInstanceProperty<&Self::fetchTime>;
constexpr auto getLastFetchStatus        = &core::makeInstanceProperty<&Self::lastFetchStatus>;
constexpr auto getLastFetchFailureReason = &core::makeInstanceProperty<&Self::lastFetchFailureReason>;
constexpr auto getThrottledEndTime       = &core::makeInstanceProperty<&Self::throttledEndTime>;
// clang-format on

SE_BIND_PROP_GET(getFetchTime)
SE_BIND_PROP_GET(getLastFetchStatus)
SE_BIND_PROP_GET(getLastFetchFailureReason)
SE_BIND_PROP_GET(getThrottledEndTime)
} // namespace
*/

bool registerJsbConfigInfo(se::Object* global) {
    /*
    Not used since ConfigInfo returns JSON object.

    auto firebase_ = core::getPath(global, "ee/firebase");
    auto cls = se::Class::create("ConfigInfo", firebase_, nullptr, nullptr);

    cls->defineProperty("fetchTime", _SE(getFetchTime), nullptr);
    cls->defineProperty("lastFetchStatus", _SE(getLastFetchStatus), nullptr);
    cls->defineProperty("lastFetchFailureReason", _SE(getLastFetchFailureReason), nullptr);
    cls->defineProperty("throttledEndTime", _SE(getThrottledEndTime), nullptr);

    cls->install();

    JSBClassType::registerClass<Self>(cls);
    clazz = cls;

    se::ScriptEngine::getInstance()->clearException();
    */
    return true;
}
} // namespace remote_config
} // namespace firebase
} // namespace ee