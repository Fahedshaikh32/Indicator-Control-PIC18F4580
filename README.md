# 🚗 Indicator Control Module (PIC18F4580)

## 📌 Overview

This project implements a vehicle Indicator Control System using the PIC18F4580 microcontroller. The system simulates real-world automotive indicator behavior including Left, Right, and Hazard lights.

The implementation is done in Embedded C using MPLAB and follows a structured, state-machine-based approach to ensure reliable and scalable design.

---

## ⚙️ Features

* 🔹 Left Indicator Control
* 🔹 Right Indicator Control
* 🔹 Hazard Mode (both indicators blinking)
* 🔹 Toggle Functionality (press once → ON/OFF)
* 🔹 Indicator Switching Logic
* 🔹 300 ms LED Blinking
* 🔹 Time-based Button Handling

---

## 🧠 System Design

The system is designed using a **state machine approach** with the following states:

* `OFF`
* `LEFT`
* `RIGHT`
* `HAZARD`

Transitions between states are controlled using button inputs and timing logic.

---

## 🛠️ Hardware Used

* PIC18F4580 Microcontroller
* Development Board (Rhydolabz)
* Onboard LEDs
* Matrix Keypad
* USB/Serial Interface

---

## 💻 Software Used

* MPLAB IDE
* XC8 Compiler
* Tera Term (for UART monitoring)

---

## 🔄 Working Logic

* Press LEFT button → Left indicator toggles
* Press RIGHT button → Right indicator toggles
* Press LEFT + RIGHT (within short time window) → Hazard mode
* Indicators blink with ~300 ms timing
* System uses counter-based timing instead of blocking delays

---

## 🎥 Demo

📹 The working demonstration is provided in the attached video.

---

## 📈 Learning Outcomes

* Embedded C Programming
* State Machine Design
* Real-Time System Behavior
* Hardware-Software Integration
* Debugging and Testing on Microcontroller

---

## 👨‍💻 Author

**Fahed Shaikh**
Embedded Systems Engineer

---

## ⭐ Acknowledgement

This project was developed as part of an embedded systems assignment to demonstrate real-time control logic and hardware interfacing.
