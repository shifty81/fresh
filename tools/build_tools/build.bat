@echo off
REM ============================================================================
REM Fresh Voxel Engine - Quick Build Script
REM ============================================================================
REM This script quickly builds the project without regenerating project files
REM Useful for incremental builds during development
REM ============================================================================

setlocal

REM Navigate to repository root
cd /d "%~dp0..\.."
set "REPO_ROOT=%CD%"

REM Setup logging
set "LOG_DIR=%REPO_ROOT%\logs"
if not exist "%LOG_DIR%" mkdir "%LOG_DIR%"
REM Note: Timestamp format may vary with locale settings - this works for US/EN locales
set "TIMESTAMP=%date:~-4,4%-%date:~-10,2%-%date:~-7,2%_%time:~0,2%-%time:~3,2%-%time:~6,2%"
set "TIMESTAMP=%TIMESTAMP: =0%"
set "LOG_FILE=%LOG_DIR%\build_errors_%TIMESTAMP%.txt"

echo ============================================================================ > "%LOG_FILE%"
echo Fresh Voxel Engine - Build Log >> "%LOG_FILE%"
echo Started: %date% %time% >> "%LOG_FILE%"
echo ============================================================================ >> "%LOG_FILE%"
echo. >> "%LOG_FILE%"

echo.
echo ============================================================================
echo        Fresh Voxel Engine - Quick Build
echo ============================================================================
echo.
echo Log file: %LOG_FILE%
echo.

REM Check if build directory exists
if not exist "build" (
    echo ERROR: Build directory does not exist
    echo ERROR: Build directory does not exist >> "%LOG_FILE%"
    echo.
    echo Please run install.bat first to generate project files
    echo Or run: tools\build_tools\install.bat
    echo. >> "%LOG_FILE%"
    echo Please run install.bat first to generate project files >> "%LOG_FILE%"
    echo.
    pause
    exit /b 1
)

REM Prompt for build configuration
echo Select build configuration:
echo   1. Debug
echo   2. Release
echo   3. RelWithDebInfo
echo.
set /p CONFIG_CHOICE="Enter choice (1-3, default is 2): "

if "%CONFIG_CHOICE%"=="" set CONFIG_CHOICE=2

if "%CONFIG_CHOICE%"=="1" set BUILD_CONFIG=Debug
if "%CONFIG_CHOICE%"=="2" set BUILD_CONFIG=Release
if "%CONFIG_CHOICE%"=="3" set BUILD_CONFIG=RelWithDebInfo

if not defined BUILD_CONFIG (
    echo Invalid choice. Using Release.
    echo Invalid choice. Using Release. >> "%LOG_FILE%"
    set BUILD_CONFIG=Release
)

echo Build Configuration: %BUILD_CONFIG% >> "%LOG_FILE%"
echo. >> "%LOG_FILE%"

echo.
echo Building %BUILD_CONFIG% configuration...
echo.

REM Build the project
cmake --build build --config %BUILD_CONFIG% 2>> "%LOG_FILE%"

if %ERRORLEVEL% NEQ 0 (
    echo.
    echo ERROR: Build failed
    echo ERROR: Build failed with exit code %ERRORLEVEL% >> "%LOG_FILE%"
    echo Check log file for details: %LOG_FILE% >> "%LOG_FILE%"
    echo.
    echo Check log file for details: %LOG_FILE%
    echo.
    pause
    exit /b 1
)

echo.
echo Build completed successfully! >> "%LOG_FILE%"
echo ============================================================================
echo Build completed successfully!
echo ============================================================================
echo.
echo Configuration: %BUILD_CONFIG%
echo Executable: build\%BUILD_CONFIG%\FreshVoxelEngine.exe
echo.

REM Ask if user wants to run the engine
set /p RUN_ENGINE="Would you like to run the engine now? (Y/N): "
if /i "%RUN_ENGINE%"=="Y" (
    echo.
    echo Starting Fresh Voxel Engine...
    echo Starting Fresh Voxel Engine... >> "%LOG_FILE%"
    start "" "build\%BUILD_CONFIG%\FreshVoxelEngine.exe"
)

echo. >> "%LOG_FILE%"
echo ============================================================================ >> "%LOG_FILE%"
echo Log ended: %date% %time% >> "%LOG_FILE%"
echo ============================================================================ >> "%LOG_FILE%"

pause
