# Dialogue Editor Visual Mockup Description

## Overall Layout (Unreal Engine Style)

```
╔════════════════════════════════════════════════════════════════════════════╗
║ Fresh Engine - Dialogue Editor                                      [_][□][X]║
╠════════════════════════════════════════════════════════════════════════════╣
║ File  Edit  View  Help                                     (#2A2A2A - Dark) ║
╠════════════════════════════════════════════════════════════════════════════╣
║ New │ Open │ Save │ + Dialogue │ + Choice │ + Lua Script │ + Condition │...║
╠════════════════════════════════════════════════════════════════════╦═══════╣
║                                                                    ║Details║
║  CANVAS AREA (Viewport)                        (#202020 - Darker) ║       ║
║  ┌─────────────────────────────────────────────────────────────┐  ║┌─────┐║
║  │ ┏━━━━━━━━━━━━━━━━━━━━┓                                       │  ║│Props││
║  │ ┃     Start      ┃  ●→ ┏━━━━━━━━━━━━━━━━━━┓                │  ║│Grid ││
║  │ ┃   (#3C8C3C)        ┃     ┃  NPC Greeting    ┃  ●→          │  ║│     ││
║  │ ┗━━━━━━━━━━━━━━━━━━━━┛     ┃  (#465A6E)       ┃              │  ║│Title││
║  │                            ┃────────────────── ┃              │  ║│Text ││
║  │                            ┃"Hello traveler!" ┃              │  ║│Etc. ││
║  │                            ┗━━━━━━━━━━━━━━━━━━┛              │  ║│     ││
║  │                                   ╲                           │  ║└─────┘║
║  │                Grid Lines          ╲    (Bezier Curve)       │  ║       ║
║  │                (#282828)            ╲                         │  ║       ║
║  │  ··········································                   │  ║       ║
║  │  ·                                  ·  ╲                      │  ║       ║
║  │  ·         Major Grid Lines         ·   ┏━━━━━━━━━━━━━━━┓   │  ║       ║
║  │  ·         (#373737 - lighter)      ·  ●→┃Player Choice  ┃   │  ║       ║
║  │  ··········································  ┃ (#508CC8)     ┃   │  ║       ║
║  │                                              ┃───────────────┃   │  ║       ║
║  │                                              ┃3 choices      ┃   │  ║       ║
║  │                                              ┗━━━━━━━━━━━━━━━┛   │  ║       ║
║  └─────────────────────────────────────────────────────────────┘  ║       ║
╠════════════════════════════════════════════════════════════════════╩═══════╣
║ Ready │ Position: (150, 200) │ Zoom: 100%                  (#1C1C1C - Dark)║
╚════════════════════════════════════════════════════════════════════════════╝
```

## Color Palette (Unreal-Inspired)

### Background Colors
- **Window Background**: `#2A2A2A` (42, 42, 42) - Main dark gray
- **Canvas/Viewport**: `#202020` (32, 32, 32) - Darker for workspace
- **Menu/Toolbar**: `#2A2A2A` (42, 42, 42) - Same as window
- **Status Bar**: `#1C1C1C` (28, 28, 28) - Darkest
- **Details Panel**: `#2A2A2A` (42, 42, 42)

### Text Colors
- **Normal Text**: `#D2D2D2` (210, 210, 210) - Light gray
- **Disabled Text**: `#646464` (100, 100, 100) - Medium gray
- **Hover Text**: `#FFFFFF` (255, 255, 255) - White

### Selection/Highlight
- **Selection**: `#FFC850` (255, 200, 80) - Gold/orange
- **Hover**: `#464646` (70, 70, 70) - Slightly lighter than background

### Node Colors (Header Bars)
- **Start Node**: `#3C8C3C` (60, 140, 60) - Green
- **End Node**: `#B43C3C` (180, 60, 60) - Red
- **Dialogue Node**: `#465A6E` (70, 90, 110) - Gray-blue
- **Choice Node**: `#508CC8` (80, 140, 200) - Blue
- **Lua Script Node**: `#8C50B4` (140, 80, 180) - Purple
- **Condition Node**: `#C88C3C` (200, 140, 60) - Orange

