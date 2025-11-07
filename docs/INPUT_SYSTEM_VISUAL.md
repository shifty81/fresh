# Input System Visual Guide

## Input Mode State Diagram

```
                    ┌─────────────────┐
                    │   GameMode      │
                    │  (Default)      │
                    │                 │
                    │ • Cursor hidden │
                    │ • Captured      │
                    │ • FPS controls  │
                    └────────┬────────┘
                             │
                    ┌────────┴────────┐
                    │                 │
            ┌───────▼──────┐   ┌─────▼─────────┐
            │              │   │               │
            │  Press 'T'   │   │  Hold Alt     │
            │  (Editor)    │   │  (Temporary)  │
            │              │   │               │
            └───────┬──────┘   └─────┬─────────┘
                    │                │
                    ▼                ▼
            ┌───────────────┐  ┌─────────────┐
            │   UIMode      │  │  UIMode     │
            │  (Persistent) │  │ (Temporary) │
            │               │  │             │
            │ • Cursor free │  │ Auto-return │
            │ • Visible     │  │ on release  │
            │ • UI active   │  │             │
            └───────┬───────┘  └──────┬──────┘
                    │                 │
            ┌───────▼──────┐          │
            │              │          │
            │  Press 'T'   │          │
            │  (Close)     │    Release Alt
            │              │          │
            └───────┬──────┘          │
                    │                 │
                    └────────┬────────┘
                             │
                    ┌────────▼────────┐
                    │   GameMode      │
                    │  (Return)       │
                    └─────────────────┘


          ┌─────────────────────────────┐
          │      BuildMode (Future)     │
          │                             │
          │  • Cursor visible           │
          │  • Specialized controls     │
          │  • Ship builder/editor      │
          └─────────────────────────────┘
```

## Cursor States

### GameMode
```
┌──────────────────────────────┐
│      Game Window             │
│                              │
│          ┌───┐               │
│          │ + │  ← Cursor     │
│          └───┘    Hidden &   │
│                   Captured   │
│                              │
│  [Voxel World Rendering]     │
│                              │
└──────────────────────────────┘
    Mouse movement = Camera rotation
    Click = Block interaction
```

### UIMode (Hold Alt or Editor)
```
┌──────────────────────────────┐
│      Game Window             │
│   ┌────────────────┐         │
│   │  [Button]   ➤  │ ← Cursor│
│   │  [Slider]      │   Free  │
│   │  [Menu]        │         │
│   └────────────────┘         │
│                              │
│  [Voxel World Background]    │
│                              │
└──────────────────────────────┘
    Mouse movement = Cursor moves
    Click = UI interaction
```

## Hold Alt Flow

### Timeline View

```
Time →

Frame 1:    [GameMode]
            • Player moving with WASD
            • Camera controlled by mouse
            • Cursor: hidden, captured

Frame 2:    User presses Alt key
            ↓
            [GameMode → UIMode transition]
            • Save current mode (baseMode = GameMode)
            • Switch to UIMode (temporary)
            • Show cursor
            • Release cursor capture

Frame 3-5:  [UIMode (temporary)]
            • Alt key held down
            • Cursor visible and free
            • Can click UI elements
            • Player movement disabled
            • Camera locked

Frame 6:    User releases Alt key
            ↓
            [UIMode → GameMode transition]
            • Restore saved mode (GameMode)
            • Hide cursor
            • Capture cursor
            • Clear temporary flag

Frame 7:    [GameMode]
            • Back to normal gameplay
            • Player moving with WASD
            • Camera controlled by mouse
            • Cursor: hidden, captured
```

## Input Processing Flow

```
┌─────────────────────────────────────────────┐
│          Input Manager Update               │
│                                             │
│  1. Clear previous frame's input            │
│  2. Update gamepad states                   │
│  3. Check Alt key state                     │
│     ├─ Alt pressed + GameMode?              │
│     │  └─ Switch to UIMode (temporary)      │
│     └─ Alt released + temporary mode?       │
│        └─ Return to base mode               │
│  4. Update mouse delta                      │
└──────────────┬──────────────────────────────┘
               │
               ▼
┌─────────────────────────────────────────────┐
│          Engine Process Input               │
│                                             │
│  • Poll window events                       │
│  • Check GUI input capture                  │
│  • Process keyboard shortcuts (T, ESC)      │
└──────────────┬──────────────────────────────┘
               │
               ▼
┌─────────────────────────────────────────────┐
│          Engine Update                      │
│                                             │
│  IF inGameMode:                             │
│    • Process player input                   │
│    • Handle mouse camera                    │
│    • Process block interactions             │
│  ELSE:                                      │
│    • Process UI input only                  │
│    • Skip player movement                   │
│    • Skip camera control                    │
└─────────────────────────────────────────────┘
```

