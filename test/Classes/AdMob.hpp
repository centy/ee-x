//
//  AdMob.hpp
//  ee_x_test
//
//  Created by Zinge on 10/16/17.
//
//

#ifndef EE_X_TEST_ADMOB_HPP
#define EE_X_TEST_ADMOB_HPP

#include <ee/AdMob.hpp>

namespace eetest {
ee::AdMob* getAdMob();

std::string getAdMobApplicationTestId();
std::string getAdMobBannerAdTestId();
std::string getAdMobNativeAdTestId();
std::string getAdMobNativeAdVideoTestId();
std::string getAdMobInterstitialAdTestId();
std::string getAdMobInterstitialAdVideoTestId();
std::string getAdMobRewardedAdTestId();

std::shared_ptr<ee::IAdView> createAdMobNativeAd();

void testAdMobBannerAd();
void testAdMobNativeAd();
void testAdMobInterstitial();
void testAdMobRewardedAd();
} // namespace eetest

#endif /* EE_X_TEST_ADMOB_HPP */
