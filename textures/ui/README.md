# UI Assets

This directory contains placeholder UI (User Interface) assets for the Fresh Voxel Engine.

## Available Assets

### Gameplay UI
- **crosshair.png** (32x32) - Standard crosshair for center screen aiming
- **crosshair_large.png** (64x64) - Larger crosshair variant
- **cursor.png** (32x32) - Mouse cursor for menu navigation

### Inventory/Hotbar
- **hotbar_slot.png** (64x64) - Normal inventory slot background
- **hotbar_slot_selected.png** (64x64) - Selected/active inventory slot

### Menu Elements
- **button_normal.png** (200x50) - Standard button background
- **button_hover.png** (200x50) - Hovered button background
- **panel_background.png** (400x300) - Semi-transparent panel background

### HUD Elements
- **health_heart_full.png** (16x16) - Full health indicator
- **health_heart_empty.png** (16x16) - Empty health indicator

### Branding
- **logo.png** (256x256) - Fresh Voxel Engine logo placeholder

## Usage Examples

### Displaying Crosshair
```cpp
auto& texManager = fresh::TextureManager::getInstance();
auto crosshair = texManager.loadTexture("textures/ui/crosshair.png");
// Render at screen center
```

### Inventory Hotbar
```cpp
// Load slot textures
auto slotNormal = texManager.loadTexture("textures/ui/hotbar_slot.png");
auto slotSelected = texManager.loadTexture("textures/ui/hotbar_slot_selected.png");

// Render 9 slots, highlight selected one
for (int i = 0; i < 9; i++) {
    auto texture = (i == selectedSlot) ? slotSelected : slotNormal;
    // Render slot at position
}
```

### Health Bar
```cpp
auto heartFull = texManager.loadTexture("textures/ui/health_heart_full.png");
auto heartEmpty = texManager.loadTexture("textures/ui/health_heart_empty.png");

// Render health (e.g., 7 out of 10 hearts)
for (int i = 0; i < maxHealth; i++) {
    auto heart = (i < currentHealth) ? heartFull : heartEmpty;
    // Render heart
}
```

## Asset Properties

All UI assets use:
- **Format**: PNG with RGBA (transparency supported)
- **Color Space**: sRGB
- **Transparency**: Alpha channel for semi-transparent backgrounds

## Future Enhancements

- Animated UI elements
- Theme variants (dark/light)
- Localized text overlays
- Higher resolution 4K assets
- Additional HUD elements (hunger, stamina, etc.)

## Credits

Procedurally generated using Python PIL for rapid prototyping and placeholder assets.
