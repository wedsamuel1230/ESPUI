/*
 * ESPUI Minimal Example for Raspberry Pi Pico W (RP2040)
 * 
 * A simple GUI test for RP2040/RP2350 support.
 * This demonstrates basic ESPUI controls working on Pico W.
 * 
 * Hardware: Raspberry Pi Pico W
 * Framework: Arduino with arduino-pico core
 * 
 * Setup Instructions:
 * 1. Install arduino-pico core in Arduino IDE:
 *    https://github.com/earlephilhower/arduino-pico/releases/download/global/package_rp2040_index.json
 * 2. Open this project in Arduino IDE 
 * 3. Edit WiFi credentials below
 * 4. Build and upload to Pico W
 * 5. Open the IP address shown in Serial Monitor
 */

#include <Arduino.h>
#include <ESPUI.h>

// WiFi credentials - UPDATE THESE FOR YOUR NETWORK
const char* ssid = "YourWiFiSSID";
const char* password = "YourWiFiPassword";

// Control IDs
uint16_t statusLabelId = 0;
uint16_t button1Id = 0;
uint16_t button2Id = 0;
uint16_t slider1Id = 0;
uint16_t switch1Id = 0;

void setup()
{
    // Serial for debugging
    Serial.begin(115200);
    delay(1000);
    Serial.println("\n=== ESPUI Pico GUI Test ===\n");

    // Initialize WiFi
    Serial.print("Connecting to WiFi...");
    Serial.println(ssid);
    
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    
    Serial.println("");
    Serial.print("WiFi connected! IP: ");
    Serial.println(WiFi.localIP());

#if ESPUI_USING_ASYNC
    Serial.println("Mode: Async WebServer (ESP32/ESP8266)");
#else
    Serial.println("Mode: Synchronous WebServer (RP2040/RP2350)");
#endif

    // Initialize ESPUI with a basic setup
    // Uses built-in UI (no filesystem required for testing)
    ESPUI.begin("Pico GUI Test");

    // ===== Create Controls =====
    
    // Status Label - shows current state
    statusLabelId = ESPUI.label("Status:", ControlColor::None, "Ready");
    
    // Button 1 - toggles LED/test
    button1Id = ESPUI.button("Toggle LED", [](Control* sender, int value)
    {
        Serial.printf("Button 1 pressed! Value: %d\n", value);
        
        if (value == B_DOWN)
        {
            ESPUI.updateLabel(statusLabelId, "Button Pressed!");
            Serial.println("LED ON simulation");
        }
        else
        {
            ESPUI.updateLabel(statusLabelId, "Ready");
            Serial.println("LED OFF simulation");
        }
    }, ControlColor::Primary);

    // Button 2 - another test button
    button2Id = ESPUI.button("Test Action", [](Control* sender, int value)
    {
        Serial.printf("Button 2 pressed! Value: %d\n", value);
        ESPUI.updateLabel(statusLabelId, "Test Action!");
    }, ControlColor::Success);

    // Slider - test slider control
    slider1Id = ESPUI.slider("Brightness", [](Control* sender, int value)
    {
        Serial.printf("Slider value: %d\n", value);
        char buf[32];
        snprintf(buf, sizeof(buf), "Brightness: %d%%", value);
        ESPUI.updateLabel(statusLabelId, buf);
    }, ControlColor::Warning, 50, 0, 100);

    // Switch/Checkbox - toggle
    switch1Id = ESPUI.switchTo("Enable Feature", [](Control* sender, int value)
    {
        Serial.printf("Switch value: %d\n", value);
        if (value == 1)
        {
            ESPUI.updateLabel(statusLabelId, "Feature Enabled");
        }
        else
        {
            ESPUI.updateLabel(statusLabelId, "Feature Disabled");
        }
    }, ControlColor::Info, 0);

    Serial.println("\n=== ESPUI Controls Created ===");
    Serial.print("Open http://");
    Serial.print(WiFi.localIP());
    Serial.println(" in your browser\n");
}

void loop()
{
    // Required for synchronous WebServer on RP2040
#if !ESPUI_USING_ASYNC
    if (ESPUI.syncServer)
    {
        ESPUI.syncServer->handleClient();
    }
#endif

    // Small delay to prevent watchdog issues
    delay(10);
}
