#include <Arduino.h>
#include "WiFi.h"
#include "esp_http_client.h"
#include "esp_log.h"
#include "FS.h"
#include "SPIFFS.h"

// Các thông tin WiFi và API
#define WIFI_SSID "VU HIEU"
#define WIFI_PASS "0702394087"
#define TAG "HTTP_CLIENT"
#define API_URL "https://fec8-171-225-185-52.ngrok-free.app/create_payment/"
#define POST_DATA "{\"mode\":\"fast\",\"amount\":50000}"

// Hàm xử lý sự kiện HTTP
esp_err_t _http_event_handler(esp_http_client_event_t *evt)
{
  switch (evt->event_id)
  {
  case HTTP_EVENT_ERROR:
    ESP_LOGE(TAG, "HTTP_EVENT_ERROR");
    break;
  case HTTP_EVENT_ON_CONNECTED:
    ESP_LOGI(TAG, "HTTP_EVENT_ON_CONNECTED");
    break;
  case HTTP_EVENT_HEADER_SENT:
    ESP_LOGI(TAG, "HTTP_EVENT_HEADER_SENT");
    break;
  case HTTP_EVENT_ON_HEADER:
    ESP_LOGI(TAG, "HTTP_EVENT_ON_HEADER, key=%s, value=%s", evt->header_key, evt->header_value);
    break;
  case HTTP_EVENT_ON_DATA:
    ESP_LOGI(TAG, "HTTP_EVENT_ON_DATA, len=%d", evt->data_len);
    if (evt->data_len < 512)
    {
      printf("%.*s\n", evt->data_len, (char *)evt->data);
    }
    break;
  case HTTP_EVENT_ON_FINISH:
    ESP_LOGI(TAG, "HTTP_EVENT_ON_FINISH");
    break;
  case HTTP_EVENT_DISCONNECTED:
    ESP_LOGI(TAG, "HTTP_EVENT_DISCONNECTED");
    break;
  }
  return ESP_OK;
}

// Hàm đọc chứng chỉ từ tệp
const char* read_cert(const char* path) {
  File certFile = SPIFFS.open(path, "r");
  if (!certFile) {
    ESP_LOGE(TAG, "Failed to open certificate file: %s", path);
    return nullptr;
  }
  String cert = certFile.readString();
  certFile.close();
  return cert.c_str();
}

// Hàm gửi HTTP POST request có chứng chỉ SSL
void send_http_post()
{
  // Đọc chứng chỉ từ tệp
  const char* server_cert = read_cert(SERVER_CERT_PATH);
  const char* ca_cert = read_cert(CA_CERT_PATH);

  if (!server_cert || !ca_cert) {
    ESP_LOGE(TAG, "Chứng chỉ không hợp lệ");
    return;
  }

  // Khởi tạo cấu trúc esp_http_client_config_t mà không dùng designated initializers
  esp_http_client_config_t config;
  config.url = API_URL;                     // Cấu hình URL
  config.event_handler = _http_event_handler; // Hàm xử lý sự kiện
  config.cert_pem = server_cert;             // Chứng chỉ server
  config.client_cert_pem = ca_cert;          // Chứng chỉ CA
  config.method = HTTP_METHOD_POST;          // Phương thức HTTP

  esp_http_client_handle_t client = esp_http_client_init(&config);

  if (client == NULL)
  {
    ESP_LOGE(TAG, "Không thể khởi tạo HTTP client");
    return;
  }

  esp_http_client_set_header(client, "Content-Type", "application/json");
  esp_http_client_set_post_field(client, POST_DATA, strlen(POST_DATA));

  esp_err_t err = esp_http_client_perform(client);
  if (err == ESP_OK)
  {
    ESP_LOGI(TAG, "HTTP POST Status = %d", esp_http_client_get_status_code(client));
  }
  else
  {
    ESP_LOGE(TAG, "Yêu cầu HTTP POST thất bại: %s", esp_err_to_name(err));
  }

  esp_http_client_cleanup(client);
}

// Hàm kết nối WiFi
void connect_wifi()
{
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  Serial.print("Connecting to WiFi...");
  int attempt = 0;
  while (WiFi.status() != WL_CONNECTED && attempt < 20)
  {
    delay(1000);
    Serial.print(".");
    attempt++;
  }

  if (WiFi.status() == WL_CONNECTED)
  {
    Serial.println("\nConnected to WiFi!");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
  }
  else
  {
    Serial.println("\nFailed to connect to WiFi.");
  }
}

// Hàm setup()
void setup()
{
  Serial.begin(115200);
  
  // Khởi tạo SPIFFS
  if (!SPIFFS.begin(true)) {
    Serial.println("SPIFFS Mount Failed");
    return;
  }

  connect_wifi();
  delay(2000);
  send_http_post();
}

// Hàm loop() (không làm gì)
void loop()
{
}
