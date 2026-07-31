#include <iostream>
#include <vector>
#include <iomanip>
#include <fstream>   
#include <sstream>  
#include <string>    
#include "../include/ecu_decoder.h"

// Hex (Onaltılık) metni (Örn: "1A6" veya "C4") C++'ın anlayacağı sayılara çeviren yardımcı fonksiyon
uint32_t hexToUInt(const std::string& hexStr) {
    uint32_t val;
    std::stringstream ss;
    ss << std::hex << hexStr; // Metni onaltılık sistemde algıla
    ss >> val;                // Sayısal değere dönüştür
    return val;
}

int main() {
    std::cout << "=== ECU CAN-BUS started ===\n\n";

    ECU_Decoder decoder;

   
    std::ifstream file("../data/can_dump.txt");

    
    if (!file.is_open()) {
        std::cout << "[HATA] '../data/can_dump.txt' dosyasi bulunamadi!\n";
        std::cout << "Lutfen Python scriptini calistirip dosyayi urettiginizden emin olun.\n";
        std::cin.get();
        return 1;
    }

    std::string line;
    int line_count = 0;

    // Dosyayı okuu
    while (std::getline(file, line)) {
        if (line.empty()) continue; 

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

      
        decoder.process_frame(frame);
        line_count++;
    }

    file.close();
    std::cout << "\n[BILGI] Toplam " << line_count << " satir CAN verisi basariyla okundu ve islendi.\n";
    
    std::cout << "Cikmak icin ENTER tusuna basin...";
    std::cin.get();
    
    return 0;
}