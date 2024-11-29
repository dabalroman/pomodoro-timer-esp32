#pragma clang diagnostic push
#pragma ide diagnostic ignored "readability-static-accessed-through-instance"

#include "DeviceState.h"
#include "PreferencesManager.h"
#include "Touch/TouchManager.h"
#include "Views/Settings/EditTimerView.h"
#include "Views/FinishView.h"
#include "Views/MainView.h"
#include "Views/TimerView.h"
#include "LEDManager/LEDManager.h"
#include "Views/SettingsView.h"
#include "Views/Settings/EditPomodoroTargetView.h"
#include "ESPAsyncWebServer.h"
#include "Fonts/FreeMonoBold18pt7b.h"
#include <Adafruit_SSD1306.h>
#include <Arduino.h>
#include <Wire.h>
#include <WiFi.h>

#define TOUCH_THRESHOLD 1500

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define WIFI_AP_SSID "ESP32_Setup" // SSID for the Access Point
#define WIFI_AP_PASSWORD "12345678" // Password for the Access Point

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

ulong lastTickMs = 0;
ulong countdownStartTickMs = 0;
ulong countdownStartValueMs = 25 * 60 * 1000;
ulong lastScreenUpdate = 0;
uint8_t pomodoroTargetAmount = 0;
uint8_t pomodoroFinishedAmount = 0;

ulong lastSyncTimestamp = 0;
const ulong syncInterval = 24 * 60 * 60 * 1000; // 24 hours in milliseconds

AsyncWebServer server(80);

TouchManager touch;
PreferencesManager preferencesManager;
DeviceState deviceState = ready;
LEDManager ledManager(lastTickMs);

MainView mainView(deviceState, display, touch, ledManager, lastTickMs, countdownStartValueMs, countdownStartTickMs,
                  pomodoroTargetAmount, pomodoroFinishedAmount);
SettingsView settingsView(deviceState, display, touch, ledManager, lastTickMs, preferencesManager);
EditTimerView editTimerView(
        deviceState, display, touch, ledManager, lastTickMs, preferencesManager, countdownStartValueMs);
EditPomodoroTargetView editPomodoroTargetView(
        deviceState, display, touch, ledManager, lastTickMs, preferencesManager, pomodoroTargetAmount);
TimerView timerView(deviceState, display, touch, ledManager, lastTickMs, countdownStartValueMs, countdownStartTickMs);
FinishView finishView(deviceState, display, touch, ledManager, lastTickMs, pomodoroFinishedAmount);

void triggerTouchLeft() {
    touch.leftButton.trigger();
}

void triggerTouchSelect() {
    touch.selectButton.trigger();
}

void triggerTouchRight() {
    touch.rightButton.trigger();
}

void startWiFiSetup();

bool connectToSavedWiFi();

void setup() {
    Serial.begin(115200);

    ledManager.setup();

    // Setup display
    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
        Serial.println(F("SSD1306 allocation failed"));
        ledManager.setState(LEDManagerState::countdownPaused);

        for (;;) {
            ledManager.update();
            delay(1);
        }
    }

    delay(2000);
    display.clearDisplay();
    display.setFont(&FreeMonoBold18pt7b);
    display.setTextColor(WHITE);


    // Setup touch
    touchAttachInterrupt(touch.leftButton.getGPIO(), triggerTouchLeft, TOUCH_THRESHOLD);
    touchAttachInterrupt(touch.selectButton.getGPIO(), triggerTouchSelect, TOUCH_THRESHOLD);
    touchAttachInterrupt(touch.rightButton.getGPIO(), triggerTouchRight, TOUCH_THRESHOLD);


    // Setup environment
    lastTickMs = millis();
    countdownStartTickMs = lastTickMs;
    countdownStartValueMs = preferencesManager.getCountdownStartValueMs();
    pomodoroTargetAmount = preferencesManager.getTargetPomodoroAmount();


    if (preferencesManager.getWiFiSSID().length() == 0) {
        startWiFiSetup();
    } else {
        connectToSavedWiFi();
    }
}

View *getCurrentView() {
    switch (deviceState) {
        case DeviceState::edit_timeMinutes:
        case DeviceState::edit_timeSeconds:
            return &editTimerView;

        case DeviceState::edit_pomodoroTarget:
            return &editPomodoroTargetView;

        case DeviceState::settings:
            return &settingsView;

        case DeviceState::ready:
            return &mainView;

        case DeviceState::counting:
            return &timerView;

        case DeviceState::finish:
            return &finishView;
    }

    return &mainView;
}

void notFound(AsyncWebServerRequest *request) {
    request->send(404, "text/plain", "Not found");
}


