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
set "LOG_FILE=%LOG_DIR%\install_log_%TIMESTAMP%.txt"

REM Create log file with header
echo ============================================================================ > "%LOG_FILE%"
echo Fresh Voxel Engine - Installation Log >> "%LOG_FILE%"
echo Started: %date% %time% >> "%LOG_FILE%"
echo ============================================================================ >> "%LOG_FILE%"
echo. >> "%LOG_FILE%"

REM Function to log both to console and file
REM We'll use echo for both and redirect when needed

echo.
echo %BLUE%============================================================================%RESET%
echo %BLUE%       Fresh Voxel Engine - Automated Installation%RESET%
echo %BLUE%============================================================================%RESET%
echo.
echo Log file: %LOG_FILE%
echo.

REM Log to file
echo ============================================================================ >> "%LOG_FILE%"
echo        Fresh Voxel Engine - Automated Installation >> "%LOG_FILE%"
echo ============================================================================ >> "%LOG_FILE%"
echo. >> "%LOG_FILE%"
echo Log file: %LOG_FILE% >> "%LOG_FILE%"
echo. >> "%LOG_FILE%"

echo This script will:
echo   1. Check for prerequisites (CMake, Visual Studio)
echo   2. Install vcpkg package manager (optional)
echo   3. Verify vcpkg manifest mode configuration
echo   4. Generate Visual Studio project files
echo   5. Build the engine (dependencies installed during CMake)
echo.

REM Log to file
echo This script will: >> "%LOG_FILE%"
echo   1. Check for prerequisites (CMake, Visual Studio) >> "%LOG_FILE%"
echo   2. Install vcpkg package manager (optional) >> "%LOG_FILE%"
echo   3. Verify vcpkg manifest mode configuration >> "%LOG_FILE%"
echo   4. Generate Visual Studio project files >> "%LOG_FILE%"
echo   5. Build the engine (dependencies installed during CMake) >> "%LOG_FILE%"
echo. >> "%LOG_FILE%"

echo %YELLOW%Press any key to continue or Ctrl+C to cancel...%RESET%
echo Press any key to continue or Ctrl+C to cancel... >> "%LOG_FILE%"
pause >nul
echo.

REM ============================================================================
REM Step 1: Check Prerequisites
REM ============================================================================
echo %BLUE%[Step 1/6] Checking prerequisites...%RESET%
echo [Step 1/6] Checking prerequisites... >> "%LOG_FILE%"
echo.
echo. >> "%LOG_FILE%"

REM Check CMake
echo Checking for CMake...
echo Checking for CMake... >> "%LOG_FILE%"
where cmake >nul 2>nul
if !ERRORLEVEL! NEQ 0 (
    echo %RED%ERROR: CMake is not installed or not in PATH%RESET%
    echo ERROR: CMake is not installed or not in PATH >> "%LOG_FILE%"
    echo.
    echo Please install CMake from: https://cmake.org/download/
    echo Make sure to add CMake to system PATH during installation
    echo. >> "%LOG_FILE%"
    echo Please install CMake from: https://cmake.org/download/ >> "%LOG_FILE%"
    echo. >> "%LOG_FILE%"
    pause
    exit /b 1
) else (
    for /f "tokens=*" %%i in ('cmake --version ^| findstr /C:"cmake version"') do set CMAKE_VERSION=%%i
    echo %GREEN%✓ CMake found: !CMAKE_VERSION!%RESET%
    echo CMake found: !CMAKE_VERSION! >> "%LOG_FILE%"
)

REM Check Visual Studio 2022
echo Checking for Visual Studio 2022...
echo Checking for Visual Studio 2022... >> "%LOG_FILE%"
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
    echo Visual Studio 2022 found at: %VS2022_PATH% >> "%LOG_FILE%"
) else (
    echo %YELLOW%WARNING: Visual Studio 2022 not found in default locations%RESET%
    echo WARNING: Visual Studio 2022 not found in default locations >> "%LOG_FILE%"
    echo The installation will continue, but you may need VS 2022 to build
    echo.
    echo Download from: https://visualstudio.microsoft.com/downloads/
    echo.
    echo The installation will continue, but you may need VS 2022 to build >> "%LOG_FILE%"
    echo Download from: https://visualstudio.microsoft.com/downloads/ >> "%LOG_FILE%"
    echo. >> "%LOG_FILE%"
    set /p CONTINUE="Continue anyway? (Y/N): "
    echo User response: !CONTINUE! >> "%LOG_FILE%"
    if /i "!CONTINUE!" NEQ "Y" (
        echo Installation cancelled.
        echo Installation cancelled. >> "%LOG_FILE%"
        pause
        exit /b 1
    )
)

