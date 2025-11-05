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
echo/ >> "%LOG_FILE%"

REM Function to log both to console and file
REM We'll use echo for both and redirect when needed

echo/
echo %BLUE%============================================================================%RESET%
echo %BLUE%       Fresh Voxel Engine - Automated Installation%RESET%
echo %BLUE%============================================================================%RESET%
echo/
echo Log file: %LOG_FILE%
echo/

REM Log to file
echo ============================================================================ >> "%LOG_FILE%"
echo        Fresh Voxel Engine - Automated Installation >> "%LOG_FILE%"
echo ============================================================================ >> "%LOG_FILE%"
echo/ >> "%LOG_FILE%"
echo Log file: %LOG_FILE% >> "%LOG_FILE%"
echo/ >> "%LOG_FILE%"

echo This script will:
echo   1. Check for prerequisites (CMake, Visual Studio)
echo   2. Install vcpkg package manager (optional)
echo   3. Verify vcpkg manifest mode configuration
echo   4. Generate Visual Studio project files
echo   5. Build the engine (dependencies installed during CMake)
echo/

REM Log to file
echo This script will: >> "%LOG_FILE%"
echo   1. Check for prerequisites (CMake, Visual Studio) >> "%LOG_FILE%"
echo   2. Install vcpkg package manager (optional) >> "%LOG_FILE%"
echo   3. Verify vcpkg manifest mode configuration >> "%LOG_FILE%"
echo   4. Generate Visual Studio project files >> "%LOG_FILE%"
echo   5. Build the engine (dependencies installed during CMake) >> "%LOG_FILE%"
echo/ >> "%LOG_FILE%"

echo %YELLOW%Press any key to continue or Ctrl+C to cancel...%RESET%
echo Press any key to continue or Ctrl+C to cancel... >> "%LOG_FILE%"
pause >nul
echo/

REM ============================================================================
REM Step 1: Check Prerequisites
REM ============================================================================
echo %BLUE%[Step 1/6] Checking prerequisites...%RESET%
echo [Step 1/6] Checking prerequisites... >> "%LOG_FILE%"
echo/
echo/ >> "%LOG_FILE%"

REM Check CMake
echo Checking for CMake...
echo Checking for CMake... >> "%LOG_FILE%"
where cmake >nul 2>nul
if !ERRORLEVEL! NEQ 0 (
    echo %RED%ERROR: CMake is not installed or not in PATH%RESET%
    echo ERROR: CMake is not installed or not in PATH >> "%LOG_FILE%"
    echo/
    echo Please install CMake from: https://cmake.org/download/
    echo Make sure to add CMake to system PATH during installation
    echo/ >> "%LOG_FILE%"
    echo Please install CMake from: https://cmake.org/download/ >> "%LOG_FILE%"
    echo/ >> "%LOG_FILE%"
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
    echo/
    echo Download from: https://visualstudio.microsoft.com/downloads/
    echo/
    echo The installation will continue, but you may need VS 2022 to build >> "%LOG_FILE%"
    echo Download from: https://visualstudio.microsoft.com/downloads/ >> "%LOG_FILE%"
    echo/ >> "%LOG_FILE%"
    set /p CONTINUE="Continue anyway? (Y/N): "
    echo User response: !CONTINUE! >> "%LOG_FILE%"
    if /i "!CONTINUE!" NEQ "Y" (
        echo Installation cancelled.
        echo Installation cancelled. >> "%LOG_FILE%"
        pause
        exit /b 1
    )
)

echo/
echo %GREEN%Prerequisites check completed!%RESET%
echo/
echo Prerequisites check completed! >> "%LOG_FILE%"
echo/ >> "%LOG_FILE%"
echo/
echo %YELLOW%Step 1 completed. Press any key to continue to Step 2 (vcpkg setup)...%RESET%
echo/
pause

REM ============================================================================
REM Step 2: vcpkg Setup (Optional)
REM ============================================================================
echo %BLUE%[Step 2/6] Setting up vcpkg package manager...%RESET%
echo [Step 2/6] Setting up vcpkg package manager... >> "%LOG_FILE%"
echo/
echo/ >> "%LOG_FILE%"

REM First try parent directory (shared location), then project directory
set "VCPKG_PARENT=%REPO_ROOT%\..\vcpkg"
set "VCPKG_LOCAL=%REPO_ROOT%\vcpkg"
set "VCPKG_ROOT="

