package com.ee.unityads;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;

import androidx.annotation.NonNull;

import com.ee.core.IMessageBridge;
import com.ee.core.Logger;
import com.ee.core.IPlugin;
import com.ee.core.internal.JsonUtils;
import com.ee.core.internal.Utils;
import com.unity3d.ads.IUnityAdsListener;
import com.unity3d.ads.UnityAds.FinishState;
import com.unity3d.ads.UnityAds.UnityAdsError;

import java.util.HashMap;
import java.util.Map;
import java.util.Objects;

import static com.google.common.truth.Truth.assertThat;

public class UnityAds implements IPlugin {
    private static final String kPrefix = "UnityAds";

    private static final String k__initialize = kPrefix + "_initialize";
    private static final String k__setDebugModeEnabled = kPrefix + "_setDebugModeEnabled";

    private static final String k__hasRewardedAd = kPrefix + "_hasRewardedAd";
    private static final String k__showRewardedAd = kPrefix + "_showRewardedAd";

    private static final String k__onLoaded = kPrefix + "_onLoaded";
    private static final String k__onFailedToShow = kPrefix + "_onFailedToShow";
    private static final String k__onClosed = kPrefix + "_onClosed";

    private static final String k__gameId = "gameId";
    private static final String k__testModeEnabled = "testModeEnabled";

    private static final Logger _logger = new Logger(UnityAds.class.getName());

    private Activity _activity;
    private IMessageBridge _bridge;
    private boolean _initialized;
    private IUnityAdsListener _listener;

    @SuppressWarnings("unused")
    public UnityAds(@NonNull Context context, @NonNull IMessageBridge bridge) {
        Utils.checkMainThread();
        _logger.debug("constructor begin.");
        _bridge = bridge;
        registerHandlers();
        _logger.debug("constructor end.");
    }

    @NonNull
    @Override
    public String getPluginName() {
        return "UnityAds";
    }

    @Override
    public void onCreate(@NonNull Activity activity) {
        _activity = activity;
    }

    @Override
    public void onStart() {
    }

    @Override
    public void onStop() {
    }

    @Override
    public void onResume() {
    }

    @Override
    public void onPause() {
    }

    @Override
    public void onDestroy() {
        _activity = null;
    }

    @Override
    public void destroy() {
        Utils.checkMainThread();
        deregisterHandlers();
        _bridge = null;
        if (!_initialized) {
            return;
        }
        com.unity3d.ads.UnityAds.removeListener(_listener);
    }

    @Override
    public boolean onActivityResult(int requestCode, int responseCode, Intent data) {
        return false;
    }

    private void registerHandlers() {
        _bridge.registerHandler(k__initialize, message -> {
            Map<String, Object> dict = JsonUtils.convertStringToDictionary(message);
            assertThat(dict).isNotNull();

            String gameId = (String) dict.get(k__gameId);
            Boolean testModeEnabled = (Boolean) dict.get(k__testModeEnabled);
            assertThat(gameId).isNotNull();
            assertThat(testModeEnabled).isNotNull();

            assertThat(_activity).isNotNull();
            initialize(_activity, gameId, testModeEnabled);
            return "";
        });

        _bridge.registerHandler(k__setDebugModeEnabled, message -> {
            setDebugModeEnabled(Utils.toBoolean(message));
            return "";
        });

        _bridge.registerHandler(k__hasRewardedAd, message -> {
            @SuppressWarnings("UnnecessaryLocalVariable")
            String adId = message;
            return Utils.toString(hasRewardedAd(adId));
        });

        _bridge.registerHandler(k__showRewardedAd, message -> {
            assertThat(_activity).isNotNull();
            @SuppressWarnings("UnnecessaryLocalVariable")
            String adId = message;
            showRewardedAd(_activity, adId);
            return "";
        });
    }

    private void deregisterHandlers() {
        _bridge.deregisterHandler(k__initialize);
        _bridge.deregisterHandler(k__setDebugModeEnabled);
        _bridge.deregisterHandler(k__hasRewardedAd);
        _bridge.deregisterHandler(k__showRewardedAd);
    }

    @SuppressWarnings("WeakerAccess")
    public void initialize(@NonNull Activity activity, @NonNull String gameId, boolean testModeEnabled) {
        Utils.checkMainThread();
        if (_initialized) {
            return;
        }
        if (!com.unity3d.ads.UnityAds.isSupported()) {
            return;
        }
        _listener = new IUnityAdsListener() {
            public void onUnityAdsReady(String adId) {
                _logger.info("onUnityAdsReady: " + adId);
                Utils.checkMainThread();
                _bridge.callCpp(k__onLoaded, adId);
            }

            @Override
            public void onUnityAdsStart(String adId) {
                _logger.info("onUnityAdsStart: " + adId);
                Utils.checkMainThread();
            }

            @Override
            public void onUnityAdsFinish(String adId, FinishState state) {
                _logger.info("onUnityAdsFinish: " + adId + " state = " + state);
                Utils.checkMainThread();
                if (state == FinishState.ERROR) {
                    Map<String, Object> dict = new HashMap<>();
                    dict.put("ad_id", adId);
                    dict.put("message", "");
                    _bridge.callCpp(k__onFailedToShow, Objects.requireNonNull(JsonUtils.convertDictionaryToString(dict)));
                    return;
                }
                if (state == FinishState.SKIPPED) {
                    Map<String, Object> dict = new HashMap<>();
                    dict.put("ad_id", adId);
                    dict.put("rewarded", false);
                    _bridge.callCpp(k__onClosed, Objects.requireNonNull(JsonUtils.convertDictionaryToString(dict)));
                    return;
                }
                if (state == FinishState.COMPLETED) {
                    Map<String, Object> dict = new HashMap<>();
                    dict.put("ad_id", adId);
                    dict.put("rewarded", true);
                    _bridge.callCpp(k__onClosed, Objects.requireNonNull(JsonUtils.convertDictionaryToString(dict)));
                    return;
                }
                assertThat(false).isTrue();
            }

            @Override
            public void onUnityAdsError(UnityAdsError unityAdsError, String s) {
                _logger.info("onUnityAdsError: " + s + " error = " + unityAdsError);
                Utils.checkMainThread();
            }
        };
        com.unity3d.ads.UnityAds.initialize(activity, gameId, testModeEnabled);
        com.unity3d.ads.UnityAds.addListener(_listener);
        _initialized = true;
    }

    @SuppressWarnings({"unused", "WeakerAccess"})
    public void setDebugModeEnabled(boolean enabled) {
        Utils.checkMainThread();
        if (!_initialized) {
            return;
        }
        com.unity3d.ads.UnityAds.setDebugMode(enabled);
    }

    @SuppressWarnings("WeakerAccess")
    public boolean hasRewardedAd(@NonNull String adId) {
        Utils.checkMainThread();
        if (!_initialized) {
            return false;
        }
        return com.unity3d.ads.UnityAds.isReady(adId);
    }

    @SuppressWarnings("WeakerAccess")
    public void showRewardedAd(@NonNull Activity activity, @NonNull String adId) {
        Utils.checkMainThread();
        if (!_initialized) {
            // FIXME: handle error.
            return;
        }
        com.unity3d.ads.UnityAds.show(activity, adId);
    }
}