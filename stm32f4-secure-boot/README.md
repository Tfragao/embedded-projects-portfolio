# STM32F4xxRE NUCLEO Secure Bootloader & Verified App Implementation

## Overview
This project demonstrates secure bootloader system for STM32F446RE family, ready for industry use in IoT and embedded applications.

**Secure boot** on STM32 MCUs means your microcontroller will:
 - Boot only code you (or a trusted authority) approve, **verifying the firmware before running it.**
 - Prevent execution of tampered or untrusted code after reset/power-on.
 - (Optionally) enable firmware updates, but only if new firmware passes cryptographic authenticity checks.

On STM32F4 (which is not a TrustZone/secure hardware device), this is accomplished in software via two-stage system:
 - A bootloader: small protected code that runs first, checks the main application.
 - An application firmware: only launched if validated by the bootloader.

## Features
- **CRC32-based integrity check for firmware images**
- **Safe bootloader/app separation & upgrade workflow**
- **UART/LED feedback for robust user diagnostics**
- **Secure boot with firmware signature verification** (To be added later on)
- **Automated CI/CD build/test pipeline (see .github/workflows/)**

## Build & Usage

1. **Build the bootloader and application** using CubeIDE or make.
2. **Prepare the application binary:**  
   `python3 prepare_app_with_crc.py`  
   (pads, appends CRC, creates `app_with_crc.bin`)
3. **Flash images to target device:**  
   - Bootloader: `0x08000000`
   - App: `0x08008000`

### Hardware Requirements
- STM32F446RE Nucleo board 
- USB cable

## CI/CD Pipeline
Every push automatically:
- Builds both binaries
- Pads/calculates/appends CRC to application
- Ensures testable, ready-to-flash output
- Publishes binaries as release artifacts

## Example Serial Output
**Bootloader (valid CRC):**
Bootloader active
Valid application
**Application after jump:**
Hello from app - this is a test to check if bootloader jumps here.

## Extensibility
- Easily extended to support OTA updates
- Add versioning, authentication, rollback protection as needed
## Contact & Questions

Taison Nhanga Fragao
www.linkedin.com/in/taison-fragao
Open to feedback and collaboration!
