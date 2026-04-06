# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

This is a Qt-based C++ application for displaying screen watermarks (信创水印demo - Xinchuang watermark demo). The application creates a transparent, always-on-top overlay window that displays repeated text content across the entire screen.

## Building the Project

### Local Build
```bash
cd screen_watermark
qmake
make
ls build/release/screen_watermark
```

## Code Architecture

### Project Structure
- **screen_watermark/**: Main application directory
  - `main.cpp`: Entry point that initializes the QApplication and displays the watermark window
  - `CTextWatermark.h/cpp`: Core watermark widget implementation
  - `screen_watermark.pro`: qmake project configuration

### Core Component: CTextWatermark

The `CTextWatermark` class (inheriting from `QWidget`) is the main component responsible for creating and rendering the watermark overlay.

**Key architectural characteristics:**

1. **Transparent Overlay Window**: The widget is configured as a frameless, transparent window that stays on top of all other windows and bypasses the window manager (X11BypassWindowManagerHint).

2. **Qt Version Compatibility**: The code handles different Qt versions (pre-5.0, 5.0+, and 5.10+) with conditional compilation for window flags and state management.

3. **Watermark Rendering**:
   - `InitWaterMark(const QString &strContent)`: Pre-renders the watermark text pattern onto a QPixmap. It calculates grid positioning based on text dimensions and spacing constants (WM_TEXT_COL_SPACE, WM_TEXT_ROW_SPACE).
   - `paintEvent(QPaintEvent *event)`: Actually draws the pre-rendered pixmap with transparency (WM_TRANSPARENCY = 0.6).

4. **Input Transparency**: The window is configured to be transparent for input events (WindowTransparentForInput in Qt 5.0+), allowing clicks to pass through to underlying applications.

### Watermark Constants (defined in CTextWatermark.h)

- `WM_TRANSPARENCY`: 0.6 (60% opacity)
- `WM_TEXT_COLOR`: Black (0, 0, 0, 255)
- `WM_TEXT_FONT`: Arial, 12pt, Bold
- `WM_TEXT_ROW_SPACE`: 100 pixels between rows
- `WM_TEXT_COL_SPACE`: 100 pixels between columns

### Build Configuration

The qmake project (`.pro` file) is configured for:
- C++17 standard
- Release builds only
- Platform-specific source file inclusion (currently Linux-specific)
- Build artifacts in `build/` subdirectory (release, moc, rcc, ui, obj)

## Platform Support

The project includes conditional compilation for:
- **Linux**: Primary target platform with full source file inclusion
- **Windows**: Recognized but no Windows-specific source files currently configured
- **Qt Versions**: Supports Qt 4.x, 5.0+, and 5.10+ with version-specific adaptations
