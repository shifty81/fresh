# Windows-Native Integration Guide

## Overview

Fresh Voxel Engine now includes deep Windows integration features that provide a native Windows 10/11 experience while maintaining cross-platform compatibility. These features are automatically enabled on Windows and gracefully disabled on other platforms.

## Features

### 1. Windows Theme System

Automatically detects and applies Windows system themes to the editor interface.

**Supported Themes:**
- **Auto** - Automatically matches Windows dark/light mode setting
- **Light** - Windows light theme
- **Dark** - Windows dark theme (default)
- **High Contrast** - Windows high contrast mode
- **Classic** - Classic Windows theme
- **Custom** - User-defined custom theme

**Features:**
- Automatic dark mode detection
- Windows accent color integration
- Modern Windows 11 styling with rounded corners
- Real-time theme updates
- Custom theme editor with color pickers

**Access:** Settings → Windows Customization → Theme tab

### 2. Native Windows Dialogs

Replaces generic dialogs with Windows-native dialogs that match the system UI.

**Available Dialogs:**
- **Open File** - Native file picker with type filters and multi-select
- **Save File** - Native save dialog with filename and extension suggestions
- **Folder Browser** - Native folder selection dialog
- **Message Boxes** - Native message boxes with various button configurations

**Features:**
- Supports all standard file filters
- Multi-file selection
- Recent folders integration
- Native Windows icons and styling
- Keyboard shortcuts and navigation

**Access:** Settings → Windows Customization → Dialogs tab

### 3. Taskbar Integration

Provides Windows taskbar features for better system integration.

**Features:**
- **Progress Indicator** - Show progress on taskbar button (Normal, Error, Paused, Indeterminate)
- **Flash Taskbar** - Get user attention with taskbar flashing
- **Overlay Icons** - Display status icons on taskbar button
- **Jump Lists** - Recent files and quick actions (planned)

**Use Cases:**
- Show build/loading progress
- Notify users of important events
- Display current status (recording, processing, etc.)

**Access:** Settings → Windows Customization → Taskbar tab

### 4. Windows Notifications

Toast notifications that appear in the Windows Action Center (planned for future update).

## How to Use

### Opening Windows Customization Panel

1. **Via Menu:** Settings → Windows Customization... (Ctrl+W)
2. **Via Code:** Call `EditorManager::getWindowsCustomizationPanel()->setVisible(true)`

### Changing Themes

```cpp
// Get theme manager from editor
auto* themeManager = editorManager->getWindowsCustomizationPanel()
                                   ->getThemeManager();

// Apply a preset theme
themeManager->setTheme(WindowsTheme::Dark);

// Or create a custom theme
ThemeColors customColors;
customColors.background = 0xFF1E1E1EFF;
customColors.accent = 0xFF0078D4FF;
customColors.text = 0xFFE0E0E0FF;
themeManager->setCustomTheme(customColors);
themeManager->setTheme(WindowsTheme::Custom);
```

### Using Native Dialogs

```cpp
// Get dialog manager from editor
auto* dialogManager = editorManager->getWindowsCustomizationPanel()
                                    ->getDialogManager();

// Open file dialog
std::vector<FileFilter> filters = {
    {"Text Files", "*.txt"},
    {"All Files", "*.*"}
};
auto files = dialogManager->showOpenFileDialog("Select a file", filters);

// Save file dialog
auto savePath = dialogManager->showSaveFileDialog("Save as", "document.txt", filters);

// Message box
auto result = dialogManager->showMessageBox(
    "Confirmation",
    "Do you want to save changes?",
    MessageBoxButtons::YesNoCancel,
    MessageBoxIcon::Question
);
```

### Using Taskbar Features

```cpp
// Get taskbar manager from editor
auto* taskbarManager = editorManager->getWindowsCustomizationPanel()
                                     ->getTaskbarManager();

// Show progress
taskbarManager->setProgressState(TaskbarProgressState::Normal);
taskbarManager->setProgressValue(50, 100); // 50%

// Flash taskbar for attention
taskbarManager->flashTaskbar(true); // Flash until window gets focus

// Show error state
taskbarManager->setProgressState(TaskbarProgressState::Error);

// Clear progress
taskbarManager->setProgressState(TaskbarProgressState::NoProgress);
```

## Technical Details

### Architecture

All Windows-native features are:
- Conditionally compiled using `#ifdef _WIN32`
- Integrated into `EditorManager` for easy access
- Built on top of Win32 API and COM interfaces
- Compatible with Windows 10 and Windows 11

### File Organization