echo Checking for vcpkg installation...
echo Checking for vcpkg installation... >> "%LOG_FILE%"
echo DEBUG: Parent location: %VCPKG_PARENT%
echo DEBUG: Parent location: %VCPKG_PARENT% >> "%LOG_FILE%"
echo DEBUG: Local location: %VCPKG_LOCAL%
echo DEBUG: Local location: %VCPKG_LOCAL% >> "%LOG_FILE%"

REM Check parent directory first
if exist "%VCPKG_PARENT%\vcpkg.exe" (
    set "VCPKG_ROOT=%VCPKG_PARENT%"
    echo %GREEN%✓ vcpkg already installed in parent directory: %VCPKG_ROOT%%RESET%
    echo vcpkg already installed in parent directory: %VCPKG_ROOT% >> "%LOG_FILE%"
    echo/
    echo Verifying vcpkg installation...
    echo Verifying vcpkg installation... >> "%LOG_FILE%"
    "%VCPKG_ROOT%\vcpkg.exe" version >> "%LOG_FILE%" 2>&1
    echo/
) else if exist "%VCPKG_LOCAL%\vcpkg.exe" (
    set "VCPKG_ROOT=%VCPKG_LOCAL%"
    echo %GREEN%✓ vcpkg already installed in project directory: %VCPKG_ROOT%%RESET%
    echo vcpkg already installed in project directory: %VCPKG_ROOT% >> "%LOG_FILE%"
    echo/
    echo Verifying vcpkg installation...
    echo Verifying vcpkg installation... >> "%LOG_FILE%"
    "%VCPKG_ROOT%\vcpkg.exe" version >> "%LOG_FILE%" 2>&1
    echo/
) else (
    echo vcpkg not found in parent directory or project directory
    echo vcpkg not found in parent directory (%VCPKG_PARENT%) or project directory (%VCPKG_LOCAL%) >> "%LOG_FILE%"
    echo/
    echo vcpkg is a package manager for C++ that makes dependency management easier.
    echo It is recommended but not required for this project.
    echo/
    echo Installing vcpkg in the parent directory allows it to be shared
    echo across multiple projects, saving disk space and download time.
    echo/
    echo vcpkg not found. >> "%LOG_FILE%"
    echo Would you like to install it? >> "%LOG_FILE%"
    echo vcpkg is a package manager for C++ that makes dependency management easier. >> "%LOG_FILE%"
    echo/ >> "%LOG_FILE%"
    set /p INSTALL_VCPKG="Install vcpkg? (Y/N): "
    echo User response: !INSTALL_VCPKG! >> "%LOG_FILE%"
    
    if /i "!INSTALL_VCPKG!"=="Y" (
        echo/
        echo Installing vcpkg...
        echo Installing vcpkg... >> "%LOG_FILE%"
        
        REM Determine installation location - prefer parent directory
        echo/
        echo Where would you like to install vcpkg?
        echo   1. Parent directory (recommended - shared across projects): %VCPKG_PARENT%
        echo   2. Project directory (local to this project): %VCPKG_LOCAL%
        echo/
        set /p VCPKG_LOCATION="Enter choice (1 or 2, default is 1): "
        if "!VCPKG_LOCATION!"=="" set VCPKG_LOCATION=1
        echo User chose vcpkg location: !VCPKG_LOCATION! >> "%LOG_FILE%"
        
        if "!VCPKG_LOCATION!"=="2" (
            set "VCPKG_ROOT=%VCPKG_LOCAL%"
            echo Installing in project directory: %VCPKG_ROOT%
            echo Installing in project directory: %VCPKG_ROOT% >> "%LOG_FILE%"
        ) else (
            set "VCPKG_ROOT=%VCPKG_PARENT%"
            echo Installing in parent directory: %VCPKG_ROOT%
            echo Installing in parent directory: %VCPKG_ROOT% >> "%LOG_FILE%"
        )
        
        REM Clone vcpkg
        if not exist "%VCPKG_ROOT%" (
            echo/
            echo Cloning vcpkg repository to %VCPKG_ROOT%...
            echo Cloning vcpkg repository to %VCPKG_ROOT%... >> "%LOG_FILE%"
            echo %YELLOW%This will download vcpkg. Progress will be shown below.%RESET%
            echo %YELLOW%Press any key to start the download...%RESET%
            pause
            echo/
            echo Running: git clone https://github.com/Microsoft/vcpkg.git "%VCPKG_ROOT%"
            echo Running: git clone https://github.com/Microsoft/vcpkg.git "%VCPKG_ROOT%" >> "%LOG_FILE%"
            git clone https://github.com/Microsoft/vcpkg.git "%VCPKG_ROOT%"
            set CLONE_EXIT_CODE=!ERRORLEVEL!
            echo DEBUG: git clone exited with code: !CLONE_EXIT_CODE!
            echo DEBUG: git clone exited with code: !CLONE_EXIT_CODE! >> "%LOG_FILE%"
            if !CLONE_EXIT_CODE! NEQ 0 (
                echo/
                echo %RED%ERROR: Failed to clone vcpkg repository%RESET%
                echo ERROR: Failed to clone vcpkg repository >> "%LOG_FILE%"
                echo Make sure git is installed and you have internet connection
                echo Make sure git is installed and you have internet connection >> "%LOG_FILE%"
                echo/
                pause
                exit /b 1
            )
        )
        
        REM Verify clone succeeded
        if not exist "%VCPKG_ROOT%" (
            echo/
            echo %RED%ERROR: vcpkg directory was not created%RESET%
            echo ERROR: vcpkg directory was not created >> "%LOG_FILE%"
            echo Expected location: %VCPKG_ROOT% >> "%LOG_FILE%"
            pause
            exit /b 1
        )
        
        REM Bootstrap vcpkg
        echo/
        echo Bootstrapping vcpkg...
        echo Bootstrapping vcpkg... >> "%LOG_FILE%"
        echo %YELLOW%Downloading and bootstrapping vcpkg. Progress will be shown below.%RESET%
        echo %YELLOW%This may take several minutes. Press any key to start...%RESET%
        pause
        echo/
        echo DEBUG: Current directory: %CD%
        echo DEBUG: Current directory: %CD% >> "%LOG_FILE%"
        echo DEBUG: Changing to vcpkg directory: %VCPKG_ROOT%
        echo DEBUG: Changing to vcpkg directory: %VCPKG_ROOT% >> "%LOG_FILE%"
        cd /d "%VCPKG_ROOT%"
        set CD_EXIT_CODE=!ERRORLEVEL!
        echo DEBUG: cd exited with code: !CD_EXIT_CODE!
        echo DEBUG: cd exited with code: !CD_EXIT_CODE! >> "%LOG_FILE%"
        echo DEBUG: New current directory: %CD%
        echo DEBUG: New current directory: %CD% >> "%LOG_FILE%"
        if !CD_EXIT_CODE! NEQ 0 (
            echo %RED%ERROR: Failed to change to vcpkg directory%RESET%
            echo ERROR: Failed to change to vcpkg directory >> "%LOG_FILE%"
            echo Expected: %VCPKG_ROOT% >> "%LOG_FILE%"
            pause
            exit /b 1
        )
        
        echo Running: bootstrap-vcpkg.bat
        echo Running: bootstrap-vcpkg.bat >> "%LOG_FILE%"
        call bootstrap-vcpkg.bat
        set BOOTSTRAP_EXIT_CODE=!ERRORLEVEL!
        echo/
        echo DEBUG: bootstrap-vcpkg.bat exited with code: !BOOTSTRAP_EXIT_CODE!
        echo DEBUG: bootstrap-vcpkg.bat exited with code: !BOOTSTRAP_EXIT_CODE! >> "%LOG_FILE%"
        if !BOOTSTRAP_EXIT_CODE! NEQ 0 (
            echo/
            echo %RED%ERROR: Failed to bootstrap vcpkg%RESET%
            echo ERROR: Failed to bootstrap vcpkg >> "%LOG_FILE%"
            echo/
            pause
            exit /b 1
        )
        
        REM Verify vcpkg.exe was created
        if not exist "%VCPKG_ROOT%\vcpkg.exe" (
            echo/
            echo %RED%ERROR: vcpkg.exe not found after bootstrapping%RESET%
            echo ERROR: vcpkg.exe not found after bootstrapping >> "%LOG_FILE%"
            echo Expected location: %VCPKG_ROOT%\vcpkg.exe >> "%LOG_FILE%"
            echo This could be due to antivirus, permissions, or bootstrap failure.
            echo This could be due to antivirus, permissions, or bootstrap failure. >> "%LOG_FILE%"
            echo/
            echo DEBUG: Listing vcpkg directory contents:
            echo DEBUG: Listing vcpkg directory contents: >> "%LOG_FILE%"
            dir "%VCPKG_ROOT%" >> "%LOG_FILE%" 2>&1
            cd /d "%REPO_ROOT%"
            pause
            exit /b 1
        )
        echo %GREEN%✓ vcpkg.exe created successfully%RESET%
        echo vcpkg.exe created successfully at: %VCPKG_ROOT%\vcpkg.exe >> "%LOG_FILE%"
        
        REM Update vcpkg repository to get latest baseline commits
        echo/
        echo Updating vcpkg repository...
        echo Updating vcpkg repository... >> "%LOG_FILE%"
        git pull origin master
        if !ERRORLEVEL! NEQ 0 (
            echo/
            echo %YELLOW%WARNING: Failed to update vcpkg repository%RESET%
            echo WARNING: Failed to update vcpkg repository >> "%LOG_FILE%"
            echo This may cause issues if the baseline commit is not available.
            echo This may cause issues if the baseline commit is not available. >> "%LOG_FILE%"
        ) else (
            echo %GREEN%✓ vcpkg repository updated%RESET%
            echo vcpkg repository updated >> "%LOG_FILE%"
        )
        
        REM Integrate vcpkg with Visual Studio
        echo/
        echo Integrating vcpkg with Visual Studio...
        echo Integrating vcpkg with Visual Studio... >> "%LOG_FILE%"
        echo/
        "%VCPKG_ROOT%\vcpkg.exe" integrate install
        if !ERRORLEVEL! NEQ 0 (
            echo/
            echo %YELLOW%WARNING: vcpkg integrate install returned error code !ERRORLEVEL!%RESET%
            echo WARNING: vcpkg integrate install returned error code !ERRORLEVEL! >> "%LOG_FILE%"
            echo This is not critical - you can integrate manually later if needed.
            echo This is not critical - you can integrate manually later if needed. >> "%LOG_FILE%"
        ) else (
            echo %GREEN%✓ vcpkg integrated with Visual Studio%RESET%
            echo vcpkg integrated with Visual Studio >> "%LOG_FILE%"
        )
        
        echo DEBUG: Returning to repository root: %REPO_ROOT%
        echo DEBUG: Returning to repository root: %REPO_ROOT% >> "%LOG_FILE%"
        cd /d "%REPO_ROOT%"
        if !ERRORLEVEL! NEQ 0 (
            echo %RED%ERROR: Failed to return to repository root%RESET%
            echo ERROR: Failed to return to repository root >> "%LOG_FILE%"
            echo Expected: %REPO_ROOT% >> "%LOG_FILE%"
            echo Current: %CD% >> "%LOG_FILE%"
            pause
            exit /b 1
        )
        echo DEBUG: Successfully returned to: %CD%
        echo DEBUG: Successfully returned to: %CD% >> "%LOG_FILE%"
        echo/
        echo %GREEN%✓ vcpkg installed successfully!%RESET%
        echo vcpkg installed successfully! >> "%LOG_FILE%"
    ) else (
        echo Skipping vcpkg installation.
        echo You will need to install dependencies manually.
        echo Skipping vcpkg installation. >> "%LOG_FILE%"
        echo You will need to install dependencies manually. >> "%LOG_FILE%"
    )
)

