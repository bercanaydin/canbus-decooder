#ifndef CLIMATE_PAGE_H
#define CLIMATE_PAGE_H

#include "imgui.h"
#include "../../clima/climacontrol.h"
#include <string>
#include <cstdio>

class ClimatePage {
public:
    void render(const ClimaControl& climate) {
        float window_width = ImGui::GetWindowSize().x;
        
        // Sayfa Başlığı
        std::string header = "KABIN IKLIMLENDIRME YONETIMI";
        float h_width = ImGui::CalcTextSize(header.c_str()).x;
        ImGui::SetCursorPosX((window_width - h_width) * 0.5f);
        ImGui::Text("%s", header.c_str());
        
        ImGui::Spacing();
        ImGui::Spacing();

        
        char clima_buf[64];
        snprintf(clima_buf, sizeof(clima_buf), "Target tempe: %.1f C", climate.cabin_temperature);
        float c_width = ImGui::CalcTextSize(clima_buf).x;
        ImGui::SetCursorPosX((window_width - c_width) * 0.5f);
        ImGui::Text("%s", clima_buf);

        ImGui::Spacing();

      
        std::string ac_status = climate.ac_status ? "Klima Durumu: AKTIF (Sogutuyor)" : "Klima Durumu: KAPALI";
        float ac_width = ImGui::CalcTextSize(ac_status.c_str()).x;
        ImGui::SetCursorPosX((window_width - ac_width) * 0.5f);
        
        ImGui::PushStyleColor(ImGuiCol_Text, climate.ac_status ? ImVec4(0.2f, 0.8f, 1.0f, 1.0f) : ImVec4(0.6f, 0.6f, 0.6f, 1.0f));
        ImGui::Text("%s", ac_status.c_str());
        ImGui::PopStyleColor();
    }
};

#endif