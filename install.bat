@echo off
REM ============================================================================
REM Fresh Voxel Engine - Installation Launcher
REM ============================================================================
REM This script lets you choose between batch and PowerShell installation
REM ============================================================================

echo.
echo ============================================================================
echo        Fresh Voxel Engine - Installation Launcher
echo ============================================================================
echo.
echo Choose your installation method:
echo.
echo   1. PowerShell (Recommended - Better progress display and error handling)
echo   2. Batch File (Classic - Works on older systems)
echo   3. Cancel
echo.

set /p CHOICE="Enter your choice (1-3): "

if "%CHOICE%"=="1" (
    echo.
    echo Launching PowerShell installation script...
    echo.
    powershell -ExecutionPolicy Bypass -File "%~dp0tools\build_tools\install.ps1"
    goto :end
)

if "%CHOICE%"=="2" (
    echo.
    echo Launching Batch installation script...
    echo.
    call "%~dp0tools\build_tools\install.bat"
    goto :end
)

if "%CHOICE%"=="3" (
    echo.
    echo Installation cancelled.
    goto :end
)

echo.
echo Invalid choice. Please run the script again and choose 1, 2, or 3.
echo.

:end
pause
