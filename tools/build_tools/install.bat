@echo off
REM ============================================================================
REM Fresh Voxel Engine - Automated Installation Script
REM ============================================================================
REM This script automates the complete setup and installation process
REM for the Fresh Voxel Engine on Windows systems.
REM
REM Features:
REM   - Checks for required prerequisites (CMake, Visual Studio)
REM   - Optionally installs vcpkg package manager
REM   - Installs required dependencies (GLFW, GLM)
REM   - Generates Visual Studio project files
REM   - Builds the engine in Release mode
REM   - Creates shortcuts for easy access
REM
REM Usage: Simply double-click this file to run the installation
REM ============================================================================

setlocal enabledelayedexpansion

REM Set colors for output (requires Windows 10+)
set "GREEN=[92m"
set "RED=[91m"
set "YELLOW=[93m"
set "BLUE=[94m"
set "RESET=[0m"

REM Navigate to repository root (2 levels up from tools/build_tools)
cd /d "%~dp0..\.."
set "REPO_ROOT=%CD%"

REM Setup logging
set "LOG_DIR=%REPO_ROOT%\logs"
if not exist "%LOG_DIR%" mkdir "%LOG_DIR%"
REM Note: Timestamp format may vary with locale settings - this works for US/EN locales
set "TIMESTAMP=%date:~-4,4%-%date:~-10,2%-%date:~-7,2%_%time:~0,2%-%time:~3,2%-%time:~6,2%"
set "TIMESTAMP=%TIMESTAMP: =0%"
set "LOG_FILE=%LOG_DIR%\install_errors_%TIMESTAMP%.txt"

echo ============================================================================ > "%LOG_FILE%"
echo Fresh Voxel Engine - Installation Log >> "%LOG_FILE%"
echo Started: %date% %time% >> "%LOG_FILE%"
echo ============================================================================ >> "%LOG_FILE%"
echo. >> "%LOG_FILE%"

echo.
echo %BLUE%============================================================================%RESET%
echo %BLUE%       Fresh Voxel Engine - Automated Installation%RESET%
echo %BLUE%============================================================================%RESET%
echo.
echo Log file: %LOG_FILE%
echo.
echo This script will:
echo   1. Check for prerequisites (CMake, Visual Studio)
echo   2. Install vcpkg package manager (optional)
echo   3. Install dependencies (GLFW, GLM)
echo   4. Generate Visual Studio project files
echo   5. Build the engine
echo.
echo %YELLOW%Press any key to continue or Ctrl+C to cancel...%RESET%
pause >nul
echo.

REM ============================================================================
REM Step 1: Check Prerequisites
REM ============================================================================
echo %BLUE%[Step 1/6] Checking prerequisites...%RESET%
echo.

REM Check CMake
echo Checking for CMake...
where cmake >nul 2>nul
if %ERRORLEVEL% NEQ 0 (
    echo %RED%ERROR: CMake is not installed or not in PATH%RESET%
    echo ERROR: CMake is not installed or not in PATH >> "%LOG_FILE%"
    echo.
    echo Please install CMake from: https://cmake.org/download/
    echo Make sure to add CMake to system PATH during installation
    echo. >> "%LOG_FILE%"
    echo Please install CMake from: https://cmake.org/download/ >> "%LOG_FILE%"
    echo.
    pause
    exit /b 1
) else (
    for /f "tokens=*" %%i in ('cmake --version ^| findstr /C:"cmake version"') do set CMAKE_VERSION=%%i
    echo %GREEN%✓ CMake found: !CMAKE_VERSION!%RESET%
)

REM Check Visual Studio 2022
echo Checking for Visual Studio 2022...
set VS2022_FOUND=0
set VS2022_PATH=

if exist "C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\IDE\devenv.exe" (
    set VS2022_FOUND=1
    set VS2022_PATH=C:\Program Files\Microsoft Visual Studio\2022\Community
)
if exist "C:\Program Files\Microsoft Visual Studio\2022\Professional\Common7\IDE\devenv.exe" (
    set VS2022_FOUND=1
    set VS2022_PATH=C:\Program Files\Microsoft Visual Studio\2022\Professional
)
if exist "C:\Program Files\Microsoft Visual Studio\2022\Enterprise\Common7\IDE\devenv.exe" (
    set VS2022_FOUND=1
    set VS2022_PATH=C:\Program Files\Microsoft Visual Studio\2022\Enterprise
)

