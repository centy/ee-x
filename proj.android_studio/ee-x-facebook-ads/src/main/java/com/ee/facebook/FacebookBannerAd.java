package com.ee.facebook;

import android.app.Activity;
import android.content.Context;
import android.content.res.Resources;
import android.graphics.Color;
import android.graphics.Point;
import android.view.Gravity;
import android.widget.FrameLayout;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;

import com.ee.ads.AdViewHelper;
import com.ee.ads.IAdView;
import com.ee.ads.ViewHelper;
import com.ee.core.IMessageBridge;
import com.ee.core.Logger;
import com.ee.core.MessageBridge;
import com.ee.core.internal.Utils;
import com.facebook.ads.Ad;
import com.facebook.ads.AdError;
import com.facebook.ads.AdListener;
import com.facebook.ads.AdSize;
import com.facebook.ads.AdView;

/**
 * Created by Zinge on 10/9/17.
 */

class FacebookBannerAd implements AdListener, IAdView {
    private static final Logger _logger = new Logger(FacebookBannerAd.class.getName());

    private Context _context;
    private Activity _activity;
    private IMessageBridge _bridge;
    private AdView _adView;
    private boolean _isAdLoaded;
    private String _adId;
    private AdSize _adSize;
    private AdViewHelper _helper;
    private ViewHelper _viewHelper;
    private boolean _customSize;

    static AdSize adSizeFor(int index) {
        if (index == 0) {
            return AdSize.BANNER_HEIGHT_50;
        }
        if (index == 1) {
            return AdSize.BANNER_HEIGHT_90;
        }
        if (index == 2) {
            return AdSize.INTERSTITIAL;
        }
        if (index == 3) {
            return AdSize.RECTANGLE_HEIGHT_250;
        }
        return AdSize.BANNER_320_50;
    }

    FacebookBannerAd(@NonNull Context context, @Nullable Activity activity, @NonNull String adId, @NonNull AdSize adSize) {
        Utils.checkMainThread();
        _context = context;
        _activity = activity;
        _bridge = MessageBridge.getInstance();
        _adId = adId;
        _adSize = adSize;
        _helper = new AdViewHelper(_bridge, this, "FacebookBannerAd", _adId);

        createInternalAd();
        registerHandlers();
    }

    void onCreate(@NonNull Activity activity) {
        _activity = activity;
        addToActivity(activity);
    }

    void onDestroy(@NonNull Activity activity) {
        assert _activity == activity;
        removeFromActivity(activity);
        _activity = null;
    }

    void destroy() {
        Utils.checkMainThread();
        deregisterHandlers();
        destroyInternalAd();
        _context = null;
        _bridge = null;
        _adId = null;
        _adSize = null;
        _helper = null;
    }

    @NonNull
    private String kOnLoaded() {
        return "FacebookBannerAd_onLoaded_" + _adId;
    }

    @NonNull
    private String kOnFailedToLoad() {
        return "FacebookBannerAd_onFailedToLoad_" + _adId;
    }

    @NonNull
    private String kOnClicked() {
        return "FacebookBannerAd_onClicked_" + _adId;
    }

    private void registerHandlers() {
        Utils.checkMainThread();
        _helper.registerHandlers();
    }

    private void deregisterHandlers() {
        Utils.checkMainThread();
        _helper.deregisterHandlers();
    }

    private boolean createInternalAd() {
        Utils.checkMainThread();
        if (_adView != null) {
            return false;
        }
        _customSize = false;
        _isAdLoaded = false;
        AdView adView = new AdView(_context, _adId, _adSize);
        adView.setAdListener(this);
        _adView = adView;

        FrameLayout.LayoutParams params = new FrameLayout.LayoutParams(FrameLayout.LayoutParams.WRAP_CONTENT, FrameLayout.LayoutParams.WRAP_CONTENT);
        params.gravity = Gravity.START | Gravity.TOP;
        _adView.setLayoutParams(params);
        _viewHelper = new ViewHelper(_adView);

        if (_activity != null) {
            addToActivity(_activity);
        }
        return true;
    }

    private boolean destroyInternalAd() {
        Utils.checkMainThread();
        if (_adView == null) {
            return false;
        }
        _customSize = false;
        _isAdLoaded = false;
        if (_activity != null) {
            removeFromActivity(_activity);
        }
        _viewHelper = null;
        _adView.destroy();
        _adView = null;
        return true;
    }

    private void addToActivity(@NonNull Activity activity) {
        FrameLayout rootView = Utils.getRootView(activity);
        rootView.addView(_adView);
    }

    private void removeFromActivity(@NonNull Activity activity) {
        FrameLayout rootView = Utils.getRootView(activity);
        rootView.removeView(_adView);
    }

    @Override
    public boolean isLoaded() {
        Utils.checkMainThread();
        return _adView != null && _isAdLoaded;
    }

    @Override
    public void load() {
        Utils.checkMainThread();
        if (_adView == null) {
            return;
        }
        _adView.loadAd();
    }

    @NonNull
    @Override
    public Point getPosition() {
        return _viewHelper.getPosition();
    }

    @Override
    public void setPosition(@NonNull Point position) {
        _viewHelper.setPosition(position);
    }

    @NonNull
    @Override
    public Point getSize() {
        if (_customSize) {
            return _viewHelper.getSize();
        }
        int width = getWidthInPixels(_adSize);
        int height = getHeightInPixels(_adSize);
        return new Point(width, height);
    }

    @Override
    public void setSize(@NonNull Point size) {
        _viewHelper.setSize(size);
        _customSize = true;
    }

    @Override
    public boolean isVisible() {
        return _viewHelper.isVisible();
    }

    @Override
    public void setVisible(boolean visible) {
        _viewHelper.setVisible(visible);
        if (visible) {
            _adView.setBackgroundColor(Color.BLACK);
        }
    }

    @Override
    public void onError(Ad ad, AdError adError) {
        _logger.info("onError: " + adError.getErrorMessage());
        Utils.checkMainThread();
        _bridge.callCpp(kOnFailedToLoad(), adError.getErrorMessage());
    }

    @Override
    public void onAdLoaded(Ad ad) {
        _logger.info("onAdLoaded");
        Utils.checkMainThread();
        _isAdLoaded = true;
        _bridge.callCpp(kOnLoaded());
    }

    @Override
    public void onAdClicked(Ad ad) {
        _logger.info("onAdClicked");
        Utils.checkMainThread();
        _bridge.callCpp(kOnClicked());
    }

    @Override
    public void onLoggingImpression(Ad ad) {
        _logger.info("onLoggingImpression");
        Utils.checkMainThread();
    }

    private static int getWidthInPixels(AdSize size) {
        switch (size.getWidth()) {
            case 0: // Interstitial.
            case -1: // Normal ads.
                return Resources.getSystem().getDisplayMetrics().widthPixels;
            default: // Deprecated ads.
                return (int) Utils.convertDpToPixel((double) size.getWidth());
        }
    }

    private static int getHeightInPixels(AdSize size) {
        switch (size.getHeight()) {
            case 0: // Interstitial.
                return Resources.getSystem().getDisplayMetrics().heightPixels;
            default: // Normal ads.
                return (int) Utils.convertDpToPixel((double) size.getHeight());
        }
    }
}
