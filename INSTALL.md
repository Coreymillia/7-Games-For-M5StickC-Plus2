# Installation Guide

Complete guide to installing and using the M5StickC Plus2 7-Game Collection.

## 📋 Prerequisites

### Hardware Required
1. **M5StickC Plus 2** device
2. **Mini Joy C Hat** (essential - game collection requires joystick)
3. **USB-C cable** (for programming/charging)

### Software Required
1. **M5Burner** application
   - Download: [M5Stack Official Site](https://docs.m5stack.com/en/download)
   - Available for Windows, macOS, Linux

## 🚀 Quick Install (M5Burner - Recommended)

### Step 1: Download Firmware
1. Go to [Releases](releases/) folder
2. Download `M5Stick-7Games-v1.0-MERGED.bin`
3. Save to a known location

### Step 2: Install M5Burner
1. Download M5Burner from official M5Stack site
2. Install and launch the application
3. No additional drivers needed (built-in)

### Step 3: Flash Firmware
1. **Connect M5StickC Plus2:**
   - Plug USB-C cable into device
   - Connect to computer
   - Device will power on (screen lights up)

2. **Select Device in M5Burner:**
   - Click "COM" dropdown
   - Select your M5StickC Plus2 port
   - (Usually shows as "USB Serial Device" or "CP210x")

3. **Load Firmware:**
   - Click "Open File" or drag-and-drop
   - Select `M5Stick-7Games-v1.0-MERGED.bin`
   - Click "Burn" button

4. **Wait for Completion:**
   - Progress bar shows upload status
   - Takes approximately 10-20 seconds
   - "Success" message appears when done

5. **Verify Installation:**
   - Device will auto-reboot
   - Menu with 7 games should appear

### Step 4: Attach Mini Joy C Hat
1. **Power off M5StickC Plus2** (hold power button)
2. **Align Hat:** 
   - Hat has Grove connector on bottom
   - M5StickC Plus2 has Grove port on side
3. **Connect firmly** - should click into place
4. **Power on** - Hold power button ~2 seconds

### Step 5: Test Games
1. Use joystick Up/Down to navigate menu
2. Press **BtnA** (front button) to select a game
3. Test joystick in game
4. Press **BtnB** (side button) to return to menu

## 🔧 Advanced Install (PlatformIO)

For developers who want to modify the code:

### Prerequisites
```bash
# Install PlatformIO Core
pip install -U platformio

# Or install PlatformIO IDE (VSCode extension)
# https://platformio.org/install/ide?install=vscode
```

### Build and Upload
```bash
# Clone or download source code
cd github-release/

# Build project
pio run

# Upload to device (connected via USB)
pio run -t upload

# Monitor serial output (optional)
pio device monitor
```

### Custom Configuration
Edit `platformio.ini`:
```ini
[env:m5stick-c-plus2]
platform = espressif32
board = m5stick-c
framework = arduino
lib_deps = 
    m5stack/M5StickCPlus2@^1.0.2
upload_speed = 115200
monitor_speed = 115200
```

## 🐛 Troubleshooting

### Issue: Device not detected
**Solutions:**
1. Try different USB cable (some are charge-only)
2. Install CP210x drivers manually:
   - [Silicon Labs CP210x Drivers](https://www.silabs.com/developers/usb-to-uart-bridge-vcp-drivers)
3. Try different USB port
4. Restart computer

### Issue: Upload fails
**Solutions:**
1. Hold BtnA during connection
2. Use slower upload speed (change `upload_speed = 115200` in platformio.ini)
3. Try different USB cable
4. Close other serial monitors

### Issue: Screen is blank
**Solutions:**
1. Hold power button to turn on
2. Check battery (charge if needed)
3. Re-flash firmware

### Issue: Joystick not working
**Solutions:**
1. Ensure Mini Joy C Hat is firmly connected
2. Check I2C address (should be 0x38)
3. Verify Grove connector alignment
4. Try reseating the hat

### Issue: Games have artifacts
**Solutions:**
1. This is fixed in v1.0.0
2. Re-flash the latest MERGED.bin
3. Do a full power cycle after flashing

### Issue: Menu navigation stuck
**Solutions:**
1. Check joystick connection
2. Calibrate by moving joystick in all directions
3. Press power button to restart

## 📱 First Time Setup

### 1. Test Each Game
Go through all 7 games to verify:
- Snake: Joystick moves snake
- Tetris: All controls work (move, rotate, drop, hold)
- Flappy: Button makes bird flap
- Breakout: Paddle moves left/right
- Space Wars: Ship moves and shoots
- Pong: Paddle moves up/down
- Dodge: Joystick moves T-piece, button rotates

### 2. Check Controls
- **Menu navigation** - Joystick up/down
- **Game selection** - BtnA to start
- **Return to menu** - BtnB in most games
- **Joystick button** - Game-specific actions

### 3. Verify Performance
- No lag or stuttering
- Smooth animations
- No screen flicker
- Clean transitions between games

## 🔋 Battery & Power

### Charging
- Plug USB-C cable while powered on or off
- Red LED indicates charging
- Takes ~1-2 hours for full charge
- Can play while charging

### Battery Life
Approximate play time per game:
- **Snake:** ~2 hours
- **Tetris:** ~2 hours  
- **Flappy:** ~2.5 hours
- **Breakout:** ~2 hours
- **Space Wars:** ~1.5 hours (more intensive)
- **Pong:** ~2.5 hours
- **Dodge:** ~2 hours

### Power Saving
- Auto-sleep in menu after inactivity
- Turn off when not in use
- Lower screen brightness (if available)

## 📊 Verification Checklist

- [ ] Firmware flashed successfully
- [ ] Mini Joy C Hat connected
- [ ] Menu displays 7 games
- [ ] Can navigate menu with joystick
- [ ] Can start games with BtnA
- [ ] Can return to menu with BtnB
- [ ] All games load without artifacts
- [ ] Joystick responsive in all games
- [ ] No lag or crashes
- [ ] Battery charging works

## 📞 Getting Help

If issues persist:

1. **Check documentation:**
   - README.md - Overview and features
   - CREDITS.md - Software versions and dependencies
   
2. **Community support:**
   - M5Stack Discord
   - M5Stack Forum: https://community.m5stack.com/
   - GitHub Issues (if you have the repo link)

3. **Hardware support:**
   - M5Stack Official Support
   - Check device warranty

## 🎉 Success!

Once installed and tested, you're ready to enjoy your 7-game arcade collection!

**Tips for best experience:**
- Start with Snake or Pong (simpler controls)
- Learn Tetris controls (most complex)
- Try Dodge for a unique challenge
- Space Wars gets harder each level
- Breakout is endless - how far can you go?

Have fun! 🎮

---

**Installation Support:** coreymillia
**Last Updated:** 2024-11-02
