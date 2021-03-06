#ifndef EE_X_FIREBASE_CRASHLYTICS_BRIDGE_HPP
#define EE_X_FIREBASE_CRASHLYTICS_BRIDGE_HPP

#ifdef __cplusplus

#include <string>

#include <ee/FirebaseFwd.hpp>

namespace ee {
namespace firebase {
namespace crashlytics {
class Bridge final {
public:
    Bridge();
    ~Bridge();

    void log(const std::string& message);

private:
    IMessageBridge& bridge_;
};
} // namespace crashlytics
} // namespace firebase
} // namespace ee

#endif // __cplusplus

#endif // EE_X_FIREBASE_CRASHLYTICS_BRIDGE_HPP