if %VS2022_FOUND%==1 (
    echo %GREEN%✓ Visual Studio 2022 found at: %VS2022_PATH%%RESET%
) else (
    echo %YELLOW%WARNING: Visual Studio 2022 not found in default locations%RESET%
    echo The installation will continue, but you may need VS 2022 to build
    echo.
    echo Download from: https://visualstudio.microsoft.com/downloads/
    echo.
    set /p CONTINUE="Continue anyway? (Y/N): "
    if /i "!CONTINUE!" NEQ "Y" (
        echo Installation cancelled.
        pause
        exit /b 1
    )
)

echo.
echo %GREEN%Prerequisites check completed!%RESET%
echo.
pause

REM ============================================================================
REM Step 2: vcpkg Setup (Optional)
REM ============================================================================
echo %BLUE%[Step 2/6] Setting up vcpkg package manager...%RESET%
echo.

set "VCPKG_ROOT=%REPO_ROOT%\vcpkg"

if exist "%VCPKG_ROOT%\vcpkg.exe" (
    echo %GREEN%✓ vcpkg already installed at: %VCPKG_ROOT%%RESET%
) else (
    echo vcpkg not found. Would you like to install it?
    echo vcpkg is a package manager for C++ that makes dependency management easier.
    echo.
    set /p INSTALL_VCPKG="Install vcpkg? (Y/N): "
    
    if /i "!INSTALL_VCPKG!"=="Y" (
        echo.
        echo Installing vcpkg...
        
        REM Clone vcpkg
        if not exist "%VCPKG_ROOT%" (
            echo Cloning vcpkg repository...
            git clone https://github.com/Microsoft/vcpkg.git "%VCPKG_ROOT%"
            if !ERRORLEVEL! NEQ 0 (
                echo %RED%ERROR: Failed to clone vcpkg repository%RESET%
                echo Make sure git is installed and you have internet connection
                pause
                exit /b 1
            )
        )
        
        REM Bootstrap vcpkg
        echo.
        echo Bootstrapping vcpkg...
        cd "%VCPKG_ROOT%"
        call bootstrap-vcpkg.bat
        if !ERRORLEVEL! NEQ 0 (
            echo %RED%ERROR: Failed to bootstrap vcpkg%RESET%
            pause
            exit /b 1
        )
        
        REM Integrate vcpkg with Visual Studio
        echo.
        echo Integrating vcpkg with Visual Studio...
        vcpkg integrate install
        
        cd "%REPO_ROOT%"
        echo %GREEN%✓ vcpkg installed successfully!%RESET%
    ) else (
        echo Skipping vcpkg installation.
        echo You will need to install dependencies manually.
    )
)

echo.
pause

REM ============================================================================
REM Step 3: Install Dependencies
REM ============================================================================
echo %BLUE%[Step 3/6] Installing dependencies...%RESET%
echo.

if exist "%VCPKG_ROOT%\vcpkg.exe" (
    echo Installing GLFW and GLM via vcpkg...
    echo This may take several minutes...
    echo.
    
    cd "%VCPKG_ROOT%"
    
    REM Check if packages are already installed
    vcpkg list | findstr "glfw3:x64-windows" >nul
    if !ERRORLEVEL! EQU 0 (
        echo %GREEN%✓ GLFW already installed%RESET%
    ) else (
        echo Installing GLFW...
        vcpkg install glfw3:x64-windows
        if !ERRORLEVEL! NEQ 0 (
            echo %RED%ERROR: Failed to install GLFW%RESET%
            cd "%REPO_ROOT%"
            pause
            exit /b 1
        )
        echo %GREEN%✓ GLFW installed successfully%RESET%
    )
    
    vcpkg list | findstr "glm:x64-windows" >nul
    if !ERRORLEVEL! EQU 0 (
        echo %GREEN%✓ GLM already installed%RESET%
    ) else (
        echo Installing GLM...
        vcpkg install glm:x64-windows
        if !ERRORLEVEL! NEQ 0 (
            echo %RED%ERROR: Failed to install GLM%RESET%
            cd "%REPO_ROOT%"
            pause
            exit /b 1
        )
        echo %GREEN%✓ GLM installed successfully%RESET%
    )
    
    cd "%REPO_ROOT%"
    echo.
    echo %GREEN%All dependencies installed successfully!%RESET%
) else (
    echo %YELLOW%vcpkg not available. Skipping automatic dependency installation.%RESET%
    echo Please ensure GLFW and GLM are installed manually.
    echo.
    echo See %REPO_ROOT%\DEVELOPER_SETUP.md for manual installation instructions.
)

echo.
pause

REM ============================================================================
REM Step 4: Generate Visual Studio Project Files
REM ============================================================================
echo %BLUE%[Step 4/6] Generating Visual Studio project files...%RESET%
echo.

