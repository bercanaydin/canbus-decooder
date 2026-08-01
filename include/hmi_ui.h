#ifndef HMI_UI_H
#define HMI_UI_H

#include "imgui.h"
#include "VehicleData.h"

class HMI_Dashboard {
public:
  
    void init_style() {
        ImGuiStyle& style = ImGui::GetStyle();
        
       
        style.WindowBorderSize = 0.0f;
        style.FrameBorderSize = 0.0f;
        style.PopupBorderSize = 0.0f;
        style.ChildBorderSize = 0.0f;
        
       
        ImVec4* colors = style.Colors;
        colors[ImGuiCol_WindowBg]       = ImVec4(0.04f, 0.06f, 0.10f, 1.00f); 
        colors[ImGuiCol_Text]           = ImVec4(0.85f, 0.93f, 1.00f, 1.00f); 
        colors[ImGuiCol_FrameBg]        = ImVec4(0.12f, 0.18f, 0.28f, 1.00f); // Widget arka planı
        colors[ImGuiCol_PlotHistogram]  = ImVec4(0.20f, 0.60f, 1.00f, 1.00f); // Devir çubuğu (Parlak Mavi)
        colors[ImGuiCol_Separator]      = ImVec4(0.20f, 0.35f, 0.55f, 1.00f); // Ayırıcı çizgi
    }

  
    void render(const VehicleState& car_state) {
        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
        
        ImGui::Begin("Dashboard", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove);

        ImGui::Text("=== OTONOM HMI SISTEMI ===");
        ImGui::Separator();
        ImGui::Spacing();

        // Görsel Devir Çubuğu (Mavi Palet)
        float rpm_progress = (float)car_state.rpm / 8000.0f;
        if (rpm_progress > 1.0f) rpm_progress = 1.0f;
        
        ImGui::Text("DEVIR: %d RPM", car_state.rpm);
        ImGui::ProgressBar(rpm_progress, ImVec2(-1.0f, 35.0f), "");

        ImGui::Spacing();
        ImGui::Text("HIZ:   %d km/h", car_state.speed);
        ImGui::Text("ISI:   %d C", car_state.temperature);
        ImGui::Spacing();

        // Uyarı Lambası Mantığı
        if (car_state.is_redline) {
            ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 50, 50, 255)); // Kritik durumda kırmızıya döner
            ImGui::Text(" [!] DONANIM UYARISI: Yakit kesme (Fuel-Cut) AKTIF!");
            ImGui::PopStyleColor();
        } else {
            ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(100, 255, 150, 255)); // Güvenli durumda yeşil/su yeşili
            ImGui::Text(" [OK] Motor degerleri guvenli sinirda.");
            ImGui::PopStyleColor();
        }

        ImGui::End();
    }
};

#endif