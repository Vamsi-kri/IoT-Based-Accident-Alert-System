#include <LiquidCrystal.h>        // For LCD Display
#include <TinyGPS++.h>            // For GPS Module
#include <Wire.h>                 // For I2C communication with MPU6050
#include <ESP8266WiFi.h>          // For WiFi functions

// IFTTT Webhook Configuration
#define HOSTIFTTT "maker.ifttt.com"
#define EVENTO "disturbance"              // Your IFTTT Event Name
#define IFTTTKEY "p7zBBT6FHzN_7-6batwm3rg8v9T4gmrsjXSajuwaDJ1"  // Your IFTTT Key

TinyGPSPlus gps;                  // Create GPS object

// MPU6050 I2C Address and I2C Pins
const uint8_t MPU6050SlaveAddress = 0x68;
const uint8_t scl = D1;
const uint8_t sda = D2;

const uint16_t AccelScaleFactor = 16384;   // MPU6050 accelerometer scale factor
const double MaxValue = 0.35;               // Disturbance threshold (+ve)
const double MinValue = -0.35;              // Disturbance threshold (-ve)

int16_t AccelX, AccelY, AccelZ;             // Raw accelerometer data
int dcount = 0, mailcount = 0, count = 0;   // Counters
unsigned long lastDisturbanceCheck = 0;     // For millis() timing

float latitude, longitude;                  // GPS Coordinates
String lat_str, lng_str;                    // GPS Coordinates as strings for IFTTT

// LCD Pins
const int RS = D0, EN = D3, d4 = D4, d5 = D5, d6 = D6, d7 = D7;
LiquidCrystal lcd(RS, EN, d4, d5, d6, d7);

// WiFi Credentials
const char* ssid = "Enter the SSID";
const char* password = "Enter the password";
WiFiClient client;  // WiFi client for HTTP requests

void setup() {
  Serial.begin(9600);             // Start Serial for debugging
  lcd.begin(16, 2);               // Initialize 16x2 LCD
  lcd.clear();
  lcd.print("Circuit Digest");    // Welcome message
  Wire.begin(sda, scl);           // Start I2C on selected pins
  MPU6050_Init();                 // Initialize MPU6050
  delay(1500);
  lcd.clear();
}

void loop() {
  double Ax, Ay, Az, xvalue, yvalue, zvalue;
  static double xvalue1, yvalue1, zvalue1;
  static double xvalue2, yvalue2, zvalue2;

  // Read accelerometer raw data
  Read_RawValue(MPU6050SlaveAddress, 0x3B);

  // Convert raw values to 'g'
  Ax = (double)AccelX / AccelScaleFactor;
  Ay = (double)AccelY / AccelScaleFactor;
  Az = (double)AccelZ / AccelScaleFactor;

  // Apply calibration offsets
  xvalue = Ax - 1.03;
  yvalue = Ay + 0.06;
  zvalue = Az - 0.07;

  // Store two sets of values for change calculation
  if (dcount % 2 == 0) {
    xvalue1 = xvalue;
    yvalue1 = yvalue;
    zvalue1 = zvalue;
  } else {
    xvalue2 = xvalue;
    yvalue2 = yvalue;
    zvalue2 = zvalue;

    // Calculate differences in acceleration
    double dx = xvalue2 - xvalue1;
    double dy = yvalue2 - yvalue1;
    double dz = zvalue2 - zvalue1;

    // Display acceleration changes on LCD
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Normal Driving");
    delay(1100);
    lcd.setCursor(0, 1);
    lcd.print(dx, 2);
    lcd.setCursor(6, 1);
    lcd.print(dy, 2);
    lcd.setCursor(11, 1);
    lcd.print(dz, 2);
    delay(500);

    // Check if disturbance exceeds threshold and 12 sec gap since last trigger
    if (((dx < MinValue) || (dx > MaxValue) || (dy < MinValue) || (dy > MaxValue) || (dz < MinValue) || (dz > MaxValue)) 
        && (millis() - lastDisturbanceCheck > 12000)) {

      count++;
      Serial.println("Disturbance Detected!");

      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Disturbance!");

      // Only send email if disturbance count high and email not sent more than 2 times
      if (count > 10 && mailcount < 2) {
        connectWiFi();      // Connect to WiFi
        getGPSData();       // Get GPS location
        sendIFTTTMail();    // Send IFTTT email
        mailcount++;
      }

      lastDisturbanceCheck = millis();  // Update timer
    }
  }
  dcount++;
}

