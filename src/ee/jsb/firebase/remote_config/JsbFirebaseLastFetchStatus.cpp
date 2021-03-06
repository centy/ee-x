//
//  JsbFirebaseLastFetchStatus.cpp
//  ee_x
//
//  Created by eps on 10/16/19
//

#include "ee/jsb/firebase/remote_config/JsbFirebaseLastFetchStatus.hpp"

#include <ee/firebase/remote_config/FirebaseRemoteConfigBridge.hpp>
#include <ee/jsb/JsbCore.hpp>

using Self = ee::firebase::remote_config::LastFetchStatus;
using UnderlyingType = std::underlying_type_t<Self>;

namespace ee {
/*
Not used since ConfigInfo returns JSON object.

namespace core {
template <>
void set_value(se::Value& value, Self& input) {
    set_value(value, static_cast<UnderlyingType>(input));
}
} // namespace core
*/

namespace firebase {
namespace remote_config {
bool registerJsbLastFetchStatus(se::Object* global) {
    auto scope = core::getPath(global, "ee");
    auto cls = core::getPath(scope, "FirebaseLastFetchStatus");
    cls->setProperty("Success", se::Value(static_cast<UnderlyingType>(Self::Success)));
    cls->setProperty("Failure", se::Value(static_cast<UnderlyingType>(Self::Failure)));
    cls->setProperty("Pending", se::Value(static_cast<UnderlyingType>(Self::Pending)));

    se::ScriptEngine::getInstance()->clearException();
    return true;
}
} // namespace remote_config
} // namespace firebase
} // namespace ee