echo.
echo %GREEN%Prerequisites check completed!%RESET%
echo.
echo Prerequisites check completed! >> "%LOG_FILE%"
echo. >> "%LOG_FILE%"
pause

REM ============================================================================
REM Step 2: vcpkg Setup (Optional)
REM ============================================================================
echo %BLUE%[Step 2/6] Setting up vcpkg package manager...%RESET%
echo [Step 2/6] Setting up vcpkg package manager... >> "%LOG_FILE%"
echo.
echo. >> "%LOG_FILE%"

set "VCPKG_ROOT=%REPO_ROOT%\vcpkg"

if exist "%VCPKG_ROOT%\vcpkg.exe" (
    echo %GREEN%✓ vcpkg already installed at: %VCPKG_ROOT%%RESET%
    echo vcpkg already installed at: %VCPKG_ROOT% >> "%LOG_FILE%"
) else (
    echo vcpkg not found. Would you like to install it?
    echo vcpkg is a package manager for C++ that makes dependency management easier.
    echo.
    echo vcpkg not found. >> "%LOG_FILE%"
    echo Would you like to install it? >> "%LOG_FILE%"
    echo vcpkg is a package manager for C++ that makes dependency management easier. >> "%LOG_FILE%"
    echo. >> "%LOG_FILE%"
    set /p INSTALL_VCPKG="Install vcpkg? (Y/N): "
    echo User response: !INSTALL_VCPKG! >> "%LOG_FILE%"
    
    if /i "!INSTALL_VCPKG!"=="Y" (
        echo.
        echo Installing vcpkg...
        echo Installing vcpkg... >> "%LOG_FILE%"
        
        REM Clone vcpkg
        if not exist "%VCPKG_ROOT%" (
            echo Cloning vcpkg repository...
            echo Cloning vcpkg repository... >> "%LOG_FILE%"
            echo %YELLOW%This will download vcpkg. Progress will be shown below.%RESET%
            echo.
            git clone https://github.com/Microsoft/vcpkg.git "%VCPKG_ROOT%"
            if !ERRORLEVEL! NEQ 0 (
                echo.
                echo %RED%ERROR: Failed to clone vcpkg repository%RESET%
                echo ERROR: Failed to clone vcpkg repository >> "%LOG_FILE%"
                echo Make sure git is installed and you have internet connection
                echo Make sure git is installed and you have internet connection >> "%LOG_FILE%"
                pause
                exit /b 1
            )
        )
        
        REM Bootstrap vcpkg
        echo.
        echo Bootstrapping vcpkg...
        echo Bootstrapping vcpkg... >> "%LOG_FILE%"
        echo %YELLOW%Downloading and bootstrapping vcpkg. Progress will be shown below.%RESET%
        echo.
        cd /d "%VCPKG_ROOT%"
        call bootstrap-vcpkg.bat
        if !ERRORLEVEL! NEQ 0 (
            echo.
            echo %RED%ERROR: Failed to bootstrap vcpkg%RESET%
            echo ERROR: Failed to bootstrap vcpkg >> "%LOG_FILE%"
            pause
            exit /b 1
        )
        
        REM Verify vcpkg.exe was created
        if not exist "%VCPKG_ROOT%\vcpkg.exe" (
            echo.
            echo %RED%ERROR: vcpkg.exe not found after bootstrapping%RESET%
            echo ERROR: vcpkg.exe not found after bootstrapping >> "%LOG_FILE%"
            echo Expected location: %VCPKG_ROOT%\vcpkg.exe >> "%LOG_FILE%"
            echo This could be due to antivirus, permissions, or bootstrap failure.
            echo This could be due to antivirus, permissions, or bootstrap failure. >> "%LOG_FILE%"
            cd /d "%REPO_ROOT%"
            pause
            exit /b 1
        )
        echo %GREEN%✓ vcpkg.exe created successfully%RESET%
        echo vcpkg.exe created successfully at: %VCPKG_ROOT%\vcpkg.exe >> "%LOG_FILE%"
        
        REM Update vcpkg repository to get latest baseline commits
        echo.
        echo Updating vcpkg repository...
        echo Updating vcpkg repository... >> "%LOG_FILE%"
        git pull origin master
        if !ERRORLEVEL! NEQ 0 (
            echo.
            echo %YELLOW%WARNING: Failed to update vcpkg repository%RESET%
            echo WARNING: Failed to update vcpkg repository >> "%LOG_FILE%"
            echo This may cause issues if the baseline commit is not available.
            echo This may cause issues if the baseline commit is not available. >> "%LOG_FILE%"
        ) else (
            echo %GREEN%✓ vcpkg repository updated%RESET%
            echo vcpkg repository updated >> "%LOG_FILE%"
        )
        
        REM Integrate vcpkg with Visual Studio
        echo.
        echo Integrating vcpkg with Visual Studio...
        echo Integrating vcpkg with Visual Studio... >> "%LOG_FILE%"
        echo.
        "%VCPKG_ROOT%\vcpkg.exe" integrate install
        if !ERRORLEVEL! NEQ 0 (
            echo.
            echo %YELLOW%WARNING: vcpkg integrate install returned error code !ERRORLEVEL!%RESET%
            echo WARNING: vcpkg integrate install returned error code !ERRORLEVEL! >> "%LOG_FILE%"
            echo This is not critical - you can integrate manually later if needed.
            echo This is not critical - you can integrate manually later if needed. >> "%LOG_FILE%"
        ) else (
            echo %GREEN%✓ vcpkg integrated with Visual Studio%RESET%
            echo vcpkg integrated with Visual Studio >> "%LOG_FILE%"
        )
        
        cd /d "%REPO_ROOT%"
        if !ERRORLEVEL! NEQ 0 (
            echo %RED%ERROR: Failed to return to repository root%RESET%
            echo ERROR: Failed to return to repository root >> "%LOG_FILE%"
            echo Expected: %REPO_ROOT% >> "%LOG_FILE%"
            echo Current: %CD% >> "%LOG_FILE%"
            pause
            exit /b 1
        )
        echo.
        echo %GREEN%✓ vcpkg installed successfully!%RESET%
        echo vcpkg installed successfully! >> "%LOG_FILE%"
    ) else (
        echo Skipping vcpkg installation.
        echo You will need to install dependencies manually.
        echo Skipping vcpkg installation. >> "%LOG_FILE%"
        echo You will need to install dependencies manually. >> "%LOG_FILE%"
    )
)

