//
//  UnityRewardedVideo.cpp
//  ee_x
//
//  Created by Zinge on 10/10/17.
//
//

#include "ee/unity_ads/private/UnityRewardedVideo.hpp"

#include <cassert>

#include <ee/ads/internal/MediationManager.hpp>
#include <ee/core/Logger.hpp>

#include "ee/unity_ads/UnityAdsBridge.hpp"

namespace ee {
namespace unity_ads {
using Self = RewardedVideo;

Self::RewardedVideo(const Logger& logger, Bridge* plugin,
                    const std::string& placementId)
    : Super(logger)
    , logger_(logger) {
    logger_.debug("%s: placementId = %s", __PRETTY_FUNCTION__,
                  placementId.c_str());
    plugin_ = plugin;
    placementId_ = placementId;
}

Self::~RewardedVideo() {
    logger_.debug("%s: begin", __PRETTY_FUNCTION__);
    plugin_->destroyRewardedVideo(placementId_);
    logger_.debug("%s: end", __PRETTY_FUNCTION__);
}

bool Self::isLoaded() const {
    return plugin_->isRewardedVideoReady(placementId_);
}

void Self::load() {
    // No op.
}

bool Self::show() {
    if (not plugin_->showRewardedVideo(placementId_)) {
        return false;
    }
    auto&& mediation = ads::MediationManager::getInstance();
    auto successful = mediation.startRewardedVideo([this](bool rewarded) { //
        this->setResult(rewarded);
    });
    assert(successful);
    return true;
}
} // namespace unity_ads
} // namespace ee