echo/
echo %YELLOW%Step 2 completed. Press any key to continue to Step 3...%RESET%
echo/
echo/ >> "%LOG_FILE%"
pause

REM ============================================================================
REM Step 3: Install Dependencies (Manifest Mode)
REM ============================================================================
echo %BLUE%[Step 3/6] Configuring dependencies...%RESET%
echo [Step 3/6] Configuring dependencies... >> "%LOG_FILE%"
echo/
echo/ >> "%LOG_FILE%"

REM Check if vcpkg executable exists
echo Checking for vcpkg executable...
echo Checking for vcpkg executable... >> "%LOG_FILE%"

REM Re-check vcpkg locations if VCPKG_ROOT is not set
if not defined VCPKG_ROOT (
    echo Vcpkg was not installed or found in Step 2, rechecking...
    echo Vcpkg was not installed or found in Step 2, rechecking... >> "%LOG_FILE%"
    
    if exist "%VCPKG_PARENT%\vcpkg.exe" (
        set "VCPKG_ROOT=%VCPKG_PARENT%"
        echo Found vcpkg in parent directory: %VCPKG_ROOT%
        echo Found vcpkg in parent directory: %VCPKG_ROOT% >> "%LOG_FILE%"
    ) else if exist "%VCPKG_LOCAL%\vcpkg.exe" (
        set "VCPKG_ROOT=%VCPKG_LOCAL%"
        echo Found vcpkg in project directory: %VCPKG_ROOT%
        echo Found vcpkg in project directory: %VCPKG_ROOT% >> "%LOG_FILE%"
    ) else (
        echo WARNING: vcpkg not found in either location
        echo WARNING: vcpkg not found in either location >> "%LOG_FILE%"
    )
)

