#ifndef HMI_UI_H
#define HMI_UI_H

#include "imgui.h"
#include "VehicleData.h"
#include <cmath>
#include <cstdio>

class HMI_Dashboard {
public:
    void init_style() {
        ImGuiStyle& style = ImGui::GetStyle();
        style.WindowBorderSize = 0.0f;
        style.FrameBorderSize = 0.0f;
        style.PopupBorderSize = 0.0f;
        style.ChildBorderSize = 0.0f;
        
        ImVec4* colors = style.Colors;
        colors[ImGuiCol_WindowBg]      = ImVec4(0.04f, 0.06f, 0.10f, 1.00f); 
        colors[ImGuiCol_Text]          = ImVec4(0.85f, 0.93f, 1.00f, 1.00f); 
        colors[ImGuiCol_FrameBg]       = ImVec4(0.12f, 0.18f, 0.28f, 1.00f); 
        colors[ImGuiCol_Separator]     = ImVec4(0.20f, 0.35f, 0.55f, 1.00f); 
    }

    void DrawGauge(const char* label, float value, float v_min, float v_max, const ImVec2& size) {
        ImDrawList* draw_list = ImGui::GetWindowDrawList();
        ImVec2 pos = ImGui::GetCursorScreenPos();
        
        float radius = std::min(size.x, size.y) * 0.42f;
        ImVec2 center = ImVec2(pos.x + size.x * 0.5f, pos.y + size.y * 0.5f);

       
        draw_list->AddCircleFilled(center, radius, IM_COL32(20, 30, 45, 255), 32);
        draw_list->AddCircle(center, radius, IM_COL32(50, 90, 140, 255), 32, 3.0f);

       
        float ang_min = 3.14159265f * 0.75f;
        float ang_max = 3.14159265f * 2.25f;
        float fraction = (value - v_min) / (v_max - v_min);
        if (fraction < 0.0f) fraction = 0.0f;
        if (fraction > 1.0f) fraction = 1.0f;
        float ang_val = ang_min + (ang_max - ang_min) * fraction;


        draw_list->PathArcTo(center, radius - 6.0f, ang_min, ang_val, 32);
        draw_list->PathStroke(IM_COL32(50, 150, 255, 255), false, 8.0f);

        
        char text_buf[32];
        snprintf(text_buf, sizeof(text_buf), "%.0f", value);
        ImVec2 text_size = ImGui::CalcTextSize(text_buf);
        draw_list->AddText(ImVec2(center.x - text_size.x * 0.5f, center.y - text_size.y * 0.5f - 10), IM_COL32(255, 255, 255, 255), text_buf);
        
        
        ImVec2 label_size = ImGui::CalcTextSize(label);
        draw_list->AddText(ImVec2(center.x - label_size.x * 0.5f, center.y + 10), IM_COL32(150, 180, 210, 255), label);

        ImGui::Dummy(size);
    }

    void render(const VehicleState& car_state) {
        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
        
        ImGui::Begin("Dashboard", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove);

        ImGui::SetWindowFontScale(1.2f);
        ImGui::Text("=== OTONOM HMI SISTEMI ===");
        ImGui::Separator();
        ImGui::Spacing();

        ImVec2 gauge_size = ImVec2(220, 220);
        
        DrawGauge("RPM", (float)car_state.rpm, 0.0f, 8000.0f, gauge_size);
        ImGui::SameLine(0, 40);
        DrawGauge("KM/H", (float)car_state.speed, 0.0f, 240.0f, gauge_size);

        ImGui::Spacing();
        ImGui::Text("ISI: %d C", car_state.temperature);
        ImGui::Spacing();

        if (car_state.is_redline) {
            ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 50, 50, 255)); 
            ImGui::Text(" [!] DONANIM UYARISI: Yakit kesme (Fuel-Cut) AKTIF!");
            ImGui::PopStyleColor();
        } else {
            ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(100, 255, 150, 255)); 
            ImGui::Text(" [OK] Motor degerleri guvenli sinirda.");
            ImGui::PopStyleColor();
        }

        ImGui::End();
    }
};

#endif