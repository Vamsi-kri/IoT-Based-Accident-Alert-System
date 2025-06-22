# IoT-Based-Accident-Alert-System
With road safety being a critical global concern, this project aims to reduce emergency response times and minimize accident impact through automated alerting. The system detects sudden deceleration or rollover using the MPU6050 sensor, determines the exact location via GPS, and sends alerts using IFTTT automation.

## 🧠 Future Enhancements

- 🔍 ML-based accident prediction from historical data
- 📡 GSM-based alert fallback for offline zones
- 🛣️ Traffic hazard warning dashboard
- 📲 Mobile app integration

## 🧠 Key Features

- 🚗 Real-time **accident detection**
- 📍 Accurate **location tracking**
- ✉️ **Automated alerting** via email/SMS using IFTTT
- 📶 **Wi-Fi connectivity** with NodeMCU
- 💡 Scalable, low-cost, and easy to deploy

## ⚙️ Components Used

| Component     | Description                              |
|---------------|------------------------------------------|
| NodeMCU (ESP8266) | Wi-Fi microcontroller for processing |
| MPU6050       | Accelerometer + Gyroscope sensor         |
| Neo-6M GPS    | Gets accurate location data              |
| LCD Display   | (Optional) Display status in-vehicle     |
| IFTTT Service | Sends alerts to emergency contacts       |

## 🧩 System Architecture

1. Monitor acceleration/orientation with **MPU6050**
2. Fetch live location using **GPS**
3. Send alerts using **Wi-Fi + IFTTT**
4. Emergency contacts receive **email/SMS with Google Maps link**
