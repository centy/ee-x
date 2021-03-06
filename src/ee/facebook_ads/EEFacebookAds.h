//
//  EEFacebookAds.h
//  ee_x
//
//  Created by Pham Xuan Han on 5/12/17.
//
//

#import <FBAudienceNetwork/FBAdSize.h>

#import <ee/core/EEIPlugin.h>

@interface EEFacebookAds : NSObject <EEIPlugin>

- (void)destroy;

- (NSString* _Nonnull)getTestDeviceHash;
- (void)addTestDevice:(NSString* _Nonnull)hash;
- (void)clearTestDevices;

- (CGSize)getBannerAdSize:(int)sizeId;
- (BOOL)createBannerAd:(NSString* _Nonnull)adId size:(FBAdSize)size;
- (BOOL)destroyBannerAd:(NSString* _Nonnull)adId;

- (BOOL)createNativeAd:(NSString* _Nonnull)adId
                layout:(NSString* _Nonnull)layout
           identifiers:(NSDictionary* _Nonnull)identifiers;
- (BOOL)destroyNativeAd:(NSString* _Nonnull)adId;

- (BOOL)createInterstitialAd:(NSString* _Nonnull)adId;
- (BOOL)destroyInterstitialAd:(NSString* _Nonnull)adId;

- (BOOL)createRewardedAd:(NSString* _Nonnull)adId;
- (BOOL)destroyRewardedAd:(NSString* _Nonnull)adId;

@end
