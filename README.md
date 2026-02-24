# Battery-Powered MCU + SPI Flash Logger

Portfolio project: low-power data logger built around a `SAMD21` MCU and external `W25Qxx` SPI flash.

This project is a work-in-progress.

## Problem Statement

Design a logging platform that captures periodic measurements while maximizing battery life and preserving data through reset and power cycles.

## Schematic And PCB

- Add KiCad schematic screenshots: MCU power tree, SPI routing, decoupling strategy, and debug header.
- Add PCB layout images: top/bottom copper, ground return path, and crystal/clock placement.

## Firmware Overview

Firmware is implemented as a deterministic wake-log-sleep cycle.

- On each wake event, the MCU acquires a sample payload, writes to external SPI flash, validates write completion, and returns to low-power mode.
- State machine: `BOOT -> WAKE -> SAMPLE -> WRITE -> VERIFY -> SLEEP`
- Flash writes are page-aligned with sequence IDs for recovery after unexpected reset.
- UART command path supports data dump and basic health checks.

Current repository status:

- SPI initialization and JEDEC ID read path are implemented in [`/spi-flash-data-logger.ino`](/spi-flash-data-logger.ino).
- Full logging state machine and low-power loop are planned next.

## Power Budget

- Active logging current: TBD mA
- Sleep current: TBD uA
- Wake interval: 10 s (current setting)
- Estimated runtime on target cell: TBD days

## Debugging Challenges

- TBD

## Results

The logger demonstrates full-stack embedded execution: hardware interface design, persistent storage firmware, low-power control, and instrumentation-led debug decisions.
