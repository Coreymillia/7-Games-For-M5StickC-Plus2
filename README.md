# M5StickC Plus2 - 7-Game Collection v1.0

![M5StickC Plus2](https://m5stack.oss-cn-shenzhen.aliyuncs.com/image/m5-docs_homepage/core/m5stickc_plus2/m5stickc_plus2_01.webp)

## 🎮 Overview

A complete arcade game collection for M5StickC Plus2 featuring 7 professionally crafted games with optimized Mini Joy C hat controls. From classic Snake to modern Tetris Dodge, this collection delivers hours of entertainment!

## 🕹️ Games Included

### 1. 🐍 Snake (Landscape)
Classic snake game with smooth controls. Eat food, grow longer, avoid walls and yourself!
- **Controls:** Joystick for direction
- **Goal:** Get the highest score without crashing

### 2. 🧱 Tetris (Portrait)
Enhanced Tetris with modern features including hold piece, ghost preview, and progressive difficulty.
- **Controls:** L/R=move, Up=hard drop, Down=soft drop, Joy Btn=rotate, BtnA=hold
- **Features:** Line counter, lock delay 500ms, level up every 10 lines
- **Goal:** Clear lines and reach high levels

### 3. 🐦 Flappy Bird (Landscape)
Navigate through pipes in this addictive side-scroller.
- **Controls:** Any button to flap
- **Goal:** Pass as many pipes as possible

### 4. 💥 Breakout (Portrait)
Endless breakout with randomly rearranging brick patterns each level.
- **Controls:** L/R=move paddle
- **Features:** Bonus life every 600 points, speed increases 5% per level
- **Goal:** Clear all bricks and survive increasing speed

### 5. 🚀 Space Wars (Portrait)
Progressive difficulty space shooter with bullet hell mechanics.
- **Controls:** L/R=move, Joy Btn=shoot, BtnA=hold
- **Features:** Enemy gets faster and more erratic, unlimited rockets, bonus life every 3 levels
- **Goal:** Defeat increasingly difficult enemies

### 6. 🏓 Pong (Landscape)
Classic Pong versus AI opponent. First to 10 points wins!
- **Controls:** Up/Down=move paddle
- **Features:** Ball speeds up with each hit, intelligent AI
- **Goal:** Beat the AI to 10 points

### 7. 🎯 Dodge (Portrait) - NEW!
Dodge falling Tetris pieces while rotating your T-piece to fit through gaps!
- **Controls:** Joystick=move in 4 directions, Joy Btn=rotate piece
- **Features:** Precise block collision, level up every 100 points, faster spawning
- **Goal:** Survive as long as possible and get the highest score

## 📦 Installation

### Requirements
- **M5StickC Plus 2** device
- **Mini Joy C Hat** (required - connects via I2C)
- **M5Burner** application

### Flash Instructions

1. **Download the firmware:**
   - Use `M5Stick-7Games-v1.0-MERGED.bin` for M5Burner

2. **Open M5Burner:**
   - Launch M5Burner application
   - Connect your M5StickC Plus2 via USB

3. **Flash the firmware:**
   - Select your device's COM port
   - Click "Burn" to flash the firmware
   - Wait for completion (usually 10-20 seconds)

4. **Attach Mini Joy C Hat:**
   - Connect the hat to the M5StickC Plus2 Grove port
   - Ensure firm connection

5. **Power on and play:**
   - Press the power button
   - Navigate menu with joystick
   - Press BtnA to start selected game

## 🎮 Controls

### Main Menu
- **Joystick Up/Down:** Select game
- **BtnA:** Start selected game

### In Games
- **BtnB:** Return to menu (most games)
- **Joystick:** Movement (varies by game)
- **Joy Button:** Action (rotate, shoot, etc.)
- **BtnA:** Special action (varies by game)

### Specific Game Controls

| Game | Move | Action 1 | Action 2 |
|------|------|----------|----------|
| Snake | Joystick | - | - |
| Tetris | L/R | Joy=Rotate | BtnA=Hold |
| Flappy | - | Any Btn=Flap | - |
| Breakout | L/R | - | - |
| Space Wars | L/R | Joy=Shoot | BtnA=Hold |
| Pong | U/D | - | - |
| Dodge | 4-Way | Joy=Rotate | - |

## 📊 Technical Details

- **Total Games:** 7
- **Firmware Size:** 519 KB (merged)
- **Memory Usage:** ~8% RAM, 35% Flash
- **Screen:** 135x240 pixels, 16-bit color
- **Chip:** ESP32-PICO-V3-02
- **Required Hardware:** Mini Joy C Hat (I2C: SDA=0, SCL=26)

## 🏗️ Project Structure

```
M5StickFinal7/
├── complete_project/       # Full source code
│   ├── src/               # Game source files
│   ├── platformio.ini     # Build configuration
│   └── .pio/              # Build outputs
└── releases/              # Ready-to-flash files
    ├── M5Stick-7Games-v1.0-MERGED.bin  # For M5Burner
    ├── M5Stick-7Games-v1.0-FINAL.bin   # App only
    ├── m5burner_config.json            # M5Burner metadata
    └── README.md                       # This file
```

## 🎯 Game Features

### Snake
- Smooth analog joystick control
- Score tracking
- Progressive difficulty

### Tetris
- ✅ Hold piece system
- ✅ Ghost piece preview
- ✅ Lock delay (500ms)
- ✅ Line counter display
- ✅ Level progression every 10 lines
- ✅ No flicker or screen tearing

### Flappy Bird
- Smooth scrolling
- Collision detection
- Score tracking

### Breakout
- ✅ Endless mode
- ✅ Random brick patterns each level
- ✅ Bonus life every 600 points
- ✅ Progressive speed increase (5%)

### Space Wars
- ✅ Progressive difficulty
- ✅ Faster enemy movement each level (25% increase)
- ✅ More frequent shooting (3% per level)
- ✅ Erratic enemy movement patterns
- ✅ Unlimited rockets
- ✅ Bonus life every 3 levels
- ✅ Rotating enemy ship graphics

### Pong
- AI opponent
- Score tracking to 10
- Ball speed increases

### Dodge
- ✅ Precise block-to-block collision
- ✅ T-piece rotation with Joy button
- ✅ 4-directional movement
- ✅ Falling Tetris pieces in random rotations
- ✅ Level progression every 100 points
- ✅ Faster spawning each level

## 🐛 Known Issues

None currently! All 7 games tested and working perfectly.

## 🔧 Building from Source

If you want to modify the games:

1. Install PlatformIO
2. Navigate to `complete_project/`
3. Run: `pio run -t upload`

See `platformio.ini` for build configuration.

## 📝 Version History

### v1.0.0 (2024-11-02)
- Initial release
- 7 complete games
- Optimized menu system
- Mini Joy C hat support
- Precise collision detection
- All games tested and polished

## 🙏 Credits

**Developer:** coreymillia with GitHub Copilot CLI

**Based on:**
- Various open-source game implementations
- M5StickC Plus2 SDK
- Mini Joy C Hat library

## 📄 License

Open source - feel free to modify and share!

## 🎉 Enjoy!

Have fun with your 7-game arcade collection! Each game is fully playable and optimized for the M5StickC Plus2 with Mini Joy C hat.

For issues or suggestions, feel free to reach out!

---

**Made with ❤️ for the M5Stack community**
