#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_opengl3.h"

#include "../include/VehicleData.h"
#include "../include/ecu_decoder.h"
#include "../include/can_reader.h" 
#include "../include/hmi/HMI_Dashboard.h" // <-- Güncellenen modüler dashboard yolu

int main(int, char**) {
    std::cout << "=== ECU CAN-BUS ve HMI Baslatiliyor ===\n";

    // 1. SDL ve OpenGL Başlatma
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER);
    SDL_Window* window = SDL_CreateWindow("panel test", 
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 480, 
        SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    SDL_GLContext gl_context = SDL_GL_CreateContext(window);
    SDL_GL_MakeCurrent(window, gl_context);
    SDL_GL_SetSwapInterval(1); 

    // 2. ImGui Kurulumu
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; 

    // Font Yükleme (Yolu ana dizine göre ayarladık)
    ImFont* font = io.Fonts->AddFontFromFileTTF("../assets/font/montserrat/NeueMontreal-Medium.otf", 20.0f);
    if (font == nullptr) {
        std::cout << "[UYARI] Font dosyasi bulunamadi, varsayilan font kullanilacak.\n";
        io.Fonts->AddFontDefault();
    }

    ImGuiStyle& style = ImGui::GetStyle();
    ImGui_ImplSDL2_InitForOpenGL(window, gl_context);
    ImGui_ImplOpenGL3_Init("#version 130");

    // 3. Modül Tanımlamaları
    ECU_Decoder decoder;
    VehicleState car_state;
    HMI_Dashboard dashboard;
    CAN_Reader can_bus("../data/can_dump.txt"); 
    dashboard.init_style();
    bool running = true;

    // 4. Ana Döngü
    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            ImGui_ImplSDL2_ProcessEvent(&event);
            if (event.type == SDL_QUIT) running = false;
        }

        // CAN Verisi Okuma
        CAN_Frame frame;
        if (can_bus.read_next_frame(frame)) {
            decoder.process_frame(frame, car_state);
        }

        // ImGui Çerçeve Başlangıcı
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();

        // Modüler Dashboard Çizimi
        dashboard.render(car_state);

        // Render ve Ekrana Basma
        ImGui::Render();
        
        int display_w, display_h;
        SDL_GL_GetDrawableSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);

        glClearColor(0.04f, 0.06f, 0.10f, 1.0f); 
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        SDL_GL_SwapWindow(window);
    }

    // 5. Temizlik
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}