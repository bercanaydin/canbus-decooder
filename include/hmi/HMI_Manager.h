#ifndef HMI_MANAGER_H
#define HMI_MANAGER_H

#include "imgui.h"
#include "VehicleData.h"
#include "../clima/climacontrol.h"
#include "HMI_Components.h"
#include "pages/DashboardPage.h"
#include "pages/ClimatePage.h"
#include <string>

class HMI_Manager {
private:


    DashboardPage dashboard_page;
    ClimatePage climate_page;
   
    enum class ActiveTab {
        Dashboard,
        Climate
    };

    ActiveTab current_tab;

    void render_top_navigation() {
        float window_width = ImGui::GetWindowSize().x;
        
        
        std::string title = "HMI SYSTEM";
        float title_width = ImGui::CalcTextSize(title.c_str()).x;
        ImGui::SetCursorPosX((window_width - title_width) * 0.5f);
        ImGui::Text("%s", title.c_str());
        
        ImGui::Separator();
        ImGui::Spacing();

       
        float buttons_total_width = 220.0f;
        ImGui::SetCursorPosX((window_width - buttons_total_width) * 0.5f);
        
        if (ImGui::Button("Gosterge Paneli", ImVec2(100, 30))) {
            current_tab = ActiveTab::Dashboard;
        }
        ImGui::SameLine(0, 20);
        if (ImGui::Button("Klima Kontrol", ImVec2(100, 30))) {
            current_tab = ActiveTab::Climate;
        }

        if(ImGui::IsKeyPressed(ImGuiKey_A)) {
            current_tab = ActiveTab::Dashboard;
        }
        
        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();
    }

    void render_dashboard_page(const VehicleState& car_state) {
        float window_width = ImGui::GetWindowSize().x;
        ImVec2 gauge_size = ImVec2(220, 220);
        float spacing = 40.0f;
        float total_width = (gauge_size.x * 2.0f) + spacing;
        float start_x = (window_width - total_width) * 0.5f;
        
        // Göstergeler
        ImGui::SetCursorPosX(start_x);
        HMIComponents::DrawCircularGauge("RPM", (float)car_state.rpm, 0.0f, 8000.0f, gauge_size);
        
        ImGui::SameLine(0, spacing);
        HMIComponents::DrawCircularGauge("KM/H", (float)car_state.speed, 0.0f, 240.0f, gauge_size);

        ImGui::Spacing();
        ImGui::Spacing();

        // Sıcaklık Bilgisi
        char temp_buf[64];
        snprintf(temp_buf, sizeof(temp_buf), "MOTOR SICAKLIGI: %d C", car_state.temperature);
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

    void render_climate_page(const ClimaControl& climate) {
        float window_width = ImGui::GetWindowSize().x;
        
        // Klima Sayfası İçeriği
        std::string header = "KABIN IKLIMLENDIRME YONETIMI";
        float h_width = ImGui::CalcTextSize(header.c_str()).x;
        ImGui::SetCursorPosX((window_width - h_width) * 0.5f);
        ImGui::Text("%s", header.c_str());
        
        ImGui::Spacing();
        ImGui::Spacing();

        char clima_buf[64];
        snprintf(clima_buf, sizeof(clima_buf), "Hedef Kabin Sicakligi: %.1f C", climate.cabin_temperature);
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

public:
    HMI_Manager() : current_tab(ActiveTab::Dashboard) {}

    void init_style() {
        ImGuiStyle& style = ImGui::GetStyle();
        style.WindowBorderSize = 0.0f;
        style.FrameBorderSize = 0.0f;
        style.PopupBorderSize = 0.0f;
        style.ChildBorderSize = 0.0f;
        
        ImVec4* colors = style.Colors;
        colors[ImGuiCol_WindowBg]      = ImVec4(0.04f, 0.06f, 0.10f, 1.00f); 
       
        colors[ImGuiCol_FrameBg]       = ImVec4(0.12f, 0.18f, 0.28f, 1.00f); 
        colors[ImGuiCol_Separator]     = ImVec4(0.20f, 0.35f, 0.55f, 1.00f); 
        colors[ImGuiCol_Button]        = ImVec4(0.15f, 0.25f, 0.40f, 1.00f);
        colors[ImGuiCol_ButtonHovered] = ImVec4(0.25f, 0.40f, 0.65f, 1.00f);
    }

    
    void render(const VehicleState& car_state, const ClimaControl& climate) {
        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
        
        ImGui::Begin("DashboardManager", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove);

        render_top_navigation();

        // Aktif sekmeye göre ilgili sayfayı ekrana bas
        if (current_tab == ActiveTab::Dashboard) {
            render_dashboard_page(car_state);
        } else if (current_tab == ActiveTab::Climate) {
            render_climate_page(climate);
        }

        ImGui::End();
    }
};

#endif