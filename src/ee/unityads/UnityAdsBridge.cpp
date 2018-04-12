//
//  UnityAds.cpp
//  ee_x
//
//  Created by Pham Xuan Han on 5/12/17.
//
//

#include <cassert>

#include "ee/ads/NullInterstitialAd.hpp"
#include "ee/ads/NullRewardedVideo.hpp"
#include "ee/ads/internal/MediationManager.hpp"
#include "ee/core/Logger.hpp"
#include "ee/core/MessageBridge.hpp"
#include "ee/core/Utils.hpp"
#include "ee/unityads/UnityAdsBridge.hpp"
#include "ee/unityads/internal/UnityInterstitialAd.hpp"
#include "ee/unityads/internal/UnityRewardedVideo.hpp"

#include <ee/nlohmann/json.hpp>

namespace ee {
namespace unityads {
using Self = UnityAds;

namespace {
// clang-format off
constexpr auto k__initialize           = "UnityAds_initialize";
constexpr auto k__setDebugModeEnabled  = "UnityAds_setDebugModeEnabled";
constexpr auto k__isRewardedVideoReady = "UnityAds_isRewardedVideoReady";
constexpr auto k__showRewardedVideo    = "UnityAds_showRewardedVideo";
constexpr auto k__onError              = "UnityAds_onError";
constexpr auto k__onSkipped            = "UnityAds_onSkipped";
constexpr auto k__onFinished           = "UnityAds_onFinished";
// clang-format on
} // namespace

namespace {
// clang-format off
constexpr auto k__gameId          = "gameId";
constexpr auto k__testModeEnabled = "testModeEnabled";
// clang-format on
} // namespace

Self::UnityAds()
    : bridge_(MessageBridge::getInstance()) {
    Logger::getSystemLogger().debug("%s", __PRETTY_FUNCTION__);
    errored_ = false;
    displayed_ = false;

    bridge_.registerHandler(
        [this](const std::string& message) {
            onError(message);
            return "";
        },
        k__onError);
    bridge_.registerHandler(
        [this](const std::string& message) {
            onSkipped(message);
            return "";
        },
        k__onSkipped);
    bridge_.registerHandler(
        [this](const std::string& message) {
            onFinished(message);
            return "";
        },
        k__onFinished);
}

Self::~UnityAds() {
    Logger::getSystemLogger().debug(__PRETTY_FUNCTION__);
    bridge_.deregisterHandler(k__onError);
    bridge_.deregisterHandler(k__onSkipped);
    bridge_.deregisterHandler(k__onFinished);
}

void Self::initialize(const std::string& gameId, bool testModeEnabled) {
    Logger::getSystemLogger().debug("%s: gameId = %s test = %s",
                                    __PRETTY_FUNCTION__, gameId.c_str(),
                                    core::toString(testModeEnabled).c_str());
    nlohmann::json json;
    json[k__gameId] = gameId;
    json[k__testModeEnabled] = core::toString(testModeEnabled);
    bridge_.call(k__initialize, json.dump());
}

void Self::setDebugModeEnabled(bool enabled) {
    bridge_.call(k__setDebugModeEnabled, core::toString(enabled));
}

std::shared_ptr<IRewardedVideo>
Self::createRewardedVideo(const std::string& placementId) {
    Logger::getSystemLogger().debug("%s: placementId = %s", __PRETTY_FUNCTION__,
                                    placementId.c_str());
    if (rewardedVideos_.count(placementId) != 0) {
        return std::make_shared<NullRewardedVideo>();
    }
    auto result = new RewardedVideo(this, placementId);
    rewardedVideos_[placementId] = result;
    return std::shared_ptr<IRewardedVideo>(result);
}

bool Self::destroyRewardedVideo(const std::string& placementId) {
    Logger::getSystemLogger().debug("%s: placementId = %s", __PRETTY_FUNCTION__,
                                    placementId.c_str());
    if (rewardedVideos_.count(placementId) == 0) {
        return false;
    }
    rewardedVideos_.erase(placementId);
    return true;
}

std::shared_ptr<IInterstitialAd>
Self::createInterstitialAd(const std::string& placementId) {
    Logger::getSystemLogger().debug("%s: placementId = %s", __PRETTY_FUNCTION__,
                                    placementId.c_str());
    if (interstitialAds_.count(placementId) != 0) {
        return std::make_shared<NullInterstitialAd>();
    }
    auto result = new InterstitialAd(this, placementId);
    interstitialAds_[placementId] = result;
    return std::shared_ptr<IInterstitialAd>(result);
}

bool Self::destroyInterstitialAd(const std::string& placementId) {
    Logger::getSystemLogger().debug("%s: placementId = %s", __PRETTY_FUNCTION__,
                                    placementId.c_str());
    if (interstitialAds_.count(placementId) == 0) {
        return false;
    }
    interstitialAds_.erase(placementId);
    return true;
}

bool Self::isRewardedVideoReady(const std::string& placementId) const {
    auto response = bridge_.call(k__isRewardedVideoReady, placementId);
    return core::toBool(response);
}

bool Self::showRewardedVideo(const std::string& placementId) {
    if (not isRewardedVideoReady(placementId)) {
        return false;
    }
    Logger::getSystemLogger().debug("%s: placementId = %s", __PRETTY_FUNCTION__,
                                    placementId.c_str());
    errored_ = false;
    displayed_ = false;
    bridge_.call(k__showRewardedVideo, placementId);
    if (not errored_) {
        displayed_ = true;
        return true;
    }
    return false;
}

void Self::onError(const std::string& placementId) {
    Logger::getSystemLogger().debug("%s: placementId = %s", __PRETTY_FUNCTION__,
                                    placementId.c_str());
    if (not errored_ && not displayed_) {
        errored_ = true;
        return;
    }
    onSkipped(placementId);
}

void Self::onSkipped(const std::string& placementId) {
    Logger::getSystemLogger().debug("%s: placementId = %s", __PRETTY_FUNCTION__,
                                    placementId.c_str());
    finish(placementId, false);
}

void Self::onFinished(const std::string& placementId) {
    Logger::getSystemLogger().debug("%s: placementId = %s", __PRETTY_FUNCTION__,
                                    placementId.c_str());
    finish(placementId, true);
}

void Self::finish(const std::string& placementId, bool result) {
    auto&& mediation = ads::MediationManager::getInstance();

    /*
     Don't care which ad is displaying.
    if (rewardedVideos_.count(placementId)) {
        auto ad = rewardedVideos_.at(placementId);
        ad->setResult(result);
        auto successful = mediation.finishRewardedVideo(ad);
        assert(successful);
        return;
    }
    if (interstitialAds_.count(placementId)) {
        auto ad = interstitialAds_.at(placementId);
        ad->setDone();
        auto successful = mediation.finishInterstitialAd(ad);
        assert(successful);
        return;
    }
     */

    // Other mediation network.
    // Not sure interstitial ad or rewarded video so check both.
    auto wasInterstitialAd = mediation.setInterstitialAdDone();
    auto wasRewardedVideo = mediation.finishRewardedVideo(result);

    assert(wasInterstitialAd || wasRewardedVideo);
}
} // namespace unityads
} // namespace ee
