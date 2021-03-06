//
//  AdMobInterstitialAd.hpp
//  ee_x
//
//  Created by Zinge on 10/12/17.
//
//

#ifndef EE_X_ADMOB_INTERSTITIAL_AD_HPP
#define EE_X_ADMOB_INTERSTITIAL_AD_HPP

#include <string>

#include <ee/ads/IInterstitialAd.hpp>
#include <ee/ads/internal/MessageHelper.hpp>
#include <ee/core/SafeObserverManager.hpp>

#include "ee/AdMobFwd.hpp"

namespace ee {
namespace admob {
class InterstitialAd final
    : public IInterstitialAd,
      public SafeObserverManager<IInterstitialAdObserver> {
public:
    virtual ~InterstitialAd() override;

    virtual void destroy() override;

    virtual bool isLoaded() const override;
    virtual Task<bool> load() override;
    virtual Task<bool> show() override;

private:
    friend Bridge;

    explicit InterstitialAd(
        IMessageBridge& bridge, const Logger& logger,
        const std::shared_ptr<ads::IAsyncHelper<bool>>& displayer,
        Bridge* plugin, const std::string& adId);

    bool createInternalAd();
    bool destroyInternalAd();

    void onLoaded();
    void onFailedToLoad(const std::string& message);
    void onFailedToShow(const std::string& message);
    void onClicked();
    void onClosed();

    IMessageBridge& bridge_;
    const Logger& logger_;
    std::shared_ptr<ads::IAsyncHelper<bool>> displayer_;
    Bridge* plugin_;
    std::string adId_;
    ads::MessageHelper messageHelper_;

    std::unique_ptr<ads::IAsyncHelper<bool>> loader_;
};
} // namespace admob
} // namespace ee

#endif /* EE_X_ADMOB_INTERSTITIAL_AD_HPP */
