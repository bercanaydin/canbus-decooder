#ifndef ECU_DECODER_H
#define ECU_DECODER_H

#include <iostream>
#include <cstdint>
#include <cstdlib>
#include <thread>
#include <chrono>
#include <string>
#include "VehicleData.h" // Ortak veri yapısını dahil ediyoruz

struct CAN_Frame {
    uint32_t id;
    uint8_t dlc;
    uint8_t data[8];
};

class ECU_Decoder {
private:
    const uint32_t ENGINE_MSG_ID = 0x1A6;
    const int MAX_SAFE_RPM = 6000;

    void trigger_fuel_cut() {
        std::cout << "  [!] DONANIM UYARISI: Yakit kesme (Fuel-Cut) AKTIF!\n";
    }

public:
   
    void process_frame(const CAN_Frame& frame) {
        VehicleState dummy_state;
        process_frame(frame, dummy_state); // Eski çalışan yapıyı bozmamak için boş state ile tetikler
    }

    // ImGui ve konsolu birlikte besleyen ana fonksiyon
    void process_frame(const CAN_Frame& frame, VehicleState& state) {
        if (frame.id != ENGINE_MSG_ID) return;

        uint16_t raw_rpm = (frame.data[2] << 8) | frame.data[3];
        uint8_t speed = frame.data[4];
        int temperature = frame.data[5] - 40;

        // 1. Ortak Veri Havuzunu Güncelle (ImGui buradan okuyacak)
        state.rpm = raw_rpm;
        state.speed = speed;
        state.temperature = temperature;
        state.is_redline = (raw_rpm > MAX_SAFE_RPM);

        // 2. MEVCUT KONSOL / DEBUG KODLARINIZ (Hiçbirine dokunulmadı, aynen çalışıyor)
        system("cls");

        int max_rpm = 8000;
        int bar_count = (raw_rpm * 20) / max_rpm;
        if (bar_count > 20) bar_count = 20;

        std::string rpm_bar(bar_count, '|');
        std::string empty_bar(20 - bar_count, ' ');
        
        std::cout << " (Mesaj ID: 0x" << std::hex << frame.id << std::dec << ")\n\n";

        std::cout << "  DEVIR: [" << rpm_bar << empty_bar << "] " << raw_rpm << " RPM\n";
        std::cout << "  HIZ:   " << (int)speed << " km/h\n";
        std::cout << "  ISI:   " << temperature << " C\n\n";

        if (raw_rpm > MAX_SAFE_RPM) {
            std::cout << "  [warn] Motorasti!\n";
            trigger_fuel_cut();
        } else {
            std::cout << "  [OK] .\n";
            std::cout << "\n";
        }
        std::cout << "===========================================\n";

        std::this_thread::sleep_for(std::chrono::milliseconds(3));
    }
};

#endif