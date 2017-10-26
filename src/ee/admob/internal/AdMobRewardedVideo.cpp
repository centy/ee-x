//
//  AdMobRewardedVideo.cpp
//  ee_x
//
//  Created by Zinge on 10/13/17.
//
//

#include <cassert>

#include "AdMobRewardedVideo.hpp"
#include "ee/admob/AdMobBridge.hpp"
#include "ee/ads/internal/MediationManager.hpp"

namespace ee {
namespace admob {
using Self = RewardedVideo;

Self::RewardedVideo(AdMob* plugin, const std::string& adId) {
    plugin_ = plugin;
    adId_ = adId;
}

Self::~RewardedVideo() {
    plugin_->destroyRewardedVideo(adId_);
}

bool Self::isLoaded() const {
    return adId_ == plugin_->currentId_ && plugin_->hasRewardedVideo();
}

void Self::load() {
    plugin_->loadRewardedVideo(adId_);
}

bool Self::show() {
    if (adId_ != plugin_->currentId_ || not plugin_->showRewardedVideo()) {
        return false;
    }
    auto&& mediation = ads::MediationManager::getInstance();
    auto successful = mediation.registerRewardedVideo(this);
    assert(successful);
    return true;
}
} // namespace admob
} // namespace ee
