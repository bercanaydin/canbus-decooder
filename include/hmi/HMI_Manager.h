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
        if (ImGui::Button("Klima Kontrol", ImVec2(100, 30)) || ImGui::IsKeyPressed(ImGuiKey_D))     current_tab = ActiveTab::Climate;
        

        if (ImGui::IsKeyPressed(ImGuiKey_A)  || ImGui::IsKeyPressed(ImGuiKey_A))     current_tab = ActiveTab::Dashboard;
        
        
        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();
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

    void render(const VehicleState& car_state, ClimaControl& climate) {
        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
        
        ImGui::Begin("DashboardManager", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove);

        render_top_navigation();

       
        if (current_tab == ActiveTab::Dashboard) {
            dashboard_page.render(car_state);
        } else if (current_tab == ActiveTab::Climate) {
            climate_page.render(climate);
        }

        ImGui::End();
    }
};

#endif