```
include/ui/
├── WindowsThemeManager.h          # Theme detection and management
├── WindowsDialogManager.h         # Native dialog wrappers
├── WindowsTaskbarManager.h        # Taskbar integration
└── WindowsCustomizationPanel.h    # UI for all Windows features

src/ui/
├── WindowsThemeManager.cpp
├── WindowsDialogManager.cpp
├── WindowsTaskbarManager.cpp
└── WindowsCustomizationPanel.cpp
```

### Dependencies

- **Windows API** - Core Win32 functions
- **dwmapi.lib** - Desktop Window Manager API for theme detection
- **shell32.lib** - Shell API for dialogs
- **COM** - Component Object Model for taskbar integration

All dependencies are standard Windows libraries included with the Windows SDK.

### Platform Compatibility

| Feature | Windows 10 | Windows 11 | Linux | macOS |
|---------|-----------|-----------|-------|-------|
| Theme Manager | ✅ | ✅ | ❌ | ❌ |
| Native Dialogs | ✅ | ✅ | ❌ | ❌ |
| Taskbar Integration | ✅ | ✅ | ❌ | ❌ |
| Notifications | 🔶 Planned | 🔶 Planned | ❌ | ❌ |

On non-Windows platforms, these features are automatically disabled at compile time, and the engine falls back to cross-platform alternatives (ImGui dialogs, etc.).

## Customization Examples

### Custom Theme Example

Create a custom theme with your brand colors:

```cpp
WindowsThemeManager themeManager;
themeManager.initialize();

ThemeColors brandTheme;
brandTheme.background = 0xFF2D2D30FF;  // Dark gray
brandTheme.foreground = 0xFF3E3E42FF;  // Medium gray
brandTheme.accent = 0xFFFF6B35FF;      // Brand orange
brandTheme.text = 0xFFFFFFFFFF;        // White
brandTheme.textDisabled = 0xFF808080FF; // Gray
brandTheme.border = 0xFF555555FF;      // Border gray
brandTheme.highlight = 0xFFCC5529FF;   // Darker orange
brandTheme.shadow = 0xFF000000FF;      // Black

themeManager.setCustomTheme(brandTheme);
themeManager.setTheme(WindowsTheme::Custom);
themeManager.applyToImGui();
```

### Progress Tracking Example

Show progress during a long operation:

```cpp
void performLongOperation(WindowsTaskbarManager* taskbar) {
    taskbar->setProgressState(TaskbarProgressState::Normal);
    
    for (int i = 0; i <= 100; i++) {
        // Do work...
        taskbar->setProgressValue(i, 100);
        
        if (errorOccurred) {
            taskbar->setProgressState(TaskbarProgressState::Error);
            taskbar->flashTaskbar(true);
            return;
        }
    }
    
    taskbar->setProgressState(TaskbarProgressState::NoProgress);
}
```

## Future Enhancements

Planned features for future releases:

1. **Toast Notifications** - Windows 10/11 toast notifications with actions
2. **Jump Lists** - Recent files and quick actions in taskbar menu
3. **Thumbnail Toolbar** - Playback controls in taskbar preview
4. **Live Tiles** - Dynamic Start menu tile (Windows 10)
5. **Share Contract** - Windows Share integration
6. **Windows Hello** - Biometric authentication integration

## Troubleshooting

### Theme not applying

**Problem:** Windows theme changes don't reflect in the application

**Solution:** 
1. Restart the application
2. Manually set theme to "Auto" in Windows Customization panel
3. Check Windows Settings → Personalization → Colors

### Dialogs not appearing

**Problem:** Native dialogs don't show up

**Solution:**
1. Ensure application has focus
2. Check if dialogs are being blocked by antivirus/security software
3. Verify window handle is valid in dialog manager initialization

### Taskbar features not working

**Problem:** Progress indicator or flash not visible

**Solution:**
1. Check taskbar settings (Windows Settings → Personalization → Taskbar)
2. Ensure taskbar is not hidden
3. Try running application as administrator

## Contributing

To contribute to Windows-native integration features:

1. Follow the existing pattern of conditional compilation (`#ifdef _WIN32`)
2. Ensure features gracefully degrade on non-Windows platforms
3. Test on both Windows 10 and Windows 11
4. Document any new Windows API dependencies
5. Add examples to this guide

## See Also

- [ARCHITECTURE.md](ARCHITECTURE.md) - Overall system architecture
- [CONTRIBUTING.md](CONTRIBUTING.md) - Contribution guidelines
- [BUILD.md](BUILD.md) - Build instructions
- [Microsoft Docs - Windows App Development](https://docs.microsoft.com/en-us/windows/apps/)