void startWiFiSetup() {
    WiFi.softAP(WIFI_AP_SSID, WIFI_AP_PASSWORD);

    IPAddress IP = WiFi.softAPIP();
    Serial.print("AP IP Address: ");
    Serial.println(IP);

    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(200, "text/html",
                      "<html><body><h1>Wi-Fi Setup</h1>"
                      "<form action='/connect' method='POST'>"
                      "SSID: <input type='text' name='ssid'><br>"
                      "Password: <input type='password' name='password'><br>"
                      "<input type='submit' value='Connect'>"
                      "</form></body></html>"
        );
    });

    // Send a POST request to <IP>/post with a form field message set to <message>
    server.on("/connect", HTTP_POST, [](AsyncWebServerRequest *request) {
        String ssid = "";
        String password = "";
        if (request->hasParam("ssid", true)) {
            ssid = request->getParam("ssid", true)->value();
        }

        if (request->hasParam("password", true)) {
            password = request->getParam("password", true)->value();
        }

        if (ssid.length() == 0 || password.length() == 0) {
            request->redirect("/");
        }

        preferencesManager.saveWiFiSSID(ssid);
        preferencesManager.saveWiFiPassword(password);

        request->send(200, "text/plain", "Trying to connect to Wifi network...");

        ESP.restart();
    });

    server.onNotFound(notFound);

    server.begin();

    Serial.println("HTTP server started");
}

bool connectToSavedWiFi() {
    String ssid = preferencesManager.getWiFiSSID();
    String password = preferencesManager.getWiFiPassword();

    if (ssid.length() == 0) {
        Serial.println("No saved Wi-Fi credentials");
        return false;
    }

    display.clearDisplay();
    display.setFont();
    display.println("CONNECTING TO WIFI");

    WiFi.begin(ssid.c_str(), password.c_str());
    Serial.print("Connecting to Wi-Fi");

    for (int i = 0; i < 20; i++) {
        if (WiFi.status() == WL_CONNECTED) {
            Serial.println("\nConnected to Wi-Fi");
            Serial.print("IP Address: ");
            Serial.println(WiFi.localIP());

            display.clearDisplay();
            display.setFont();
            display.println("CONNECTED");
            return true;
        }

        delay(500);
        Serial.print(".");
    }

    Serial.println("\nFailed to connect to Wi-Fi");
    return false;
}

void disconnectWiFi() {
    WiFi.disconnect(true);
    WiFi.mode(WIFI_OFF); // Turn off Wi-Fi to save power
    Serial.println("Wi-Fi disconnected");
}

void syncTimeWithNTP() {
    if (!connectToSavedWiFi()) {
        Serial.println("Failed to connect to Wi-Fi for NTP sync.");
        return;
    }

    display.clearDisplay();
    display.setFont();
    display.println("NTP SYNC");

    // NTP Server and timezone configuration
    const char *ntpServer = "pool.ntp.org";
    const long gmtOffset_sec = 0; // Replace with your timezone offset (e.g., -3600 for GMT-1)
    const int daylightOffset_sec = 3600; // Daylight savings time offset (if applicable)

    // Configure NTP
    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

    // Wait for time to be set
    struct tm timeInfo;
    if (!getLocalTime(&timeInfo, 10000)) { // Timeout after 10 seconds
        Serial.println("Failed to obtain time from NTP server.");
        disconnectWiFi();
        return;
    }

    // Convert the time to Unix timestamp
    time_t currentTimestamp = mktime(&timeInfo);
    Serial.print("Current Timestamp: ");
    Serial.println(currentTimestamp);

    // Save the timestamp
    preferencesManager.saveCountdownStartValueMs(static_cast<ulong>(currentTimestamp));
    Serial.println("Timestamp saved to preferences.");

    // Print the human-readable time
    Serial.println(&timeInfo, "%A, %B %d %Y %H:%M:%S");

    // Disconnect Wi-Fi
    disconnectWiFi();
}

void loop() {
    lastTickMs = millis();
//
//    if (millis() - lastSyncTimestamp >= syncInterval) {
//        syncTimeWithNTP();
//        lastSyncTimestamp = millis();
//        return;
//    }

    touch.update();

    // Touch feedback is the most important one, so let it override other values before render
    if (touch.leftButton.isTouched()) {
        ledManager.setState(
                touch.leftButton.isLongTouch() ? LEDManagerState::longTouchLeft : LEDManagerState::touchLeft);
    } else if (touch.selectButton.isTouched()) {
        ledManager.setState(
                touch.selectButton.isLongTouch() ? LEDManagerState::longTouchSelect : LEDManagerState::touchSelect);
    } else if (touch.rightButton.isTouched()) {
        ledManager.setState(
                touch.rightButton.isLongTouch() ? LEDManagerState::longTouchRight : LEDManagerState::touchRight);
    }

    ledManager.update();

    // Handle input with higher polling to get more tactile UX
    View *currentView = getCurrentView();
    currentView->handleInput();

    if (lastTickMs - lastScreenUpdate >= 50) {
        lastScreenUpdate = lastTickMs;

        currentView->render();
        display.display();
    }
}

#pragma clang diagnostic pop
