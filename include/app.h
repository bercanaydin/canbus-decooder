#ifndef APP_H
#define APP_H

#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_opengl3.h"

#include "VehicleData.h"
#include "ecu_decoder.h"
#include "can_reader.h"
#include "clima/climacontrol.h"
#include "hmi/HMI_Manager.h"

class App {
private:
    SDL_Window* window;
    SDL_GLContext gl_context;
    bool running;

    ECU_Decoder decoder;
    VehicleState car_state;
    ClimaControl climate_state;
    HMI_Manager hmi_manager;
    CAN_Reader can_bus;

public:
    App() : window(nullptr), gl_context(nullptr), running(true), can_bus("../data/can_dump.txt") {}

    bool init() {
        std::cout << "=== ECU CAN-BUS ve HMI Baslatiliyor ===\n";

        if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0) return false;

        window = SDL_CreateWindow("panel test", 
            SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 480, 
            SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_BORDERLESS);
        if (!window) return false;

        gl_context = SDL_GL_CreateContext(window);
        SDL_GL_MakeCurrent(window, gl_context);
        SDL_GL_SetSwapInterval(1); 

        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; 

        ImFont* font = io.Fonts->AddFontFromFileTTF("../assets/font/montserrat/NeueMontreal-Medium.otf", 20.0f);
        if (font == nullptr) {
            io.Fonts->AddFontDefault();
        }

        ImGui_ImplSDL2_InitForOpenGL(window, gl_context);
        ImGui_ImplOpenGL3_Init("#version 130");

        hmi_manager.init_style();
        return true;
    }

    void run() {
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

            hmi_manager.render(car_state, climate_state, &running);

            ImGui::Render();
            
            int display_w, display_h;
            SDL_GL_GetDrawableSize(window, &display_w, &display_h);
            glViewport(0, 0, display_w, display_h);

            glClearColor(0.04f, 0.06f, 0.10f, 1.0f); 
            glClear(GL_COLOR_BUFFER_BIT);
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
            SDL_GL_SwapWindow(window);
        }
    }

    void shutdown() {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplSDL2_Shutdown();
        ImGui::DestroyContext();
        SDL_DestroyWindow(window);
        SDL_Quit();
    }
};

#endif