# Battery-Powered MCU + SPI Flash Logger

Portfolio project: low-power data logger built around a `SAMD21` MCU and external `W25Qxx` SPI flash.

This project is a work-in-progress.

## Problem Statement

Design a logging platform that captures periodic measurements while maximizing battery life and preserving data through reset and power cycles.

## Schematic And PCB

- Add KiCad schematic screenshots: MCU power tree, SPI routing, decoupling strategy, and debug header.
- Add PCB layout images: top/bottom copper, ground return path, and crystal/clock placement.

## Firmware Overview

Target firmware architecture is a deterministic wake-log-sleep cycle.

- Planned flow: `BOOT -> WAKE -> SAMPLE -> WRITE -> VERIFY -> SLEEP`
- Planned features: page-aligned writes, sequence IDs for recovery, UART command path for dump/health checks.

Current repository status (implemented now):

- SPI initialization on Arduino `SPI` bus is implemented in [`spi-flash-data-logger.ino`](/spi-flash-data-logger.ino).
- SPI settings currently used:
  - `4 MHz`
  - `MSBFIRST`
  - `SPI_MODE0`
- Flash JEDEC ID read path (`0x9F`) is implemented and prints:
  - Manufacturer ID
  - Memory Type
  - Capacity
- Basic flash command helpers are implemented:
  - `writeEnable()` (`0x06`)
  - `sectorErase(address)` (`0x20`, 4 KB sector erase)
  - `readStatus()` (`0x05`)
  - `waitUntilReady()` polling SR1 BUSY bit
- Startup test currently performs `sectorErase(0x000000)` and prints status.
- `loop()` is empty; full periodic logging state machine and low-power loop are still planned.

## Power Budget

- Active logging current: TBD mA
- Sleep current: TBD uA
- Wake interval: TBD in firmware (no periodic wake loop implemented yet)
- Estimated runtime on target cell: TBD days

## Debugging Challenges

- Confirming stable SPI bring-up and JEDEC ID reads across reset conditions.
- Verifying erase completion through status polling.
- Still pending: end-to-end write/readback validation and recovery-path testing.

## Results

Current result: successful SPI flash bring-up and erase/status test path on target firmware.

Next result target: implement Page Program and Read Data.

## Notes

- Chip select is currently hardcoded as `CS_PIN = 0`.
- Warning: the current startup test erases the flash sector at address `0x000000`.
