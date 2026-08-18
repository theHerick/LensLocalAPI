/*
  =================================================================================
  FIRMWARE ESP32-CAM (AI-THINKER OV2640) - LENSLOCALAPI
  =================================================================================
*/

#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include "esp_camera.h"
#include "mbedtls/base64.h"
#include "soc/soc.h"             
#include "soc/rtc_cntl_reg.h"    

#ifndef RTC_CNTL_BROWNOUT_REG
  #ifdef RTC_CNTL_BROWN_OUT_REG
    #define RTC_CNTL_BROWNOUT_REG RTC_CNTL_BROWN_OUT_REG
  #endif
#endif

// =================================================================================
// 1. CONFIGURAÇÕES DE REDE E FIREBASE
// =================================================================================
const char* WIFI_SSID     = "YOUR_WIFI_SSID";
const char* WIFI_PASSWORD = "YOUR_WIFI_PASSWORD";

const char* FIREBASE_URL  = "https://YOUR_FIREBASE_PROJECT.firebaseio.com";
const char* DEVICE_ID     = "esp32cam_01";

#define STATUS_LED_PIN  33

// =================================================================================
// MODELO DE CÂMERA (AI-THINKER ESP32-CAM)
// =================================================================================
#define CAMERA_MODEL_AI_THINKER

#if defined(CAMERA_MODEL_AI_THINKER)
  #define PWDN_GPIO_NUM     32
  #define RESET_GPIO_NUM    -1
  #define XCLK_GPIO_NUM      0
  #define SIOD_GPIO_NUM     26
  #define SIOC_GPIO_NUM     27
  #define Y9_GPIO_NUM       35
  #define Y8_GPIO_NUM       34
  #define Y7_GPIO_NUM       39
  #define Y6_GPIO_NUM       36
  #define Y5_GPIO_NUM       21
  #define Y4_GPIO_NUM       19
  #define Y3_GPIO_NUM       18
  #define Y2_GPIO_NUM        5
  #define VSYNC_GPIO_NUM    25
  #define HREF_GPIO_NUM     23
  #define PCLK_GPIO_NUM     22
#endif

void initCamera();
void connectWiFi();
String encodeBase64(const uint8_t* data, size_t length);
void captureAndSendProcess();
void checkFirebaseResult(String requestId);

void setup() {
  #ifdef RTC_CNTL_BROWNOUT_REG
    WRITE_PERI_REG(RTC_CNTL_BROWNOUT_REG, 0);
  #endif

  Serial.begin(115200);
  delay(500);

  Serial.println("\n--- LensLocalAPI ESP32-CAM Firmare ---");

  pinMode(STATUS_LED_PIN, OUTPUT);
  digitalWrite(STATUS_LED_PIN, HIGH);

  initCamera();
  connectWiFi();

  for (int i = 3; i >= 1; i--) {
    Serial.printf("Captura em %d segundos...\n", i);
    digitalWrite(STATUS_LED_PIN, LOW);
    delay(200);
    digitalWrite(STATUS_LED_PIN, HIGH);
    delay(800);
  }

  captureAndSendProcess();
}

void loop() {
  delay(1000);
}

void initCamera() {
  pinMode(PWDN_GPIO_NUM, OUTPUT);
  digitalWrite(PWDN_GPIO_NUM, HIGH);
  delay(150);
  digitalWrite(PWDN_GPIO_NUM, LOW);
  delay(150);

  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM; 
  config.pin_sscb_scl = SIOC_GPIO_NUM; 
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;

  if (psramFound()) {
    config.frame_size = FRAMESIZE_VGA;
    config.jpeg_quality = 12;
    config.fb_count = 2;
  } else {
    config.frame_size = FRAMESIZE_CIF;
    config.jpeg_quality = 15;
    config.fb_count = 1;
  }

  esp_err_t err = esp_camera_init(&config);
  
  if (err != ESP_OK) {
    esp_camera_deinit();
    delay(200);
    digitalWrite(PWDN_GPIO_NUM, HIGH);
    delay(200);
    digitalWrite(PWDN_GPIO_NUM, LOW);
    delay(200);

    err = esp_camera_init(&config);
    if (err != ESP_OK) {
      while (true) {
        digitalWrite(STATUS_LED_PIN, !digitalRead(STATUS_LED_PIN));
        delay(100);
      }
    }
  }

  sensor_t * s = esp_camera_sensor_get();
  if (s != NULL) {
    s->set_whitebal(s, 1);
    s->set_awb_gain(s, 1);
    s->set_wb_mode(s, 0);
    s->set_exposure_ctrl(s, 1);
    s->set_aec2(s, 1);
    s->set_gain_ctrl(s, 1);
    s->set_bpc(s, 1);
    s->set_wpc(s, 1);
    s->set_raw_gma(s, 1);
    s->set_lenc(s, 1);
    s->set_brightness(s, 0);     
    s->set_contrast(s, 0);       
    s->set_saturation(s, 0);     
  }
}

