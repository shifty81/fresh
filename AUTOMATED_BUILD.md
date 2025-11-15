# Automated Build Script Guide

This guide explains how to use the automated build scripts that set up and build Fresh Voxel Engine with a single command.

## Overview

Fresh Voxel Engine provides automated build scripts for both Windows and Linux/macOS platforms:

- **Windows:** `setup-and-build.ps1` (PowerShell)
- **Linux/macOS:** `setup-and-build.sh` (Bash)

These scripts automate the entire build process:

1. ✅ Check prerequisites (compiler, CMake, Git)
2. ✅ Install/verify dependencies
3. ✅ Generate build files (Visual Studio solution or Makefiles)
4. ✅ Build the project
5. ✅ (Optional) Run tests

---

## Windows: PowerShell Script

### Quick Start

#### Option 1: Full Automated Build (Recommended)

Open PowerShell in the project root directory and run:

```powershell
.\setup-and-build.ps1
```

This will:
- Check all prerequisites
- Set up vcpkg if needed
- Generate the Visual Studio solution
- Build the Release configuration
- Open Visual Studio 2022 with the solution

**Time:** 10-20 minutes on first run (includes downloading dependencies)

#### Option 2: Setup Only (No Build)

If you want to just set up and generate the solution without building:

```powershell
.\setup-and-build.ps1 -SkipBuild
```

You can then build manually in Visual Studio.

#### Option 3: Build Debug Configuration

To build the Debug configuration instead of Release:

```powershell
.\setup-and-build.ps1 -BuildConfig Debug
```

### Prerequisites

The script will check for these prerequisites and guide you if anything is missing:

#### Required
- **Windows 10/11** (x64)
- **Visual Studio 2022** (Community, Professional, or Enterprise)
  - With "Desktop development with C++" workload
  - Download: https://visualstudio.microsoft.com/downloads/
- **CMake 3.20+**
  - Download: https://cmake.org/download/
  - Must be in system PATH
- **Git for Windows**
  - Download: https://git-scm.com/download/win

