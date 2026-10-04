```cmake
cmake_minimum_required(VERSION 3.22.1)
project("mymenu")

set(IMGUI_DIR ${CMAKE_SOURCE_DIR}/src/main/cpp/imgui)

add_library(imgui STATIC
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
    ${IMGUI_DIR}/backends/imgui_impl_android.cpp
    ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
)

target_include_directories(imgui PUBLIC
    ${IMGUI_DIR}
    ${IMGUI_DIR}/backends
)

add_library(native-lib SHARED
    src/main/cpp/native-lib.cpp
    src/main/cpp/Main.cpp
)

target_link_libraries(native-lib
    imgui
    android
    log
    GLESv3
    EGL
)

target_include_directories(native-lib PRIVATE
    ${IMGUI_DIR}
    ${IMGUI_DIR}/backends
    src/main/cpp
)
```

---
