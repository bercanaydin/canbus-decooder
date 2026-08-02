#ifndef DASHBOARD_PAGE_H
#define DASHBOARD_PAGE_H

#include "imgui.h"
#include "VehicleData.h"
#include "../HMI_Components.h"
#include <string>
#include <cstdio>

class DashboardPage {
private:
    void render_header() {
        float window_width = ImGui::GetWindowSize().x;
        std::string title = "DASHBOARD";
        float title_width = ImGui::CalcTextSize(title.c_str()).x;
        
        ImGui::SetCursorPosX((window_width - title_width) * 0.5f);
        ImGui::Text("%s", title.c_str());
        ImGui::Separator();
        ImGui::Spacing();
    }

    void render_gauges(const VehicleState& car_state) {
        float window_width = ImGui::GetWindowSize().x;
        ImVec2 gauge_size = ImVec2(220, 220);
        float spacing = 40.0f;
        float total_width = (gauge_size.x * 2.0f) + spacing;
        float start_x = (window_width - total_width) * 0.5f;
        
        ImGui::SetCursorPosX(start_x);
        HMIComponents::DrawCircularGauge("RPM", (float)car_state.rpm, 0.0f, 8000.0f, gauge_size);
        
        ImGui::SameLine(0, spacing);
        HMIComponents::DrawCircularGauge("KM/H", (float)car_state.speed, 0.0f, 240.0f, gauge_size);
    }

    void render_status(const VehicleState& car_state) {
        float window_width = ImGui::GetWindowSize().x;
        ImGui::Spacing();
        ImGui::Spacing();

        // Sıcaklık Bilgisi
        char temp_buf[64];
        snprintf(temp_buf, sizeof(temp_buf), "SICAKLIK: %d C", car_state.temperature);
        float temp_width = ImGui::CalcTextSize(temp_buf).x;
        ImGui::SetCursorPosX((window_width - temp_width) * 0.5f);
        ImGui::Text("%s", temp_buf);

        ImGui::Spacing();

        // Uyarı Mesajı
        std::string status_msg;
        ImVec4 status_color;
        if (car_state.is_redline) {
            status_msg = "[!] DONANIM UYARISI: Yakit kesme (Fuel-Cut) AKTIF!";
            status_color = ImVec4(1.0f, 0.2f, 0.2f, 1.0f);
        } else {
            status_msg = "[OK] Motor degerleri guvenli sinirda.";
            status_color = ImVec4(0.4f, 1.0f, 0.6f, 1.0f);
        }

        float status_width = ImGui::CalcTextSize(status_msg.c_str()).x;
        ImGui::SetCursorPosX((window_width - status_width) * 0.5f);
        ImGui::PushStyleColor(ImGuiCol_Text, status_color);
        ImGui::Text("%s", status_msg.c_str());
        ImGui::PopStyleColor();
    }

public:
    void render(const VehicleState& car_state) {
        render_header();
        render_gauges(car_state);
        render_status(car_state);
    }
};

#endif