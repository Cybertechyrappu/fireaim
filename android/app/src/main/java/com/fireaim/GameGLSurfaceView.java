package com.fireaim;

import android.content.Context;
import android.opengl.GLSurfaceView;
import android.util.Log;
import android.view.MotionEvent;

public class GameGLSurfaceView extends GLSurfaceView {
    
    private FireaimRenderer renderer;
    private float lastTouchX, lastTouchY;
    
    public GameGLSurfaceView(Context context) {
        super(context);
        
        setEGLContextClientVersion(1);
        renderer = new FireaimRenderer();
        setRenderer(renderer);
        
        setRenderMode(RENDERMODE_CONTINUOUSLY);
    }
    
    @Override
    public boolean onTouchEvent(MotionEvent event) {
        float x = event.getX();
        float y = event.getY();
        
        switch (event.getAction()) {
            case MotionEvent.ACTION_DOWN:
                lastTouchX = x;
                lastTouchY = y;
                break;
                
            case MotionEvent.ACTION_MOVE:
                float dx = x - lastTouchX;
                float dy = y - lastTouchY;
                
                renderer.updateLook(dx * 0.01f, dy * 0.01f);
                
                lastTouchX = x;
                lastTouchY = y;
                break;
                
            case MotionEvent.ACTION_UP:
                long downTime = event.getDownTime();
                long eventTime = event.getEventTime();
                if (eventTime - downTime < 200) {
                    renderer.shoot();
                }
                break;
        }
        
        return true;
    }
    
    public FireaimRenderer getRenderer() {
        return renderer;
    }
}