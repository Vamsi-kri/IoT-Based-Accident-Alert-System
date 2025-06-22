# IoT-Based Accident Alert System

A smart, IoT-enabled system that detects vehicular accidents in real-time using motion sensors and GPS modules, and sends immediate alerts to emergency contacts and services via the IFTTT platform. Built using NodeMCU (ESP8266), MPU6050 accelerometer/gyroscope, and Neo-6M GPS.

##  Overview

Road safety is a critical global concern. This project aims to reduce emergency response times and minimize accident impact through automated alerting. The system detects sudden deceleration or rollover using the MPU6050 sensor, determines the exact location via GPS, and sends alerts to emergency contacts using IFTTT automation.

##  System Architecture

[MPU6050 Sensor] 
      │ (detects impact/rollover)
      ▼
[NodeMCU (ESP8266)]
      │ (processes data & connects to Wi-Fi)
      ▼
[Neo-6M GPS Module]
      │ (fetches live coordinates)
      ▼
[IFTTT Webhook]
      │ (triggers SMS/Email alert)
      ▼
[Emergency Contacts]

##  Components Used

| Component          | Description                                   |
|--------------------|-----------------------------------------------|
| NodeMCU (ESP8266)  | Wi-Fi microcontroller for processing          |
| MPU6050            | Accelerometer + Gyroscope sensor              |
| Neo-6M GPS         | Provides precise location data                |
| LCD Display        | (Optional) Displays status in-vehicle         |
| IFTTT Service      | Sends alerts to emergency contacts            |

##  Key Features

- Real-Time Accident Detection: Uses MPU6050 to sense sudden deceleration or rollover.
- Accurate Location Tracking: Captures precise GPS coordinates.
- Automated Emergency Alerts: Instantly notifies contacts via IFTTT (Email/SMS with a Google Maps link).
- Wi-Fi Enabled: NodeMCU ensures seamless cloud connectivity.
- Low Cost & Scalable: Designed with affordability and easy deployment in mind.
- Modular: Optional LCD for in-vehicle display.

##  Getting Started

### Prerequisites

- NodeMCU (ESP8266)
- MPU6050 sensor
- Neo-6M GPS module
- IFTTT account
- (Optional) LCD Display

### Setup Instructions

1. Connect the hardware components as per the circuit diagram.
2. Flash the provided firmware/code to the NodeMCU.
3. Configure your Wi-Fi credentials in the code.
4. Set up IFTTT applets to trigger email/SMS alerts based on webhooks.
5. Power the system and test by simulating an accident event.
