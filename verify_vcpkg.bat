@echo off
setlocal enabledelayedexpansion
REM ============================================================================
REM Fresh Voxel Engine - vcpkg Verification Script
REM ============================================================================
REM This script checks if vcpkg is properly installed and accessible
REM ============================================================================

echo ================================================
echo vcpkg Verification
echo ================================================
echo/

REM Check for vcpkg in parent directory
set "VCPKG_PARENT=%~dp0..\vcpkg"
set "VCPKG_LOCAL=%~dp0vcpkg"
set "VCPKG_FOUND=0"
set "VCPKG_LOCATION="

echo Checking for vcpkg installation...
echo/

if exist "%VCPKG_PARENT%\vcpkg.exe" (
    set "VCPKG_FOUND=1"
    set "VCPKG_LOCATION=%VCPKG_PARENT%"
    echo [OK] vcpkg found in parent directory
    echo      Location: %VCPKG_PARENT%
) else (
    echo [NOT FOUND] vcpkg not in parent directory: %VCPKG_PARENT%
)

echo/

if exist "%VCPKG_LOCAL%\vcpkg.exe" (
    if "!VCPKG_FOUND!"=="0" (
        set "VCPKG_FOUND=1"
        set "VCPKG_LOCATION=%VCPKG_LOCAL%"
        echo [OK] vcpkg found in project directory
    ) else (
        echo [INFO] vcpkg also exists in project directory
    )
    echo      Location: %VCPKG_LOCAL%
) else (
    echo [NOT FOUND] vcpkg not in project directory: %VCPKG_LOCAL%
)

echo/
echo ================================================

if "!VCPKG_FOUND!"=="1" (
    echo Status: vcpkg is installed
    echo Using: !VCPKG_LOCATION!
    echo/
    
    REM Show vcpkg version
    echo vcpkg version:
    "!VCPKG_LOCATION!\vcpkg.exe" version
    echo/
    
    REM Check if dependencies are installed
    echo Checking installed packages...
    "!VCPKG_LOCATION!\vcpkg.exe" list
    echo/
    
    echo Recommendation: vcpkg is ready to use
) else (
    echo Status: vcpkg is NOT installed
    echo/
    echo To install vcpkg, run: install.bat
    echo This will set up vcpkg and all required dependencies.
)

echo ================================================
echo/
pause
