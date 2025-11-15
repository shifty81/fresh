# Dark Night Mode Theme - Visual Mockup

This document provides a visual representation of the new dark night mode theme implemented for the Fresh Voxel Engine GUI.

## Color Swatches

### Background Colors
```
┌─────────────────────────────────────────┐
│ Main Window Background                  │
│ RGB(0.10, 0.11, 0.13) = #1A1C21        │
│ ████████████████████████████████████    │
└─────────────────────────────────────────┘

┌─────────────────────────────────────────┐
│ Child Window / Panel Background         │
│ RGB(0.12, 0.13, 0.15) = #1F2126        │
│ ████████████████████████████████████    │
└─────────────────────────────────────────┘

┌─────────────────────────────────────────┐
│ Menu Bar Background                     │
│ RGB(0.08, 0.09, 0.11) = #14171C        │
│ ████████████████████████████████████    │
└─────────────────────────────────────────┘

┌─────────────────────────────────────────┐
│ Input Field Background                  │
│ RGB(0.15, 0.16, 0.19) = #262930        │
│ ████████████████████████████████████    │
└─────────────────────────────────────────┘
```

### Text Colors
```
┌─────────────────────────────────────────┐
│ Main Text - High Contrast               │
│ RGB(0.95, 0.96, 0.98) = #F2F4F9        │
│ ████████████████████████████████████    │
└─────────────────────────────────────────┘

┌─────────────────────────────────────────┐
│ Disabled Text                           │
│ RGB(0.50, 0.52, 0.55) = #80858C        │
│ ████████████████████████████████████    │
└─────────────────────────────────────────┘
```

### Interactive Element Colors
```
┌─────────────────────────────────────────┐
│ Button Normal                           │
│ RGB(0.20, 0.35, 0.55) = #33598C        │
│ ████████████████████████████████████    │
└─────────────────────────────────────────┘

┌─────────────────────────────────────────┐
│ Button Hover                            │
│ RGB(0.28, 0.48, 0.70) = #477AB3        │
│ ████████████████████████████████████    │
└─────────────────────────────────────────┘

┌─────────────────────────────────────────┐
│ Button Active/Pressed                   │
│ RGB(0.18, 0.32, 0.50) = #2E5280        │
│ ████████████████████████████████████    │
└─────────────────────────────────────────┘

┌─────────────────────────────────────────┐
│ Slider/Checkbox Accent                  │
│ RGB(0.35, 0.65, 0.95) = #59A6F2        │
│ ████████████████████████████████████    │
└─────────────────────────────────────────┘
```

## Main Menu Window Mockup

```
╔══════════════════════════════════════════════════════════════╗
║ Welcome to Fresh Voxel Engine                         [-][X] ║ ← Title Bar (RGB 0.12, 0.14, 0.18)
╠══════════════════════════════════════════════════════════════╣
║                                                              ║
║                      ────────────────────                    ║
║                      FRESH VOXEL ENGINE                      ║ ← White Text (RGB 0.95, 0.96, 0.98)
║                      ────────────────────                    ║
║                                                              ║
║                   ┌─────────────────────┐                   ║
║                   │  Create New World   │                   ║ ← Button (RGB 0.20, 0.35, 0.55)
║                   └─────────────────────┘                   ║
║                                                              ║
║                   ┌─────────────────────┐                   ║
║                   │  Load Existing World│                   ║
║                   └─────────────────────┘                   ║
║                                                              ║
║                   ┌─────────────────────┐                   ║
║                   │        Exit         │                   ║
║                   └─────────────────────┘                   ║
║                                                              ║
║              Version 0.1.0 - C++20 Voxel Engine             ║ ← Disabled Text (RGB 0.50, 0.52, 0.55)
║                                                              ║
╚══════════════════════════════════════════════════════════════╝
```
Background: RGB(0.10, 0.11, 0.13) - Very dark blue-gray

## Settings Panel Mockup

```
╔══════════════════════════════════════════════════════════════════╗
║ Settings                                                   [X]   ║
╠══════════════════════════════════════════════════════════════════╣
║ ┌────────┬────────┬──────────┬──────────┐                       ║
║ │ Video  │ Audio  │ Controls │ Gameplay │                       ║ ← Tabs
║ └────────┴────────┴──────────┴──────────┘                       ║
║ ▲ Active Tab (RGB 0.22, 0.38, 0.58)                             ║
║                                                                  ║
║  Mouse Settings                                                  ║ ← Section Header
║  ────────────────────────────────────────────                   ║
║                                                                  ║
║  Mouse Sensitivity:                                              ║ ← White Text
║  ├───────●───────────────────────┤ 0.50                         ║ ← Slider (RGB 0.28, 0.48, 0.70)
║  0.1                           5.0                               ║
║                                                                  ║
║  ☑ Invert Mouse Y-Axis                                          ║ ← Checkbox (RGB 0.35, 0.65, 0.95)
║                                                                  ║
║  ────────────────────────────────────────────                   ║
║                                                                  ║
║  Camera Settings                                                 ║
║                                                                  ║
║  Field of View:                                                  ║
║  ├──────────●──────────────────┤ 75.0 degrees                   ║
║  60                         120                                  ║
║                                                                  ║
║  Tip: Lower sensitivity = more precise aiming                    ║ ← Helper Text
║                                                                  ║
║  ┌────────┐  ┌──────────────┐  ┌────────┐                      ║
║  │ Apply  │  │ Reset Defaults│  │ Close  │                      ║ ← Bottom Buttons
║  └────────┘  └──────────────┘  └────────┘                      ║
║                                                                  ║
╚══════════════════════════════════════════════════════════════════╝
```
Background: RGB(0.10, 0.11, 0.13) - Very dark
Input Fields: RGB(0.15, 0.16, 0.19) - Darker with contrast

