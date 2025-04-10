// src/main.cpp

#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

const char* ssid = "VU HIEU";
const char* password = "0702394087";
const char* serverCreateUrl = "http://192.168.101.12:5000/api/create-payment";
const char* serverCheckUrl = "http://192.168.101.12:5000/api/check-payment";

String inputMode = "";
int inputAmount = 0;
bool requestSent = false;
String appTransId = "";

void sendPaymentRequest();
void checkPaymentStatus();

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.println("🔌 Đang kết nối Wi-Fi...");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\n✅ Đã kết nối Wi-Fi!");
  Serial.println("Nhập chế độ sạc (1 = chậm, 2 = nhanh):");
}

void loop() {
  if (Serial.available()) {
    String input = Serial.readStringUntil('\n');
    input.trim();

    if (inputMode == "") {
      if (input == "1") inputMode = "slow";
      else if (input == "2") inputMode = "fast";
      else Serial.println("❌ Giá trị không hợp lệ. Nhập 1 hoặc 2");

      if (inputMode != "") {
        Serial.println("✅ Chế độ đã chọn: " + inputMode);
        Serial.println("Nhập số tiền muốn sạc (VNĐ):");
      }
    }
    else if (inputAmount == 0) {
      inputAmount = input.toInt();
      if (inputAmount <= 0) {
        Serial.println("❌ Số tiền không hợp lệ. Nhập lại:");
        inputAmount = 0;
      } else {
        Serial.println("⏳ Đang gửi yêu cầu tạo đơn hàng...");
        sendPaymentRequest();
      }
    }
  }

  if (requestSent) {
    delay(5000); // chờ 5 giây mỗi lần kiểm tra
    checkPaymentStatus();
  }
}

void sendPaymentRequest() {
  if ((WiFi.status() == WL_CONNECTED)) {
    HTTPClient http;
    http.begin(serverCreateUrl);
    http.addHeader("Content-Type", "application/json");

    String jsonPayload;
    StaticJsonDocument<256> doc;
    doc["amount"] = inputAmount;
    doc["mode"] = inputMode;
    serializeJson(doc, jsonPayload);

    int httpCode = http.POST(jsonPayload);

    if (httpCode > 0) {
      String response = http.getString();
      Serial.println("📡 Phản hồi từ server:");
      Serial.println(response);

      StaticJsonDocument<512> resDoc;
      DeserializationError err = deserializeJson(resDoc, response);

      if (!err && resDoc["order_url"].is<String>()) {
        String orderUrl = resDoc["order_url"].as<String>();
        appTransId = resDoc["app_trans_id"].as<String>();
        Serial.println("📲 Vui lòng quét mã QR tại link sau:");
        Serial.println(orderUrl);
        requestSent = true;
      } else {
        Serial.println("❌ Lỗi phản hồi từ server.");
      }
    } else {
      Serial.printf("❌ Lỗi HTTP: %s\n", http.errorToString(httpCode).c_str());
    }
    http.end();
  } else {
    Serial.println("❌ Mất kết nối Wi-Fi");
  }
}

void checkPaymentStatus() {
  if ((WiFi.status() == WL_CONNECTED) && appTransId != "") {
    HTTPClient http;
    http.begin(serverCheckUrl);
    http.addHeader("Content-Type", "application/json");

    String jsonPayload;
    StaticJsonDocument<128> checkDoc;
    checkDoc["app_trans_id"] = appTransId;
    serializeJson(checkDoc, jsonPayload);

    int httpCode = http.POST(jsonPayload);

    if (httpCode > 0) {
      String response = http.getString();
      StaticJsonDocument<512> resDoc;
      DeserializationError err = deserializeJson(resDoc, response);

      if (!err && resDoc["status"] == "paid") {
        Serial.println("\n✅ Thanh toán thành công!");
        Serial.printf("🔢 Số tiền: %d VNĐ\n", resDoc["amount"].as<int>());
        Serial.printf("⚡ Chế độ sạc: %s\n", resDoc["mode"].as<const char*>());
        Serial.printf("⏱️ Thời gian sạc: %d phút\n", resDoc["duration"].as<int>());
        Serial.printf("🔌 Điện tiêu thụ: %.2f kWh\n", resDoc["kwh"].as<float>());

        requestSent = false;
        inputAmount = 0;
        inputMode = "";
        appTransId = "";
        Serial.println("\n--- Giao dịch hoàn tất. Nhập giao dịch mới ---");
        Serial.println("Nhập chế độ sạc (1 = chậm, 2 = nhanh):");
      }
      else {
        Serial.println("⌛ Đang chờ thanh toán...");
      }
    } else {
      Serial.printf("❌ Lỗi khi kiểm tra thanh toán: %s\n", http.errorToString(httpCode).c_str());
    }
    http.end();
  }
}
