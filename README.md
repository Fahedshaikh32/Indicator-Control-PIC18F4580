<div align="center">

# 🚦 Indicator Control Module — PIC18F4580

**A state-machine-driven automotive turn-signal & hazard controller, written in Embedded C for the PIC18F4580 microcontroller.**

Simulates real vehicle indicator behavior — left, right, and hazard blinking — driven entirely by non-blocking, counter-based timing logic.

[![Platform](https://img.shields.io/badge/MCU-PIC18F4580-blue)](#hardware)
[![Language](https://img.shields.io/badge/language-Embedded%20C-00599C?logo=c&logoColor=white)](#)
[![Toolchain](https://img.shields.io/badge/compiler-XC8-orange)](#software--toolchain)
[![License](https://img.shields.io/badge/license-MIT-green)](#license)

[Overview](#overview) •
[Features](#features) •
[Hardware](#hardware) •
[Pin Mapping](#pin-mapping) •
[State Machine](#state-machine) •
[Build & Flash](#build--flash) •
[Behavior](#behavior) •
[Project Structure](#project-structure)

</div>

---

## Overview

This project implements a **vehicle indicator control system** on a PIC18F4580 microcontroller. It replicates the behavior of a real car's turn-signal stalk and hazard switch using a matrix keypad for input and onboard LEDs for output, all governed by a compact, non-blocking state machine.

The firmware avoids blocking delays for control flow — timing (button-hold duration, blink rate, and the hazard "both buttons" detection window) is tracked using simple loop counters driven by a fixed `100 ms` tick, which keeps the main loop predictable and easy to extend.

## Features

- 🔸 **Left / Right indicator control** — held-button activation with toggle-on-repeat behavior
- 🔸 **Hazard mode** — triggered by pressing left and right in quick succession; toggles all four LEDs together
- 🔸 **Toggle semantics** — activating the same indicator again turns it off, matching real turn-signal stalks
- 🔸 **Counter-based timing** — no `__delay_ms()` blocking inside the control loop, aside from the fixed tick
- 🔸 **Configurable thresholds** — hold time and hazard window are defined in loop-tick counts, easy to retune
- 🔸 **Clean state machine design** — four explicit states (`OFF`, `LEFT`, `RIGHT`, `HAZARD`) with well-defined transitions

## Hardware

| Component | Details |
|---|---|
| Microcontroller | PIC18F4580 |
| Development board | Rhydolabz PIC development board |
| Input | Matrix keypad on `PORTC` (lower nibble) |
| Output | Onboard LEDs on `PORTB` |
| Interface | USB/Serial (UART), monitored via Tera Term |
| Oscillator | External HS, `20 MHz` (`_XTAL_FREQ`) |

### Configuration bits

```c
#pragma config OSC    = HS   // External high-speed oscillator
#pragma config WDT    = OFF  // Watchdog timer disabled
#pragma config LVP    = OFF  // Low-voltage programming disabled
#pragma config PBADEN = OFF  // PORTB<4:0> as digital I/O, not analog
```

## Pin Mapping

| Signal | Port/Pin | Direction |
|---|---|---|
| Left indicator LED 1 | `RB0` | Output |
| Left indicator LED 2 | `RB1` | Output |
| Right indicator LED 1 | `RB6` | Output |
| Right indicator LED 2 | `RB7` | Output |
| Keypad input | `PORTC` (bits 0–3) | Input |

`TRISB` is configured fully as output; the lower nibble of `TRISC` is configured as input for the keypad.

### Key codes

| Key reading (`PORTC & 0x0F`) | Meaning |
|---|---|
| `0x0E` | Left button pressed |
| `0x0D` | Right button pressed |
| other | Idle / no press |

## State Machine

```
                 ┌────────┐
        ┌───────▶│  OFF   │◀───────┐
        │        └───┬────┘        │
        │            │             │
  toggle│      hold L│      hold R │toggle
        │            ▼             │
   ┌────┴───┐   ┌─────────┐   ┌────┴────┐
   │ HAZARD │   │  LEFT   │   │  RIGHT  │
   └────┬───┘   └────┬────┘   └────┬────┘
        │            │             │
        └── L+R release within window ──┘
```

| State | Trigger to enter | Trigger to exit |
|---|---|---|
| `OFF` | Default / re-toggling an active state | — |
| `LEFT` | Left button held ≥ `1.0 s` (10 ticks) | Pressed again → `OFF`; hazard sequence → `HAZARD` |
| `RIGHT` | Right button held ≥ `1.0 s` (10 ticks) | Pressed again → `OFF`; hazard sequence → `HAZARD` |
| `HAZARD` | Left and right both released within `1.5 s` (15 ticks) of each other | Hazard sequence repeated → `OFF` |

All LEDs are toggled by a shared `blink` flag that flips once per `100 ms` loop tick, giving a `~200 ms` blink period (5 Hz toggle rate).

## Software & Toolchain

- **IDE:** MPLAB X IDE
- **Compiler:** Microchip XC8
- **Monitoring:** Tera Term (UART, optional — used for debugging/observation, not required for core functionality)

## Build & Flash

This is an MPLAB X IDE project (the included `Makefile` delegates to MPLAB X's generated `nbproject/Makefile-impl.mk`, so it is not intended to be run as a standalone hand-written Makefile).

1. Install [MPLAB X IDE](https://www.microchip.com/mplab/mplab-x-ide) and the [XC8 compiler](https://www.microchip.com/mplab/compilers).
2. Open this folder as an existing project in MPLAB X (`File → Open Project`).
3. Select the PIC18F4580 device and your programmer/debugger (e.g., PICkit, ICD).
4. Build the project (`Production → Build Main Project`), or from the command line inside the project directory:
   ```bash
   make build
   ```
5. Flash the resulting `.hex` file to the target board using your programmer of choice.

## Behavior

| Action | Result |
|---|---|
| Hold **Left** for ≥1 s | Left indicator starts blinking; hold again to cancel |
| Hold **Right** for ≥1 s | Right indicator starts blinking; hold again to cancel |
| Release **Left**, then **Right** within 1.5 s (or vice versa) | Hazard mode: all four LEDs blink together |
| Repeat the same left→right / right→left sequence in hazard mode | Hazard mode turns off |

## Project Structure

```
.
├── newmain.c    # Main firmware: keypad read, state machine, LED control
├── Makefile     # MPLAB X-generated project makefile
├── LICENSE      # MIT License
└── README.md
```

## Learning Outcomes

- Embedded C development targeting an 8-bit PIC microcontroller
- Designing and implementing a finite state machine for real-time control
- Non-blocking, tick-based timing as an alternative to blocking delays
- Digital I/O configuration (`TRIS`, `PORT` registers) and debounced button handling
- Hardware/software integration and on-target debugging

## License

This project is licensed under the MIT License — see [`LICENSE`](./LICENSE) for details.

## Author

**Fahed Shaikh**
Embedded Systems Engineer

<div align="center">

If you find this project useful, consider giving it a ⭐

</div>
