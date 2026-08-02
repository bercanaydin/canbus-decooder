#ifndef CLIMA_CONTROL_H
#define CLIMA_CONTROL_H




#include <cstdint>
class ClimaControl
{
public:
   uint32_t   control;
    float cabin_tempeerature;
    bool ac_status;

    Climacontrol(){
        control = 0;
        cabin_tempeerature = 0.0f;
        ac_status = true;
    }
   
    
    void update_status(uint32_t rawdata){
        control = rawdata;
    }
};




#endif