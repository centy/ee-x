//
//  AdMobInterstitialAd.cpp
//  ee_x
//
//  Created by Zinge on 10/12/17.
//
//

#include "ee/admob/private/AdMobInterstitialAd.hpp"

#include <cassert>

#include <ee/ads/internal/AsyncHelper.hpp>
#include <ee/core/Logger.hpp>
#include <ee/core/Utils.hpp>
#include <ee/core/internal/IMessageBridge.hpp>

#include "ee/admob/AdMobBridge.hpp"

namespace ee {
namespace admob {
using Self = InterstitialAd;

Self::InterstitialAd(IMessageBridge& bridge, const Logger& logger,
                     const std::shared_ptr<ads::IAsyncHelper<bool>>& displayer,
                     AdMob* plugin, const std::string& adId)
    : bridge_(bridge)
    , logger_(logger)
    , displayer_(displayer)
    , plugin_(plugin)
    , adId_(adId)
    , messageHelper_("AdMobInterstitialAd", adId) {
    logger_.debug("%s", __PRETTY_FUNCTION__);
    loader_ = std::make_unique<ads::AsyncHelper<bool>>();

    bridge_.registerHandler(
        [this](const std::string& message) {
            onLoaded();
            return "";
        },
        messageHelper_.onLoaded());
    bridge_.registerHandler(
        [this](const std::string& message) {
            onFailedToLoad(message);
            return "";
        },
        messageHelper_.onFailedToLoad());
    bridge_.registerHandler(
        [this](const std::string& message) {
            onFailedToShow(message);
            return "";
        },
        messageHelper_.onFailedToShow());
    bridge_.registerHandler(
        [this](const std::string& message) {
            onClicked();
            return "";
        },
        messageHelper_.onClicked());
    bridge_.registerHandler(
        [this](const std::string& message) {
            onClosed();
            return "";
        },
        messageHelper_.onClosed());
}

Self::~InterstitialAd() {
    logger_.debug("%s", __PRETTY_FUNCTION__);

    bridge_.deregisterHandler(messageHelper_.onLoaded());
    bridge_.deregisterHandler(messageHelper_.onFailedToLoad());
    bridge_.deregisterHandler(messageHelper_.onFailedToShow());
    bridge_.deregisterHandler(messageHelper_.onClicked());
    bridge_.deregisterHandler(messageHelper_.onClosed());

    auto succeeded = plugin_->destroyInterstitialAd(adId_);
    assert(succeeded);
}

bool Self::createInternalAd() {
    logger_.debug("%s", __PRETTY_FUNCTION__);
    auto response = bridge_.call(messageHelper_.createInternalAd());
    return core::toBool(response);
}

bool Self::destroyInternalAd() {
    logger_.debug("%s", __PRETTY_FUNCTION__);
    auto response = bridge_.call(messageHelper_.destroyInternalAd());
    return core::toBool(response);
}

bool Self::isLoaded() const {
    auto response = bridge_.call(messageHelper_.isLoaded());
    return core::toBool(response);
}

Task<bool> Self::load() {
    logger_.debug("%s: loading = %s", __PRETTY_FUNCTION__,
                  core::toString(loader_->isProcessing()).c_str());
    auto result = co_await loader_->process(
        [this] { //
            bridge_.call(messageHelper_.load());
        },
        [](bool result) {
            // OK.
        });
    co_return result;
}

Task<bool> Self::show() {
    logger_.debug("%s", __PRETTY_FUNCTION__);
    auto result = co_await displayer_->process(
        [this] { //
            bridge_.call(messageHelper_.show());
        },
        [this](bool result) {
            if (result) {
                destroyInternalAd();
                createInternalAd();
            }
        });
    co_return result;
}

void Self::onLoaded() {
    logger_.debug("%s: loading = %s", __PRETTY_FUNCTION__,
                  core::toString(loader_->isProcessing()).c_str());
    if (loader_->isProcessing()) {
        loader_->resolve(true);
    } else {
        assert(false);
    }
    dispatchEvent([](auto&& observer) {
        if (observer.onLoaded) {
            observer.onLoaded();
        }
    });
}

void Self::onFailedToLoad(const std::string& message) {
    logger_.debug("%s: message = %s loading = %s", __PRETTY_FUNCTION__,
                  message.c_str(),
                  core::toString(loader_->isProcessing()).c_str());
    if (loader_->isProcessing()) {
        loader_->resolve(false);
    } else {
        assert(false);
    }
}

void Self::onFailedToShow(const std::string& message) {
    logger_.debug("%s: message = %s displaying = %s", __PRETTY_FUNCTION__,
                  message.c_str(),
                  core::toString(displayer_->isProcessing()).c_str());
    if (displayer_->isProcessing()) {
        displayer_->resolve(false);
    } else {
        assert(false);
    }
}

void Self::onClicked() {
    logger_.debug("%s", __PRETTY_FUNCTION__);
    dispatchEvent([](auto&& observer) {
        if (observer.onClicked) {
            observer.onClicked();
        }
    });
}

void Self::onClosed() {
    logger_.debug("%s", __PRETTY_FUNCTION__);
    if (displayer_->isProcessing()) {
        displayer_->resolve(true);
    } else {
        assert(false);
    }
}
} // namespace admob
} // namespace ee
