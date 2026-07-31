#include <iostream>
#include <vector>
#include <iomanip>
#include <fstream>   
#include <sstream>  
#include <string>    

// ImGui ve SDL2 Kütüphaneleri
#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_opengl3.h"
#include <SDL.h>

// Proje Başlık Dosyalarımız
#include "../include/VehicleData.h"
#include "../include/ecu_decoder.h"
#include "../include/utils.h"

int main(int, char**) {
    std::cout << "=== ECU CAN-BUS ve HMI Baslatiliyor ===\n\n";

    // 1. SDL2 ve OpenGL Pencere Kurulumu
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0) {
        std::cout << "[HATA] SDL baslatilamadi!\n";
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("Otomotiv HMI - Dijital Panel", 
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 480, 
        SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    
    if (!window) {
        std::cout << "[HATA] Pencere olusturulamadi!\n";
        SDL_Quit();
        return 1;
    }

    SDL_GLContext gl_context = SDL_GL_CreateContext(window);
    SDL_GL_MakeCurrent(window, gl_context);
    SDL_GL_SetSwapInterval(1); // VSync aç

    // Dear ImGui Başlatma
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();

    ImGui_ImplSDL2_InitForOpenGL(window, gl_context);
    ImGui_ImplOpenGL3_Init("#version 130");

    // 2. ECU ve Veri Hazırlığı
    ECU_Decoder decoder;
    VehicleState car_state; // ImGui ve konsolun ortak besleneceği veri sepeti

    std::ifstream file("../data/can_dump.txt");
    if (!file.is_open()) {
        std::cout << "[HATA] '../data/can_dump.txt' dosyasi bulunamadi!\n";
        std::cout << "Lutfen once Python veri uretici scriptini calistirin.\n";
    }

    std::string line;
    bool running = true;

    // 3. ANA UYGULAMA VE HMI DÖNGÜSÜ
    // (Dosyadan satır okudukça hem konsola basar hem ImGui ekranını anlık günceller)
    while (running) {
        // SDL Olaylarını Yakala (Pencereyi kapatma tuşu vb.)
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            ImGui_ImplSDL2_ProcessEvent(&event);
            if (event.type == SDL_QUIT)
                running = false;
        }

        // Eğer dosya hala okunuyorsa sıradaki satırı işle
        if (file.is_open() && std::getline(file, line)) {
            if (!line.empty()) {
                std::stringstream ss(line);
                std::string tempStr;
                CAN_Frame frame;

                ss >> tempStr;
                frame.id = hexToUInt(tempStr);
                frame.dlc = 8; 

                for (int i = 0; i < 8; ++i) {
                    if (ss >> tempStr) {
                        frame.data[i] = static_cast<uint8_t>(hexToUInt(tempStr));
                    } else {
                        frame.data[i] = 0; 
                    }
                }
              
                // Çift parametreli fonksiyon: Hem konsola debug basar hem car_state doldurur
                decoder.process_frame(frame, car_state);
            }
        } else if (file.is_open()) {
            // Dosya bittiğinde başa sarabilir veya döngüyü sonlandırabilirsiniz
            file.clear();
            file.seekg(0, std::ios::beg);
        }

        // --- ImGui Arayüz Çizimi ---
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();

        // Otomotiv Gösterge Penceresi Tasarımı
        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::SetNextWindowSize(ImVec2(800, 480));
        ImGui::Begin("Dashboard", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove);

        ImGui::Text("=== KOTOLAR OTO - CAN-BUS TELEMETRI ===");
        ImGui::Separator();
        ImGui::Spacing();

        // Görsel Devir Çubuğu (Bar)
        float rpm_progress = (float)car_state.rpm / 8000.0f;
        if (rpm_progress > 1.0f) rpm_progress = 1.0f;
        
        ImGui::Text("DEVIR: %d RPM", car_state.rpm);
        ImGui::ProgressBar(rpm_progress, ImVec2(-1.0f, 35.0f), "");

        ImGui::Spacing();
        ImGui::Text("HIZ:   %d km/h", car_state.speed);
        ImGui::Text("ISI:   %d C", car_state.temperature);
        ImGui::Spacing();

        // Redline Uyarı Lambası Mantığı
        if (car_state.is_redline) {
            ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 0, 0, 255));
            ImGui::Text(" [!] DONANIM UYARISI: Yakit kesme (Fuel-Cut) AKTIF!");
            ImGui::PopStyleColor();
        } else {
            ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0, 255, 0, 255));
            ImGui::Text(" [OK] Motor degerleri guvenli sinirda.");
            ImGui::PopStyleColor();
        }

        ImGui::End();

        // Çizimleri Ekrana Bas
        ImGui::Render();
        glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
        glClearColor(0.08f, 0.08f, 0.08f, 1.0f); // Koyu gri otomotiv arka planı
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        SDL_GL_SwapWindow(window);
    }

    // 4. Temizlik İşlemleri
    if (file.is_open()) file.close();
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
    SDL_DestroyWindow(window);
    SDL_Quit();

    std::cout << "\n[BILGI] Program güvenli bir şekilde kapatildi.\n";
    return 0;
}