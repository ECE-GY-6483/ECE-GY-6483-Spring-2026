# ECE-GY 6483: Real-Time Embedded Systems

This repository is a supplementary repository for **ECE-GY 6483 – Real-Time Embedded Systems** at New York University.  
It is primarily used to host **recitation material**, example code, and supporting resources discussed during TA-led sessions.

---

# Introduction

ECE-GY 6483 focuses on the design and implementation of real-time embedded systems with strict timing and resource constraints.  
The course covers fundamental concepts such as task scheduling, interrupts, concurrency, real-time operating systems, and timing analysis, along with practical embedded software development.

# Development Hardware

The course utilizes the STM32L4 Discovery Kit for IoT Node (B-L475E-IOT01A). This board is powered by an ARM® Cortex®-M4 core with 1 MB of Flash memory and 128 KB of SRAM, providing a robust platform for testing embedded programming concepts.

The figure Below shows the development board.
![STM32L4 Discovery Kit IoT Node](Images/STM_Board.png)

The board features a variety of sensors and communication peripherals designed for seamless cloud integration and real-time data processing:

- 6-Axis IMU (Inertial Measurement Unit): High-precision motion tracking (LSM6DSL).

- Bluetooth Low Energy (BLE) Module: Low-power wireless data transmission (BlueNRG-MS).

- Wi-Fi Module: Integrated 802.11 b/g/n connectivity (Inventek ISM43362).

- ToF & Gesture Detection Sensor: Laser-ranging Time-of-Flight sensor (VL53L0X).

 The board also features Expansion capabilities via ARDUINO® Uno V3 and PMOD™ connectivity.


**Instructor**  
Professor. Matthew Campisi, PhD

**Teaching Assistants**  
- Jotheesh Reddy Kummathi   (jrk8067@nyu.edu)
- Rahul Reghunath           (rr4660@nyu.edu)
- Divij kapur               (dk4999@nyu.edu)


---

# Repository Usage

- This repository contains **recitation code, demos, and explanations**.
- Students are encouraged to explore, modify, and experiment with the examples for learning purposes.

---

# Repository Structure

```text
.
├── recitations/     # Recitation-specific code and walkthroughs
└── resources/       # Reference material and external links