### Node Body
- **Node Body**: `#303030` (48, 48, 48) - Dark gray
- **Node Border**: `#505050` (80, 80, 80) - Light gray
- **Node Shadow**: `#000000` 50% opacity - Black shadow

### Grid
- **Minor Grid Lines**: `#282828` (40, 40, 40) - Very subtle
- **Major Grid Lines**: `#373737` (55, 55, 55) - Slightly visible

### Connections
- **Normal Link**: `#969696` (150, 150, 150) - Gray
- **Selected Link**: `#FFC850` (255, 200, 80) - Gold
- **Connection Pins**: `#50B450` (80, 180, 80) - Green

## Node Design Details

### Standard Node Structure
```
┏━━━━━━━━━━━━━━━━━━━━━┓
┃   Node Title       ┃ ← Header (colored based on type)
┃   (Bold, centered) ┃    Height: 28px, Color varies
┣━━━━━━━━━━━━━━━━━━━━━┫
┃                    ┃
┃  Content Area      ┃ ← Body (dark gray #303030)
┃  (Node-specific)   ┃    Contains preview text
┃                    ┃
●                   ● ← Connection pins (circles)
┗━━━━━━━━━━━━━━━━━━━━━┛
 ↑                   ↑
Input Pin          Output Pin
```

### Start Node (Green)
```
┏━━━━━━━━━━━━━━━━━━━━━┓
┃      Start        ┃ ← #3C8C3C green header
┣━━━━━━━━━━━━━━━━━━━━━┫
┃                    ┃
┃                    ┃ ← Minimal content
┃                   ●┃ ← Output pin only
┗━━━━━━━━━━━━━━━━━━━━━┛
Size: 200x100px
```

### Dialogue Node (Gray-blue)
```
┏━━━━━━━━━━━━━━━━━━━━━━━━┓
┃  NPC Greeting         ┃ ← #465A6E gray-blue header
┣━━━━━━━━━━━━━━━━━━━━━━━━┫
┃ Speaker: Village Elder┃
┃                        ┃
┃ "Greetings, traveler! ┃ ← Preview of dialogue text
┃  I have a quest..."   ┃    (truncated if long)
●                       ●┃
┗━━━━━━━━━━━━━━━━━━━━━━━━┛
Size: 250x120px
```

### Choice Node (Blue)
```
┏━━━━━━━━━━━━━━━━━━━━━┓
┃  Player Choice      ┃ ← #508CC8 blue header
┣━━━━━━━━━━━━━━━━━━━━━┫
┃                     ┃
┃  3 choices          ┃ ← Shows number of choices
┃                     ┃
●                    ●┃ ← Multiple output pins
                     ●┃    (one per choice)
                     ●┃
┗━━━━━━━━━━━━━━━━━━━━━┛
Size: 220x120px
```

### Lua Script Node (Purple)
```
┏━━━━━━━━━━━━━━━━━━━━━┓
┃  Start Quest       ┃ ← #8C50B4 purple header
┣━━━━━━━━━━━━━━━━━━━━━┫
┃                     ┃
┃  Lua Script         ┃ ← Script indicator
┃                     ┃
●                    ●┃
┗━━━━━━━━━━━━━━━━━━━━━┛
Size: 200x100px
```

### End Node (Red)
```
┏━━━━━━━━━━━━━━━━━━━━━┓
┃       End          ┃ ← #B43C3C red header
┣━━━━━━━━━━━━━━━━━━━━━┫
┃                     ┃
┃                     ┃ ← Minimal content
●                     ┃ ← Input pin only
┗━━━━━━━━━━━━━━━━━━━━━┛
Size: 150x80px
```

## Connection Style (Bezier Curves)

```
Start Node              Dialogue Node
    ●───────────────────────●
       ╲                   ╱
        ╲                 ╱
         ╲_______________╱
         
    Smooth Bezier curve like Unreal Blueprint wires
    - Width: 2.5px
    - Color: #969696 (normal), #FFC850 (selected)
    - Arrow head at end
    - Label in middle (if present) with background box
```

## Details Panel (Right Side)