void connectWiFi() {
  if (WiFi.status() == WL_CONNECTED) return;

  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);
  delay(100);
  WiFi.mode(WIFI_STA);

  WiFi.setTxPower(WIFI_POWER_19_5dBm);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  int tries = 0;
  while (WiFi.status() != WL_CONNECTED && tries < 40) {
    delay(500);
    digitalWrite(STATUS_LED_PIN, !digitalRead(STATUS_LED_PIN));
    tries++;
  }

  digitalWrite(STATUS_LED_PIN, HIGH);
}

String encodeBase64(const uint8_t* input, size_t length) {
  size_t outputLen = 0;
  mbedtls_base64_encode(NULL, 0, &outputLen, input, length);

  unsigned char * base64Buf = (unsigned char *)malloc(outputLen + 1);
  if (!base64Buf) return "";

  mbedtls_base64_encode(base64Buf, outputLen, &outputLen, input, length);
  base64Buf[outputLen] = '\0';

  String result = String((char*)base64Buf);
  free(base64Buf);
  return result;
}

void captureAndSendProcess() {
  digitalWrite(STATUS_LED_PIN, LOW);

  if (WiFi.status() != WL_CONNECTED) {
    digitalWrite(STATUS_LED_PIN, HIGH);
    return;
  }

  for (int i = 0; i < 4; i++) {
    camera_fb_t * dummy_fb = esp_camera_fb_get();
    if (dummy_fb) {
      esp_camera_fb_return(dummy_fb);
    }
    delay(150);
  }

  camera_fb_t * fb = esp_camera_fb_get();
  if (!fb) {
    digitalWrite(STATUS_LED_PIN, HIGH);
    return;
  }

  String base64Image = encodeBase64(fb->buf, fb->len);
  esp_camera_fb_return(fb);

  if (base64Image.length() == 0) {
    digitalWrite(STATUS_LED_PIN, HIGH);
    return;
  }

  String requestId = "esp_auto_" + String(millis());

  WiFiClientSecure client;
  client.setInsecure();
  
  HTTPClient http;
  String requestUrl = String(FIREBASE_URL) + "/requests/" + requestId + ".json";
  
  http.begin(client, requestUrl);
  http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);
  http.setTimeout(20000);
  http.addHeader("Content-Type", "application/json");

  String jsonPayload = "{\"deviceId\":\"" + String(DEVICE_ID) + "\",\"status\":\"pending\",\"timestamp\":" + String(millis()/1000) + ",\"image\":\"" + base64Image + "\"}";
  
  int httpCodeRequest = http.PUT(jsonPayload);
  
  if (httpCodeRequest == 200 || httpCodeRequest == 204) {
    http.end();
    
    String queueUrl = String(FIREBASE_URL) + "/queue/" + requestId + ".json";
    
    http.begin(client, queueUrl);
    http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);
    http.setTimeout(15000);
    http.addHeader("Content-Type", "application/json");
    
    int httpCodeQueue = http.PUT("true");
    http.end();

    digitalWrite(STATUS_LED_PIN, HIGH);
    
    if (httpCodeQueue == 200 || httpCodeQueue == 204) {
      checkFirebaseResult(requestId);
    }
  } else {
    digitalWrite(STATUS_LED_PIN, HIGH);
    http.end();
  }
}

void checkFirebaseResult(String requestId) {
  WiFiClientSecure client;
  client.setInsecure();
  HTTPClient http;
  
  String resultUrl = String(FIREBASE_URL) + "/requests/" + requestId + ".json";
  
  for (int i = 0; i < 30; i++) {
    delay(1000);
    http.begin(client, resultUrl);
    http.setTimeout(5000);
    int httpCode = http.GET();
    
    if (httpCode == 200) {
      String response = http.getString();
      http.end();
      if (response.indexOf("\"completed\"") > 0) {
        Serial.println(response);
        return;
      }
    } else {
      http.end();
    }
  }
}
