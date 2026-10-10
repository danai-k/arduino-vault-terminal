# Arduino Digital Vault & Security Terminal

https://github.com/user-attachments/assets/cb6c26ef-1367-4229-ba59-7a3c221a6508

An interactive, dual-screen vault hacking game built with an Arduino. This project simulates a high-security physical vault using a potentiometer as a rotary dial, complete with array-based memory, and physical hardware feedback (LEDs and a buzzer alarm). 
This project demonstrates the ability to manage multiple I2C devices simultaneously, mixing a standard PCF8574 controller (2004A LCD) with a custom AiP31068 controller (Waveshare 1602 LCD).

---

## Features

* **Dual-Display I2C Architecture:** Runs a primary 2004A "Vault Door" interface and a secondary LCD1602 "Security Guard" terminal simultaneously on the same A4/A5 pins.
* **Logic Puzzle:** Features a Wordle-style hint system to guide the user toward the correct 3-digit combination.
* **Permanent Lockout:** A fail-state loop that permanently disables the keypad and triggers an alarm if the user runs out of attempts, requiring a hard hardware reboot.
* **Synchronized Fanfare:** Combines precise buzzer frequencies with a fading LED via Pulse Width Modulation (PWM), to create a victory sound when the vault opens.

---

## The Hint System (How to Play)

The security terminal (small screen) provides real-time feedback after every full combination attempt. It compares your guessed numbers against the secret code and outputs visual hints:

| Symbol | Meaning | Action Required |
| :---: | :--- | :--- |
| `=` | **Correct** | The number you dialed is an exact match for this slot. |
| `^` | **Too Low** | The actual secret number is **higher** than your guess. |
| `v` | **Too High** | The actual secret number is **lower** than your guess. |

*Example: If the secret code is `[ 50, 3, 99 ]` and you guess `[ 50, 6, 7 ]`, the hint screen will display: `=  v  ^`*

---

## Hardware Required
* **Microcontroller:** Arduino Nano.
* **Primary Display:** 2004A LCD I2C Module (Standard PCF8574 controller - `0x27`).
* **Secondary Display:** Waveshare LCD1602 I2C Module (AiP31068 controller - `0x3E`).
* **Inputs:** 1x Potentiometer (10kΩ), 2x Push Buttons.
* **Outputs:** 1x Green LED, 1x Red LED, 1x Piezo Buzzer.
* **Miscellaneous:** Breadboard, some Jumper Wires, 2x 220Ω-330Ω Resistors (for LEDs).

---

## Wiring Instructions

### 1. Power & Ground
* Connect Arduino **5V** to the **`+` (power) rail**.
* Connect Arduino **GND** to the **`-` (ground) rail**.

### 2. The I2C Displays (Shared Bus)
Both screens share the same data lines! 
* Connect **VCC** on both screens to the **5V rail**.
* Connect **GND** on both screens to the **GND rail**.
* Connect **SDA** on both screens to Arduino pin **A4**.
* Connect **SCL** on both screens to Arduino pin **A5**.

### 3. Inputs (Dial & Buttons)
* **Rotary Dial (Potentiometer):**
  * Outside Pin 1: **5V rail**
  * Outside Pin 3: **GND rail**
  * Middle Pin (Wiper): Arduino Analog Pin **A0**
* **"Enter" Button:** Connect Leg 1 to Arduino Pin **2**. Connect Leg 2 to **GND**.
* **"Clear/Reset" Button:** Connect Leg 1 to Arduino Pin **3**. Connect Leg 2 to **GND**.

### 4. Audio / Visual Feedback (Outputs)
* **Green LED (Access Granted):** Positive leg to Pin **6**. Negative leg through a resistor to **GND**.
* **Red LED (Access Denied / Lockout):** Positive leg to Pin **5**. Negative leg through a resistor to **GND**.
* **Buzzer:** Positive leg to Pin **7**. Negative leg to **GND**.

---

## File Structure

```text
arduino-vault-terminal/
├── WaveshareLCD.h      # Custom driver header for the secondary 1602 display
├── VaultTerminal.ino   # Main sketch
└── README.md           # Project documentation
```

---

## How to Install and Run

1. **Clone or Download** this repository:
   ```bash
   git clone [https://github.com/YOUR-USERNAME/arduino-vault-terminal.git](https://github.com/YOUR-USERNAME/arduino-vault-terminal.git)
   ```
2. Open `VaultTerminal.ino` in the **Arduino IDE**.
3. Ensure `WaveshareLCD.h` is located in the same directory as `VaultTerminal.ino` 
4. Verify the I2C address in `WaveshareLCD.h` matches your module:
   ```cpp
   #define LCD_ADDR 0x3E  // Default Waveshare address (change to 0x27 if needed)
   ```
5. Select your Board (e.g., **Arduino Uno**) under **Tools > Board**.
6. Select your COM port under **Tools > Port**.
7. Click **Upload**.

---

## License

This project is open-source and available under the [MIT License](LICENSE).
