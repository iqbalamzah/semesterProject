#include <Arduino.h>
#include <WiFi.h>
#include <Firebase_ESP_Client.h>
#include <Adafruit_Sensor.h>
// #include <Wire.h>
#include <DHT.h> // DHT sensor library

// DHT Sensor type
#define DHTTYPE DHT22

// Replace with your Wi-Fi credentials and Firebase details
#define WIFI_SSID "SENTRA SURYA"
#define WIFI_PASSWORD "sseP2APC"
#define API_KEY "AIzaSyC-W0eoIgtE7_tPaCGRexpJ0hgeumMEWLs"
#define DATABASE_URL "https://iot-unchpamparam-bb8be-default-rtdb.firebaseio.com/" // Example: https://your-project-id.firebaseio.com/
#define USER_EMAIL "iotunchpamparam@gmail.com"
#define USER_PASSWORD "makankuy"

// Define DHT22 sensor pins
#define DHTPIN1 17
// #define DHT_PIN2 D6

// Define Sensor PIR
#define sensorPIR 4

// Initialize Firebase and DHT sensors
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

DHT dht1(DHTPIN1, DHTTYPE); // DHT sensor 1 on 17

unsigned long sendDataPrevMillis = 0;
int delayBetweenPosts = 10000; // Delay between posts (10 seconds)

void setup()
{
  Serial.begin(115200);

  // Connect to Wi-Fi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi...");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.println("Connected to Wi-Fi");

  // Firebase configuration
  config.api_key = API_KEY;
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;
  config.database_url = DATABASE_URL;

  // Begin Firebase connection
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);

  // Initialize DHT sensors
  dht1.begin();
}

void loop()
{
  if (Firebase.ready() && (millis() - sendDataPrevMillis > delayBetweenPosts || sendDataPrevMillis == 0))
  {
    sendDataPrevMillis = millis();

    // Read temperature and humidity from DHT22 sensor 1
    float temperature1 = dht1.readTemperature();
    float humidity1 = dht1.readHumidity();

    // Check if readings are valid
    if (isnan(temperature1) || isnan(humidity1))
    {
      Serial.println("Failed to read from DHT sensor!");
      return;
    }

    // Log sensor data to Serial Monitor
    Serial.println("Sensor 1 - Temp: " + String(temperature1) + "°C, Humidity: " + String(humidity1) + "%");

    // Upload sensor 1 data to Firebase using correct method
    String path1 = "/Sensor Suhu dan Temprature";
    if (Firebase.RTDB.setFloat(&fbdo, path1 + "/temperature", temperature1))
    {
      Serial.println("Sensor 1 temperature uploaded successfully");
    }
    else
    {
      Serial.println("Failed to upload Sensor 1 temperature: " + fbdo.errorReason());
    }

    if (Firebase.RTDB.setFloat(&fbdo, path1 + "/humidity", humidity1))
    {
      Serial.println("Sensor 1 humidity uploaded successfully");
    }
    else
    {
      Serial.println("Failed to upload Sensor 1 humidity: " + fbdo.errorReason());
    }
  }
}