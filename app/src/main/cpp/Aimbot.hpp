```cpp
#pragma once
#include "Offsets.hpp"
#include "Utils.hpp"

struct AimConfig {
    bool  Enable    = false;
    bool  Silent    = false;
    bool  Visible   = true;
    bool  DrawFOV   = true;
    float Smooth    = 0.5f;
    float FOV       = 120.0f;
    int   Hitbox    = 0;
};

extern AimConfig g_Aim;

inline void RunAimbot() {
    if (!g_Aim.Enable) return;

    uintptr_t local = Read<uintptr_t>(Offset::INIT_BASE + Offset::LOCAL_PLAYER);
    if (!local) return;

    Vec3 localPos   = Read<Vec3>(local + Offset::LOCAL_POS);
    Vec3 curAngle   = Read<Vec3>(local + Offset::LOCAL_ANGLE);
    int  localTeam  = Read<int>(local + Offset::LOCAL_TEAM);

    uintptr_t list  = Read<uintptr_t>(Offset::INIT_BASE + Offset::ENTITY_LIST);
    int count       = Read<int>(Offset::INIT_BASE + Offset::ENTITY_COUNT);
    if (!list || count <= 0) return;

    float closest = g_Aim.FOV;
    Vec3  best;
    bool  found = false;

    for (int i = 0; i < count; i++) {
        uintptr_t ent = Read<uintptr_t>(list + i * 0x8);
        if (!ent || ent == local) continue;
        if (Read<int>(ent + Offset::ENTITY_TEAM) == localTeam) continue;
        if (Read<float>(ent + Offset::ENTITY_HEALTH) <= 0) continue;

        uintptr_t hb = Offset::ENTITY_HEAD;
        if (g_Aim.Hitbox == 1) hb = Offset::ENTITY_CHEST;
        else if (g_Aim.Hitbox == 2) hb = Offset::ENTITY_PELVIS;

        Vec3 tgt = Read<Vec3>(ent + hb);
        Vec3 ang = CalcAngle(localPos, tgt);
        float d  = sqrtf(pow(ang.x - curAngle.x, 2) + pow(ang.y - curAngle.y, 2));

        if (d < closest) { closest = d; best = ang; found = true; }
    }

    if (found) {
        Write<Vec3>(local + Offset::LOCAL_ANGLE,
                    Smooth(curAngle, best, g_Aim.Smooth));
    }
}
```
