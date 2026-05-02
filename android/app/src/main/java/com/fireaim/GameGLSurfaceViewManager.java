package com.fireaim;

import android.view.View;

import androidx.annotation.NonNull;

import com.facebook.react.bridge.Arguments;
import com.facebook.react.bridge.ReactContext;
import com.facebook.react.bridge.WritableMap;
import com.facebook.react.uimanager.events.RCTEventEmitter;

public class GameGLSurfaceViewManager extends com.facebook.react.uimanager.ViewManager<View> {

    @NonNull
    @Override
    public String getName() {
        return "GameGLSurfaceView";
    }

    @NonNull
    @Override
    protected View createViewInstance(@NonNull ReactContext reactContext) {
        return new GameGLSurfaceView(reactContext);
    }

    @Override
    public void receiveCommand(@NonNull View view, String commandId, Object[] args) {
        GameGLSurfaceView glView = (GameGLSurfaceView) view;
        
        if ("shoot".equals(commandId)) {
            glView.getRenderer().shoot();
        } else if ("reset".equals(commandId)) {
            glView.getRenderer().resetGame();
        }
    }

    @Override
    public Map<String,Object> getExportedCustomDirectEventTypeConstants() {
        return Map.of(
            "onScoreChange", Map.of("registrationName", "onScoreChange"),
            "onWaveChange", Map.of("registrationName", "onWaveChange"),
            "onGameOver", Map.of("registrationName", "onGameOver")
        );
    }
}