echo.
echo. >> "%LOG_FILE%"
pause

REM ============================================================================
REM Step 3: Install Dependencies (Manifest Mode)
REM ============================================================================
echo %BLUE%[Step 3/6] Configuring dependencies...%RESET%
echo [Step 3/6] Configuring dependencies... >> "%LOG_FILE%"
echo.
echo. >> "%LOG_FILE%"

REM Check if vcpkg executable exists
echo Checking for vcpkg executable...
echo Checking for vcpkg executable... >> "%LOG_FILE%"
echo Checking path: %VCPKG_ROOT%\vcpkg.exe >> "%LOG_FILE%"
echo.
echo. >> "%LOG_FILE%"

REM Use a more robust existence check
if not defined VCPKG_ROOT (
    echo %RED%ERROR: VCPKG_ROOT variable is not set%RESET%
    echo ERROR: VCPKG_ROOT variable is not set >> "%LOG_FILE%"
    pause
    exit /b 1
)

if exist "%VCPKG_ROOT%\vcpkg.exe" (
    echo %GREEN%✓ vcpkg found at: %VCPKG_ROOT%\vcpkg.exe%RESET%
    echo vcpkg found at: %VCPKG_ROOT%\vcpkg.exe >> "%LOG_FILE%"
    echo.
    echo This project uses vcpkg manifest mode (vcpkg.json).
    echo Dependencies will be installed automatically during CMake configuration.
    echo.
    echo This project uses vcpkg manifest mode (vcpkg.json). >> "%LOG_FILE%"
    echo Dependencies will be installed automatically during CMake configuration. >> "%LOG_FILE%"
    echo. >> "%LOG_FILE%"
    
    echo Dependencies defined in vcpkg.json:
    echo   - GLFW3 (window management)
    echo   - GLM (math library)
    echo   - ImGui (editor UI)
    echo.
    echo Dependencies defined in vcpkg.json: >> "%LOG_FILE%"
    echo   - GLFW3 (window management) >> "%LOG_FILE%"
    echo   - GLM (math library) >> "%LOG_FILE%"
    echo   - ImGui (editor UI) >> "%LOG_FILE%"
    echo. >> "%LOG_FILE%"
    
    echo %GREEN%✓ vcpkg is configured for manifest mode%RESET%
    echo   Dependencies will be installed in the next step during CMake configuration.
    echo.
    echo vcpkg is configured for manifest mode >> "%LOG_FILE%"
    echo Dependencies will be installed in the next step during CMake configuration. >> "%LOG_FILE%"
    echo. >> "%LOG_FILE%"
) else (
    echo %YELLOW%WARNING: vcpkg not available at: %VCPKG_ROOT%\vcpkg.exe%RESET%
    echo %YELLOW%Dependencies must be installed manually.%RESET%
    echo.
    echo WARNING: vcpkg not available at: %VCPKG_ROOT%\vcpkg.exe >> "%LOG_FILE%"
    echo Dependencies must be installed manually. >> "%LOG_FILE%"
    echo. >> "%LOG_FILE%"
    
    echo The following libraries are required:
    echo   - GLFW 3.3+
    echo   - GLM
    echo   - ImGui (optional, for editor UI)
    echo.
    echo The following libraries are required: >> "%LOG_FILE%"
    echo   - GLFW 3.3+ >> "%LOG_FILE%"
    echo   - GLM >> "%LOG_FILE%"
    echo   - ImGui (optional, for editor UI) >> "%LOG_FILE%"
    echo. >> "%LOG_FILE%"
    
    echo See %REPO_ROOT%\DEVELOPER_SETUP.md for manual installation instructions.
    echo.
    echo See DEVELOPER_SETUP.md for manual installation instructions. >> "%LOG_FILE%"
    echo. >> "%LOG_FILE%"
)

