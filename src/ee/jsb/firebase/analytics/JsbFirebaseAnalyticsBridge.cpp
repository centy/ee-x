//
// jsb_firebase_analytics.cpp
// ee_x
//
//  Created by Nguyen Van Quynh on 8/22/18
//
//

#include "ee/jsb/firebase/analytics/JsbFirebaseAnalyticsBridge.hpp"

#include <ee/Firebase.hpp>
#include <ee/jsb/JsbCore.hpp>

namespace ee {
namespace firebase {
namespace analytics {
namespace {
se::Class* clazz = nullptr;

using Self = Bridge;

// clang-format off
constexpr auto finalize                   = &core::makeFinalize<Self>;
constexpr auto constructor                = &core::makeConstructor<Self>;
constexpr auto initialize                 = &core::makeMethod<&Self::initialize>;
constexpr auto analyticsCollectionEnabled = &core::makeMethod<&Self::analyticsCollectionEnabled>;
constexpr auto setSessionTimeoutDuration  = &core::makeMethod<&Self::setSessionTimeoutDuration>;
constexpr auto setUserId                  = &core::makeMethod<&Self::setUserId>;
constexpr auto logEvent                   = &core::makeMethod<&Self::logEvent>;
// clang-format on

SE_BIND_FINALIZE_FUNC(finalize)
SE_BIND_CTOR(constructor, clazz, finalize)
SE_BIND_FUNC(initialize)
SE_BIND_FUNC(analyticsCollectionEnabled)
SE_BIND_FUNC(setSessionTimeoutDuration)
SE_BIND_FUNC(setUserId)
SE_BIND_FUNC(logEvent)
} // namespace

bool registerJsbBridge(se::Object* global) {
    auto scope = core::getPath(global, "ee");
    auto cls = se::Class::create("FirebaseAnalytics", scope, nullptr,
                                 _SE(constructor));
    cls->defineFinalizeFunction(_SE(finalize));

    EE_JSB_DEFINE_FUNCTION(cls, initialize);
    EE_JSB_DEFINE_FUNCTION(cls, analyticsCollectionEnabled);
    EE_JSB_DEFINE_FUNCTION(cls, setSessionTimeoutDuration);
    EE_JSB_DEFINE_FUNCTION(cls, setUserId);
    EE_JSB_DEFINE_FUNCTION(cls, logEvent);

    cls->install();

    JSBClassType::registerClass<Self>(cls);
    clazz = cls;

    se::ScriptEngine::getInstance()->clearException();
    return true;
}
} // namespace analytics
} // namespace firebase
} // namespace ee
