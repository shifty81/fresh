# Fresh Voxel Engine Documentation

Welcome to the Fresh Voxel Engine documentation!

## ⚠️ Important Notice: ImGui Removed

**ImGui has been completely removed from Fresh Voxel Engine as of 2025-11-22.**

If you encounter references to ImGui in documentation:
- These are **historical references only**
- The engine now uses **Windows Native Win32 UI exclusively**
- See [../IMGUI_DEPRECATION_NOTICE.md](../IMGUI_DEPRECATION_NOTICE.md) for details

All UI panels are now implemented using native Win32 controls for better Windows integration and performance.

---

## Documentation Index

### Getting Started
- [Getting Started Guide](getting-started/GETTING_STARTED.md)
- [Build Instructions](getting-started/BUILD.md)
- [Automated Build](getting-started/AUTOMATED_BUILD.md)

### Architecture
- [System Architecture](architecture/ARCHITECTURE.md)
- [UI Architecture](architecture/UI_ARCHITECTURE.md) - **Native Win32 UI only**
- [Windows Integration](architecture/WINDOWS_INTEGRATION.md)
- [.NET Integration](architecture/DOTNET_INTEGRATION.md)

### Editor
- [Editor Integration](editor/EDITOR_INTEGRATION.md)
- [GUI Guide](editor/GUI_GUIDE.md) - **Win32 Native UI Guide**
- [Editor Features Status](editor/EDITOR_FEATURES_STATUS.md)

### Guides
- [Controls](guides/CONTROLS.md)
- [Input System](guides/INPUT_SYSTEM.md)
- [Audio System](guides/AUDIO_SYSTEM.md)
- [Logging](guides/LOGGING.md)
- [Terraforming](guides/TERRAFORMING.md)

### Development
- [Contributing Guidelines](community/CONTRIBUTING.md)
- [C++ Guidelines](community/CPP_GUIDELINES.md)
- [Development Tools](community/DEVELOPMENT_TOOLS.md)
- [FAQ](community/FAQ.md)

### Testing
- [Testing Guide](testing/TESTING.md)
- [GUI Testing Guide](testing/GUI_TESTING_GUIDE.md)

### Status
- [Project Status](status/PROJECT_STATUS.md)
- [Roadmap](status/ROADMAP.md)

---

## Quick Links

- **Main README**: [../README.md](../README.md)
- **Documentation Guide**: [../DOCUMENTATION.md](../DOCUMENTATION.md)
- **Windows Native UI Policy**: [../WINDOWS_NATIVE_ONLY_POLICY.md](../WINDOWS_NATIVE_ONLY_POLICY.md)
- **ImGui Deprecation Notice**: [../IMGUI_DEPRECATION_NOTICE.md](../IMGUI_DEPRECATION_NOTICE.md)

---

## Contributing to Documentation

Found an issue or want to improve the docs?

1. Check [community/CONTRIBUTING.md](community/CONTRIBUTING.md)
2. Submit a pull request
3. Open an issue for major changes

---

**Note**: This documentation is for the latest version of Fresh Voxel Engine. Some older documents may reference deprecated features like ImGui. Always refer to the latest documentation for current practices.
