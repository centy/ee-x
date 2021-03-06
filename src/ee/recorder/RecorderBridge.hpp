//
//  Created by Zinge on 7/4/16.
//
//

#ifndef EE_X_RECORDER_BRIDGE_HPP
#define EE_X_RECORDER_BRIDGE_HPP

#ifdef __cplusplus

#include <string>

#include "ee/RecorderFwd.hpp"

namespace ee {
namespace recorder {
class Bridge final {
public:
    Bridge();
    ~Bridge();

    /// Checks whether recorder is supported on the current device.
    bool isSupported() const;

    void startRecording();
    void stopRecording();
    void cancelRecording();

    std::string getRecordingUrl() const;
    bool checkRecordingPermission() const;

private:
    MessageBridge& bridge_;
};
} // namespace recorder
} // namespace ee

#endif // __cplusplus

#endif /* EE_X_RECORDER_BRIDGE_HPP */
