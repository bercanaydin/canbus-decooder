import random
import os


BASE_DIR = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
DATA_DIR = os.path.join(BASE_DIR, "data")
DOSYA_YOLU = os.path.join(DATA_DIR, "can_dump.txt")



lineCount= 5000


def create():

    os.makedirs(DATA_DIR, exist_ok=True)


    with open(DOSYA_YOLU, "w", encoding="utf-8") as f:
        rpm = 800      # Rölanti 
        speed = 0      # hız
        temp = 85      # ısı


        print(lineCount, "creating")

        for i in range(lineCount):
                   
                    rpm += random.randint(-50, 150) 
                    if rpm > 6800: 
                        rpm = 4500 # Vites 
                        speed += random.randint(1, 3)
                    
                    if rpm < 800: rpm = 800 # Rölantinin altına inemez
                    
                    
                    speed += random.randint(0, 1) if rpm > 2000 else 0
                    if speed > 220: speed = 220
                    rpm_high = (rpm >> 8) & 0xFF
                    rpm_low = rpm & 0xFF
                    speed_hex = speed & 0xFF
                    temp_hex = (temp + 40) & 0xFF
                    
                 
                    f.write(f"1A6 00 00 {rpm_high:02X} {rpm_low:02X} {speed_hex:02X} {temp_hex:02X} 00 00\n")
                    
                   
                    if random.random() > 0.7:
                        f.write("2B0 01 00 00 00 00 00 00 00\n") 
                    if random.random() > 0.8:
                        f.write("3C4 0A 0B 0C 0D 0E 0F 00 00\n") 
                        
        print(f"basarılı")

  

if __name__ == "__main__":
    create()