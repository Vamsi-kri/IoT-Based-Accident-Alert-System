# IoT-Based Accident Alert System

A smart, IoT-enabled system that detects vehicular accidents in real-time using motion sensors and GPS modules, sending immediate alerts to emergency contacts and services via the IFTTT platform.

## Overview

Road safety is a critical global concern. This project aims to reduce emergency response times and minimize accident impact through automated alerting. The system detects sudden deceleration or rollover using the MPU6050 sensor, determines location via GPS, and sends alerts using IFTTT automation.

## System Architecture


[MPU6050 Sensor]       (Detects impact/rollover)
         │
         ▼
   [NodeMCU (ESP8266)] (Processes data & connects to Wi-Fi)
         │
         ▼
   [Neo-6M GPS Module] (Fetches live coordinates)
         │
         ▼
     [IFTTT Webhook]   (Triggers SMS/Email alert)
         │
         ▼
 [Emergency Contacts]

## Components Used

| Component          | Description                                   |
|--------------------|-----------------------------------------------|
| NodeMCU (ESP8266)  | Wi-Fi microcontroller for processing          |
| MPU6050            | Accelerometer + Gyroscope sensor              |
| Neo-6M GPS         | Provides precise location data                |
| LCD Display        | (Optional) Displays status in-vehicle         |
| IFTTT Service      | Sends alerts to emergency contacts            |

## Key Features

- **Real-Time Accident Detection:** Uses MPU6050 to sense sudden deceleration or rollover.
- **Accurate Location Tracking:** Captures precise GPS coordinates.
- **Automated Emergency Alerts:** Instantly notifies contacts via IFTTT (Email/SMS with Google Maps link).
- **Wi-Fi Enabled:** NodeMCU ensures seamless cloud connectivity.
- **Low Cost & Scalable:** Designed for affordability and easy deployment.
- **Modular Design:** Optional LCD for in-vehicle display.

## Getting Started

### Prerequisites

- NodeMCU (ESP8266)
- MPU6050 sensor
- Neo-6M GPS module
- IFTTT account
- 16x2 LCD Display

### Setup Instructions

1. Connect the hardware components according to the circuit diagram.
2. Flash the provided firmware/code to the NodeMCU.
3. Add your Wi-Fi credentials in the code.
4. Set up IFTTT applets to trigger email/SMS alerts using webhooks.
5. Power the system and test by simulating an accident event.

### Hardware Connections

#### 1. MPU6050 (Accelerometer & Gyroscope) with NodeMCU

| NodeMCU Pin | MPU6050 Pin |
|-------------|-------------|
| D1          | SCL         |
| D2          | SDA         |
| 3.3V        | Vcc         |
| G           | Gnd         |

- Communication: I2C (D1 = SCL, D2 = SDA)
- Power: 3.3V and Ground from NodeMCU

#### 2. Neo-6M GPS Module with NodeMCU

| NodeMCU Pin | GPS Module Pin |
|-------------|---------------|
| Tx          | Rx            |
| Rx          | Tx            |
| 3.3V        | Vcc           |
| Gnd         | Gnd           |

- Communication: Serial (Tx↔Rx, Rx↔Tx)
- Power: 3.3V and Ground from NodeMCU

#### 3. 16x2 LCD with NodeMCU (Optional)

**Power:**
- Vdd → 5V (via DC adapter)
- Vss → Ground
- Vo  → Center pin of potentiometer (10KΩ)
- Potentiometer sides → 5V and Ground

**Data/Control:**

| LCD Pin | NodeMCU Pin | Notes                |
|---------|-------------|----------------------|
| RS      | D0          | Register Select      |
| E       | D3          | Enable               |
| D4      | D4          | Data                 |
| D5      | D5          | Data                 |
| D6      | D6          | Data                 |
| D7      | D7          | Data                 |
| A       | 5V (via 220Ω)| Anode (Backlight)   |
| K       | Ground      | Cathode (Backlight)  |

**Special Notes:**
- LCD must be powered by 5V (not 3.3V).
- Use a potentiometer to adjust LCD contrast via Vo.
- Use a 220Ω resistor to limit current to the LCD backlight (A pin).
  
---

## 📧 Email Alert and IFTTT Integration

**IFTTT (If This Then That)** is a free web-based automation service that connects different apps and devices using simple conditional logic called **"Applets."**

In this project, IFTTT is used to **send an automated email alert to the vehicle owner with the GPS location** whenever a disturbance (accident, vibration, or tampering) is detected.

---

### ✅ What is IFTTT?

* IFTTT allows you to **trigger actions (like sending emails)** when a specific event happens.
* In our case:
  **If ESP device detects a disturbance → Then send an Email with GPS Location**

---

### ✅ How IFTTT Works for This Project:

The ESP module triggers IFTTT by making an **HTTP GET API call** like this:

https://maker.ifttt.com/trigger/[Event_Name]/with/key/[Your_IFTTT_Key]?value1=[Latitude]&value2=[Longitude]

* **Event Name** → Your custom event (example: `disturbance`)
* **IFTTT Key** → Unique key from your IFTTT Webhooks account
* **value1** and **value2** → Latitude and Longitude (from GPS)

---

### ✅ Steps to Create the IFTTT Applet:

#### **Step 1: Get Your IFTTT Webhooks Key**

1. Go to [https://ifttt.com/maker\_webhooks](https://ifttt.com/maker_webhooks)
2. Click on **Documentation** (top-right)
3. Copy your unique **Webhook Key**
   Example Key:
   p7zBBT6FHzN_7-6batwm3rg8v9T4gmrsjXSajuwaDJ1

#### **Step 2: Create the Applet**

1. Go to **[https://ifttt.com/create](https://ifttt.com/create)**

2. Click **"+ This"**

   * Choose **Webhooks**
   * Select **"Receive a web request"**
   * Enter your Event Name → Example: `disturbance`

3. Click **"+ That"**

   * Choose **Gmail** or **Email**
   * Select **"Send me an email"**

4. In the **email body**, you can insert the following Google Maps link to display location:

https://www.google.com/maps/search/?api=1&query={{Value1}},{{Value2}}

This will automatically convert the GPS coordinates into a clickable Google Maps location in your email.

###  Step 3: Finalize and Activate

* Review your applet setup.
* Click **Finish**.
* Your IFTTT applet is now ready.

Whenever your ESP module sends a disturbance alert, IFTTT will send an email with a clickable GPS location.

###  Example Email Triggered:

Subject: Disturbance Detected!

Body:
A disturbance has been detected at the following location:

https://www.google.com/maps/search/?api=1&query=17.385044,78.486671