if defined VCPKG_ROOT (
    echo Checking path: %VCPKG_ROOT%\vcpkg.exe
    echo Checking path: %VCPKG_ROOT%\vcpkg.exe >> "%LOG_FILE%"
) else (
    echo VCPKG_ROOT is not set - continuing without vcpkg
    echo VCPKG_ROOT is not set - continuing without vcpkg >> "%LOG_FILE%"
)
echo/
echo/ >> "%LOG_FILE%"

if exist "%VCPKG_ROOT%\vcpkg.exe" (
    echo %GREEN%✓ vcpkg found at: %VCPKG_ROOT%\vcpkg.exe%RESET%
    echo vcpkg found at: %VCPKG_ROOT%\vcpkg.exe >> "%LOG_FILE%"
    echo/
    echo This project uses vcpkg manifest mode (vcpkg.json).
    echo Dependencies will be installed automatically during CMake configuration.
    echo/
    echo This project uses vcpkg manifest mode (vcpkg.json). >> "%LOG_FILE%"
    echo Dependencies will be installed automatically during CMake configuration. >> "%LOG_FILE%"
    echo/ >> "%LOG_FILE%"
    
    echo Dependencies defined in vcpkg.json:
    echo   - GLFW3 (window management)
    echo   - GLM (math library)
    echo   - ImGui (editor UI)
    echo/
    echo Dependencies defined in vcpkg.json: >> "%LOG_FILE%"
    echo   - GLFW3 (window management) >> "%LOG_FILE%"
    echo   - GLM (math library) >> "%LOG_FILE%"
    echo   - ImGui (editor UI) >> "%LOG_FILE%"
    echo/ >> "%LOG_FILE%"
    
    echo %GREEN%✓ vcpkg is configured for manifest mode%RESET%
    echo   Dependencies will be installed in the next step during CMake configuration.
    echo/
    echo vcpkg is configured for manifest mode >> "%LOG_FILE%"
    echo Dependencies will be installed in the next step during CMake configuration. >> "%LOG_FILE%"
    echo/ >> "%LOG_FILE%"
) else (
    echo %YELLOW%WARNING: vcpkg not available at: %VCPKG_ROOT%\vcpkg.exe%RESET%
    echo %YELLOW%Dependencies must be installed manually.%RESET%
    echo/
    echo WARNING: vcpkg not available at: %VCPKG_ROOT%\vcpkg.exe >> "%LOG_FILE%"
    echo Dependencies must be installed manually. >> "%LOG_FILE%"
    echo/ >> "%LOG_FILE%"
    
    REM Additional debug - check if directory exists
    if exist "%VCPKG_ROOT%" (
        echo DEBUG: vcpkg directory exists but vcpkg.exe is missing
        echo DEBUG: vcpkg directory exists but vcpkg.exe is missing >> "%LOG_FILE%"
        echo DEBUG: Listing contents of %VCPKG_ROOT%:
        echo DEBUG: Listing contents of %VCPKG_ROOT%: >> "%LOG_FILE%"
        dir "%VCPKG_ROOT%" >> "%LOG_FILE%" 2>&1
    ) else (
        echo DEBUG: vcpkg directory does not exist at %VCPKG_ROOT%
        echo DEBUG: vcpkg directory does not exist at %VCPKG_ROOT% >> "%LOG_FILE%"
    )
    echo/
    echo/ >> "%LOG_FILE%"
    
    echo The following libraries are required:
    echo   - GLFW 3.3+
    echo   - GLM
    echo   - ImGui (optional, for editor UI)
    echo/
    echo The following libraries are required: >> "%LOG_FILE%"
    echo   - GLFW 3.3+ >> "%LOG_FILE%"
    echo   - GLM >> "%LOG_FILE%"
    echo   - ImGui (optional, for editor UI) >> "%LOG_FILE%"
    echo/ >> "%LOG_FILE%"
    
    echo See %REPO_ROOT%\DEVELOPER_SETUP.md for manual installation instructions.
    echo/
    echo See DEVELOPER_SETUP.md for manual installation instructions. >> "%LOG_FILE%"
    echo/ >> "%LOG_FILE%"
)

