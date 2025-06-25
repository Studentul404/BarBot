# BARBOTOS v1.0.6 🍸🤖

**An intelligent automated bartender robot capable of mixing drinks, dispensing ice, and providing an interactive user experience.**

[🔗 Try it in Tinkercad!](https://www.tinkercad.com/things/fXrM3DRYCUC-barmenbot-v106)

## 🎯 Overview

BARBOTOS is a sophisticated drink-mixing robot built around Arduino that can prepare four different beverages (Milkshake, Coca-Cola, Fanta, Sprite) with customizable toppings, ice, and cream. The system features intelligent glass detection, temperature monitoring, and a comprehensive statistics tracking system.

## 🔧 Hardware Components

### Core Control System
- **Arduino Uno (Primary)** — main system controller
- **Arduino Uno (Secondary)** — LED effects controller
- **16x2 LCD Display** — user interface and status display
- **Analog Button Panel** — 8-button interface for drink selection and navigation

### Dispensing System
- **8x Peristaltic Pumps** — precise liquid dispensing
  - Pin 0: Milkshake base
  - Pin 1: Cream
  - Pin 2: Chocolate topping  
  - Pin 3: Fruit syrup
  - Pin 4: Caramel topping
  - Pin 5: Coca-Cola
  - Pin 6: Fanta
  - Pin 7: Sprite
- **74HC595 Shift Register (2x)** — pump control expansion
- **Servo Motor** — automated ice cube dispensing

### Sensor Array
- **Force Sensor (A0)** — glass detection and weight measurement
- **Temperature Sensor (A4)** — ambient temperature monitoring
- **Analog Multiplexer (A5)** — button input processing

### Visual Feedback
- **16-LED NeoPixel Ring** — status indication and animations
- **Custom LCD Characters** — enhanced display symbols

## 🧠 Software Architecture

### Core Classes

#### `BarBotInterface`
- **LCD Management**: Custom character creation, cursor control
- **User Input**: Multi-format question handling (numbers, yes/no, multiple choice)
- **Sensor Integration**: Temperature reading, glass detection
- **Menu Systems**: Settings, statistics, calibration interfaces

#### `CustomHC595<T>`
- **Template-based Design**: Supports multiple shift register configurations
- **Precise Control**: Individual pin state management
- **Bulk Operations**: Set/clear all pins simultaneously
- **Error Handling**: Pin range validation

#### `CocktailMaker`
- **Recipe Management**: Ingredient-specific pouring logic
- **Timing Control**: Volume-based dispensing calculations
- **Ice System**: Automated cube breaking and dispensing
- **Safety Features**: Immediate stop functionality

### Key Features

#### 🍹 Drink Preparation
- **4 Base Drinks**: Milkshake, Coca-Cola, Fanta, Sprite
- **Smart Mixing**: Proportional ingredient calculation
- **Customizable Toppings**: Chocolate, caramel, fruit syrups
- **Ice/Cream Options**: Automated dispensing with volume compensation
- **Glass Size Support**: Small (150ml), Medium (330ml), Large (500ml)

#### 📊 Advanced Statistics
- **Consumption Tracking**: Individual drink counters
- **Volume Monitoring**: Total liquid dispensed
- **Ice Usage Statistics**: Cubes dispensed tracking
- **Temperature Logging**: 16-point rolling average
- **Session Persistence**: Statistics maintained during operation

#### ⚙️ Calibration & Settings
- **Weight Calibration**: Automatic glass detection setup
- **Volume Configuration**: Customizable glass sizes
- **Animation Selection**: 4 LED ring effects ("Rainbow", "Snake", "Ukraine", "Fire")
- **Topping Preferences**: 6 fruit syrup options
- **Ice Inventory**: Manual count management

#### 🎨 User Experience
- **Intuitive Navigation**: Context-aware button functions
- **Progress Indication**: Real-time filling animation
- **Error Handling**: Glass detection, volume validation

## 🔌 Pin Configuration

### Digital Pins
```
Pin 3:  Servo Motor (Ice Dispenser)
Pin 4:  LCD D4
Pin 5:  LCD D5  
Pin 6:  LCD D6
Pin 7:  LCD D7
Pin 12: LCD Enable
Pin 13: LCD RS
```

### Analog Pins
```
A0: Force Sensor (Glass Detection)
A1: Shift Register Latch Pin
A2: Shift Register Data Pin  
A3: Shift Register Clock Pin
A4: Temperature Sensor (TMP36)
A5: Button Input Multiplexer
```

## 🚀 Operation Modes

### Normal Operation
1. **Glass Detection**: System waits for glass placement
2. **Drink Selection**: User chooses from 4 options via dedicated buttons
3. **Size Selection**: Small/Medium/Large glass configuration
4. **Customization**: Ice, cream, toppings based on drink type
5. **Preparation**: Automated dispensing with progress display
6. **Completion**: Ready notification with removal detection

### Settings Menu
- **Ice Management**: Count adjustment and calibration
- **System Calibration**: Weight sensor baseline setting
- **Volume Configuration**: Custom glass size definition
- **Visual Effects**: LED animation selection
- **Topping Selection**: Default fruit syrup choice
- **Information Display**: Statistics and system info

## 📈 Technical Specifications

### Performance Metrics
- **Pump Rate**: 28ml/second base flow rate
- **Ice Capacity**: 50 cube maximum storage
- **Temperature Range**: -40°C to +125°C monitoring
- **Weight Detection**: 0-1000g range with glass detection
- **Response Time**: <25ms button debouncing

### Memory Management
- **Efficient Storage**: Template-based shift register control
- **Minimal Footprint**: Custom implementations over libraries
- **Statistics Buffer**: 16-point temperature history
- **String Optimization**: PROGMEM usage for static data

## 🛠️ Assembly Instructions

### Circuit Connection
1. **Power Distribution**: Ensure adequate 5V/12V supply separation
2. **Shift Register Chain**: Connect 74HC595 modules in series
3. **Sensor Calibration**: Position force sensor under glass platform
4. **Pump Placement**: Mount pumps with proper liquid line routing
5. **Display Mount**: Secure LCD for optimal viewing angle

### Software Setup
1. **Library Dependencies**: LiquidCrystal, Servo (Arduino IDE built-in)
2. **Pin Verification**: Confirm all connections match pin definitions
3. **Calibration**: Run weight sensor calibration before first use
4. **Testing**: Use `testPouringSystem()` for pump verification

## 🎯 Project Achievements

This project represents several technical milestones:

- **🏗️ Complex System Integration**: Successfully coordinated 8 pumps, sensors, display, and servo
- **📚 Custom Library Development**: Built specialized 74HC595 control from scratch
- **⚡ Efficient Resource Usage**: Template-based design for minimal memory footprint
- **🎨 User Experience Focus**: Comprehensive menu system with intuitive navigation
- **📊 Data Management**: Advanced statistics with persistent tracking
- **🔧 Real-world Problem Solving**: Addressed hardware limitations through software solutions

## 🚧 Known Limitations & Future Improvements

### Current Constraints
- **Simulation Environment**: Tinkercad physics limitations affect liquid behavior representation
- **Calibration Dependency**: Requires manual adjustment for real-world pump variations
- **Single-threaded Operation**: Sequential operation limits parallel processing

### Planned Enhancements
- **Physical Implementation**: Migration to real hardware with 3D-printed chassis
- **Parallel Processing**: ATtiny85 integration for background LED effects
- **I2C LCD Upgrade**: More stable display communication
- **Recipe Expansion**: User-programmable drink combinations
- **Network Connectivity**: Remote monitoring and control capabilities
- **Voice Interface**: Audio feedback and command recognition

## 📋 Usage Examples

### Basic Drink Preparation
```cpp
// System automatically handles:
// 1. Glass detection via checkGlass()
// 2. User input via askDrink()  
// 3. Customization via askToppings()
// 4. Preparation via cookDrink()
```

### Manual Component Control
```cpp
CocktailMaker cm;
cm.begin(A1, A2, A3);
cm.startPouring("Coca-Cola", 250);  // Pour 250ml
cm.breakIce();                      // Dispense ice cube
cm.stopPouring();                   // Emergency stop
```

## 📜 License

This project is available under the **MIT License**. Feel free to use, modify, and build your own BARBOTOS — cheers! 🍻

---

**Created by a 15-year-old maker | BARBOTOS v1.0.6 | 2024**

Cheers! 🤖🍸
