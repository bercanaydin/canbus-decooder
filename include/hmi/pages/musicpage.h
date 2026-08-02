#ifndef musicpage_h
#define musicpage_h



#include "imgui.h"
#include <string>
#include <cstdio>
class MusicPage{

public:

    void render(std::string name){
        std::string header = "MUSIC PAGE";
         ImGui::Text("%s", name.c_str());
    
        };

};


#endif