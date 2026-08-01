#ifndef HMI_COMPONENTS_H
#define HMI_COMPONENTS_H

#include "imgui.h"
#include <cmath>
#include <cstdio>

namespace HMIComponents {
    // Dairesel Gösterge Bileşeni
    inline void DrawCircularGauge(const char* label, float value, float v_min, float v_max, const ImVec2& size) {
        ImDrawList* draw_list = ImGui::GetWindowDrawList();
        ImVec2 pos = ImGui::GetCursorScreenPos();
        
        float radius = std::min(size.x, size.y) * 0.42f;
        ImVec2 center = ImVec2(pos.x + size.x * 0.5f, pos.y + size.y * 0.5f);

        // Arka plan çemberi
        draw_list->AddCircleFilled(center, radius, IM_COL32(20, 30, 45, 255), 32);
        draw_list->AddCircle(center, radius, IM_COL32(50, 90, 140, 255), 32, 3.0f);

        // Yay açıları (240 derece)
        float ang_min = 3.14159265f * 0.75f;
        float ang_max = 3.14159265f * 2.25f;
        float fraction = (value - v_min) / (v_max - v_min);
        if (fraction < 0.0f) fraction = 0.0f;
        if (fraction > 1.0f) fraction = 1.0f;
        float ang_val = ang_min + (ang_max - ang_min) * fraction;

        // Doluluk yayı
        draw_list->PathArcTo(center, radius - 6.0f, ang_min, ang_val, 32);
        draw_list->PathStroke(IM_COL32(50, 150, 255, 255), false, 8.0f);

        // Değer yazısı
        char text_buf[32];
        snprintf(text_buf, sizeof(text_buf), "%.0f", value);
        ImVec2 text_size = ImGui::CalcTextSize(text_buf);
        draw_list->AddText(ImVec2(center.x - text_size.x * 0.5f, center.y - text_size.y * 0.5f - 10), IM_COL32(255, 255, 255, 255), text_buf);
        
        // Etiket
        ImVec2 label_size = ImGui::CalcTextSize(label);
        draw_list->AddText(ImVec2(center.x - label_size.x * 0.5f, center.y + 10), IM_COL32(150, 180, 210, 255), label);

        ImGui::Dummy(size);
    }
}

#endif