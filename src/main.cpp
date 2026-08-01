#include <iostream>


#include <SDL2/SDL.h>
#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_opengl3.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h> // <-- EKSİK OLAN SATIR BUYDU, BUNU EKLEYİN
#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_opengl3.h"

#include "../include/VehicleData.h"
#include "../include/ecu_decoder.h"
#include "../include/hmi_ui.h"
#include "../include/can_reader.h" 

int main(int, char**) {
    std::cout << "=== ECU CAN-BUS ve HMI Baslatiliyor ===\n";

   
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER);
    SDL_Window* window = SDL_CreateWindow("panel test", 
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 480, 
        SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    SDL_GLContext gl_context = SDL_GL_CreateContext(window);
    SDL_GL_MakeCurrent(window, gl_context);
    SDL_GL_SetSwapInterval(1); 

    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;


    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; 
    
   ImFont* font = io.Fonts->AddFontFromFileTTF("../assets/font/montserrat/NeueMontreal-Medium.otf", 20.0f);
    if (font == nullptr) {
        // Eğer dosya bulunamazsa veya adında hata olursa varsayılan fonta düşer
        io.Fonts->AddFontDefault();
    }
    ImGuiStyle& style = ImGui::GetStyle();
    ImGui_ImplSDL2_InitForOpenGL(window, gl_context);
    ImGui_ImplOpenGL3_Init("#version 130");

    
    ECU_Decoder decoder;
    VehicleState car_state;
    HMI_Dashboard dashboard;
    CAN_Reader can_bus("../data/can_dump.txt"); 
    dashboard.init_style();
    bool running = true;

    // 3. ANA DÖNGÜ
    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            ImGui_ImplSDL2_ProcessEvent(&event);
            if (event.type == SDL_QUIT) running = false;
        }

       
        CAN_Frame frame;
        if (can_bus.read_next_frame(frame)) {
            decoder.process_frame(frame, car_state);
        }

       
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();

        dashboard.render(car_state);

       ImGui::Render();
        
      
        int display_w, display_h;
        SDL_GL_GetDrawableSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);

        glClearColor(0.04f, 0.06f, 0.10f, 1.0f); 
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        SDL_GL_SwapWindow(window);
    }

   
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}