// Function to write single byte to MPU6050 register
void I2C_Write(uint8_t deviceAddress, uint8_t regAddress, uint8_t data) {
  Wire.beginTransmission(deviceAddress);
  Wire.write(regAddress);
  Wire.write(data);
  Wire.endTransmission();
}

// Function to read raw accelerometer data (14 bytes)
void Read_RawValue(uint8_t deviceAddress, uint8_t regAddress) {
  Wire.beginTransmission(deviceAddress);
  Wire.write(regAddress);
  Wire.endTransmission();
  Wire.requestFrom(deviceAddress, (uint8_t)14);

  AccelX = (((int16_t)Wire.read() << 8) | Wire.read());
  AccelY = (((int16_t)Wire.read() << 8) | Wire.read());
  AccelZ = (((int16_t)Wire.read() << 8) | Wire.read());
  Wire.read(); Wire.read(); // Skip temperature
  Wire.read(); Wire.read(); // Skip GyroX
  Wire.read(); Wire.read(); // Skip GyroY
  Wire.read(); Wire.read(); // Skip GyroZ
}

// MPU6050 Configuration Registers
void MPU6050_Init() {
  delay(150);
  I2C_Write(MPU6050SlaveAddress, 0x19, 0x07);  // Sample Rate Divider
  I2C_Write(MPU6050SlaveAddress, 0x6B, 0x01);  // Power Management
  I2C_Write(MPU6050SlaveAddress, 0x6C, 0x00);
  I2C_Write(MPU6050SlaveAddress, 0x1A, 0x00);  // Configuration
  I2C_Write(MPU6050SlaveAddress, 0x1B, 0x00);  // Gyro Config
  I2C_Write(MPU6050SlaveAddress, 0x1C, 0x00);  // Accel Config
  I2C_Write(MPU6050SlaveAddress, 0x23, 0x00);  // FIFO Enable
  I2C_Write(MPU6050SlaveAddress, 0x38, 0x01);  // Interrupt Enable
  I2C_Write(MPU6050SlaveAddress, 0x68, 0x00);  // Signal Path Reset
  I2C_Write(MPU6050SlaveAddress, 0x6A, 0x00);  // User Control
}

// Connect to WiFi Network with retry logic
void connectWiFi() {
  Serial.println("Connecting to WiFi...");
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Connecting WiFi");

  WiFi.begin(ssid, password);
  int retry = 0;
  while (WiFi.status() != WL_CONNECTED && retry < 20) {
    delay(500);
    Serial.print(".");
    retry++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nWiFi Connected.");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("WiFi Connected");
  } else {
    Serial.println("\nWiFi Failed.");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("WiFi Failed");
  }
  delay(1000);
}

// Read GPS data with timeout
void getGPSData() {
  Serial.println("Getting GPS Data...");
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Getting GPS");

  unsigned long startTime = millis();
  bool gpsFound = false;

  while (millis() - startTime < 5000) {  // 5-second timeout
    while (Serial.available() > 0) {
      gps.encode(Serial.read());
    }
    if (gps.location.isUpdated()) {
      latitude = gps.location.lat();
      longitude = gps.location.lng();
      lat_str = String(latitude, 6);
      lng_str = String(longitude, 6);

      Serial.print("Lat: "); Serial.println(lat_str);
      Serial.print("Lon: "); Serial.println(lng_str);

      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(lat_str);
      lcd.setCursor(0, 1);
      lcd.print(lng_str);

      gpsFound = true;
      break;
    }
  }

  if (!gpsFound) {
    Serial.println("GPS Data Not Found.");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("GPS Not Found");
  }
  delay(1000);
}

// Send an IFTTT webhook email with GPS data
void sendIFTTTMail() {
  if (client.connected()) client.stop();
  client.flush();

  Serial.println("Sending IFTTT Email...");
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Sending Email");

  if (client.connect(HOSTIFTTT, 80)) {
    String toSend = "GET /trigger/" + String(EVENTO) + "/with/key/" + String(IFTTTKEY);
    toSend += "?value1=" + lat_str;
    toSend += "&value2=" + lng_str;
    toSend += " HTTP/1.1\r\nHost: " + String(HOSTIFTTT) + "\r\nConnection: close\r\n\r\n";

    client.print(toSend);
    delay(250);

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Email Sent");

    Serial.println("IFTTT Trigger Sent.");
  } else {
    Serial.println("IFTTT Connection Failed!");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Email Failed");
  }

  client.flush();
  client.stop();
  delay(1000);
}
