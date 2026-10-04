```cpp
#include "Main.h"
#include "imgui.h"
#include "imgui_impl_android.h"
#include "imgui_impl_opengl3.h"
#include "Aimbot.hpp"
#include "ESP.hpp"
#include <android/log.h>
#include <GLES3/gl3.h>

#define TAG "MyMenu"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, TAG, __VA_ARGS__)

AimConfig g_Aim;
ESPConfig g_ESP;

static bool g_Init = false;
static bool g_Open = true;
static int  g_W = 0, g_H = 0;

static void Theme() {
    ImGuiStyle& s = ImGui::GetStyle();
    s.WindowRounding = 8; s.FrameRounding = 4;
    s.WindowPadding  = ImVec2(12,12);
    s.ItemSpacing    = ImVec2(8,8);

    ImVec4* c = s.Colors;
    c[ImGuiCol_WindowBg]        = ImVec4(0.05f,0.05f,0.08f,0.97f);
    c[ImGuiCol_Text]            = ImVec4(0.94f,0.94f,0.96f,1.0f);
    c[ImGuiCol_CheckMark]       = ImVec4(1.0f,0.32f,0.15f,1.0f);
    c[ImGuiCol_SliderGrab]      = ImVec4(1.0f,0.32f,0.15f,1.0f);
    c[ImGuiCol_Button]          = ImVec4(0.15f,0.15f,0.20f,1.0f);
    c[ImGuiCol_ButtonHovered]   = ImVec4(0.35f,0.15f,0.10f,1.0f);
    c[ImGuiCol_FrameBg]         = ImVec4(0.12f,0.12f,0.16f,1.0f);
    c[ImGuiCol_Header]          = ImVec4(0.15f,0.15f,0.20f,1.0f);
    c[ImGuiCol_HeaderHovered]   = ImVec4(0.35f,0.15f,0.10f,1.0f);
    c[ImGuiCol_Tab]             = ImVec4(0.10f,0.10f,0.14f,1.0f);
    c[ImGuiCol_TabHovered]      = ImVec4(0.35f,0.15f,0.10f,1.0f);
    c[ImGuiCol_TabSelected]     = ImVec4(1.0f,0.32f,0.15f,1.0f);
}

static void DrawMenu() {
    ImGui::SetNextWindowSize(ImVec2(600,400), ImGuiCond_FirstUseEver);
    if (!ImGui::Begin("MY MENU | FREE FIRE", &g_Open,
            ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize)) {
        ImGui::End(); return;
    }

    if (ImGui::BeginTabBar("##tabs")) {
        if (ImGui::BeginTabItem("Aim Assist")) {
            ImGui::Checkbox("Enable Aimbot", &g_Aim.Enable);
            ImGui::Checkbox("Silent Aim",   &g_Aim.Silent);
            ImGui::Checkbox("Visible",      &g_Aim.Visible);
            ImGui::Checkbox("Draw FOV",     &g_Aim.DrawFOV);
            ImGui::SliderFloat("Smooth", &g_Aim.Smooth, 0.1f, 1.0f, "%.2f");
            ImGui::SliderFloat("FOV",    &g_Aim.FOV,   10.f, 300.f, "%.0f px");
            const char* hb[] = {"Head","Neck","Pelvis"};
            ImGui::Combo("Hitbox", &g_Aim.Hitbox, hb, 3);
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Visuals/ESP")) {
            ImGui::Checkbox("Enable ESP", &g_ESP.Enable);
            ImGui::Checkbox("Box",        &g_ESP.Box);
            ImGui::Checkbox("Skeleton",   &g_ESP.Skeleton);
            ImGui::Checkbox("Line",       &g_ESP.Line);
            ImGui::Checkbox("Health",     &g_ESP.Health);
            ImGui::ColorEdit4("Color", g_ESP.Color,
                ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaPreview);
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Settings")) {
            ImGui::Text("Menu Key: INSERT");
            ImGui::Text("Aim Key : RIGHT MOUSE");
            if (ImGui::Button("Reset", ImVec2(-1,34))) {
                g_Aim = AimConfig{};
                g_ESP = ESPConfig{};
            }
            ImGui::Text("Build: %s", __DATE__);
            ImGui::EndTabItem();
        }
        ImGui::EndTabBar();
    }
    ImGui::End();
}

void MenuInit() {
    if (g_Init) return;
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.IniFilename = nullptr;
    Theme();
    ImGui_ImplAndroid_Init(nullptr);
    ImGui_ImplOpenGL3_Init("#version 300 es");
    g_Init = true;
    LOGI("Menu init OK");
}

void MenuResize(int w, int h) {
    g_W = w; g_H = h;
    glViewport(0, 0, w, h);
}

void MenuRender() {
    if (!g_Init) return;

    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT);

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplAndroid_NewFrame(g_W, g_H);
    ImGui::NewFrame();

    if (g_Open) DrawMenu();

    ImDrawList* dl = ImGui::GetBackgroundDrawList();
    if (g_Aim.Enable && g_Aim.DrawFOV)
        dl->AddCircle(ImVec2(g_W/2.0f, g_H/2.0f), g_Aim.FOV,
                      IM_COL32(255,80,50,180), 64, 2.0f);

    RunESP(dl, g_W, g_H);
    RunAimbot();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void MenuTouch(int action, int pointerId, float x, float y) {
    if (!g_Init) return;
    ImGui_ImplAndroid_HandleInputEvent(action, pointerId, x, y);
}
```

---
