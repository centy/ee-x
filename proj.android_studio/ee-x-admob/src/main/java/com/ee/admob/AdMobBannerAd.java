package com.ee.admob;

import android.app.Activity;
import android.content.Context;
import android.graphics.Color;
import android.graphics.Point;
import android.view.Gravity;
import android.widget.FrameLayout;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;

import com.ee.ads.AdViewHelper;
import com.ee.ads.IAdView;
import com.ee.ads.MessageHelper;
import com.ee.ads.ViewHelper;
import com.ee.core.IMessageBridge;
import com.ee.core.Logger;
import com.ee.core.internal.Utils;
import com.google.android.gms.ads.AdListener;
import com.google.android.gms.ads.AdRequest;
import com.google.android.gms.ads.AdSize;
import com.google.android.gms.ads.AdView;

import static com.google.common.truth.Truth.assertThat;

/**
 * Created by Zinge on 10/13/17.
 */

class AdMobBannerAd extends AdListener implements IAdView {
    private static final Logger _logger = new Logger(AdMobBannerAd.class.getName());

    private Context _context;
    private Activity _activity;
    private IMessageBridge _bridge;
    private String _adId;
    private AdSize _adSize;
    private AdViewHelper _helper;
    private MessageHelper _messageHelper;
    private ViewHelper _viewHelper;
    private boolean _isLoaded;
    private AdView _ad;

    AdMobBannerAd(@NonNull Context context,
                  @Nullable Activity activity,
                  @NonNull IMessageBridge bridge,
                  @NonNull String adId,
                  @NonNull AdSize adSize) {
        _logger.info("constructor: adId = %s", adId);
        Utils.checkMainThread();
        _context = context;
        _activity = activity;
        _bridge = bridge;
        _adId = adId;
        _adSize = adSize;
        _messageHelper = new MessageHelper("AdMobBannerAd", adId);
        _helper = new AdViewHelper(_bridge, this, _messageHelper);

        createInternalAd();
        registerHandlers();
    }

    void onCreate(@NonNull Activity activity) {
        _activity = activity;
        addToActivity(activity);
    }

    void onResume() {
        Utils.checkMainThread();
        _ad.resume();
    }

    void onPause() {
        Utils.checkMainThread();
        _ad.pause();
    }

    void onDestroy(@NonNull Activity activity) {
        assertThat(_activity).isEqualTo(activity);
        removeFromActivity(activity);
        _activity = null;
    }

    void destroy() {
        _logger.info("destroy: adId = " + _adId);
        Utils.checkMainThread();
        deregisterHandlers();
        destroyInternalAd();
        _context = null;
        _bridge = null;
        _adId = null;
        _adSize = null;
        _messageHelper = null;
        _helper = null;
    }

    private void registerHandlers() {
        _helper.registerHandlers();
    }

    private void deregisterHandlers() {
        _helper.deregisterHandlers();
    }

    private boolean createInternalAd() {
        Utils.checkMainThread();
        if (_ad != null) {
            return false;
        }
        _isLoaded = false;

        _ad = new AdView(_context);
        _ad.setAdSize(_adSize);
        _ad.setAdListener(this);
        _ad.setAdUnitId(_adId);

        FrameLayout.LayoutParams params = new FrameLayout.LayoutParams(
            FrameLayout.LayoutParams.WRAP_CONTENT,
            FrameLayout.LayoutParams.WRAP_CONTENT);
        params.gravity = Gravity.START | Gravity.TOP;
        _ad.setLayoutParams(params);
        _viewHelper = new ViewHelper(_ad);

        if (_activity != null) {
            addToActivity(_activity);
        }
        return true;
    }

    private boolean destroyInternalAd() {
        Utils.checkMainThread();
        if (_ad == null) {
            return false;
        }
        _isLoaded = false;
        if (_activity != null) {
            removeFromActivity(_activity);
        }
        _viewHelper = null;
        _ad.destroy();
        _ad = null;
        return true;
    }

    private void addToActivity(@NonNull Activity activity) {
        FrameLayout rootView = Utils.getRootView(activity);
        rootView.addView(_ad);
    }

    private void removeFromActivity(@NonNull Activity activity) {
        FrameLayout rootView = Utils.getRootView(activity);
        rootView.removeView(_ad);
    }

    @Override
    public boolean isLoaded() {
        Utils.checkMainThread();
        assertThat(_adId).isNotNull();
        return _isLoaded;
    }

    @Override
    public void load() {
        Utils.checkMainThread();
        assertThat(_adId).isNotNull();
        _logger.info("load");
        AdRequest.Builder builder = new AdRequest.Builder();
        _ad.loadAd(builder.build());
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
        return _viewHelper.getSize();
    }

    @Override
    public void setSize(@NonNull Point size) {
        _viewHelper.setSize(size);
    }

    @Override
    public boolean isVisible() {
        return _viewHelper.isVisible();
    }

    @Override
    public void setVisible(boolean visible) {
        _viewHelper.setVisible(visible);
        if (visible) {
            // https://stackoverflow.com/questions/21408178/admob-wont-show-the-banner-until
            // -refresh-or-sign-in-to-google-plus
            _ad.setBackgroundColor(Color.BLACK);
        }
    }

    @Override
    public void onAdClosed() {
        _logger.info("onAdClosed");
        Utils.checkMainThread();
    }

    @Override
    public void onAdFailedToLoad(int errorCode) {
        _logger.info("onAdFailedToLoad: code = " + errorCode);
        Utils.checkMainThread();
        _bridge.callCpp(_messageHelper.onFailedToLoad(), String.valueOf(errorCode));
    }

    @Override
    public void onAdLeftApplication() {
        _logger.info("onAdLeftApplication");
        Utils.checkMainThread();
        _bridge.callCpp(_messageHelper.onClicked());
    }

    @Override
    public void onAdOpened() {
        _logger.info("onAdOpened");
        Utils.checkMainThread();
    }

    @Override
    public void onAdLoaded() {
        _logger.info("onAdLoaded");
        Utils.checkMainThread();
        _isLoaded = true;
        _bridge.callCpp(_messageHelper.onLoaded());
    }

    @Override
    public void onAdClicked() {
        _logger.info("onAdClicked");
        Utils.checkMainThread();
    }

    @Override
    public void onAdImpression() {
        _logger.info("onAdImpression");
        Utils.checkMainThread();
    }
}