## Key Features Visualization

### Feature 1: Seamless Mode Switching
```
Gameplay → [Hold Alt] → UI Access → [Release Alt] → Gameplay
  ↓                        ↓                          ↓
Shooting              Accessing menu            Back to shooting
blocks                options                   blocks
```

### Feature 2: Context-Aware Cursor
```
Context          │ Cursor State      │ Controls Active
─────────────────┼───────────────────┼─────────────────
Playing game     │ Hidden, Captured  │ WASD, Mouse look
Holding Alt      │ Visible, Free     │ UI clicks only
Opening editor   │ Visible, Free     │ UI + keyboard
In menu          │ Visible, Free     │ UI + keyboard
Building ship    │ Visible, Free     │ Build controls
```

### Feature 3: Input Isolation
```
┌────────────┐         ┌────────────┐
│  GameMode  │         │   UIMode   │
│            │         │            │
│ WASD ✓     │         │ WASD ✗     │
│ Mouse ✓    │         │ Mouse ✗    │
│ Clicks ✓   │         │ Clicks ✗   │
│ UI ✗       │         │ UI ✓       │
└────────────┘         └────────────┘

No conflicts! Each mode has exclusive control.
```

## Example Scenarios

### Scenario 1: Quick Menu Access
```
1. Playing game (GameMode)
   └─ Press Alt → Cursor appears
2. Click on settings button (UIMode temporary)
   └─ Adjust volume slider
3. Release Alt → Return to game (GameMode)
   └─ Continue playing immediately
```

### Scenario 2: Editor Workflow
```
1. Playing game (GameMode)
   └─ Press T → Switch to UIMode
2. Editor opens with cursor visible
   └─ Select terrain tools, adjust settings
3. Press T again → Return to GameMode
   └─ Cursor hidden, back to gameplay
```

### Scenario 3: Build Menu (Future)
```
1. Playing game (GameMode)
   └─ Press B → Switch to BuildMode
2. Ship builder opens
   └─ Place components, rotate pieces
3. Press B or ESC → Return to GameMode
   └─ Back to normal controls
```

## Technical Architecture

```
┌─────────────────────────────────────────────┐
│              InputManager                    │
│                                             │
│  ┌─────────────┐  ┌──────────────┐         │
│  │ Key States  │  │ Mouse States │         │
│  │             │  │              │         │
│  │ • Alt held  │  │ • Delta      │         │
│  │ • WASD      │  │ • Buttons    │         │
│  │ • Actions   │  │ • Position   │         │
│  └─────────────┘  └──────────────┘         │
│         │                  │                │
│         └──────┬───────────┘                │
│                ▼                            │
│  ┌─────────────────────────┐               │
│  │   Mode State Machine    │               │
│  │                         │               │
│  │  • currentMode          │               │
│  │  • baseMode             │               │
│  │  • temporaryModeSet     │               │
│  └─────────────────────────┘               │
│                │                            │
│                ▼                            │
│  ┌─────────────────────────┐               │
│  │   Cursor Controller     │               │
│  │                         │               │
│  │  • setCursorMode()      │               │
│  │  • GLFW cursor state    │               │
│  └─────────────────────────┘               │
└─────────────────────────────────────────────┘
```

## Benefits

### 1. No Cursor Trap
```
Before: Menu opens → Cursor stuck in center → Can't click
After:  Menu opens → Cursor free → Click buttons easily
```

### 2. Quick UI Access
```
Before: ESC → Pause → Navigate → Resume → ESC again
After:  Hold Alt → Click what you need → Release Alt
```

### 3. Context Preservation
```
Before: Mode changes → Lose game state → Confusion
After:  Hold Alt → Temporary mode → Auto-return → Seamless
```

## Summary

✅ Three distinct input modes (Game, UI, Build)
✅ Hold Alt for temporary UI access
✅ Automatic cursor management
✅ No input conflicts
✅ Context-aware controls
✅ Seamless transitions
✅ Future-proof architecture