REM Create build directory if it doesn't exist
if not exist "build" mkdir build

cd build

REM Generate project files
echo Running CMake...
if exist "%VCPKG_ROOT%\scripts\buildsystems\vcpkg.cmake" (
    echo Using vcpkg toolchain file...
    cmake -G "Visual Studio 17 2022" -A x64 -DCMAKE_TOOLCHAIN_FILE="%VCPKG_ROOT%\scripts\buildsystems\vcpkg.cmake" ..
) else (
    echo Using system-installed dependencies...
    cmake -G "Visual Studio 17 2022" -A x64 ..
)

if %ERRORLEVEL% NEQ 0 (
    echo.
    echo %RED%ERROR: CMake project generation failed%RESET%
    echo Please check the error messages above
    cd "%REPO_ROOT%"
    pause
    exit /b 1
)

cd "%REPO_ROOT%"

echo.
echo %GREEN%✓ Visual Studio project files generated successfully!%RESET%
echo   Location: %REPO_ROOT%\build\FreshVoxelEngine.sln
echo.
pause

REM ============================================================================
REM Step 5: Build the Project
REM ============================================================================
echo %BLUE%[Step 5/6] Building the project...%RESET%
echo.
echo This may take several minutes depending on your system...
echo.

REM Build in Release mode
echo Building Release configuration...
echo Building Release configuration... >> "%LOG_FILE%"
echo. >> "%LOG_FILE%"
cmake --build build --config Release >> "%LOG_FILE%" 2>&1

if %ERRORLEVEL% NEQ 0 (
    echo.
    echo %RED%ERROR: Build failed%RESET%
    echo ERROR: Build failed with exit code %ERRORLEVEL% >> "%LOG_FILE%"
    echo Please check the error messages above
    echo.
    echo Full build output has been saved to: %LOG_FILE%
    echo.
    echo You can try building manually by opening:
    echo   %REPO_ROOT%\build\FreshVoxelEngine.sln
    pause
    exit /b 1
)

echo.
echo %GREEN%✓ Build completed successfully!%RESET%
echo.
pause

REM ============================================================================
REM Step 6: Create Shortcuts (Optional)
REM ============================================================================
echo %BLUE%[Step 6/6] Creating shortcuts...%RESET%
echo.

set /p CREATE_SHORTCUTS="Would you like to create desktop shortcuts? (Y/N): "

if /i "!CREATE_SHORTCUTS!"=="Y" (
    REM Create a shortcut script
    echo Creating shortcuts...
    
    REM Note: Creating .lnk files from batch is complex
    REM Instead, we'll create simple batch file shortcuts
    
    REM Shortcut to open Visual Studio solution
    echo @echo off > "%REPO_ROOT%\Open_Solution.bat"
    echo start "" "build\FreshVoxelEngine.sln" >> "%REPO_ROOT%\Open_Solution.bat"
    
    REM Shortcut to run the engine
    if exist "build\Release\FreshVoxelEngine.exe" (
        echo @echo off > "%REPO_ROOT%\Run_Engine.bat"
        echo cd /d "%REPO_ROOT%" >> "%REPO_ROOT%\Run_Engine.bat"
        echo start "" "build\Release\FreshVoxelEngine.exe" >> "%REPO_ROOT%\Run_Engine.bat"
        echo %GREEN%✓ Created Run_Engine.bat%RESET%
    )
    
    echo %GREEN%✓ Created Open_Solution.bat%RESET%
)

echo.

REM ============================================================================
REM Installation Complete
REM ============================================================================
echo.
echo %GREEN%============================================================================%RESET%
echo %GREEN%                   Installation Completed Successfully!%RESET%
echo %GREEN%============================================================================%RESET%
echo.
echo Next steps:
echo.
echo   1. Open the solution:
echo      - Double-click: Open_Solution.bat
echo      - Or manually: build\FreshVoxelEngine.sln
echo.
echo   2. Run the engine:
if exist "build\Release\FreshVoxelEngine.exe" (
    echo      - Double-click: Run_Engine.bat
    echo      - Or manually: build\Release\FreshVoxelEngine.exe
) else (
    echo      - Build the project first in Visual Studio
    echo      - Then run: build\Release\FreshVoxelEngine.exe
)
echo.
echo   3. Learn more:
echo      - README.md - Project overview
echo      - GETTING_STARTED.md - Usage guide
echo      - DEVELOPER_SETUP.md - Development setup
echo.
echo %BLUE%Thank you for installing Fresh Voxel Engine!%RESET%
echo.
pause
