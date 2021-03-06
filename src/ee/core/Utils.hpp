//
//  Utils.hpp
//  ee_x
//
//  Created by Zinge on 10/9/17.
//
//

#ifndef EE_X_UTILS_HPP
#define EE_X_UTILS_HPP

#ifdef __cplusplus

#include <cstdarg>
#include <functional>
#include <future>
#include <random>
#include <string>

#include "ee/CoreFwd.hpp"

namespace ee {
namespace core {

// lower string
[[deprecated]] std::string str_tolower(std::string s);

/// Converts float to string without trailing zeroes.
std::string toString(float value);

/// Converts bool to string, used internally.
std::string toString(bool value);

/// Convert string to bool, used internally.
bool toBool(const std::string& value);

/// http://stackoverflow.com/questions/2342162/stdstring-formatting-like-sprintf
std::string format(std::string formatString, ...);
std::string format(std::string formatString, std::va_list args);

namespace detail {
std::mt19937& getRandomEngine();
} // namespace detail

/// Randomizes using C++11 engine.
template <class T, class = std::enable_if_t<std::is_arithmetic_v<T>>>
T random(T min, T max) {
    if constexpr (std::is_floating_point_v<T>) {
        std::uniform_real_distribution<T> dist(min, max);
        return dist(detail::getRandomEngine());
    } else {
        std::uniform_int_distribution<T> dist(min, max);
        return dist(detail::getRandomEngine());
    }
}

/// bit_cast.
///
/// https://gist.github.com/socantre/3472964
template <class Dest, class Source>
inline Dest bitCast(const Source& source) {
    static_assert(sizeof(Dest) == sizeof(Source),
                  "size of destination and source objects must be equal");

    static_assert(std::is_trivially_copyable<Dest>::value,
                  "destination type must be trivially copyable");

    static_assert(std::is_trivially_copyable<Source>::value,
                  "source type must be trivially copyable");
    Dest dest;
    std::memcpy(&dest, &source, sizeof(dest));
    return dest;
}

/// Logs a message using __android_log_print (Android) and NSLog (iOS).
void log(const LogLevel& level, const std::string& tag,
         const std::string& message);

std::string dumpBacktrace(size_t count);

/// Checks whether the current thread is the UI thread (Android) or Main thread
/// (iOS).
bool isMainThread();

template <class T = void>
using Runnable = std::function<T()>;

/// Runs the specified runnable on the main thread.
/// @returns Whether the function is executed immediately.
bool runOnUiThread(const Runnable<>& runnable);

/// Runs the specifieid runnable on the main thread after a delay.
void runOnUiThreadDelayed(const Runnable<>& runnable, float delay);

/// Runs the specified runnable on the main thread and block the current thread.
/// If the current thread is the main thread, it will be executed immediately.
[[deprecated]] void runOnUiThreadAndWait(const Runnable<>& runnable);

template <class T>
[[deprecated]] T runOnUiThreadAndWaitResult(const Runnable<T>& runnable) {
    std::promise<T> promise;
    runOnUiThread([runnable, &promise] { // Fix clang-format.
        promise.set_value(runnable());
    });
    return promise.get_future().get();
}

/// Checks whether an application with the specified package name (Android) or
/// scheme (iOS) is installed.
bool isApplicationInstalled(const std::string& applicationId);

/// Opens an application with the specified package name (Android) or scheme
/// (iOS).
bool openApplication(const std::string& applicationId);

/// Gets the application ID (Android) or bundle identifier (iOS).
std::string getApplicationId();

/// Gets the name of the application.
std::string getApplicationName();

/// Gets the version name of the application.
std::string getVersionName();

/// Gets the version code of the application.
std::string getVersionCode();

/// Android only.
/// iOS returns an empty string.
std::string getSHA1CertificateFingerprint();

/// Checks whether the current application is an Google Instant application
/// (Android only).
bool isInstantApp();

/// Checks whether the current device is a table.
bool isTablet();

/// Gets the screen density, i.e. pixel to dp ratio.
float getDensity();

/// Gets device's unique ID.
Task<std::string> getDeviceId();

/// https://developer.android.com/reference/android/view/DisplayCutout
struct SafeInset {
    /// The inset from the left which avoids the display cutout in pixels.
    int left;

    /// The inset from the right which avoids the display cutout in pixels.
    int right;

    /// The inset from the top which avoids the display cutout in pixels.
    int top;

    /// The inset from the bottom which avoids the display cutout in pixels.
    int bottom;
};

SafeInset getSafeInset();

bool sendMail(const std::string& recipient, const std::string& subject,
              const std::string& body);

/// Tests whether the specified host name can be resolved.
Task<bool> testConnection(const std::string& hostName, float timeOut);

/// Show Google Instant application installation prompt (Android only).
void showInstallPrompt(const std::string& url, const std::string& referrer);
} // namespace core

using core::bitCast;
using core::format;
using core::getApplicationId;
using core::getApplicationName;
using core::getDensity;
using core::getDeviceId;
using core::getSafeInset;
using core::getSHA1CertificateFingerprint;
using core::getVersionCode;
using core::getVersionName;
using core::isApplicationInstalled;
using core::isMainThread;
using core::isTablet;
using core::log;
using core::openApplication;
using core::random;
using core::runOnUiThread;
using core::runOnUiThreadAndWait;
using core::runOnUiThreadAndWaitResult;
using core::sendMail;
using core::testConnection;

using core::SafeInset;
} // namespace ee

#endif // __cplusplus

#endif /* EE_X_UTILS_HPP */
