# BarBot 🍸

BarBot is an automated beverage mixing machine capable of preparing carbonated drinks and milkshakes, implemented in Tinkercad. Features a user interface, LED ring, LCD display, calibration system, and custom mixing logic.

## 📦 Project Components

- **Arduino Uno x2** — main control units
- **8x 12V DC pumps (Model 385)** — beverage dispensing
- **4x L293D motor drivers** — pump control
- **74HC595 shift register** — consolidates motor control to 3 pins
- **Neopixel Ring (16 LED)** — visual status indication
- **LCD 16x2** — menu and status display (controlled via standard library)
- **RGB button panel** — user interface
- **TMP36** — temperature sensor
- **Load cell (up to 10kg)** — cup detection
- **Servo motor** — ice dispensing mechanism
- **5V→12V step-up converter** — pump power supply
- **Type-C power and interface** — separate power lines

## 🧠 Software

The main controller runs **BarBotOS v1.0.6**, hand-written firmware focused on compactness and low-level control:

- Proportional beverage mixing (recipes in code)
- Weight-based calibration support
- Preparation counter reset on reboot
- Settings menu
- Custom 74HC595 library implementation
- Avoids third-party universal solutions incompatible with Tinkercad

Separate firmware (`firmwarePixel.ino`) handles NeoPixel visual effects and potential future audio feedback (planned for ATtiny85).

## 🎯 Project Highlights

This project represents several personal milestones:
- **First well-documented project** — comprehensive code comments and structured approach
- **Created at age 15** — demonstrates early electronics and programming skills
- **Custom library development** — built specialized 74HC595 control from scratch
- **Complex system integration** — successfully coordinated multiple subsystems
- **Real-world problem solving** — addressed Tinkercad limitations through creative workarounds

## 🚧 Known Limitations

- **Simulation constraints**: Tinkercad doesn't always accurately display component behavior, espacialy when it comes to different physics operation, like liquids.
- **ATtiny85**: originally planned for parallel effect processing, replaced with Uno due to firmware upload failures
- **Calibration**: requires physical pump adjustment — simulation shows perfect performance, but real pumps may have variance

## 🔧 Assembly and Testing

Project fully implemented in Tinkercad and accessible through the simulator.

> Physical assembly possible with 3D case, sufficient drivers, and 12V power source.

## 🛠️ Firmware Files

- `firmwareMain.ino` — core logic and control
- `firmwarePixel.ino` — lighting and planned multimedia support

## 🧪 Potential Improvements

- Migration from simulation to physical assembly with calibrated pumps
- ATtiny85 integration for background tasks
- More stable LCD display support (I2C version)
- Weight calibration debugging and adaptation for real ingredients (viscosity, density, carbonation)

## 📄 License

Project available under MIT License. Use, modify, build your own BarBots — cheers!