#### Optional
- **.NET 9 SDK** (for C# bindings)
  - Download: https://dotnet.microsoft.com/download/dotnet/9.0

### Script Parameters

The script supports several optional parameters:

| Parameter | Type | Default | Description |
|-----------|------|---------|-------------|
| `-SkipBuild` | Switch | false | Skip the build step, only generate solution |
| `-BuildConfig` | String | "Release" | Build configuration: "Debug" or "Release" |
| `-SkipVcpkgBootstrap` | Switch | false | Skip vcpkg bootstrap if already exists |
| `-OpenVS` | Boolean | true | Open Visual Studio after completion |

### Examples

**Build Release configuration (default):**
```powershell
.\setup-and-build.ps1
```

**Build Debug configuration:**
```powershell
.\setup-and-build.ps1 -BuildConfig Debug
```

**Setup and generate solution without building:**
```powershell
.\setup-and-build.ps1 -SkipBuild
```

**Don't open Visual Studio automatically:**
```powershell
.\setup-and-build.ps1 -OpenVS $false
```

**Build Debug without opening Visual Studio:**
```powershell
.\setup-and-build.ps1 -BuildConfig Debug -OpenVS $false
```

---

## Linux/macOS: Bash Script

### Quick Start

#### Option 1: Full Automated Build (Recommended)

Open a terminal in the project root directory and run:

```bash
./setup-and-build.sh
```

This will:
- Check all prerequisites
- Install missing dependencies (via apt-get or brew)
- Generate Makefiles with CMake
- Build the Release configuration
- Display instructions for running the engine

**Time:** 5-10 minutes on first run (includes installing dependencies)

#### Option 2: Setup Only (No Build)

If you want to just configure without building:

```bash
./setup-and-build.sh --skip-build
```

You can then build manually with `make`.

#### Option 3: Build Debug Configuration

To build the Debug configuration instead of Release:

```bash
./setup-and-build.sh --config Debug
```

#### Option 4: Build and Run Tests

To build and automatically run the test suite:

```bash
./setup-and-build.sh --run-tests
```

### Prerequisites

The script will check for these prerequisites and guide you if anything is missing:

#### Required (Linux)
- **Ubuntu 20.04+** or **Debian 11+** (or compatible distribution)
- **GCC 11+** or **Clang 12+**
- **CMake 3.20+**
- **Git**
- **Development libraries:** libglfw3-dev, libglm-dev, libglew-dev, libopenal-dev, libgtest-dev

The script will attempt to install missing dependencies automatically using your system's package manager.

#### Required (macOS)
- **macOS 12.0+** (Monterey or later)
- **Xcode Command Line Tools** or **Clang 12+**
- **Homebrew** package manager
- **CMake 3.20+**
- **Git**
- **Development libraries:** glfw, glm, openal-soft, googletest

The script will install missing dependencies via Homebrew.

### Script Options

The script supports several optional parameters:

| Option | Description |
|--------|-------------|
| `--skip-build` | Skip the build step, only configure with CMake |
| `--config <type>` | Build configuration: "Debug" or "Release" (default: Release) |
| `--skip-deps` | Skip dependency installation check |
| `--run-tests` | Run test suite after successful build |
| `--help` | Show help message with all options |

### Examples

**Build Release configuration (default):**
```bash
./setup-and-build.sh
```

**Build Debug configuration:**
```bash
./setup-and-build.sh --config Debug
```

**Build and run tests:**
```bash
./setup-and-build.sh --run-tests
```

**Configure only (no build):**
```bash
./setup-and-build.sh --skip-build
```

**Build without checking dependencies:**
```bash
./setup-and-build.sh --skip-deps
```

**Debug build with tests:**
```bash
./setup-and-build.sh --config Debug --run-tests
```

### After the Script Completes

#### Running the Engine

After a successful build:

```bash
# From project root
cd build && ./FreshVoxelEngine

# Or directly
./build/FreshVoxelEngine
```

#### Running Tests

```bash
# From project root
cd build && ./FreshVoxelEngineTests

# Or directly
./build/FreshVoxelEngineTests
```

#### Manual Building After Configuration

If you used `--skip-build`, you can build manually:

```bash
cd build
cmake --build . -j$(nproc)  # Linux
# or
cmake --build . -j$(sysctl -n hw.ncpu)  # macOS
```

---

## What Happens During Execution (Windows)

### Phase 1: Prerequisites Check
The script verifies:
- Git is installed and in PATH
- CMake 3.20+ is installed and in PATH
- Visual Studio 2022 is installed
- Desktop development with C++ workload is present (warning if not)
- .NET 9 SDK (optional, warning if not present)

### Phase 2: vcpkg Setup
The script:
- Checks for vcpkg in project directory (`./vcpkg/`)
- Checks for vcpkg in parent directory (`../vcpkg/`)
- If not found, clones vcpkg into project directory
- Bootstraps vcpkg if needed
- Verifies vcpkg.exe and toolchain file exist

### Phase 3: Solution Generation
The script:
- Creates `build/` directory if it doesn't exist
- Runs CMake to generate Visual Studio 2022 solution
- Uses vcpkg toolchain for automatic dependency management
- Dependencies (GLFW, GLM, ImGui, OpenAL, LuaJIT, GLEW) are automatically downloaded and built
- Creates `build/FreshVoxelEngine.sln`

**Note:** First run takes 5-15 minutes as vcpkg downloads and builds dependencies.

### Phase 4: Build
The script (unless `-SkipBuild` is used):
- Builds the project using CMake
- Uses Release or Debug configuration (based on `-BuildConfig`)
- Uses multiple processors for faster compilation (`/m` flag)
- Creates executable in `build/Release/` or `build/Debug/`

**Note:** Build takes 2-5 minutes depending on your system.

### Phase 5: Open Visual Studio
The script (unless `-OpenVS $false` is used):
- Opens the generated solution in Visual Studio 2022
- You take over from here!

## What Happens During Execution (Linux/macOS)

### Phase 1: Prerequisites Check
The script verifies:
- Git is installed and in PATH
- CMake 3.20+ is installed and in PATH
- C++ compiler (GCC/Clang) is installed
- Detects operating system and distribution

### Phase 2: Dependency Installation
The script (unless `--skip-deps` is used):
- Checks for required development libraries
- On Linux: Uses apt-get (Ubuntu/Debian) or dnf (Fedora/RHEL)
- On macOS: Uses Homebrew
- Installs missing dependencies automatically (requires sudo on Linux)

**Note:** You may be prompted for your password to install system packages.

### Phase 3: CMake Configuration
The script:
- Creates `build/` directory if it doesn't exist
- Runs CMake to generate Makefiles
- Uses OpenGL rendering (native on Linux/macOS)
- Configures for Release or Debug build type
- Creates build configuration files

### Phase 4: Build
The script (unless `--skip-build` is used):
- Builds the project using CMake
- Uses all available CPU cores for parallel compilation
- Creates executable in `build/` directory
- Shows progress and any warnings/errors

**Note:** Build takes 2-5 minutes depending on your system.

### Phase 5: Test Execution (Optional)
The script (if `--run-tests` is used):
- Runs the test executable
- Displays test results
- Reports pass/fail status

---

## After the Script Completes (Windows)

### Next Steps in Visual Studio

1. **Set Startup Project:**
   - In Solution Explorer, right-click `FreshVoxelEngine` project
   - Select "Set as Startup Project"
   - The project name will appear **bold**

2. **Run the Engine:**
   - Press **F5** to run with debugging
   - Or press **Ctrl+F5** to run without debugging

3. **Make Changes:**
   - Edit source code
   - Build with **F7**
   - The engine is now ready for development!

### Running the Engine from Command Line

After a successful build:

```batch
# Run Release build
build\Release\FreshVoxelEngine.exe

# Run Debug build
build\Debug\FreshVoxelEngine.exe
```

## Troubleshooting

### Script Execution Policy Error

If you get an error about execution policy:

```
.\setup-and-build.ps1 : File cannot be loaded because running scripts is disabled on this system.
```

**Solution:** Run PowerShell as Administrator and execute:
```powershell
Set-ExecutionPolicy -ExecutionPolicy RemoteSigned -Scope CurrentUser
```

Then try running the script again.

### Prerequisites Not Found

The script will check for missing prerequisites and provide download links. Install the missing software and run the script again.

### vcpkg Clone or Bootstrap Fails

**Possible causes:**
- No internet connection
- Git not configured properly
- Firewall blocking git

**Solution:**
1. Verify internet connection
2. Try cloning vcpkg manually:
   ```powershell
   git clone https://github.com/microsoft/vcpkg.git
   cd vcpkg
   .\bootstrap-vcpkg.bat
   cd ..
   ```
3. Run the script again with `-SkipVcpkgBootstrap`

### CMake Generation Fails

**Common causes:**
- vcpkg not set up properly
- CMake version too old
- Visual Studio not installed correctly

**Solution:**
1. Check the error message carefully
2. Verify CMake version: `cmake --version` (must be 3.20+)
3. Verify Visual Studio 2022 is installed with C++ workload
4. Try deleting `build/` directory and running again
5. See BUILD.md for manual setup instructions

### Build Fails

**Common causes:**
- Missing dependencies
- Incorrect Visual Studio configuration
- Disk space issues

**Solution:**
1. Check error messages in the output
2. Open the solution in Visual Studio to see detailed errors
3. Try building manually in Visual Studio
4. Check BUILD.md for troubleshooting guidance

### Visual Studio Doesn't Open

If Visual Studio doesn't open automatically:

**Solution:**
- Open manually: `build\FreshVoxelEngine.sln`
- Or use the `-OpenVS $false` parameter and open manually after

## Comparison with Manual Build

### Automated Script
```powershell
# One command
.\setup-and-build.ps1
```
**Pros:**
- Single command
- Automatic prerequisite checking
- Automatic vcpkg setup
- Error handling and helpful messages
- Consistent results

**Time:** 10-20 minutes (first run), 2-5 minutes (subsequent builds)

### Manual Build
```batch
# Multiple steps
git clone https://github.com/microsoft/vcpkg.git
cd vcpkg
bootstrap-vcpkg.bat
cd ..
mkdir build
cd build
cmake -G "Visual Studio 17 2022" -A x64 -DCMAKE_TOOLCHAIN_FILE=..\vcpkg\scripts\buildsystems\vcpkg.cmake ..
cmake --build . --config Release
```
**Pros:**
- More control over each step
- Easier to debug specific steps
- Can modify build options

**Time:** 15-25 minutes (first run), manual attention required

## Advanced Usage

### Using vcpkg from Parent Directory

The script automatically detects vcpkg in both project and parent directories. If you prefer to keep vcpkg in the parent directory to share across projects:

```powershell
# From project root
cd ..
git clone https://github.com/microsoft/vcpkg.git
cd vcpkg
.\bootstrap-vcpkg.bat
cd ..\fresh

# Run script - it will auto-detect parent directory vcpkg
.\setup-and-build.ps1
```

### Rebuilding After Changes

If you've already run the script once:

```powershell
# Rebuild without regenerating solution
.\setup-and-build.ps1
```

The script is smart enough to use existing vcpkg and build directory.

### Clean Build

To start fresh:

```powershell
# Delete build directory
Remove-Item -Recurse -Force build

# Run script again
.\setup-and-build.ps1
```

## Integration with Development Workflow

### First Time Setup
```powershell
# Clone repository
git clone https://github.com/shifty81/fresh.git
cd fresh

# Run automated setup and build
.\setup-and-build.ps1

# Visual Studio opens - set FreshVoxelEngine as startup project
# Press F5 to run
```

### Daily Development
1. Open `build\FreshVoxelEngine.sln` in Visual Studio
2. Make code changes
3. Press F7 to build
4. Press F5 to run

### After Pulling New Changes
```powershell
# If CMakeLists.txt or dependencies changed
.\setup-and-build.ps1

# Otherwise, just rebuild in Visual Studio
```

## Additional Resources

- **Build Instructions:** [BUILD.md](BUILD.md) - Manual build process
- **Visual Studio Setup:** [VISUAL_STUDIO_SETUP.md](VISUAL_STUDIO_SETUP.md) - Detailed VS2022 guide
- **Developer Setup:** [DEVELOPER_SETUP.md](DEVELOPER_SETUP.md) - Development environment
- **Getting Started:** [GETTING_STARTED.md](GETTING_STARTED.md) - First steps with the engine
- **Controls:** [CONTROLS.md](CONTROLS.md) - How to use the engine
- **Documentation Index:** [DOCUMENTATION_INDEX.md](DOCUMENTATION_INDEX.md) - All documentation

## Support

If you encounter issues:

1. Check the error messages - they usually indicate the problem
2. Review the Troubleshooting section above
3. See [BUILD.md](BUILD.md) for detailed build instructions
4. Open an issue: https://github.com/shifty81/fresh/issues

## Summary

The `setup-and-build.ps1` script provides a streamlined, automated way to set up and build Fresh Voxel Engine. It handles all the complex setup steps, checks prerequisites, and gets you to a working build with a single command.

**Quick Command:**
```powershell
.\setup-and-build.ps1
```

**Expected Time:**
- First run: 10-20 minutes (includes dependency download/build)
- Subsequent runs: 2-5 minutes (just build)

**Result:**
- Visual Studio 2022 opens with solution
- Project is built and ready to run
- You're ready to develop!