echo vcpkg check completed >> "%LOG_FILE%"
echo %YELLOW%Press any key to continue to CMake configuration...%RESET%
echo User prompted to continue... >> "%LOG_FILE%"
pause >nul
echo Continuing to Step 4... >> "%LOG_FILE%"
echo.
echo. >> "%LOG_FILE%"

REM ============================================================================
REM Step 4: Generate Visual Studio Project Files
REM ============================================================================
echo.
echo. >> "%LOG_FILE%"
echo %BLUE%[Step 4/6] Generating Visual Studio project files...%RESET%
echo [Step 4/6] Generating Visual Studio project files... >> "%LOG_FILE%"
echo.
echo. >> "%LOG_FILE%"

echo %YELLOW%NOTE: This step will automatically install dependencies from vcpkg.json%RESET%
echo %YELLOW%      if vcpkg is available. This may take several minutes on first run.%RESET%
echo.
echo NOTE: This step will automatically install dependencies from vcpkg.json >> "%LOG_FILE%"
echo       if vcpkg is available. This may take several minutes on first run. >> "%LOG_FILE%"
echo. >> "%LOG_FILE%"

REM Create build directory if it doesn't exist
if not exist "build" mkdir build

cd build

REM Generate project files
echo Running CMake...
echo Running CMake... >> "%LOG_FILE%"
if exist "%VCPKG_ROOT%\scripts\buildsystems\vcpkg.cmake" (
    echo Using vcpkg toolchain file (manifest mode)...
    echo Dependencies will be installed automatically from vcpkg.json...
    echo.
    echo Using vcpkg toolchain file (manifest mode)... >> "%LOG_FILE%"
    echo Dependencies will be installed automatically from vcpkg.json... >> "%LOG_FILE%"
    echo. >> "%LOG_FILE%"
    echo %YELLOW%NOTE: vcpkg will now install dependencies. This may take 10-15 minutes on first run.%RESET%
    echo %YELLOW%      You will see download and build progress below. Please be patient.%RESET%
    echo.
    cmake -G "Visual Studio 17 2022" -A x64 -DCMAKE_TOOLCHAIN_FILE="%VCPKG_ROOT%\scripts\buildsystems\vcpkg.cmake" ..
) else (
    echo Using system-installed dependencies...
    echo Using system-installed dependencies... >> "%LOG_FILE%"
    cmake -G "Visual Studio 17 2022" -A x64 ..
)

if !ERRORLEVEL! NEQ 0 (
    echo.
    echo %RED%ERROR: CMake project generation failed%RESET%
    echo ERROR: CMake project generation failed with exit code !ERRORLEVEL! >> "%LOG_FILE%"
    echo Please check the error messages above
    echo.
    echo Full CMake output has been saved to: %LOG_FILE%
    echo.
    cd /d "%REPO_ROOT%"
    pause
    exit /b 1
)

cd /d "%REPO_ROOT%"

