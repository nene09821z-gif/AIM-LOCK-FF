```java
package com.mymenu.app;

import android.app.Activity;
import android.content.Intent;
import android.net.Uri;
import android.os.Build;
import android.os.Bundle;
import android.provider.Settings;
import android.widget.Button;
import android.widget.LinearLayout;
import android.widget.TextView;
import android.widget.Toast;

public class MainActivity extends Activity {

    private static final int OVERLAY_CODE = 100;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        LinearLayout root = new LinearLayout(this);
        root.setOrientation(LinearLayout.VERTICAL);
        root.setPadding(60, 120, 60, 60);

        TextView title = new TextView(this);
        title.setText("MY MENU\n\nBuoc 1: Cap quyen Overlay\nBuoc 2: Kich hoat Shizuku\nBuoc 3: Mo Menu\nBuoc 4: Mo Free Fire");
        title.setTextSize(16);
        title.setPadding(0, 0, 0, 60);
        root.addView(title);

        Button btnOverlay = new Button(this);
        btnOverlay.setText("1. Cap quyen Overlay");
        btnOverlay.setOnClickListener(v -> requestOverlay());
        root.addView(btnOverlay);

        Button btnShizuku = new Button(this);
        btnShizuku.setText("2. Yeu cau quyen Shizuku");
        btnShizuku.setOnClickListener(v -> {
            if (!ShizukuHelper.isAvailable()) {
                Toast.makeText(this, "Shizuku chua chay!", Toast.LENGTH_LONG).show();
                return;
            }
            ShizukuHelper.request();
        });
        root.addView(btnShizuku);

        Button btnStart = new Button(this);
        btnStart.setText("3. BAT MENU");
        btnStart.setOnClickListener(v -> {
            if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M
                    && !Settings.canDrawOverlays(this)) {
                Toast.makeText(this, "Can cap quyen Overlay!", Toast.LENGTH_LONG).show();
                return;
            }
            Intent i = new Intent(this, MenuView.class);
            startActivity(i);
        });
        root.addView(btnStart);

        setContentView(root);
    }

    private void requestOverlay() {
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M
                && !Settings.canDrawOverlays(this)) {
            Intent i = new Intent(Settings.ACTION_MANAGE_OVERLAY_PERMISSION,
                    Uri.parse("package:" + getPackageName()));
            startActivityForResult(i, OVERLAY_CODE);
        } else {
            Toast.makeText(this, "Da co quyen Overlay", Toast.LENGTH_SHORT).show();
        }
    }
}
```

---
