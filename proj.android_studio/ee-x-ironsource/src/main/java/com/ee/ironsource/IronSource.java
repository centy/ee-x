package com.ee.ironsource;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.support.annotation.NonNull;

import com.ee.core.Logger;
import com.ee.core.PluginProtocol;
import com.ee.core.internal.MessageBridge;
import com.ee.core.internal.MessageHandler;
import com.ee.core.internal.Utils;
import com.ironsource.mediationsdk.logger.IronSourceError;
import com.ironsource.mediationsdk.model.Placement;
import com.ironsource.mediationsdk.sdk.RewardedVideoListener;

/**
 * Created by Pham Xuan Han on 17/05/17.
 */
public class IronSource implements PluginProtocol {
    private static final String k__initialize        = "IronSource_initialize";
    private static final String k__hasRewardedVideo  = "IronSource_hasRewardedVideo";
    private static final String k__showRewardedVideo = "IronSource_showRewardedVideo";
    private static final String k__onRewarded        = "IronSource_onRewarded";
    private static final String k__onFailed          = "IronSource_onFailed";
    private static final String k__onOpened          = "IronSource_onOpened";
    private static final String k__onClosed          = "IronSource_onClosed";

    private static final Logger _logger = new Logger(IronSource.class.getName());

    private Activity _context;
    private boolean  _initialized;

    public IronSource(Context context) {
        Utils.checkMainThread();
        _logger.debug("constructor begin: context = " + context);
        _context = (Activity) context;
        _initialized = false;
        registerHandlers();
        _logger.debug("constructor end.");
    }

    @NonNull
    @Override
    public String getPluginName() {
        return "IronSource";
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
        Utils.checkMainThread();
        deregisterHandlers();
        destroy();
    }

    @Override
    public boolean onActivityResult(int requestCode, int responseCode, Intent data) {
        return false;
    }

    @Override
    public boolean onBackPressed() {
        return false;
    }

    private void registerHandlers() {
        MessageBridge bridge = MessageBridge.getInstance();

        bridge.registerHandler(new MessageHandler() {
            @SuppressWarnings("UnnecessaryLocalVariable")
            @NonNull
            @Override
            public String handle(@NonNull String message) {
                String gameId = message;
                initialize(gameId);
                return "";
            }
        }, k__initialize);

        bridge.registerHandler(new MessageHandler() {
            @SuppressWarnings("UnnecessaryLocalVariable")
            @NonNull
            @Override
            public String handle(@NonNull String message) {
                return Utils.toString(hasRewardedVideo());
            }
        }, k__hasRewardedVideo);

        bridge.registerHandler(new MessageHandler() {
            @SuppressWarnings("UnnecessaryLocalVariable")
            @NonNull
            @Override
            public String handle(@NonNull String message) {
                String placementId = message;
                showRewardedVideo(placementId);
                return "";
            }
        }, k__showRewardedVideo);
    }

    private void deregisterHandlers() {
        MessageBridge bridge = MessageBridge.getInstance();

        bridge.deregisterHandler(k__initialize);
        bridge.deregisterHandler(k__hasRewardedVideo);
        bridge.deregisterHandler(k__showRewardedVideo);
    }

    @SuppressWarnings("WeakerAccess")
    public void initialize(@NonNull String gameId) {
        Utils.checkMainThread();
        if (_initialized) {
            return;
        }
        com.ironsource.mediationsdk.IronSource.init(_context, gameId,
            com.ironsource.mediationsdk.IronSource.AD_UNIT.REWARDED_VIDEO);
        com.ironsource.mediationsdk.IronSource.setRewardedVideoListener(
            new RewardedVideoListener() {
                @Override
                public void onRewardedVideoAvailabilityChanged(boolean available) {
                    _logger.info("onRewardedVideoAvailabilityChanged: " + available);
                    Utils.checkMainThread();
                }

                @Override
                public void onRewardedVideoAdRewarded(Placement placement) {
                    _logger.debug("onRewardedVideoAdRewarded: " + placement.getPlacementName());
                    Utils.checkMainThread();

                    MessageBridge bridge = MessageBridge.getInstance();
                    bridge.callCpp(k__onRewarded, placement.getPlacementName());
                }

                @Override
                public void onRewardedVideoAdShowFailed(IronSourceError ironSourceError) {
                    _logger.debug(
                        "onRewardedVideoAdShowFailed: " + ironSourceError.getErrorMessage());
                    Utils.checkMainThread();

                    MessageBridge bridge = MessageBridge.getInstance();
                    bridge.callCpp(k__onFailed);
                }

                @Override
                public void onRewardedVideoAdOpened() {
                    _logger.debug("onRewardedVideoAdOpened");
                    Utils.checkMainThread();

                    MessageBridge bridge = MessageBridge.getInstance();
                    bridge.callCpp(k__onOpened);
                }

                @Override
                public void onRewardedVideoAdClosed() {
                    _logger.debug("onRewardedVideoAdClosed");
                    Utils.checkMainThread();

                    MessageBridge bridge = MessageBridge.getInstance();
                    bridge.callCpp(k__onClosed);
                }

                @Override
                public void onRewardedVideoAdStarted() {
                    _logger.debug("onRewardedVideoAdStarted");
                    Utils.checkMainThread();
                }

                @Override
                public void onRewardedVideoAdEnded() {
                    _logger.debug("onRewardedVideoAdEnded");
                    Utils.checkMainThread();
                }
            });
        _initialized = true;
    }

    private void destroy() {
        Utils.checkMainThread();
        if (!_initialized) {
            return;
        }
        com.ironsource.mediationsdk.IronSource.setRewardedVideoListener(null);
    }

    @SuppressWarnings("WeakerAccess")
    public boolean hasRewardedVideo() {
        Utils.checkMainThread();
        return com.ironsource.mediationsdk.IronSource.isRewardedVideoAvailable();
    }

    @SuppressWarnings("WeakerAccess")
    public void showRewardedVideo(@NonNull String placementId) {
        Utils.checkMainThread();
        com.ironsource.mediationsdk.IronSource.showRewardedVideo(placementId);
    }
}
