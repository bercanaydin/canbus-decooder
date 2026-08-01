#ifndef CAN_READER_H
#define CAN_READER_H

#include <fstream>
#include <sstream>
#include <string>
#include "ecu_decoder.h"
#include "utils.h"

class CAN_Reader {
private:
    std::ifstream file;

public:
    // Sınıf başlarken dosyayı açar
    CAN_Reader(const std::string& filepath) {
        file.open(filepath);
    }

    // Sınıf kapanırken dosyayı kapatır
    ~CAN_Reader() {
        if (file.is_open()) file.close();
    }

    // Dosyadan bir sonraki çerçeveyi okur, başarılıysa true döner
    bool read_next_frame(CAN_Frame& frame) {
        if (!file.is_open()) return false;

        std::string line;
        if (std::getline(file, line) && !line.empty()) {
            std::stringstream ss(line);
            std::string tempStr;

            ss >> tempStr;
            frame.id = hexToUInt(tempStr);
            frame.dlc = 8; 

            for (int i = 0; i < 8; ++i) {
                if (ss >> tempStr) frame.data[i] = static_cast<uint8_t>(hexToUInt(tempStr));
                else frame.data[i] = 0; 
            }
            return true;
        } else {
            // Dosya bittiyse başa sar
            file.clear();
            file.seekg(0, std::ios::beg);
            return false;
        }
    }
};

#endif