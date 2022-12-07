# ESP32-AC-Generator-Power-Monitor
Very low budget method of monitoring how much current (Amps) the portable generator is putting out when connected to house.
It is not highly accurate, doesn't calculate Power, etc.  The main point is it simple for DIYer to implement, very low cost , does not connect to any external server, internet is not used (but local wifi is needed for the local house network, and you can see the current (Amps) readings using any web browser.  It is good enough to get a ball park idea if I am getting close to the max generator output.

This C++ program executes on ESP32-PICO-KIT V4.  Arduino IDE select board: DOIT ESP32 DEV1 40MHz
Latest compile was Nov 2022 using Arduino IDE 1.8.13 with ESP32 core 1.0.1 from the IDE Boards Manager.
Inspired by the following code, modified for my needs:
 https://github.com/codetiger/PowerConsumptionMonitor-ESP32/blob/main/PowerConsumptionMonitor/PowerConsumptionMonitor.ino

The Generator is connected to a input receptacle box on the outside of the house which then feeds 
two 30A circuit breakers in the main breaker panel in the garage.  There is a mechanical lockout so the power 
can come from either the Generator or the Power Utility Company, but not both at the same time.  The 
Generator outputs 240VAC at 14A max for each of  the two phases of 120VAC.  The two sct-013-030 split core current 
transformers are clamped around the two Generator hot wires and connected to two analog input pins on the ESP32.

The user needs to edit the file config.h which has the wifi passwords and esp32 pin definitions.

Note, I did not calibrate my system once built.  I just used the default values in the files.   I checked it against a couple other cheap meters and readings were within 6%.
