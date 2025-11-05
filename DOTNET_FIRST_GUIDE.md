# .NET First Development Guide

## Overview

Fresh Voxel Engine has transitioned to a **.NET 9 first** development approach. This means that .NET 9 is now the primary and recommended way to build and develop with the engine.

## What Changed?

### Before (Old Approach)
- CMake and Visual Studio were the primary build method
- .NET bindings were optional/secondary
- Documentation positioned C++ development first

### After (New Approach - Current)
- **.NET 9 SDK is checked first** during installation
- **C# managed wrapper is built first**
- **Documentation positions .NET development as primary**
- C++ native engine is still available as the high-performance backend
- CMake/Visual Studio are used for the native engine backend

## Why .NET First?

### Benefits of .NET First Approach

1. **Modern Development Experience**
   - C# 12 with latest language features
   - Better IDE support and tooling
   - Hot reload and rapid iteration

2. **Safer Development**
   - Memory safety by default
   - Automatic garbage collection
   - Fewer crashes and memory leaks

3. **Faster Prototyping**
   - Write game logic quickly in C#
   - Immediate feedback with managed debugging
   - Rich ecosystem of NuGet packages

4. **Lower Barrier to Entry**
   - More approachable for new developers
   - Easier to learn than C++
   - Better error messages

5. **Best of Both Worlds**
   - High-level C# for game logic
   - High-performance C++ for engine internals
   - Seamless P/Invoke interop

## Getting Started with .NET First

### Step 1: Install .NET 9 SDK

**Download:** https://dotnet.microsoft.com/download/dotnet/9.0

**Verify installation:**
```batch
dotnet --version
```

Should output `9.0.x` or higher.

### Step 2: Run the Installer

```batch
git clone https://github.com/shifty81/fresh.git
cd fresh
install.bat
```

The installer will now:
1. ✅ Check for .NET 9 SDK first
2. ✅ Build the managed wrapper (C# bindings)
3. ✅ Set up vcpkg and dependencies
4. ✅ Generate Visual Studio solution
5. ✅ Build the native engine backend

### Step 3: Start Developing

**Recommended: C# Development**
```csharp
using FreshEngine.Managed;

class Program
{
    static void Main()
    {
        using var engine = new Engine();
        
        if (engine.Initialize())
        {
            engine.WindowTitle = "My Voxel Game";
            engine.Run();
            engine.Shutdown();
        }
    }
}
```

**Alternative: C++ Development**
- Open `build\FreshVoxelEngine.sln` in Visual Studio
- Work directly with the native engine

## Development Paths

### Path 1: C# Game Development (Recommended)

**Best For:**
- Game logic and scripting
- Rapid prototyping
- UI and gameplay systems
- Most development scenarios

**Tools:**
- Visual Studio 2022 or VS Code
- .NET 9 SDK
- FreshEngine.Managed.dll

**Documentation:**
- [dotnet/README.md](dotnet/README.md)
- [DOTNET_INTEGRATION.md](DOTNET_INTEGRATION.md)

### Path 2: C++ Engine Development

**Best For:**
- Engine internals
- Performance-critical code
- Graphics API implementation
- Low-level optimizations

**Tools:**
- Visual Studio 2022
- CMake
- C++ compiler

**Documentation:**
- [DEVELOPER_SETUP.md](DEVELOPER_SETUP.md)
- [ARCHITECTURE.md](ARCHITECTURE.md)

## Migration Guide

### For Existing Users

If you were using the old CMake-first approach:

1. **Install .NET 9 SDK**
   - Download from: https://dotnet.microsoft.com/download/dotnet/9.0

2. **Rebuild with new installer**
   ```batch
   install.bat
   ```

3. **Existing C++ code still works**
   - Your native code is unchanged
   - You can now also use C# bindings

4. **Try C# development**
   - See examples in [dotnet/README.md](dotnet/README.md)
   - Experiment with managed wrapper

### For New Users

Just follow the Quick Start in [README.md](README.md):
1. Install .NET 9 SDK
2. Clone repository
3. Run `install.bat`
4. Start developing in C#

## Build Scripts

### Main Scripts (Updated)

| Script | Purpose | .NET Support |
|--------|---------|--------------|
| `install.bat` | Full installation | ✅ Checks .NET first |
| `check_dotnet.bat` | .NET check & build | ✅ Primary script |
| `build_all.bat` | Complete build | ✅ Builds .NET first |
| `generate_vs2022.bat` | CMake generation | Native backend |

### Execution Flow

```
install.bat
    ↓
1. Check .NET 9 SDK (required)
    ↓
2. Build managed wrapper (C# bindings)
    ↓
3. Check CMake & VS2022
    ↓
4. Set up vcpkg
    ↓
5. Generate VS solution
    ↓
6. Build native engine
    ↓
7. Done!
```

## FAQ

### Q: Do I still need CMake and Visual Studio?

**A:** Yes, for the native engine backend. But .NET SDK is checked first and is the primary development method.

### Q: Can I still develop in C++?

**A:** Absolutely! The native engine is still C++ and you can work on it directly. The .NET wrapper provides an additional development path.

### Q: What if I don't want to use .NET?

**A:** You can skip the .NET check in the installer, but we strongly recommend using it for the best development experience.

### Q: Will this make the engine slower?

**A:** No! The native engine remains high-performance C++. C# is for game logic, which benefits from the managed environment. Performance-critical code stays in C++.

### Q: Is this a complete rewrite?

**A:** No! This is just a change in the recommended development approach. The core engine is the same, but we've added C# bindings as the primary interface.

### Q: Can I mix C# and C++ code?

**A:** Yes! That's the whole point. Use C# for game logic and UI, while the engine core runs in native C++.

## Resources

### Documentation
- [README.md](README.md) - Main documentation
- [DOTNET_INTEGRATION.md](DOTNET_INTEGRATION.md) - .NET integration details
- [dotnet/README.md](dotnet/README.md) - C# bindings documentation
- [QUICKSTART_VISUAL.md](QUICKSTART_VISUAL.md) - Visual quick start guide

### Build Tools
- [tools/build_tools/README.md](tools/build_tools/README.md) - Build tools documentation

### Community
- GitHub Issues: https://github.com/shifty81/fresh/issues
- GitHub Discussions: https://github.com/shifty81/fresh/discussions

## Support

If you encounter issues with the .NET first approach:

1. **Check .NET SDK installation**
   ```batch
   dotnet --version
   ```

2. **Review installation logs**
   - Check `logs/` directory for detailed output

3. **Report issues**
   - Open an issue on GitHub with logs and error messages

4. **Ask for help**
   - Use GitHub Discussions for questions

---

**Welcome to .NET first development with Fresh Voxel Engine!** 🎮🚀
