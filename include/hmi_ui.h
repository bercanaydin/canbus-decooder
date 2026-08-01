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
        
        // Etiket (Tam merkezleme)
        ImVec2 label_size = ImGui::CalcTextSize(label);
        draw_list->AddText(ImVec2(center.x - label_size.x * 0.5f, center.y + 10), IM_COL32(150, 180, 210, 255), label);

        ImGui::Dummy(size);
    }

    void render(const VehicleState& car_state) {
        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
        
        ImGui::Begin("Dashboard", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove);

       
        float window_width = ImGui::GetWindowSize().x;
        
        std::string title = "DASHBOARD";
        float title_width = ImGui::CalcTextSize(title.c_str()).x;
        ImGui::SetCursorPosX((window_width - title_width) * 0.5f);
        ImGui::Text("%s", title.c_str());
        
        ImGui::Separator();
        ImGui::Spacing();

        
        ImVec2 gauge_size = ImVec2(220, 220);
        float spacing = 40.0f;
        float total_gauges_width = (gauge_size.x * 2.0f) + spacing;
        
        // İki göstergeyi ortalamak için başlangıç X koordinatını hesaplıyoruz
        float start_x = (window_width - total_gauges_width) * 0.5f;
        
        ImGui::SetCursorPosX(start_x);
        DrawGauge("RPM", (float)car_state.rpm, 0.0f, 8000.0f, gauge_size);
        
        ImGui::SameLine(0, spacing);
        DrawGauge("KM/H", (float)car_state.speed, 0.0f, 240.0f, gauge_size);

        ImGui::Spacing();
        ImGui::Spacing();

        // 3. Alt Bilgileri ve Uyarıları Yatay Olarak Ortalamak
        char temp_buf[64];
        snprintf(temp_buf, sizeof(temp_buf), "SICAKLIK: %d C", car_state.temperature);
        float temp_width = ImGui::CalcTextSize(temp_buf).x;
        ImGui::SetCursorPosX((window_width - temp_width) * 0.5f);
        ImGui::Text("%s", temp_buf);

        ImGui::Spacing();

        std::string status_msg;
        ImVec4 status_color;
        if (car_state.is_redline) {
            status_msg = "[!] DONANIM UYARISI: Yakit kesme (Fuel-Cut) AKTIF!";
            status_color = ImVec4(1.0f, 0.2f, 0.2f, 1.0f); // Kırmızı
        } else {
            status_msg = "[OK] Motor degerleri guvenli sinirda.";
            status_color = ImVec4(0.4f, 1.0f, 0.6f, 1.0f); // Yeşil
        }

        float status_width = ImGui::CalcTextSize(status_msg.c_str()).x;
        ImGui::SetCursorPosX((window_width - status_width) * 0.5f);
        ImGui::PushStyleColor(ImGuiCol_Text, status_color);
        ImGui::Text("%s", status_msg.c_str());
        ImGui::PopStyleColor();

        ImGui::End();
    }
};

#endif