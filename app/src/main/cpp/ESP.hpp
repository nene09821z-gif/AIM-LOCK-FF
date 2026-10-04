```cpp
#pragma once
#include "Offsets.hpp"
#include "Utils.hpp"
#include "imgui.h"

struct ESPConfig {
    bool  Enable   = false;
    bool  Box      = true;
    bool  Skeleton = false;
    bool  Line     = false;
    bool  Health   = true;
    float Color[4] = { 1.0f, 0.2f, 0.1f, 1.0f };
};

extern ESPConfig g_ESP;

inline bool WorldToScreen(const Vec3& w, Vec2& o, float* m, int sw, int sh) {
    if (!m) return false;
    float ww = m[12]*w.x + m[13]*w.y + m[14]*w.z + m[15];
    if (ww < 0.01f) return false;
    float xx = m[0]*w.x + m[1]*w.y + m[2]*w.z + m[3];
    float yy = m[4]*w.x + m[5]*w.y + m[6]*w.z + m[7];
    o.x = (sw / 2.0f) + (sw / 2.0f) * xx / ww;
    o.y = (sh / 2.0f) - (sh / 2.0f) * yy / ww;
    return true;
}

inline void RunESP(ImDrawList* dl, int sw, int sh) {
    if (!g_ESP.Enable || !dl) return;

    uintptr_t local = Read<uintptr_t>(Offset::INIT_BASE + Offset::LOCAL_PLAYER);
    if (!local) return;
    int localTeam = Read<int>(local + Offset::LOCAL_TEAM);

    uintptr_t list = Read<uintptr_t>(Offset::INIT_BASE + Offset::ENTITY_LIST);
    int count      = Read<int>(Offset::INIT_BASE + Offset::ENTITY_COUNT);
    if (!list || count <= 0) return;

    float* vm = reinterpret_cast<float*>(Offset::INIT_BASE + Offset::VIEW_MATRIX);

    ImU32 col = ImGui::ColorConvertFloat4ToU32(
        ImVec4(g_ESP.Color[0], g_ESP.Color[1], g_ESP.Color[2], g_ESP.Color[3]));

    for (int i = 0; i < count; i++) {
        uintptr_t ent = Read<uintptr_t>(list + i * 0x8);
        if (!ent || ent == local) continue;
        if (Read<int>(ent + Offset::ENTITY_TEAM) == localTeam) continue;
        float hp = Read<float>(ent + Offset::ENTITY_HEALTH);
        if (hp <= 0) continue;

        Vec3 head   = Read<Vec3>(ent + Offset::ENTITY_HEAD);
        Vec3 chest  = Read<Vec3>(ent + Offset::ENTITY_CHEST);
        Vec3 pelvis = Read<Vec3>(ent + Offset::ENTITY_PELVIS);

        Vec2 sH, sC, sP;
        if (!WorldToScreen(head, sH, vm, sw, sh)) continue;
        if (!WorldToScreen(pelvis, sP, vm, sw, sh)) continue;
        WorldToScreen(chest, sC, vm, sw, sh);

        if (g_ESP.Box)
            dl->AddRect(ImVec2(sH.x-25, sH.y-10),
                        ImVec2(sP.x+25, sP.y+10), col, 0, 0, 1.8f);

        if (g_ESP.Line)
            dl->AddLine(ImVec2(sw/2.0f, (float)sh),
                        ImVec2(sP.x, sP.y), col, 1.5f);

        if (g_ESP.Skeleton) {
            dl->AddLine(ImVec2(sH.x,sH.y), ImVec2(sC.x,sC.y), col, 1.5f);
            dl->AddLine(ImVec2(sC.x,sC.y), ImVec2(sP.x,sP.y), col, 1.5f);
        }

        if (g_ESP.Health) {
            float r = hp / 200.0f; if (r > 1) r = 1;
            float bh = sP.y - sH.y;
            ImU32 hc = IM_COL32((int)(255*(1-r)), (int)(255*r), 0, 255);
            dl->AddRectFilled(ImVec2(sH.x-32, sP.y-bh*r),
                              ImVec2(sH.x-28, sP.y), hc);
        }
    }
}
```
