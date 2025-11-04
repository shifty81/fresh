@echo off
REM ============================================================================
REM Fresh Voxel Engine - Installation Launcher
REM ============================================================================
REM This script runs the batch file installation (PowerShell no longer used)
REM ============================================================================

echo.
echo ============================================================================
echo        Fresh Voxel Engine - Installation
echo ============================================================================
echo.
echo Starting Batch File installation...
echo.

call "%~dp0tools\build_tools\install.bat"

echo.
echo Installation complete. Check logs directory for detailed output.
pause
