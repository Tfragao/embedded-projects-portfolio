# Application secure boot test

## overview
This is the application app that will be used to flash in the board at the specified offset.
The goal is:
	- After the board boots, the bootloader will load or not this app if the CRC match

## Flashing the app with CubeProgrammer GUI

1) Open STM32CubeProgrammer.

2) Connect your STM32F446RE board:

   2.1) On the right panel, select your ST-LINK interface (usually USB or ST-LINK).

   2.2) Click the green Connect button.

3) Go to the "Erasing and Programming" section.

   3.1) Click the “Download” tab or button (this is the flashing section).

4) Select your app binary:

   4.1) Click Browse/Open File.

   4.2) Select your padded and CRC-appended binary (app_with_crc.bin).

5) Set the correct start address for the app:

   5.1) For a binary file (.bin), after you select it, a field appears:
    	“Start Address”.
	For example you can enter a start address for the app : 0x08008000 (This is the start address matching your app linker script.)

6) Programming options:

   6.1)  You can optionally enable “Verify after programming” for safety.

    6.2) Leave "Run after programming" unchecked—your bootloader will handle jumping to the app.

7) Start programming:

   7.1) Click “Download” or “Start Programming”.

    Wait for a success message.

8) Reset the board:
    Press the reset button, or power-cycle your STM32.

9) Observe the bootloader behavior:

    If everything worked, your bootloader will validate the app (CRC), and should jump to it: you’ll see your app running (e.g., its LED pattern or UART message).

10) After flashing you can disconnect by pressing the Disconnect button in the upper right screen.




