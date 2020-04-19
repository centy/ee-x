#ifndef EE_X_GUARDED_REWARDED_AD_HPP
#define EE_X_GUARDED_REWARDED_AD_HPP

#include <ee/core/ObserverManager.hpp>

#include "ee/ads/IRewardedAd.hpp"

namespace ee {
namespace ads {
class GuardedRewardedAd : public IRewardedAd,
                          public ObserverManager<IRewardedAdObserver> {
public:
    explicit GuardedRewardedAd(const std::shared_ptr<IRewardedAd>& ad);
    virtual ~GuardedRewardedAd() override;

    virtual void destroy() override;

    virtual bool isLoaded() const override;
    virtual Task<bool> load() override;
    virtual Task<IRewardedAdResult> show() override;

private:
    std::shared_ptr<IRewardedAd> ad_;

    bool loading_;
    bool loaded_;
    bool displaying_;
    std::unique_ptr<ObserverHandle> handle_;
};
} // namespace ads
} // namespace ee

#endif // EE_X_GUARDED_REWARDED_AD_HPP
