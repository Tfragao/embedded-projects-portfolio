#!/usr/bin/env python3
import os
import zlib
import struct

# --- CONFIG ----
APP_BIN = 'app-secure-boot-test/Debug/app-secure-boot-test.bin'  # Input file
FW_SIZE = 491516      # Region size (bytes), must be multiple of 4
OUT_BIN = 'app-secure-boot-test/Debug/app_with_crc.bin'          # Output file

# --- READ AND PAD ----
with open(APP_BIN, 'rb') as f:
    app_data = f.read()

if len(app_data) > FW_SIZE:
    raise RuntimeError(f'Bin too large: {len(app_data)} > {FW_SIZE}')
if len(app_data) < FW_SIZE:
    app_data += b'\xFF' * (FW_SIZE - len(app_data))  # Pad with erased-flash value

if FW_SIZE % 4 != 0:
    raise RuntimeError("FW_SIZE must be multiple of 4!")

# --- CRC CALCULATION ----
crc = (zlib.crc32(app_data) & 0xFFFFFFFF)
print(f"Padded app size: {len(app_data)} bytes; CRC32: 0x{crc:08X}")

# --- APPEND CRC (little-endian) ---
with open(OUT_BIN, 'wb') as f:
    f.write(app_data)
    f.write(struct.pack('<I', crc))  # 4-byte CRC in little-endian

print(f"Wrote output file: {OUT_BIN} ({len(app_data)+4} bytes)")

# --- OPTIONAL: Debug first/last words for comparison
def print_words(data, count=4):
    for i in range(count):
        w = struct.unpack('<I', data[i*4:(i+1)*4])[0]
        print(f"Word {i}: 0x{w:08X}")
    for i in range(count):
        w = struct.unpack('<I', data[-(i+1)*4:len(data)-i*4])[0]
        print(f"Last word {-i}: 0x{w:08X}")

print_words(app_data)
