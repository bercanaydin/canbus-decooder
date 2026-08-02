#ifndef HMI_TYPES_H
#define HMI_TYPES_H

#include "imgui.h"


enum class HMI_Tab {
    Dashboard,
    Climate,
    Diagnostics 
};


namespace HMI_Colors {
    inline const ImVec4 Background = ImVec4(0.04f, 0.06f, 0.10f, 1.00f);
    inline const ImVec4 TextPrimary = ImVec4(0.85f, 0.93f, 1.00f, 1.00f);
    inline const ImVec4 AccentBlue = ImVec4(0.20f, 0.60f, 1.00f, 1.00f);
    inline const ImVec4 AlertRed = ImVec4(1.00f, 0.20f, 0.20f, 1.00f);
    inline const ImVec4 SafeGreen = ImVec4(0.40f, 1.00f, 0.60f, 1.00f);
}

#endif