```java
package com.mymenu.app;

import android.content.Context;
import android.opengl.GLSurfaceView;
import android.view.MotionEvent;
import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

public class MenuView extends GLSurfaceView implements GLSurfaceView.Renderer {

    static { System.loadLibrary("native-lib"); }

    public MenuView(Context context) {
        super(context);
        setEGLContextClientVersion(3);
        setEGLConfigChooser(8, 8, 8, 8, 16, 0);
        getHolder().setFormat(android.graphics.PixelFormat.TRANSLUCENT);
        setRenderer(this);
        setZOrderOnTop(true);
        setRenderMode(RENDERMODE_CONTINUOUSLY);
    }

    @Override public void onSurfaceCreated(GL10 gl, EGLConfig config) { NativeInit(); }
    @Override public void onSurfaceChanged(GL10 gl, int w, int h)      { NativeResize(w, h); }
    @Override public void onDrawFrame(GL10 gl)                         { NativeRender(); }

    @Override
    public boolean onTouchEvent(MotionEvent e) {
        NativeTouch(e.getActionMasked(), e.getPointerId(e.getActionIndex()),
                    e.getX(e.getActionIndex()), e.getY(e.getActionIndex()));
        return true;
    }

    // Native methods
    public static native void NativeInit();
    public static native void NativeResize(int w, int h);
    public static native void NativeRender();
    public static native void NativeTouch(int action, int pointerId, float x, float y);
}
```

---
  
