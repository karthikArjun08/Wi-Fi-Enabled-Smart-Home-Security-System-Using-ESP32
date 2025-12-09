🏠 Wi-Fi Enabled Smart Home Security System Using ESP32

A simple and effective IoT-based smart home security system built using an ESP32, PIR motion sensor, buzzer, and LED indicator.
The ESP32 hosts a live web dashboard that displays real-time motion status using AJAX.

🚀 Project Overview

This project detects human motion using a PIR sensor, triggers an LED + buzzer alarm, and updates a Wi-Fi web dashboard accessible from any phone or laptop connected to the same network.

✔️ Real-time motion detection
✔️ Wireless monitoring
✔️ Low-cost smart home system
✔️ Clean and responsive web UI

🎯 Features

Motion detection using PIR (HC-SR501)

LED and buzzer alarm activation

Wi-Fi dashboard hosted on ESP32

AJAX-based live updates every 1 second

Mobile-friendly UI

Easy to deploy and extend

🧩 Hardware Components
Component	Quantity
ESP32 DevKit	1
PIR Sensor (HC-SR501)	1
Active Buzzer	1
LED + 220Ω resistor	1
Jumper Wires	—
Breadboard	—

 ##Circuit Diagram

                  +---------------------+
                |       ESP32         |
                |                     |
VIN (5V)  ----------------> PIR VCC
GND       ----------------> PIR GND
GPIO 13   <---------------- PIR OUT

GPIO 14   ----------------> Buzzer +
GND       ----------------> Buzzer -

GPIO 2    -----> 220Ω ---> LED +
GND       ----------------> LED -
                +---------------------+



🌐 Web Dashboard

The ESP32 hosts a lightweight webpage that shows:

✔️ Motion status

✔️ LED status

✔️ Auto-refresh every 1 second (AJAX)

✔️ Works on any smartphone or PC
