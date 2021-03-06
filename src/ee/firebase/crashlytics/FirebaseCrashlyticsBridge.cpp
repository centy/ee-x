#include "ee/firebase/crashlytics/FirebaseCrashlyticsBridge.hpp"

#include <ee/core/internal/MessageBridge.hpp>

namespace ee {
namespace firebase {
namespace crashlytics {
using Self = Bridge;

namespace {
constexpr auto kLog = "FirebaseCrashlytics_log";
} // namespace

Self::Bridge()
    : bridge_(MessageBridge::getInstance()) {}

Self::~Bridge() = default;

void Self::log(const std::string& message) {
    bridge_.call(kLog, message);
}
} // namespace crashlytics
} // namespace firebase
} // namespace ee