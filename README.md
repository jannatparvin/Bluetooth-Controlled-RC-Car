# Bluetooth Controlled Car — Arduino Uno

A Bluetooth-controlled robot car built on an Arduino Uno, driven by two DC motors through an L298N motor driver, controlled wirelessly via an HC-05 Bluetooth module, with a servo mount reserved for a future sensor (e.g. ultrasonic obstacle detection).

---

## Overview

This project lets you drive a small car in real time — forward, backward, left, right, stop — using any standard "Bluetooth RC Car" controller app on your phone. The phone app sends single-character commands over Bluetooth, the HC-05 module receives them, and the Arduino translates them into motor driver signals.

The servo is wired in and centered on startup but not yet doing anything active — it's there so you can later mount an ultrasonic distance sensor (like the HC-SR04) on it for obstacle scanning/avoidance, without having to rewire anything.

---

## Features

- **Real-time Bluetooth control** — F (forward), B (backward), L (left), R (right), S (stop)
- **Differential steering** — turns are done by spinning the two motors in opposite directions (tank-style pivot turn), not a steering servo
- **Adjustable speed** — a single `motorSpeed` variable in the code controls how fast the motors run (0–255 PWM)
- **Servo mount ready** — pin and power wired for a future sensor upgrade
- **Works with off-the-shelf apps** — no custom app needed; any Bluetooth RC car controller app that sends F/B/L/R/S works out of the box

---

## Hardware Used

| Component | Spec / Notes |
|---|---|
| Arduino Uno | Main controller |
| HC-05 Bluetooth Module | Serial Bluetooth receiver, default baud rate 9600 |
| L298N Motor Driver | Dual H-bridge, drives both DC motors; has an onboard 220µF/35V smoothing capacitor |
| 2x DC Motors | One per side (left/right), driven differentially for turning |
| Servo Motor | Mounted for a future sensor (e.g. ultrasonic), centered at 90° by default |
| Battery Pack | 2x 3.7V Li-ion cells in series (~7.4V) — powers the motors via L298N |
| Chassis + Wheels | Car frame carrying all the above |

> **Why 7.4V?** A single 3.7V Li-ion cell isn't enough to reliably drive two DC motors through the L298N (which itself drops ~1.5–2V). Two cells in series giving ~7.4V is the standard, reliable setup — which is exactly what this build uses.

---

## Wiring

### HC-05 Bluetooth Module
| HC-05 Pin | Connects To |
|---|---|
| VCC | 5V (check your module — some need 3.3V) |
| GND | GND |
| TXD | Arduino pin 10 (RX via SoftwareSerial) |
| RXD | Arduino pin 11 (TX via SoftwareSerial) — **use a voltage divider**, HC-05 RXD is 3.3V logic only and can be damaged by Arduino's 5V output |

### L298N Motor Driver
| L298N Pin | Connects To |
|---|---|
| ENA | Arduino pin 5 (PWM — left motor speed) |
| IN1 | Arduino pin 6 |
| IN2 | Arduino pin 7 |
| ENB | Arduino pin 3 (PWM — right motor speed) |
| IN3 | Arduino pin 8 |
| IN4 | Arduino pin 4 |
| 12V / VCC | Battery pack positive (7.4V) |
| GND | Battery pack negative **and** Arduino GND (common ground is essential — the car won't work reliably without this) |
| OUT1/OUT2 | Left DC motor |
| OUT3/OUT4 | Right DC motor |

### Servo (sensor mount)
| Servo Wire | Connects To |
|---|---|
| Signal | Arduino pin 9 |
| VCC | 5V |
| GND | GND |

---

## Software Setup

1. Install the [Arduino IDE](https://www.arduino.cc/en/software) if you don't already have it.
2. Open `bluetooth_car.ino` in the IDE.
3. Make sure the `Servo` and `SoftwareSerial` libraries are available — both come pre-installed with the Arduino IDE, no extra downloads needed.
4. Select **Board: Arduino Uno** and the correct **Port** under the Tools menu.
5. Click **Upload**.

---

## Controlling the Car

1. Power the Arduino and L298N from the battery pack.
2. On your phone, install any **"Bluetooth RC Car" / "Arduino Bluetooth Controller"** app (several free ones exist on the Play Store).
3. Go to your phone's Bluetooth settings, pair with the HC-05 (default name is usually `HC-05`, default pairing PIN is `1234` or `0000`).
4. Open the app, connect to the paired HC-05, and use the on-screen buttons — they send the same F/B/L/R/S characters this code listens for.

You can also test it manually without an app: open the Arduino IDE's **Serial Monitor**, set baud rate to 9600, and type `F`, `B`, `L`, `R`, or `S` — though note the Serial Monitor talks to the USB connection, not Bluetooth, so for a true wireless test you'd need a Bluetooth serial terminal app instead.

---

## Command Reference

| Command | Action |
|---|---|
| `F` | Move forward (both motors forward) |
| `B` | Move backward (both motors backward) |
| `L` | Pivot left (left motor backward, right motor forward) |
| `R` | Pivot right (left motor forward, right motor backward) |
| `S` | Stop (both motors off) |

---

## Adjusting Speed

In the code, near the top of `setup()`:
```cpp
int motorSpeed = 200; // 0-255, higher = faster
```
Lower this if the car is too fast/jerky for your surface, or raise it (up to 255) if it feels sluggish.

---

## Future Improvements

- **Obstacle avoidance** — mount an HC-SR04 ultrasonic sensor on the servo, sweep it to scan for obstacles, and auto-stop/reroute
- **Speed control from the app** — many Bluetooth car apps send a speed byte in addition to direction; this could be parsed to dynamically adjust `motorSpeed`
- **Battery voltage monitoring** — add a voltage divider + analog read to warn when the Li-ion pack is running low
- **Line-following mode** — add IR sensors underneath for an autonomous line-following mode alongside manual control

---

## ⚠️ Safety Notes

- Always connect a common ground between the Arduino, L298N, and battery — without it, the Bluetooth commands may be received but motors won't respond correctly.
- Don't power the Arduino's 5V pin directly from the battery pack — power the Arduino via USB or its barrel jack, and let the L298N handle motor power only.
- Double-check your Li-ion pack's protection circuit (most packs include one) — bare Li-ion cells without protection circuitry are a fire risk if short-circuited or over-discharged.