echo vcpkg check completed >> "%LOG_FILE%"
echo/
echo %YELLOW%Step 3 completed. Press any key to continue to Step 4 (CMake configuration)...%RESET%
echo %YELLOW%Or press Ctrl+C to stop and review the log file.%RESET%
echo/
echo User prompted to continue to Step 4... >> "%LOG_FILE%"
pause
echo User chose to continue to Step 4 >> "%LOG_FILE%"
echo Continuing to Step 4... >> "%LOG_FILE%"
echo/
echo/ >> "%LOG_FILE%"

REM ============================================================================
REM Step 4: Generate Visual Studio Project Files
REM ============================================================================
echo/
echo/ >> "%LOG_FILE%"
echo %BLUE%[Step 4/6] Generating Visual Studio project files...%RESET%
echo [Step 4/6] Generating Visual Studio project files... >> "%LOG_FILE%"
echo/
echo/ >> "%LOG_FILE%"

echo %YELLOW%NOTE: This step will automatically install dependencies from vcpkg.json%RESET%
echo %YELLOW%      if vcpkg is available. This may take several minutes on first run.%RESET%
echo/
echo NOTE: This step will automatically install dependencies from vcpkg.json >> "%LOG_FILE%"
echo       if vcpkg is available. This may take several minutes on first run. >> "%LOG_FILE%"
echo/ >> "%LOG_FILE%"