## Input Field Example

```
╔═══════════════════════════════════════════════╗
║ World Name:                                   ║
║ ┌───────────────────────────────────────────┐ ║
║ │ My Awesome World                         │ ║ ← Input (RGB 0.15, 0.16, 0.19)
║ └───────────────────────────────────────────┘ ║   Text (RGB 0.95, 0.96, 0.98)
╚═══════════════════════════════════════════════╝
```

## Button States Comparison

```
Normal State:
┌─────────────────┐
│  Create World   │ ← RGB(0.20, 0.35, 0.55)
└─────────────────┘

Hover State:
┌─────────────────┐
│  Create World   │ ← RGB(0.28, 0.48, 0.70) - Brighter
└─────────────────┘

Active/Pressed State:
┌─────────────────┐
│  Create World   │ ← RGB(0.18, 0.32, 0.50) - Darker
└─────────────────┘

Disabled State:
┌─────────────────┐
│  Create World   │ ← Same background but text RGB(0.50, 0.52, 0.55)
└─────────────────┘
```

## Dropdown/Combo Box Example

```
Resolution: ┌──────────────────────────▼┐
            │ 1920 x 1080 (Full HD)    │ ← Selected (RGB 0.15, 0.16, 0.19)
            └──────────────────────────┘

When opened:
            ┌──────────────────────────┐
            │  1280 x 720 (HD)         │
            │  1366 x 768              │
            │  1600 x 900 (HD+)        │
            │► 1920 x 1080 (Full HD)   │ ← Active (RGB 0.22, 0.38, 0.58)
            │  2560 x 1440 (2K)        │
            │  3840 x 2160 (4K)        │
            └──────────────────────────┘
```

## List Box Example (Load World)

```
╔═══════════════════════════════════════════════╗
║ Select a world to load:                       ║
║                                               ║
║ ┌───────────────────────────────────────────┐ ║
║ │ ▸ MyAwesomeWorld                         │ ║ ← Selected (RGB 0.28, 0.48, 0.70)
║ │   TestWorld                               │ ║
║ │   ShowcaseDemo                            │ ║
║ │   ExperimentalBuild                       │ ║
║ │   CreativeMode                            │ ║
║ └───────────────────────────────────────────┘ ║
╚═══════════════════════════════════════════════╝
```

## Contrast Comparison

### Before (Poor Readability)
```
Background: Dark gray (#3C3C3C)
Text: Light gray (#CCCCCC)
Contrast Ratio: ~4.5:1 (Marginal)
```

### After (Excellent Readability)
```
Background: Very dark blue-gray (#1A1C21)
Text: Bright white (#F2F4F9)
Contrast Ratio: ~15:1 (Excellent - WCAG AAA)
```

## Key Improvements

1. **Text Readability**: 
   - Old: Light gray text on gray background
   - New: Bright white text on very dark background
   - Improvement: ~3.3x better contrast ratio

2. **Visual Hierarchy**:
   - Old: Flat appearance with minimal depth
   - New: Clear separation with varied background shades

3. **Interactive Feedback**:
   - Old: Subtle hover states
   - New: Clear blue accent on hover/active states

4. **Modern Aesthetics**:
   - Old: Sharp corners, tight spacing
   - New: Rounded corners (4-6px), generous padding

5. **Eye Comfort**:
   - Old: Bright gray backgrounds
   - New: Very dark backgrounds with blue tint
   - Benefit: Reduced eye strain in low-light conditions

## Accessibility

- **WCAG 2.1 Compliance**: Meets AAA standard for text contrast (>7:1)
- **Color Blind Friendly**: Blue accent colors work well for most color vision types
- **Low Light Friendly**: Dark backgrounds reduce screen glare
- **High Contrast**: Clear distinction between all UI elements

## Implementation Notes

All colors are implemented as floating-point RGB values (0.0-1.0) in ImGui's color system:
- Values shown as RGB(0.10, 0.11, 0.13) = 10%, 11%, 13% brightness
- Converted to hex: RGB(26, 28, 33) = #1A1C21
- ImGui uses ImVec4 with RGBA channels

The theme is applied once during ImGui initialization in `ImGuiContext::initialize()` and affects all ImGui windows, panels, and widgets throughout the application.
