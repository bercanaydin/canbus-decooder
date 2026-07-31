#include <iostream>
#include <vector>
#include <iomanip>
#include "../include/ecu_decoder.h"


int main(){
    std::cout << "=== ECU CAN-BUS started ===\n\n";


    ECU_Decoder decoder;


    std::vector<CAN_Frame> hardware_buffer = {
        {0x1A6, 8, {0x00, 0x00, 0x09, 0xC4, 0x46, 0x82, 0x00, 0x00}},
        
    };


    for(const auto& frame : hardware_buffer){
        decoder.process_frame(frame);
    }


    std::cin.get();
 
}