```
╔════════════════╗
║ Details        ║ ← Header (#1C1C1C, bold)
╠════════════════╣
║ Title          ║
║ ┌────────────┐ ║
║ │NPC Greeting│ ║
║ └────────────┘ ║
║                ║
║ Speaker        ║
║ ┌────────────┐ ║
║ │Elder       │ ║
║ └────────────┘ ║
║                ║
║ DialogueText   ║
║ ┌────────────┐ ║
║ │Greetings...│ ║
║ │            │ ║
║ └────────────┘ ║
║                ║
║ Position       ║
║ X: [300]       ║
║ Y: [200]       ║
╚════════════════╝
```

## Menu Bar (Unreal Style)

```
╔════════════════════════════════════════════╗
║ File  Edit  View  Help                    ║ ← Background: #2A2A2A
╚════════════════════════════════════════════╝    Text: #D2D2D2
     ↓ (on click)                                 Hover: #464646
┌─────────────┐
│ New         │
│ Open...     │
│ Save        │
│ Save As...  │
│ ─────────── │ ← Separator
│ Exit        │
└─────────────┘
Background: #2A2A2A
Hover: #464646
```

## Toolbar (Unreal Style)

```
╔════════════════════════════════════════════════════════════════╗
║ [New] [Open] [Save] │ [+ Dialogue] [+ Choice] [+ Lua Script]... ║
╚════════════════════════════════════════════════════════════════╝

Button colors:
- Normal: #2A2A2A
- Hover: #464646
- Pressed: #191919
- Text color matches node type for creation buttons
```

## Status Bar (Bottom)

```
╔════════════════════════════════════════════════════════════╗
║ Ready │ Position: (150, 200) │ Zoom: 100%                  ║
╚════════════════════════════════════════════════════════════╝
Background: #1C1C1C (darkest)
Text: #B4B4B4 (lighter gray)
Separators: │ in #505050
```

## Interaction Visual Feedback

### Node Selection
- **Unselected**: 1px gray border (#505050)
- **Selected**: 2px gold border (#FFC850)
- **Hover**: Slight glow effect

### Link Creation (Temporary)
```
Source Node ●───────────────→ Mouse Cursor
              Yellow dashed line (#FFEB3B)
              Follows mouse pointer
```

### Context Menu
```
┌─────────────────────┐
│ Edit Node           │ ← Hover: #464646
│ Delete Node         │
│ ─────────────────── │
│ Add Dialogue Node   │
│ Add Choice Node     │
│ Add Lua Script Node │
│ Add Condition Node  │
│ Add End Node        │
└─────────────────────┘
Background: #2A2A2A
Text: #D2D2D2
Border: 1px #505050
```

## Typography

- **Headers**: Segoe UI, 9pt, Bold
- **Body Text**: Segoe UI, 8pt, Regular
- **Menu Items**: Segoe UI, 9pt, Regular
- **Status Bar**: Segoe UI, 8pt, Regular
- **Node Titles**: Segoe UI, 9pt, Bold
- **Node Content**: Segoe UI, 8pt, Regular

## Comparison to Unreal Engine

| Element | Unreal Engine | Dialogue Editor |
|---------|---------------|-----------------|
| Canvas Background | Dark gray | #202020 ✓ |
| Node Headers | Colored bars | Colored bars ✓ |
| Node Bodies | Dark gray | #303030 ✓ |
| Connections | Bezier curves | Bezier curves ✓ |
| Connection Pins | Circles | Circles ✓ |
| Details Panel | Right side | Right side ✓ |
| Grid Lines | Major/minor | Major/minor ✓ |
| Selection Color | Orange/gold | #FFC850 ✓ |
| Menu Style | Dark theme | Dark theme ✓ |

## Accessibility Notes

- **High Contrast**: Light text on dark background
- **Color Coding**: Node types distinguishable by color
- **Clear Selection**: Bold gold border for selected items
- **Status Feedback**: Status bar shows current state
- **Tooltips**: Available on hover (not shown in mockup)
- **Keyboard Shortcuts**: Shown in menus

## Responsive Behavior

- **Zoom**: 25% to 300% (mouse wheel)
- **Pan**: Move canvas with middle mouse (future)
- **Splitter**: Resizable between canvas and details panel
- **Window Resize**: All panels scale appropriately
- **Font Scaling**: Respects system DPI settings

This visual design creates a familiar, professional environment for users transitioning from Unreal Engine to Fresh Engine, while maintaining clarity and usability for beginners.
