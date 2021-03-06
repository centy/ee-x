#ifndef EE_X_VUNGLE_BRIDGE_HPP
#define EE_X_VUNGLE_BRIDGE_HPP

#ifdef __cplusplus

#include <map>

#include "ee/VungleFwd.hpp"

namespace ee {
namespace vungle {
class Bridge final {
public:
    Bridge();
    ~Bridge();

    explicit Bridge(const Logger& logger);

    void destroy();

    /// Initializes Vungle with the specified game ID.
    void initialize(const std::string& gameId);

    [[deprecated]] void initialize(const std::string& gameId,
                                   const std::string& adId);

    /// Creates a rewarded video.
    std::shared_ptr<IRewardedAd> createRewardedAd(const std::string& adId);

private:
    friend RewardedAd;

    bool destroyRewardedAd(const std::string& adId);

    bool hasRewardedAd(const std::string& adId) const;
    void loadRewardedAd(const std::string& adId) const;
    void showRewardedAd(const std::string& adId);

    void onLoaded(const std::string& adId);
    void onFailedToLoad(const std::string& adId, const std::string& message);
    void onFailedToShow(const std::string& adId, const std::string& message);
    void onClosed(const std::string& adId, bool rewarded);

    void onMediationAdClosed(const std::string& adId, bool rewarded);

    IMessageBridge& bridge_;
    const Logger& logger_;

    template <class Strong, class Weak>
    struct Ad {
        std::shared_ptr<Strong> strong;
        Weak* weak;

        explicit Ad(const std::shared_ptr<Strong>& strong_, Weak* weak_)
            : strong(strong_)
            , weak(weak_) {}
    };

    std::map<std::string, Ad<IRewardedAd, RewardedAd>> rewardedAds_;

    std::shared_ptr<ads::IAsyncHelper<IRewardedAdResult>> rewardedAdDisplayer_;
};
} // namespace vungle
} // namespace ee

#endif // __cplusplus

#endif /* EE_X_VUNGLE_BRIDGE_HPP */