REM Create build directory if it doesn't exist
echo DEBUG: Creating build directory if needed...
echo DEBUG: Creating build directory if needed... >> "%LOG_FILE%"
if not exist "build" (
    mkdir build
    if !ERRORLEVEL! NEQ 0 (
        echo %RED%ERROR: Failed to create build directory%RESET%
        echo ERROR: Failed to create build directory >> "%LOG_FILE%"
        echo/
        pause
        exit /b 1
    )
    echo DEBUG: Build directory created successfully
    echo DEBUG: Build directory created successfully >> "%LOG_FILE%"
) else (
    echo DEBUG: Build directory already exists
    echo DEBUG: Build directory already exists >> "%LOG_FILE%"
)

echo DEBUG: Current directory before cd: %CD%
echo DEBUG: Current directory before cd: %CD% >> "%LOG_FILE%"

cd /d build
if !ERRORLEVEL! NEQ 0 (
    echo %RED%ERROR: Failed to change to build directory%RESET%
    echo ERROR: Failed to change to build directory >> "%LOG_FILE%"
    echo/
    pause
    exit /b 1
)

echo DEBUG: Current directory after cd: %CD%
echo DEBUG: Current directory after cd: %CD% >> "%LOG_FILE%"

REM Generate project files
echo/
echo Running CMake...
echo Running CMake... >> "%LOG_FILE%"
echo/

if exist "%VCPKG_ROOT%\scripts\buildsystems\vcpkg.cmake" (
    echo Using vcpkg toolchain file (manifest mode)...
    echo Dependencies will be installed automatically from vcpkg.json...
    echo/
    echo Using vcpkg toolchain file (manifest mode)... >> "%LOG_FILE%"
    echo Dependencies will be installed automatically from vcpkg.json... >> "%LOG_FILE%"
    echo Toolchain file: %VCPKG_ROOT%\scripts\buildsystems\vcpkg.cmake >> "%LOG_FILE%"
    echo/ >> "%LOG_FILE%"
    echo %YELLOW%NOTE: vcpkg will now install dependencies. This may take 10-15 minutes on first run.%RESET%
    echo %YELLOW%      You will see download and build progress below. Please be patient.%RESET%
    echo/
    echo %YELLOW%Press any key to start CMake generation...%RESET%
    pause
    echo/
    echo Starting CMake...
    echo Starting CMake... >> "%LOG_FILE%"
    cmake -G "Visual Studio 17 2022" -A x64 -DCMAKE_TOOLCHAIN_FILE="%VCPKG_ROOT%\scripts\buildsystems\vcpkg.cmake" ..
) else (
    echo Using system-installed dependencies...
    echo Using system-installed dependencies... >> "%LOG_FILE%"
    echo WARNING: vcpkg toolchain not found at: %VCPKG_ROOT%\scripts\buildsystems\vcpkg.cmake >> "%LOG_FILE%"
    echo/
    echo %YELLOW%Press any key to start CMake generation...%RESET%
    pause
    echo/
    echo Starting CMake...
    echo Starting CMake... >> "%LOG_FILE%"
    cmake -G "Visual Studio 17 2022" -A x64 ..
)

set CMAKE_EXIT_CODE=!ERRORLEVEL!
echo/
echo DEBUG: CMake exited with code: !CMAKE_EXIT_CODE!
echo DEBUG: CMake exited with code: !CMAKE_EXIT_CODE! >> "%LOG_FILE%"

