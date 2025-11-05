# Fresh Voxel Engine - Build Tools

This directory contains automated build and installation tools for the Fresh Voxel Engine.

## 🚀 .NET 9 First Approach

Fresh Voxel Engine uses **.NET 9 as the primary development method**. All installation scripts now:

1. **Check for .NET 9 SDK first** - Required for the recommended development approach
2. **Build the managed wrapper** - Creates C# bindings automatically
3. **Set up native backend** - Configures the C++ engine for optimal performance
4. **Provide dual development paths** - C# for game logic, C++ for engine internals

### Prerequisites

**Required (Primary Development):**
- **.NET 9 SDK** - https://dotnet.microsoft.com/download/dotnet/9.0
  - Verify: `dotnet --version` (should show 9.0.x)

**Required (Native Backend):**
- **CMake 3.20+** - https://cmake.org/download/
- **Visual Studio 2022** with "Desktop development with C++" workload
  - Download from [visualstudio.microsoft.com](https://visualstudio.microsoft.com/downloads/)
- **Git** - For vcpkg package management

**Optional:**
- **Windows 10 or later**
- **Internet connection** - For downloading dependencies

## Installation Scripts

### Recommended: PowerShell Installation (`install.ps1`)

A modern, feature-rich installation script with animated banners and real-time progress display.

#### Features

- ✅ **.NET 9 SDK Check** - Verifies .NET 9 is installed (primary requirement)
- ✅ **Managed Wrapper Build** - Builds C# bindings automatically
- ✅ **Animated ASCII Art Banner** - Eye-catching Fresh Voxel Engine banner on startup
- ✅ **Real-time Progress Display** - See live output from all operations
- ✅ **Colored Output** - Clear, color-coded messages for easy reading
- ✅ **Section Banners** - Visual separation between installation steps
- ✅ **Better Error Handling** - Try-catch blocks with detailed error messages
- ✅ **Prerequisites Check** - Verifies CMake and Visual Studio 2022 installation
- ✅ **Package Manager Setup** - Optionally installs vcpkg for dependency management
- ✅ **Dependency Installation** - Automatically installs GLFW, GLM, and ImGui
- ✅ **Project Generation** - Creates Visual Studio 2022 solution files
- ✅ **Automated Build** - Compiles the engine in Release mode
- ✅ **Comprehensive Logging** - Captures all build output to timestamped log files
- ✅ **Shortcut Creation** - Creates convenient shortcuts for running the engine

#### Why PowerShell is Recommended

- **No Silent Operations**: All long-running commands (vcpkg clone, CMake, build) show real-time output
- **No Freezing**: Users can see progress during dependency installation (10-15 minutes)
- **Better UX**: Animated banners, emojis, and colored output make the process engaging
- **Clearer Errors**: Detailed error messages with context when something fails
- **Modern**: Uses PowerShell's advanced features for better reliability
- **.NET Integration**: Better support for checking and building .NET components

#### How to Use

**Option 1: Double-click from File Explorer**
1. Right-click `install.ps1`
2. Select "Run with PowerShell"
3. Follow the on-screen prompts

**Option 2: From PowerShell console**
```powershell
cd path\to\fresh\tools\build_tools
.\install.ps1
```

**Option 3: From Command Prompt**
```batch
powershell -ExecutionPolicy Bypass -File tools\build_tools\install.ps1
```

**Option 4: Using the Launcher**
```batch
# From repository root
install.bat
# Then choose option 1 (PowerShell)
```

### Classic: Batch File Installation (`install.bat`)

A traditional batch file installation script that works on all Windows versions, now with .NET 9 first approach.

#### Features

- ✅ **.NET 9 SDK Check** - Verifies .NET 9 first (primary development method)
- ✅ **Managed Wrapper Build** - Builds C# bindings before native engine
- ✅ **Prerequisites Check** - Verifies CMake and Visual Studio 2022 installation
- ✅ **Package Manager Setup** - Optionally installs vcpkg for dependency management
- ✅ **Dependency Installation** - Automatically installs GLFW, GLM, and ImGui
- ✅ **Project Generation** - Creates Visual Studio 2022 solution files
- ✅ **Automated Build** - Compiles the engine in Release mode
- ✅ **Comprehensive Logging** - Captures all build output to timestamped log files
- ✅ **Shortcut Creation** - Creates convenient shortcuts for running the engine
- ✅ **Interactive** - Provides clear prompts and feedback throughout the process
- ✅ **Real-time Output** - Shows progress from long-running operations (improved in v2.0)

#### Recent Improvements (v3.0)

- **.NET First**: Now checks for .NET 9 SDK before other prerequisites
- **Managed Priority**: Builds C# wrapper before native engine
- **Better Guidance**: Clear messages about .NET as the primary development method
- **Flexible Installation**: Option to continue without .NET (not recommended)
- **Visible Progress**: CMake and build commands display output in real-time
- **No More Freezing**: Users can see what's happening during dependency installation
- **Better Feedback**: Added warning messages before long operations
- **Improved Error Visibility**: Errors are displayed immediately, not just in log files

#### How to Use

1. **Install .NET 9 SDK first** (Required)
   - Download from: https://dotnet.microsoft.com/download/dotnet/9.0
   - Verify: `dotnet --version`

2. **Double-click** `install.bat` to start the installation
3. Follow the on-screen prompts
4. Wait for the installation to complete
5. Use the generated shortcuts to run the engine

#### Requirements

Before running the installation script, ensure you have:

**Primary Requirements:**
- **.NET 9 SDK** - https://dotnet.microsoft.com/download/dotnet/9.0
  - **This is the primary development method**
  - Verify: `dotnet --version` (should show 9.0.x)

**Secondary Requirements:**
- **Windows 10 or later**
- **Internet connection** (for downloading dependencies)
- **Git** (for cloning vcpkg, if needed)
- **CMake 3.20+** - Download from [cmake.org](https://cmake.org/download/)
- **Visual Studio 2022** (Community, Professional, or Enterprise)
  - With "Desktop development with C++" workload
  - With ".NET desktop development" workload (for C# development)
  - Download from [visualstudio.microsoft.com](https://visualstudio.microsoft.com/downloads/)

#### What the Script Does

##### Step 1: Prerequisites Check
- Verifies CMake is installed and accessible
- Checks for Visual Studio 2022 installation
- Reports versions and paths

##### Step 2: vcpkg Setup (Optional)
- Offers to install vcpkg package manager
- Clones vcpkg repository if not present
- Bootstraps vcpkg and integrates with Visual Studio
- Can be skipped if you prefer manual dependency management

##### Step 3: Install Dependencies
- Installs GLFW (window management library)
- Installs GLM (mathematics library)
- Uses vcpkg if available, otherwise expects manual installation

##### Step 4: Generate Project Files
- Creates `build` directory
- Runs CMake to generate Visual Studio 2022 solution
- Configures project with appropriate settings

##### Step 5: Build the Project
- Builds the engine in Release configuration
- May take several minutes depending on system performance
- Outputs to `build/Release/FreshVoxelEngine.exe`

##### Step 6: Create Shortcuts
- Optionally creates convenient batch file shortcuts:
  - `Open_Solution.bat` - Opens the Visual Studio solution
  - `Run_Engine.bat` - Launches the compiled engine

#### Installation Time

Expected installation time varies by system:
- **Fast System (SSD, 8+ cores)**: 10-15 minutes
- **Average System**: 15-25 minutes
- **Slower System**: 25-40 minutes

Most time is spent downloading and compiling dependencies.

#### Troubleshooting

##### CMake Not Found
**Error**: "CMake is not installed or not in PATH"

**Solution**:
1. Download and install CMake from [cmake.org](https://cmake.org/download/)
2. During installation, select "Add CMake to system PATH"
3. Restart the command prompt and try again

##### Visual Studio Not Found
**Error**: "Visual Studio 2022 not found"

**Solution**:
1. Download Visual Studio 2022 from [visualstudio.microsoft.com](https://visualstudio.microsoft.com/downloads/)
2. Install with "Desktop development with C++" workload
3. Restart the installation script

##### Git Not Found
**Error**: Failed to clone vcpkg repository

**Solution**:
1. Install Git from [git-scm.com](https://git-scm.com/downloads)
2. Add Git to system PATH
3. Restart the installation script

##### Build Fails
**Error**: "Build failed"

**Solution**:
1. Check the timestamped log file in the `logs/` directory (path shown in error message)
2. The log file contains the complete build output including all error details
3. Look for specific compilation errors or missing dependencies
4. Ensure all prerequisites are properly installed
5. Try opening `build\FreshVoxelEngine.sln` in Visual Studio
6. Build manually to see detailed error messages in the IDE
7. Check [DEVELOPER_SETUP.md](../../DEVELOPER_SETUP.md) for manual build instructions

**Note**: All build scripts now capture complete stdout and stderr output to log files with timestamps like `install_errors_YYYY-MM-DD_HH-MM-SS.txt` in the `logs/` directory.

##### Network Issues
**Error**: Cannot download vcpkg or dependencies

**Solution**:
1. Check your internet connection
2. Verify firewall/proxy settings
3. Try again after network issues are resolved
4. Consider manual dependency installation (see below)

#### Manual Installation Alternative

If the automated script doesn't work for you, follow the manual installation guide:

1. **Manual Setup**: See [DEVELOPER_SETUP.md](../../DEVELOPER_SETUP.md)
2. **Quick Setup**: See [GETTING_STARTED.md](../../GETTING_STARTED.md)
3. **Visual Studio Specific**: See [VISUAL_STUDIO_SETUP.md](../../VISUAL_STUDIO_SETUP.md)

#### Advanced Options

##### Custom vcpkg Location
If you already have vcpkg installed elsewhere:
1. Edit `install.bat`
2. Change the `VCPKG_ROOT` variable to your vcpkg path
3. Run the script

##### Custom Build Configuration
To build in Debug mode instead of Release:
1. Edit `install.bat`
2. Change `--config Release` to `--config Debug` in the build step
3. Run the script

##### Skip Steps
You can modify the script to skip certain steps:
- Comment out Step 2 to skip vcpkg installation
- Comment out Step 5 to skip building (generate project only)
- Comment out Step 6 to skip shortcut creation

#### Output

After successful installation, you'll have:
- `build/` - Build directory with Visual Studio solution
- `build/FreshVoxelEngine.sln` - Visual Studio solution file
- `build/Release/FreshVoxelEngine.exe` - Compiled engine executable
- `logs/` - Log directory with timestamped installation and build logs
- `vcpkg/` - vcpkg package manager (if installed)
- `Open_Solution.bat` - Shortcut to open VS solution (if created)
- `Run_Engine.bat` - Shortcut to run engine (if created)

#### Logging

All build scripts create timestamped log files in the `logs/` directory:
- `install_errors_YYYY-MM-DD_HH-MM-SS.txt` - Complete output from install.bat
- `build_errors_YYYY-MM-DD_HH-MM-SS.txt` - Complete output from build.bat
- `rebuild_errors_YYYY-MM-DD_HH-MM-SS.txt` - Complete output from rebuild.bat

These logs capture:
- Both stdout and stderr from all cmake commands
- Build progress and status messages
- Complete error details when builds fail
- Timestamps for tracking issues

When a build fails, the error message will display the exact log file path for detailed troubleshooting.

#### Script Maintenance

This script is maintained as part of the Fresh Voxel Engine project. 

If you encounter issues:
1. Check the [Issues](https://github.com/shifty81/fresh/issues) page
2. Create a new issue with the error details
3. Include the output from the installation script

#### Contributing

Improvements to the installation script are welcome! Please submit pull requests with:
- Clear description of the change
- Justification for the improvement
- Testing on different Windows configurations

## Additional Tools

### `build.bat` - Quick Build Script

A fast build script for incremental builds during development.

#### Features
- ✅ Quick builds without regenerating project files
- ✅ Choice of build configuration (Debug, Release, RelWithDebInfo)
- ✅ Complete build logging to timestamped files
- ✅ Option to run the engine after building

#### Usage
1. Double-click `build.bat`
2. Select your desired build configuration (1-3)
3. Wait for the build to complete
4. Optionally run the engine

**Note**: Requires that `install.bat` or Visual Studio project generation has been run at least once.

### `rebuild.bat` - Clean Rebuild Script

Performs a complete clean rebuild of the project.

#### Features
- ✅ Cleans all build artifacts
- ✅ Regenerates Visual Studio project files
- ✅ Builds from scratch in Release mode
- ✅ Complete logging of all steps

#### Usage
1. Double-click `rebuild.bat`
2. Wait for the clean, regenerate, and build steps to complete

**Use when**: 
- Build cache issues occur
- CMake configuration changes
- Complete rebuild is needed

### `clean.bat` - Clean Build Artifacts

Removes all build artifacts and generated files.

#### Features
- ✅ Removes build directory
- ✅ Preserves source code and dependencies

#### Usage
1. Double-click `clean.bat`
2. Build artifacts will be removed

## Logging System

All build tools use a comprehensive logging system:

- **Log Location**: `logs/` directory in repository root
- **File Format**: `<script>_errors_YYYY-MM-DD_HH-MM-SS.txt`
- **Content**: Complete stdout and stderr from all operations
- **Retention**: Manual cleanup (not auto-deleted)

### Log Files
- `install_errors_*.txt` - Full installation process output
- `build_errors_*.txt` - Complete build output
- `rebuild_errors_*.txt` - Clean rebuild process output

### When Errors Occur
All scripts will:
1. Display a user-friendly error message
2. Show the exact log file path
3. Include the exit code in the log file
4. Preserve complete output for debugging

Example error message:
```
ERROR: Build failed
Full build output has been saved to: logs\build_errors_2025-11-03_14-30-45.txt
```

Future tools planned for this directory:
- `package.bat` - Create distribution package
- `update_deps.bat` - Update dependencies

## Support

For help and support:
- 📖 [Documentation](../../docs/)
- 💬 [GitHub Discussions](https://github.com/shifty81/fresh/discussions)
- 🐛 [Report Issues](https://github.com/shifty81/fresh/issues)

---

**Fresh Voxel Engine** - Build tools to simplify your development experience.
