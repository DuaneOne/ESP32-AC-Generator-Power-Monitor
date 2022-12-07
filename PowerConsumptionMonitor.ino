
/*
This C++ program executes on ESP32-PICO-KIT V4.  Arduino IDE select board: DOIT ESP32 DEV1 40MHz
Latest compile was Nov 2022 using Arduino IDE 1.8.13 with ESP32 core 1.0.1 from the IDE Boards Manager.
Inspired by the following code, modified for my needs:
 https://github.com/codetiger/PowerConsumptionMonitor-ESP32/blob/main/PowerConsumptionMonitor/PowerConsumptionMonitor.ino

Creates a simple webpage which displays the AC current(Amps)from the small portable Generator, refreshed every 1sec.
The Generator is connected to a input receptacle box on the outside of the house which then feeds 
two 30A circuit breakers in the main breaker panel in the garage.  There is a mechanical lockout so the power 
can come from either the Generator or the Power Utility Company, but not both at the same time.  The 
Generator outputs 240VAC at 14A max for each of  the two phases of 120VAC.  The two sct-013-030 split core current 
transformers are clamped around the two Generator hot wires and connected to two analog input pins on the ESP32.

The file config.h has the wifi passwords and esp32 pin definitions.
*/


#include <WiFi.h>
#include <WebServer.h>
#include "ESP.h"
// The libraries below this line are modified/unique so keep in the same folder as this file.
#include "EmonLib.h"    // Emonlib converts raw, sinus wave readings from the Current Transformer into amps.
#include "config.h"

WebServer httpServer(80);
EnergyMonitor emon1;    // 1 and 2 are used in this program to differentiate one 120VAC pahse from the other
EnergyMonitor emon2;   


void setup() {
    analogReadResolution(12);  // use esp32 max resolution

    Serial.begin(115200);
    while (!Serial) {}
    Serial.println("Tring to connect to Wifi SSID: ");
    Serial.println(ssid);

    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    if (WiFi.waitForConnectResult() != WL_CONNECTED) {
        Serial.println("WiFi Connect Failed! Rebooting...");
        delay(1000);
        ESP.restart();
    }
    Serial.println("");
    Serial.println("WiFi connected successfully");
    Serial.print("Assigned IP: ");
    Serial.println(WiFi.localIP());

    // Initialize emon library (30 = calibration number) for each phase
    emon1.current(ADC_INPUT1, 30);   // esp32 input pins define in config.h
    emon2.current(ADC_INPUT2, 30);
    Serial.println(ADC_BITS);  // for debug, should be 12 for esp32
  
    httpServer.on("/", handle_request);   // when user goes to this website from any browser
    httpServer.onNotFound(handle_NotFound);   
    
    httpServer.begin();
    Serial.println("HTTP server started");
    delay(100);
}

void handle_request() {      // // called for each new browser user or browser page refresh
    unsigned long time1 = millis();
    double amps1 = emon1.calcIrms(1480);  // (number of samples)
    double amps2 = emon2.calcIrms(1480);
    Serial.print(amps1); Serial.print(" "); Serial.print(amps2); Serial.print(" ");
    Serial.println(millis()-time1);  //  appears to take about 160mS for both calculations
    httpServer.send(200, "text/html", SendHTML(String(amps1), String(amps2)));
    delay(10);
} 


void handle_NotFound(){
  httpServer.send(404, "text/plain", "Not found");
}


// create simple webpage to display the two AC current values in the header.   
String SendHTML(String Amps1, String Amps2){
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr +="<head><meta http-equiv=\"refresh\" content=\"1\">\n";   // tells browser to refresh entire webpage every 1 sec
  ptr +="</head>\n";
  ptr +="<h1>real-time Amps: " + Amps1 + "&nbsp;&nbsp;&nbsp;&nbsp;" + Amps2 + "</h1>\n";  // include 4 blank spaces
  ptr +="</head>\n";
  ptr +="</html>\n";
  return ptr;
}




void loop() {
    
    httpServer.handleClient();   
    delay(10);
}
