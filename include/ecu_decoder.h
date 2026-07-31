#ifndef ECU_DECODER_H
#define ECU_DECODER_H



#include <iostream>
#include <vector>
#include <iomanip>

#include <cstdint>

//ECUbufferdan okunan değer
struct CAN_Frame {
    uint32_t id;
    uint8_t dlc;
    uint8_t data[8];
};


class ECU_Decoder {
    private:
    const uint32_t ENGINE_MSG_ID = 0X1A6;
    const int MAX_SAFE_RPM= 6000;


    void trigger_fuel_cut() {
        std::cout << "FUEL CUT TRIGGERED" << std::endl;
    }


    public:
    void process_frame(const CAN_Frame& frame){
        if(frame.id != ENGINE_MSG_ID) return;
      
        uint16_t raw_rpm = (frame.data[2] << 8) | frame.data[3];

        uint8_t speed = frame.data[4];
        int temperature = frame.data[5] - 40;

        std::cout << " (ID: 0x" << std::hex << frame.id << std::dec << ")\n";
        std::cout << "   Devir: " << raw_rpm << " RPM\n";
        std::cout << "   Hz:   " << (int)speed << " km/h\n";
        std::cout << "   Sicaaklik: " << temperature << " C\n";


        if (raw_rpm > MAX_SAFE_RPM) {
            std::cout << "   [UYARI] Motor Redline sinirini asti\n";
            trigger_fuel_cut();
        }

        std::cout << "-------------------------------------------\n";
    }
};
#endif


