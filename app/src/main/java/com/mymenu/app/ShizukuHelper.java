```java
package com.mymenu.app;

import android.content.pm.PackageManager;
import android.util.Log;
import rikka.shizuku.Shizuku;

public class ShizukuHelper {
    private static final String TAG = "ShizukuHelper";
    public static final int CODE = 1001;

    public static boolean isAvailable() {
        try { return Shizuku.pingBinder(); }
        catch (Throwable t) { return false; }
    }

    public static boolean hasPermission() {
        if (!isAvailable()) return false;
        try {
            return Shizuku.checkSelfPermission() == PackageManager.PERMISSION_GRANTED;
        } catch (Throwable t) { return false; }
    }

    public static void request() {
        if (!isAvailable()) return;
        try {
            if (Shizuku.shouldShowRequestPermissionRationale()) {
                Log.w(TAG, "User denied before");
            } else {
                Shizuku.requestPermission(CODE);
            }
        } catch (Throwable t) {
            Log.e(TAG, "request failed", t);
        }
    }
}
```

---
      
