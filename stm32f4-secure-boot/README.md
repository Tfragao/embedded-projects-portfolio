# STM32F4 Secure Boot Implementation

## Overview
This project demonstrates a secure bootloader for the STM32F4 platform, verifying firmware authenticity using cryptographic signatures.

**Secure boot** on STM32 MCUs means your microcontroller will:
 - Boot only code you (or a trusted authority) approve, **verifying the firmware before running it.**
 - Prevent execution of tampered or untrusted code after reset/power-on.
 - (Optionally) enable firmware updates, but only if new firmware passes cryptographic authenticity checks.

On STM32F4 (which is not a TrustZone/secure hardware device), this is accomplished in software via two-stage system:
 - A bootloader: small protected code that runs first, checks the main application.
 - An application firmware: only launched if validated by the bootloader.

## Features
- Secure boot with firmware signature verification
- Written in C, tested with Google Test
- Example firmware and test suite included

## Getting Started

### Hardware Requirements
- STM32F4 Discovery board

### Software Requirements
- GCC ARM toolchain
- OpenOCD
- CMake/Make

### Build Instructions
