# Automated Build Quick Reference

## 🚀 One Command Build

```powershell
.\setup-and-build.ps1
```

**That's it!** The script will:
- ✅ Check prerequisites
- ✅ Set up vcpkg
- ✅ Generate Visual Studio solution
- ✅ Build the project
- ✅ Open Visual Studio

**Time:** 10-20 minutes (first run)

---

## Common Usage

### Default (Full Build)
```powershell
.\setup-and-build.ps1
```

### Just Setup (No Build)
```powershell
.\setup-and-build.ps1 -SkipBuild
```

### Debug Build
```powershell
.\setup-and-build.ps1 -BuildConfig Debug
```

### Build Without Opening VS
```powershell
.\setup-and-build.ps1 -OpenVS $false
```

---

## Prerequisites

✅ **Visual Studio 2022** (with C++ workload)  
✅ **CMake 3.20+** (in PATH)  
✅ **Git for Windows**  
⚪ **.NET 9 SDK** (optional)

---

## Troubleshooting

### Execution Policy Error
```powershell
Set-ExecutionPolicy -ExecutionPolicy RemoteSigned -Scope CurrentUser
```

### Any Other Issues
See **[AUTOMATED_BUILD.md](AUTOMATED_BUILD.md)** for detailed troubleshooting

---

## After Build Completes

1. Visual Studio opens automatically
2. Right-click `FreshVoxelEngine` project → "Set as Startup Project"
3. Press **F5** to run

---

## Documentation

- **Detailed Guide:** [AUTOMATED_BUILD.md](AUTOMATED_BUILD.md)
- **Manual Build:** [BUILD.md](BUILD.md)
- **Getting Started:** [GETTING_STARTED.md](GETTING_STARTED.md)
- **All Docs:** [DOCUMENTATION_INDEX.md](DOCUMENTATION_INDEX.md)
