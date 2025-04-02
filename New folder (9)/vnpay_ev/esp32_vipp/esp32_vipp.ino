#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "qrcode.h"

const char* ssid = "your_SSID";
const char* password = "your_PASSWORD";
const char* serverUrl = "https://fc8d-171-225-185-52.ngrok-free.app/create_payment/";

void setup() {
    Serial.begin(115200);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nWiFi connected!");
}

void loop() {
    Serial.println("\nChọn chế độ sạc:");
    Serial.println("1. Sạc nhanh (5.000 VNĐ/kWh)");
    Serial.println("2. Sạc chậm (3.000 VNĐ/kWh)");
    
    while (Serial.available() == 0) {}
    int modeChoice = Serial.parseInt();
    String mode = (modeChoice == 1) ? "fast" : "slow";
    int price = (mode == "fast") ? 5000 : 3000;
    
    Serial.println("Nhập số tiền muốn thanh toán (VNĐ):");
    while (Serial.available() == 0) {}
    int amount = Serial.parseInt();
    
    if (amount < 1000) {
        Serial.println("❌ Số tiền phải từ 1.000 VNĐ trở lên.");
        return;
    }
    float kwh = (float)amount / price;
    Serial.printf("\n👉 Bạn đã chọn chế độ %s với số tiền %d VNĐ.\n", mode.c_str(), amount);
    Serial.printf("⚡ Sẽ sạc được khoảng %.2f kWh.\n", kwh);

    HTTPClient http;
    http.begin(serverUrl);
    http.addHeader("Content-Type", "application/json");
    
    StaticJsonDocument<200> jsonDoc;
    jsonDoc["mode"] = mode;
    jsonDoc["amount"] = amount;
    String jsonString;
    serializeJson(jsonDoc, jsonString);
    
    int httpResponseCode = http.POST(jsonString);
    if (httpResponseCode == 200) {
        String response = http.getString();
        Serial.println("✅ Giao dịch thành công!");
        
        StaticJsonDocument<200> responseDoc;
        deserializeJson(responseDoc, response);
        String paymentUrl = responseDoc["payment_url"].as<String>();
        Serial.println("🔗 URL: " + paymentUrl);
        
        QRCode qrcode;
        uint8_t qrcodeData[qrcode_getBufferSize(3)];
        qrcode_initText(&qrcode, qrcodeData, 3, ECC_MEDIUM, paymentUrl.c_str());
        for (uint8_t y = 0; y < qrcode.size; y++) {
            for (uint8_t x = 0; x < qrcode.size; x++) {
                Serial.print(qrcode_getModule(&qrcode, x, y) ? "██" : "  ");
            }
            Serial.println();
        }
    } else {
        Serial.printf("❌ Lỗi tạo thanh toán: %d\n", httpResponseCode);
    }
    http.end();
}