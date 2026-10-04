```cpp
#include <jni.h>
#include "Main.h"

extern "C" {

JNIEXPORT void JNICALL
Java_com_mymenu_app_MenuView_NativeInit(JNIEnv*, jclass) {
    MenuInit();
}

JNIEXPORT void JNICALL
Java_com_mymenu_app_MenuView_NativeResize(JNIEnv*, jclass, jint w, jint h) {
    MenuResize(w, h);
}

JNIEXPORT void JNICALL
Java_com_mymenu_app_MenuView_NativeRender(JNIEnv*, jclass) {
    MenuRender();
}

JNIEXPORT void JNICALL
Java_com_mymenu_app_MenuView_NativeTouch(JNIEnv*, jclass,
    jint action, jint pointerId, jfloat x, jfloat y) {
    MenuTouch(action, pointerId, x, y);
}

}
```

---
