#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid = "VU HIEU";        // Thay đổi với tên WiFi của bạn
const char* password = "0702394087"; // Thay đổi với mật khẩu WiFi của bạn

void setup() {
  // Bắt đầu giao tiếp nối tiếp
  Serial.begin(115200);

  // Kết nối đến WiFi
  Serial.println("Đang kết nối đến WiFi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Đang kết nối đến WiFi...");
  }
  Serial.println("Đã kết nối đến WiFi");

  // In ra địa chỉ IP của ESP32
  Serial.print("Địa chỉ IP ESP32: ");
  Serial.println(WiFi.localIP());

  // Gửi yêu cầu HTTP POST
  HTTPClient http;
  
  // Thay đổi localhost thành IP của máy tính của bạn
  String serverPath = "http://192.168.101.12:5000/create_payment/";  // Thay 192.168.x.x bằng IP của máy tính của bạn
  Serial.print("Kết nối đến server: ");
  Serial.println(serverPath);
  
  http.begin(serverPath);  // Khởi tạo kết nối đến server

  // Thiết lập header Content-Type cho yêu cầu
  http.addHeader("Content-Type", "application/json");

  // Dữ liệu JSON cần gửi đi
  String payload = "{\"mode\":\"fast\",\"amount\":50000}";

  // Gửi yêu cầu POST và nhận phản hồi
  int httpCode = http.POST(payload);

  // Kiểm tra trạng thái của yêu cầu
  if (httpCode > 0) {
    Serial.printf("HTTP Code: %d\n", httpCode);
    String response = http.getString();  // Nhận phản hồi từ server
    Serial.println("Phản hồi từ server: ");
    Serial.println(response);
  } else {
    Serial.printf("Lỗi: %s\n", http.errorToString(httpCode).c_str());
  }

  http.end();  // Đóng kết nối
}

void loop() {
  // Không làm gì trong loop, chỉ cần gửi yêu cầu trong setup
}
