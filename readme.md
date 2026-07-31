# CAN-Bus ECU Decoder & Telemetry Simulator

A lightweight, C++ based CAN-Bus frame decoder designed to simulate and process automotive ECU telemetry data. 

Currently, the project operates offline by parsing standard CAN dump files. The core logic (byte extraction, engine protection, and real-time monitoring) is structured to be easily ported to physical vehicle networks using SocketCAN in embedded Linux environments.

## Features
* **Raw Frame Parsing:** Converts hex strings into standard CAN frames (ID, DLC, 8-byte payload).
* **Live CLI Dashboard:** Real-time console UI with an active RPM bar, speed (km/h), and temperature tracking.
* **Engine Protection Logic:** Simulates a hardware-level fuel-cut response when the engine exceeds the defined redline (6000 RPM).
* **Data Generator:** Includes a Python tool to generate realistic, physics-based CAN dump datasets for testing.
* **Cross-Platform Build:** Configured with CMake.

## Data Protocol (ID: 0x1A6)
The decoder specifically listens for the ECU message ID `0x1A6`. The 8-byte payload is mapped as follows:
* `Byte 2-3`: Raw RPM (Bitwise shifted)
* `Byte 4`: Vehicle Speed (Hex to Int)
* `Byte 5`: Engine Temperature (OBD2 Standard: Value - 40)

## Project Structure
```text
.
├── bin/                    # Compiled executable output
├── data/                   # Generated CAN dump files
├── include/
│   ├── ecu_decoder.h       # Core ECU processing logic
│   └── utils.h             # Helper functions (Hex parsing)
├── src/
│   └── main.cpp            # Entry point and file I/O
├── tools/
│   └── generate_dump.py    # Python script for generating test data
└── CMakeLists.txt          # Build configuration