echo.
echo %GREEN%✓ Visual Studio project files generated successfully!%RESET%
echo   Location: %REPO_ROOT%\build\FreshVoxelEngine.sln
echo.
echo Visual Studio project files generated successfully! >> "%LOG_FILE%"
echo Location: %REPO_ROOT%\build\FreshVoxelEngine.sln >> "%LOG_FILE%"
echo. >> "%LOG_FILE%"
pause

REM ============================================================================
REM Step 5: Build the Project
REM ============================================================================
echo %BLUE%[Step 5/6] Building the project...%RESET%
echo [Step 5/6] Building the project... >> "%LOG_FILE%"
echo.
echo This may take several minutes depending on your system...
echo.
echo This may take several minutes depending on your system... >> "%LOG_FILE%"
echo. >> "%LOG_FILE%"

REM Build in Release mode
echo Building Release configuration...
echo Building Release configuration... >> "%LOG_FILE%"
echo. >> "%LOG_FILE%"
echo %YELLOW%Building... This may take several minutes. Progress will be shown below.%RESET%
echo.
cmake --build build --config Release

if !ERRORLEVEL! NEQ 0 (
    echo.
    echo %RED%ERROR: Build failed%RESET%
    echo ERROR: Build failed with exit code !ERRORLEVEL! >> "%LOG_FILE%"
    echo Please check the error messages above
    echo.
    echo Full build output has been saved to: %LOG_FILE%
    echo.
    echo You can try building manually by opening:
    echo   %REPO_ROOT%\build\FreshVoxelEngine.sln
    echo. >> "%LOG_FILE%"
    echo You can try building manually by opening: >> "%LOG_FILE%"
    echo   %REPO_ROOT%\build\FreshVoxelEngine.sln >> "%LOG_FILE%"
    pause
    exit /b 1
)

echo.
echo %GREEN%✓ Build completed successfully!%RESET%
echo.
echo Build completed successfully! >> "%LOG_FILE%"
echo. >> "%LOG_FILE%"
pause

REM ============================================================================
REM Step 6: Create Shortcuts (Optional)
REM ============================================================================
echo %BLUE%[Step 6/6] Creating shortcuts...%RESET%
echo [Step 6/6] Creating shortcuts... >> "%LOG_FILE%"
echo.
echo. >> "%LOG_FILE%"

set /p CREATE_SHORTCUTS="Would you like to create desktop shortcuts? (Y/N): "
echo User response: !CREATE_SHORTCUTS! >> "%LOG_FILE%"

if /i "!CREATE_SHORTCUTS!"=="Y" (
    REM Create a shortcut script
    echo Creating shortcuts...
    echo Creating shortcuts... >> "%LOG_FILE%"
    
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
        echo Created Run_Engine.bat >> "%LOG_FILE%"
    )
    
    echo %GREEN%✓ Created Open_Solution.bat%RESET%
    echo Created Open_Solution.bat >> "%LOG_FILE%"
)

echo.
echo. >> "%LOG_FILE%"

REM ============================================================================
REM Installation Complete
REM ============================================================================
echo.
echo %GREEN%============================================================================%RESET%
echo %GREEN%                   Installation Completed Successfully!%RESET%
echo %GREEN%============================================================================%RESET%
echo.
echo ============================================================================ >> "%LOG_FILE%"
echo                   Installation Completed Successfully! >> "%LOG_FILE%"
echo ============================================================================ >> "%LOG_FILE%"
echo. >> "%LOG_FILE%"
echo Installation finished: %date% %time% >> "%LOG_FILE%"
echo. >> "%LOG_FILE%"
echo %BLUE%Installation Summary:%RESET%
echo   - Visual Studio solution: build\FreshVoxelEngine.sln
if exist "build\Release\FreshVoxelEngine.exe" (
    echo   - Engine executable: build\Release\FreshVoxelEngine.exe
)
echo   - Installation log: %LOG_FILE%
echo.
echo Installation Summary: >> "%LOG_FILE%"
echo   - Visual Studio solution: build\FreshVoxelEngine.sln >> "%LOG_FILE%"
if exist "build\Release\FreshVoxelEngine.exe" (
    echo   - Engine executable: build\Release\FreshVoxelEngine.exe >> "%LOG_FILE%"
)
echo   - Installation log: %LOG_FILE% >> "%LOG_FILE%"
echo. >> "%LOG_FILE%"
echo %YELLOW%The root install.bat will show next steps.%RESET%
echo %YELLOW%Press any key to return to the main installer...%RESET%
echo.
pause