if !CMAKE_EXIT_CODE! NEQ 0 (
    echo/
    echo %RED%ERROR: CMake project generation failed%RESET%
    echo ERROR: CMake project generation failed with exit code !CMAKE_EXIT_CODE! >> "%LOG_FILE%"
    echo Please check the error messages above
    echo/
    echo Full CMake output has been saved to: %LOG_FILE%
    echo/
    echo Troubleshooting:
    echo   1. Check that vcpkg.json is valid
    echo   2. Verify internet connection for dependency downloads
    echo   3. Check CMakeLists.txt for syntax errors
    echo   4. Review the log file: %LOG_FILE%
    echo/
    cd /d "%REPO_ROOT%"
    pause
    exit /b 1
)

echo DEBUG: Returning to repository root...
echo DEBUG: Returning to repository root... >> "%LOG_FILE%"
cd /d "%REPO_ROOT%"
if !ERRORLEVEL! NEQ 0 (
    echo %RED%ERROR: Failed to return to repository root%RESET%
    echo ERROR: Failed to return to repository root >> "%LOG_FILE%"
    pause
    exit /b 1
)

echo/
echo %GREEN%✓ Visual Studio project files generated successfully!%RESET%
echo   Location: %REPO_ROOT%\build\FreshVoxelEngine.sln
echo/
echo Visual Studio project files generated successfully! >> "%LOG_FILE%"
echo Location: %REPO_ROOT%\build\FreshVoxelEngine.sln >> "%LOG_FILE%"
echo/ >> "%LOG_FILE%"
echo/
echo %YELLOW%Step 4 completed. Press any key to continue to Step 5 (Build)...%RESET%
echo %YELLOW%Or press Ctrl+C to stop here and manually open Visual Studio.%RESET%
echo/
pause

REM ============================================================================
REM Step 5: Build the Project
REM ============================================================================
echo/
echo/ >> "%LOG_FILE%"
echo %BLUE%[Step 5/6] Building the project...%RESET%
echo [Step 5/6] Building the project... >> "%LOG_FILE%"
echo/
echo This may take several minutes depending on your system...
echo/
echo This may take several minutes depending on your system... >> "%LOG_FILE%"
echo/ >> "%LOG_FILE%"

echo %YELLOW%This step will compile the engine. You can skip this and build manually in Visual Studio.%RESET%
echo/
set /p DO_BUILD="Would you like to build now? (Y/N): "
echo User response to build prompt: !DO_BUILD! >> "%LOG_FILE%"

if /i "!DO_BUILD!" NEQ "Y" (
    echo/
    echo %YELLOW%Skipping build step.%RESET%
    echo %YELLOW%You can build manually by opening build\FreshVoxelEngine.sln in Visual Studio.%RESET%
    echo/
    echo Skipping build step. >> "%LOG_FILE%"
    echo User chose to skip build and build manually in Visual Studio. >> "%LOG_FILE%"
    echo/ >> "%LOG_FILE%"
    goto skip_build
)

REM Build in Release mode
echo/
echo Building Release configuration...
echo Building Release configuration... >> "%LOG_FILE%"
echo/ >> "%LOG_FILE%"
echo %YELLOW%Building... This may take several minutes. Progress will be shown below.%RESET%
echo/
echo DEBUG: About to run: cmake --build build --config Release
echo DEBUG: About to run: cmake --build build --config Release >> "%LOG_FILE%"
echo/

cmake --build build --config Release

set BUILD_EXIT_CODE=!ERRORLEVEL!
echo/
echo DEBUG: Build exited with code: !BUILD_EXIT_CODE!
echo DEBUG: Build exited with code: !BUILD_EXIT_CODE! >> "%LOG_FILE%"

if !BUILD_EXIT_CODE! NEQ 0 (
    echo/
    echo %RED%ERROR: Build failed%RESET%
    echo ERROR: Build failed with exit code !BUILD_EXIT_CODE! >> "%LOG_FILE%"
    echo Please check the error messages above
    echo/
    echo Full build output has been saved to: %LOG_FILE%
    echo/
    echo Troubleshooting:
    echo   1. Check for compiler errors in the output above
    echo   2. Try building manually in Visual Studio for better error messages
    echo   3. Review the log file: %LOG_FILE%
    echo/
    echo You can try building manually by opening:
    echo   %REPO_ROOT%\build\FreshVoxelEngine.sln
    echo/ >> "%LOG_FILE%"
    echo You can try building manually by opening: >> "%LOG_FILE%"
    echo   %REPO_ROOT%\build\FreshVoxelEngine.sln >> "%LOG_FILE%"
    echo/
    pause
    exit /b 1
)

