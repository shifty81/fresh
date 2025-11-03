@echo off
REM ============================================================================
REM Fresh Voxel Engine - Clean Build Script
REM ============================================================================
REM This script cleans all build artifacts and generated files
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
set "LOG_FILE=%LOG_DIR%\clean_errors_%TIMESTAMP%.txt"

echo ============================================================================ > "%LOG_FILE%"
echo Fresh Voxel Engine - Clean Log >> "%LOG_FILE%"
echo Started: %date% %time% >> "%LOG_FILE%"
echo ============================================================================ >> "%LOG_FILE%"
echo. >> "%LOG_FILE%"

echo.
echo ============================================================================
echo        Fresh Voxel Engine - Clean Build Artifacts
echo ============================================================================
echo.
echo Log file: %LOG_FILE%
echo.
echo This will delete:
echo   - build/ directory (Visual Studio project files and binaries)
echo   - Generated shortcuts (Open_Solution.bat, Run_Engine.bat)
echo.
set /p CONFIRM="Are you sure you want to continue? (Y/N): "

if /i "%CONFIRM%" NEQ "Y" (
    echo Cancelled.
    pause
    exit /b 0
)

echo.
echo Cleaning build artifacts...
echo.

REM Remove build directory
if exist "build" (
    echo Deleting build directory...
    echo Deleting build directory... >> "%LOG_FILE%"
    rmdir /s /q "build" 2>> "%LOG_FILE%"
    if %ERRORLEVEL% EQU 0 (
        echo   [OK] build/ removed
        echo   [OK] build/ removed >> "%LOG_FILE%"
    ) else (
        echo   [ERROR] Failed to remove build/ >> "%LOG_FILE%"
    )
) else (
    echo   [SKIP] build/ does not exist
    echo   [SKIP] build/ does not exist >> "%LOG_FILE%"
)

REM Remove generated shortcuts
if exist "Open_Solution.bat" (
    echo Deleting Open_Solution.bat...
    del /q "Open_Solution.bat"
    echo   [OK] Open_Solution.bat removed
)

if exist "Run_Engine.bat" (
    echo Deleting Run_Engine.bat...
    del /q "Run_Engine.bat"
    echo   [OK] Run_Engine.bat removed
)

echo.
echo ============================================================================
echo Clean completed successfully!
echo ============================================================================
echo.
echo Clean completed successfully! >> "%LOG_FILE%"
echo To rebuild the project, run: tools\build_tools\install.bat
echo.
echo ============================================================================ >> "%LOG_FILE%"
echo Log ended: %date% %time% >> "%LOG_FILE%"
echo ============================================================================ >> "%LOG_FILE%"
pause
