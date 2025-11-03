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

echo.
echo ============================================================================
echo        Fresh Voxel Engine - Quick Rebuild
echo ============================================================================
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
if exist "build" (
    rmdir /s /q "build"
    echo Build directory cleaned.
)

REM Step 2: Generate
echo.
echo [Step 2/3] Generating project files...
if not exist "build" mkdir build
cd build

REM Check for vcpkg
if exist "%REPO_ROOT%\vcpkg\scripts\buildsystems\vcpkg.cmake" (
    cmake -G "Visual Studio 17 2022" -A x64 -DCMAKE_TOOLCHAIN_FILE="%REPO_ROOT%\vcpkg\scripts\buildsystems\vcpkg.cmake" ..
) else (
    cmake -G "Visual Studio 17 2022" -A x64 ..
)

if %ERRORLEVEL% NEQ 0 (
    echo.
    echo ERROR: Project generation failed
    cd "%REPO_ROOT%"
    pause
    exit /b 1
)

cd "%REPO_ROOT%"
echo Project files generated successfully.

REM Step 3: Build
echo.
echo [Step 3/3] Building...
cmake --build build --config Release

if %ERRORLEVEL% NEQ 0 (
    echo.
    echo ERROR: Build failed
    pause
    exit /b 1
)

echo.
echo ============================================================================
echo Rebuild completed successfully!
echo ============================================================================
echo.
echo Executable: build\Release\FreshVoxelEngine.exe
echo.
pause