echo/
echo %GREEN%✓ Build completed successfully!%RESET%
echo/
echo Build completed successfully! >> "%LOG_FILE%"
echo/ >> "%LOG_FILE%"

:skip_build
echo/
echo %YELLOW%Step 5 completed. Press any key to continue to Step 6 (Shortcuts)...%RESET%
echo/
pause

REM ============================================================================
REM Step 6: Create Shortcuts (Optional)
REM ============================================================================
echo/
echo/ >> "%LOG_FILE%"
echo %BLUE%[Step 6/6] Creating shortcuts...%RESET%
echo [Step 6/6] Creating shortcuts... >> "%LOG_FILE%"
echo/
echo/ >> "%LOG_FILE%"

echo Creating convenient batch file shortcuts will make it easier to:
echo   - Open the Visual Studio solution
echo   - Run the engine after building
echo/

set /p CREATE_SHORTCUTS="Would you like to create desktop shortcuts? (Y/N): "
echo User response: !CREATE_SHORTCUTS! >> "%LOG_FILE%"

if /i "!CREATE_SHORTCUTS!"=="Y" (
    REM Create a shortcut script
    echo/
    echo Creating shortcuts...
    echo Creating shortcuts... >> "%LOG_FILE%"
    
    REM Note: Creating .lnk files from batch is complex
    REM Instead, we'll create simple batch file shortcuts
    
    REM Shortcut to open Visual Studio solution
    echo @echo off > "%REPO_ROOT%\Open_Solution.bat"
    echo start "" "build\FreshVoxelEngine.sln" >> "%REPO_ROOT%\Open_Solution.bat"
    echo %GREEN%✓ Created Open_Solution.bat%RESET%
    echo Created Open_Solution.bat >> "%LOG_FILE%"
    
    REM Shortcut to run the engine
    if exist "build\Release\FreshVoxelEngine.exe" (
        echo @echo off > "%REPO_ROOT%\Run_Engine.bat"
        echo cd /d "%REPO_ROOT%" >> "%REPO_ROOT%\Run_Engine.bat"
        echo start "" "build\Release\FreshVoxelEngine.exe" >> "%REPO_ROOT%\Run_Engine.bat"
        echo %GREEN%✓ Created Run_Engine.bat%RESET%
        echo Created Run_Engine.bat >> "%LOG_FILE%"
    ) else (
        echo %YELLOW%Note: Run_Engine.bat not created (executable not found)%RESET%
        echo Note: Run_Engine.bat not created (executable not found) >> "%LOG_FILE%"
    )
) else (
    echo Skipping shortcuts.
    echo Skipping shortcuts. >> "%LOG_FILE%"
)

echo/
echo/ >> "%LOG_FILE%"

REM ============================================================================
REM Installation Complete
REM ============================================================================
echo/
echo %GREEN%============================================================================%RESET%
echo %GREEN%                   Installation Completed Successfully!%RESET%
echo %GREEN%============================================================================%RESET%
echo/
echo ============================================================================ >> "%LOG_FILE%"
echo                   Installation Completed Successfully! >> "%LOG_FILE%"
echo ============================================================================ >> "%LOG_FILE%"
echo/ >> "%LOG_FILE%"
echo Installation finished: %date% %time% >> "%LOG_FILE%"
echo/ >> "%LOG_FILE%"
echo %BLUE%Installation Summary:%RESET%
echo   - Visual Studio solution: build\FreshVoxelEngine.sln
if exist "build\Release\FreshVoxelEngine.exe" (
    echo   - Engine executable: build\Release\FreshVoxelEngine.exe
)
echo   - Installation log: %LOG_FILE%
echo/
echo Installation Summary: >> "%LOG_FILE%"
echo   - Visual Studio solution: build\FreshVoxelEngine.sln >> "%LOG_FILE%"
if exist "build\Release\FreshVoxelEngine.exe" (
    echo   - Engine executable: build\Release\FreshVoxelEngine.exe >> "%LOG_FILE%"
)
echo   - Installation log: %LOG_FILE% >> "%LOG_FILE%"
echo/ >> "%LOG_FILE%"
echo %YELLOW%The root install.bat will show next steps.%RESET%
echo %YELLOW%Press any key to return to the main installer...%RESET%
echo/
pause
