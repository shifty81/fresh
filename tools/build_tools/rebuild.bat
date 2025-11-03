@echo off
REM ============================================================================
REM Fresh Voxel Engine - Quick Rebuild Script
REM ============================================================================
REM This script performs a clean rebuild of the project
REM ============================================================================

setlocal

REM Navigate to repository root
cd /d "%~dp0..\.."
set "REPO_ROOT=%CD%"

REM Setup logging
set "LOG_DIR=%REPO_ROOT%\logs"
if not exist "%LOG_DIR%" mkdir "%LOG_DIR%"
set "TIMESTAMP=%date:~-4,4%-%date:~-10,2%-%date:~-7,2%_%time:~0,2%-%time:~3,2%-%time:~6,2%"
set "TIMESTAMP=%TIMESTAMP: =0%"
set "LOG_FILE=%LOG_DIR%\rebuild_errors_%TIMESTAMP%.txt"

echo ============================================================================ > "%LOG_FILE%"
echo Fresh Voxel Engine - Rebuild Log >> "%LOG_FILE%"
echo Started: %date% %time% >> "%LOG_FILE%"
echo ============================================================================ >> "%LOG_FILE%"
echo. >> "%LOG_FILE%"

echo.
echo ============================================================================
echo        Fresh Voxel Engine - Quick Rebuild
echo ============================================================================
echo.
echo Log file: %LOG_FILE%
echo.
echo This will:
echo   1. Clean existing build artifacts
echo   2. Regenerate Visual Studio project files
echo   3. Build the project in Release mode
echo.
pause

REM Step 1: Clean
echo.
echo [Step 1/3] Cleaning...
echo [Step 1/3] Cleaning... >> "%LOG_FILE%"
if exist "build" (
    rmdir /s /q "build" 2>> "%LOG_FILE%"
    echo Build directory cleaned.
    echo Build directory cleaned. >> "%LOG_FILE%"
)

REM Step 2: Generate
echo.
echo [Step 2/3] Generating project files...
echo [Step 2/3] Generating project files... >> "%LOG_FILE%"
if not exist "build" mkdir build
cd build

REM Check for vcpkg
if exist "%REPO_ROOT%\vcpkg\scripts\buildsystems\vcpkg.cmake" (
    cmake -G "Visual Studio 17 2022" -A x64 -DCMAKE_TOOLCHAIN_FILE="%REPO_ROOT%\vcpkg\scripts\buildsystems\vcpkg.cmake" .. 2>> "%LOG_FILE%"
) else (
    cmake -G "Visual Studio 17 2022" -A x64 .. 2>> "%LOG_FILE%"
)

if %ERRORLEVEL% NEQ 0 (
    echo.
    echo ERROR: Project generation failed
    echo ERROR: Project generation failed >> "%LOG_FILE%"
    cd "%REPO_ROOT%"
    pause
    exit /b 1
)

cd "%REPO_ROOT%"
echo Project files generated successfully.
echo Project files generated successfully. >> "%LOG_FILE%"

REM Step 3: Build
echo.
echo [Step 3/3] Building...
echo [Step 3/3] Building... >> "%LOG_FILE%"
cmake --build build --config Release 2>> "%LOG_FILE%"

if %ERRORLEVEL% NEQ 0 (
    echo.
    echo ERROR: Build failed
    echo ERROR: Build failed >> "%LOG_FILE%"
    pause
    exit /b 1
)

echo.
echo ============================================================================
echo Rebuild completed successfully!
echo ============================================================================
echo Rebuild completed successfully! >> "%LOG_FILE%"
echo.
echo Executable: build\Release\FreshVoxelEngine.exe
echo.
echo ============================================================================ >> "%LOG_FILE%"
echo Log ended: %date% %time% >> "%LOG_FILE%"
echo ============================================================================ >> "%LOG_FILE%"
pause
