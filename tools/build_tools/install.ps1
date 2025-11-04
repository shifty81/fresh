#Requires -Version 5.1
<#
.SYNOPSIS
    Fresh Voxel Engine - Automated Installation Script
.DESCRIPTION
    This script automates the complete setup and installation process
    for the Fresh Voxel Engine on Windows systems.
    
    Features:
    - Checks for required prerequisites (CMake, Visual Studio)
    - Optionally installs vcpkg package manager
    - Installs required dependencies (GLFW, GLM)
    - Generates Visual Studio project files
    - Builds the engine in Release mode
    - Creates shortcuts for easy access
.NOTES
    Usage: Right-click and select "Run with PowerShell" or run from PowerShell console
#>

# Set error action preference to stop on errors
$ErrorActionPreference = "Stop"

# Enable verbose output
$VerbosePreference = "Continue"

#region Helper Functions

function Show-AnimatedBanner {
    param(
        [Parameter(Mandatory=$false)]
        [int]$ScrollSpeed = 100
    )
    
    Clear-Host
    
    # ASCII Art Banner for Fresh Voxel Engine
    $banner = @"
    
    ███████╗██████╗ ███████╗███████╗██╗  ██╗    ██╗   ██╗ ██████╗ ██╗  ██╗███████╗██╗         
    ██╔════╝██╔══██╗██╔════╝██╔════╝██║  ██║    ██║   ██║██╔═══██╗╚██╗██╔╝██╔════╝██║         
    █████╗  ██████╔╝█████╗  ███████╗███████║    ██║   ██║██║   ██║ ╚███╔╝ █████╗  ██║         
    ██╔══╝  ██╔══██╗██╔══╝  ╚════██║██╔══██║    ╚██╗ ██╔╝██║   ██║ ██╔██╗ ██╔══╝  ██║         
    ██║     ██║  ██║███████╗███████║██║  ██║     ╚████╔╝ ╚██████╔╝██╔╝ ██╗███████╗███████╗    
    ╚═╝     ╚═╝  ╚═╝╚══════╝╚══════╝╚═╝  ╚═╝      ╚═══╝   ╚═════╝ ╚═╝  ╚═╝╚══════╝╚══════╝    
                                                                                                 
    ███████╗███╗   ██╗ ██████╗ ███████╗███╗   ██╗███████╗                                      
    ██╔════╝████╗  ██║██╔════╝ ██╔════╝████╗  ██║██╔════╝                                      
    █████╗  ██╔██╗ ██║██║  ███╗█████╗  ██╔██╗ ██║█████╗                                        
    ██╔══╝  ██║╚██╗██║██║   ██║██╔══╝  ██║╚██╗██║██╔══╝                                        
    ███████╗██║ ╚████║╚██████╔╝███████╗██║ ╚████║███████╗                                      
    ╚══════╝╚═╝  ╚═══╝ ╚═════╝ ╚══════╝╚═╝  ╚═══╝╚══════╝                                      
                                                                                                 
"@

    $bannerLines = $banner -split "`n"
    
    # Animate banner appearing line by line
    foreach ($line in $bannerLines) {
        Write-Host $line -ForegroundColor Cyan
        Start-Sleep -Milliseconds 50
    }
    
    # Animated loading bar
    Write-Host ""
    Write-Host "    " -NoNewline
    $loadingChars = @('▰', '▰', '▰', '▰', '▰', '▰', '▰', '▰', '▰', '▰', '▰', '▰', '▰', '▰', '▰', '▰', '▰', '▰', '▰', '▰')
    for ($i = 0; $i -lt $loadingChars.Length; $i++) {
        Write-Host $loadingChars[$i] -ForegroundColor Green -NoNewline
        Start-Sleep -Milliseconds 30
    }
    Write-Host ""
    Write-Host ""
    
    # Futuristic/Cyberpunk Style Designer Credit with glitch effect borders
    $designerBanner = @"
    ▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓
    ▒▒                                                                         ▒▒
    ▒▒  ░▒▓█  I N S T A L L E R   S C R I P T   D E S I G N E D   B Y:  █▓▒░  ▒▒
    ▒▒                                                                         ▒▒
    ▒▒  ░▒▓█►                    S H I F T Y                    ◄█▓▒░         ▒▒
    ▒▒                                                                         ▒▒
    ▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓
"@

    foreach ($line in ($designerBanner -split "`n")) {
        Write-Host $line -ForegroundColor Magenta
        Start-Sleep -Milliseconds 30
    }
    
    Write-Host ""
    
    # Tagline with typing effect
    $tagline = "       [ A U T O M A T E D   I N S T A L L A T I O N   S Y S T E M ]"
    Write-Host "    " -NoNewline
    foreach ($char in $tagline.ToCharArray()) {
        Write-Host $char -ForegroundColor Yellow -NoNewline
        Start-Sleep -Milliseconds 20
    }
    Write-Host ""
    Write-Host ""
    
    # Version and info with futuristic borders
    Write-Host "    ▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓" -ForegroundColor DarkCyan
    Write-Host "                      Automated Windows Installation Script                      " -ForegroundColor White
    Write-Host "                           PowerShell Edition v2.0                               " -ForegroundColor Gray
    Write-Host "    ▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓" -ForegroundColor DarkCyan
    Write-Host ""
    
    Start-Sleep -Milliseconds 800
}

function Show-SectionBanner {
    param(
        [Parameter(Mandatory=$true)]
        [string]$Title,
        
        [Parameter(Mandatory=$false)]
        [string]$Icon = "►"
    )
    
    Write-Host ""
    Write-Host "    ╔═══════════════════════════════════════════════════════════════════════════╗" -ForegroundColor Cyan
    Write-Host "    ║  $Icon " -ForegroundColor Cyan -NoNewline
    Write-Host $Title.PadRight(72) -ForegroundColor White -NoNewline
    Write-Host "║" -ForegroundColor Cyan
    Write-Host "    ╚═══════════════════════════════════════════════════════════════════════════╝" -ForegroundColor Cyan
    Write-Host ""
}

function Write-ColorOutput {
    param(
        [Parameter(Mandatory=$true)]
        [string]$Message,
        
        [Parameter(Mandatory=$false)]
        [string]$ForegroundColor = "White",
        
        [Parameter(Mandatory=$false)]
        [switch]$NoNewline
    )
    
    if ($NoNewline) {
        Write-Host $Message -ForegroundColor $ForegroundColor -NoNewline
    } else {
        Write-Host $Message -ForegroundColor $ForegroundColor
    }
}

function Write-Step {
    param(
        [Parameter(Mandatory=$true)]
        [string]$Message
    )
    
    Write-ColorOutput "`n============================================================================" -ForegroundColor Cyan
    Write-ColorOutput $Message -ForegroundColor Cyan
    Write-ColorOutput "============================================================================`n" -ForegroundColor Cyan
}

function Write-Success {
    param(
        [Parameter(Mandatory=$true)]
        [string]$Message
    )
    
    Write-ColorOutput "✓ $Message" -ForegroundColor Green
}

function Write-Warning {
    param(
        [Parameter(Mandatory=$true)]
        [string]$Message
    )
    
    Write-ColorOutput "⚠ $Message" -ForegroundColor Yellow
}

function Write-ErrorMessage {
    param(
        [Parameter(Mandatory=$true)]
        [string]$Message
    )
    
    Write-ColorOutput "✗ ERROR: $Message" -ForegroundColor Red
}

function Write-LogAndConsole {
    param(
        [Parameter(Mandatory=$true)]
        [string]$Message,
        
        [Parameter(Mandatory=$false)]
        [string]$LogFile
    )
    
    Write-Host $Message
    if ($LogFile) {
        Add-Content -Path $LogFile -Value $Message
    }
}

function Invoke-CommandWithLogging {
    param(
        [Parameter(Mandatory=$true)]
        [scriptblock]$Command,
        
        [Parameter(Mandatory=$true)]
        [string]$ErrorMessage,
        
        [Parameter(Mandatory=$false)]
        [string]$LogFile
    )
    
    try {
        & $Command
        if ($LASTEXITCODE -ne 0 -and $null -ne $LASTEXITCODE) {
            throw "$ErrorMessage (Exit code: $LASTEXITCODE)"
        }
        return $true
    }
    catch {
        Write-ErrorMessage $ErrorMessage
        if ($LogFile) {
            Add-Content -Path $LogFile -Value "ERROR: $ErrorMessage"
            Add-Content -Path $LogFile -Value "Details: $_"
        }
        Write-Host "`nPress any key to exit..."
        $null = $Host.UI.RawUI.ReadKey("NoEcho,IncludeKeyDown")
        exit 1
    }
}

#endregion

#region Main Script

# Navigate to repository root (2 levels up from tools/build_tools)
$ScriptRoot = Split-Path -Parent $PSCommandPath
$RepoRoot = Split-Path -Parent (Split-Path -Parent $ScriptRoot)
Set-Location $RepoRoot

# Setup logging
$LogDir = Join-Path $RepoRoot "logs"
if (-not (Test-Path $LogDir)) {
    New-Item -ItemType Directory -Path $LogDir | Out-Null
}

$Timestamp = Get-Date -Format "yyyy-MM-dd_HH-mm-ss"
$LogFile = Join-Path $LogDir "install_log_$Timestamp.txt"

# Create log file with header
$LogHeader = @"
============================================================================
Fresh Voxel Engine - Installation Log
Started: $(Get-Date -Format "yyyy-MM-dd HH:mm:ss")
PowerShell Version: $($PSVersionTable.PSVersion)
============================================================================

"@
Set-Content -Path $LogFile -Value $LogHeader

# Display animated banner
Show-AnimatedBanner

# Display welcome message
Write-Host "    📁 Log file: " -ForegroundColor Gray -NoNewline
Write-Host $LogFile -ForegroundColor White
Write-Host ""
Add-Content -Path $LogFile -Value "Log file: $LogFile`n"

Write-Host "    This automated installation will:" -ForegroundColor White
Write-Host "      1. " -ForegroundColor DarkGray -NoNewline
Write-Host "Check for prerequisites (CMake, Visual Studio)" -ForegroundColor Gray
Write-Host "      2. " -ForegroundColor DarkGray -NoNewline
Write-Host "Install vcpkg package manager (optional)" -ForegroundColor Gray
Write-Host "      3. " -ForegroundColor DarkGray -NoNewline
Write-Host "Verify vcpkg manifest mode configuration" -ForegroundColor Gray
Write-Host "      4. " -ForegroundColor DarkGray -NoNewline
Write-Host "Generate Visual Studio project files" -ForegroundColor Gray
Write-Host "      5. " -ForegroundColor DarkGray -NoNewline
Write-Host "Build the engine (dependencies installed during CMake)" -ForegroundColor Gray
Write-Host ""

Add-Content -Path $LogFile -Value @"
This script will:
  1. Check for prerequisites (CMake, Visual Studio)
  2. Install vcpkg package manager (optional)
  3. Verify vcpkg manifest mode configuration
  4. Generate Visual Studio project files
  5. Build the engine (dependencies installed during CMake)

"@

Write-Host ""
Write-Host "    " -NoNewline
Write-ColorOutput "⚡ Press any key to begin installation or Ctrl+C to cancel..." -ForegroundColor Yellow
$null = $Host.UI.RawUI.ReadKey("NoEcho,IncludeKeyDown")
Write-Host ""

#region Step 1: Check Prerequisites
Show-SectionBanner -Title "STEP 1/6: Checking Prerequisites" -Icon "🔍"
Add-Content -Path $LogFile -Value "[Step 1/6] Checking prerequisites...`n"

# Check CMake
Write-Host "    Checking for CMake..." -ForegroundColor White
Add-Content -Path $LogFile -Value "Checking for CMake..."

$cmakeCmd = Get-Command cmake -ErrorAction SilentlyContinue
if (-not $cmakeCmd) {
    Write-ErrorMessage "CMake is not installed or not in PATH"
    Add-Content -Path $LogFile -Value "ERROR: CMake is not installed or not in PATH"
    Write-Host "`n    Please install CMake from: https://cmake.org/download/"
    Write-Host "    Make sure to add CMake to system PATH during installation`n"
    Write-Host "    Press any key to exit..."
    $null = $Host.UI.RawUI.ReadKey("NoEcho,IncludeKeyDown")
    exit 1
}

$cmakeVersion = & cmake --version 2>&1 | Select-Object -First 1
Write-Host "    " -NoNewline
Write-Success "CMake found: $cmakeVersion"
Add-Content -Path $LogFile -Value "CMake found: $cmakeVersion"

# Check Visual Studio 2022
Write-Host "`n    Checking for Visual Studio 2022..." -ForegroundColor White
Add-Content -Path $LogFile -Value "Checking for Visual Studio 2022..."

$VS2022Paths = @(
    "C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\IDE\devenv.exe",
    "C:\Program Files\Microsoft Visual Studio\2022\Professional\Common7\IDE\devenv.exe",
    "C:\Program Files\Microsoft Visual Studio\2022\Enterprise\Common7\IDE\devenv.exe"
)

$VS2022Found = $false
$VS2022Path = ""

foreach ($path in $VS2022Paths) {
    if (Test-Path $path) {
        $VS2022Found = $true
        $VS2022Path = Split-Path -Parent (Split-Path -Parent $path)
        break
    }
}

if ($VS2022Found) {
    Write-Host "    " -NoNewline
    Write-Success "Visual Studio 2022 found at: $VS2022Path"
    Add-Content -Path $LogFile -Value "Visual Studio 2022 found at: $VS2022Path"
} else {
    Write-Host "    " -NoNewline
    Write-Warning "Visual Studio 2022 not found in default locations"
    Add-Content -Path $LogFile -Value "WARNING: Visual Studio 2022 not found in default locations"
    Write-Host "    The installation will continue, but you may need VS 2022 to build"
    Write-Host "`n    Download from: https://visualstudio.microsoft.com/downloads/`n"
    Add-Content -Path $LogFile -Value "Download from: https://visualstudio.microsoft.com/downloads/"
    
    $continue = Read-Host "    Continue anyway? (Y/N)"
    if ($continue -ne "Y" -and $continue -ne "y") {
        Write-Host "    Installation cancelled."
        Add-Content -Path $LogFile -Value "Installation cancelled by user."
        exit 0
    }
}

Write-Host ""
Write-Host "    " -NoNewline
Write-Success "Prerequisites check completed!"
Add-Content -Path $LogFile -Value "`nPrerequisites check completed!`n"
Write-Host "`n    Press any key to continue..." -ForegroundColor Gray
$null = $Host.UI.RawUI.ReadKey("NoEcho,IncludeKeyDown")

#endregion

#region Step 2: vcpkg Setup
Show-SectionBanner -Title "STEP 2/6: Setting Up vcpkg Package Manager" -Icon "📦"
Add-Content -Path $LogFile -Value "[Step 2/6] Setting up vcpkg package manager...`n"

$VcpkgRoot = Join-Path $RepoRoot "vcpkg"
$VcpkgExe = Join-Path $VcpkgRoot "vcpkg.exe"

if (Test-Path $VcpkgExe) {
    Write-Host "    " -NoNewline
    Write-Success "vcpkg already installed at: $VcpkgRoot"
    Add-Content -Path $LogFile -Value "vcpkg already installed at: $VcpkgRoot"
} else {
    Write-Host "    ℹ️  vcpkg not found. Would you like to install it?" -ForegroundColor Cyan
    Write-Host "    vcpkg is a package manager for C++ that makes dependency management easier.`n" -ForegroundColor Gray
    Add-Content -Path $LogFile -Value "vcpkg not found.`n"
    
    $installVcpkg = Read-Host "    Install vcpkg? (Y/N)"
    
    if ($installVcpkg -eq "Y" -or $installVcpkg -eq "y") {
        Write-Host ""
        Write-Host "    📥 Installing vcpkg..." -ForegroundColor Yellow
        Add-Content -Path $LogFile -Value "Installing vcpkg..."
        
        # Clone vcpkg
        if (-not (Test-Path $VcpkgRoot)) {
            Write-Host "    Cloning vcpkg repository..." -ForegroundColor White
            Add-Content -Path $LogFile -Value "Cloning vcpkg repository..."
            Write-Host "    " -NoNewline
            Write-Warning "This will download vcpkg. Progress will be shown below."
            Write-Host ""
            
            try {
                git clone https://github.com/Microsoft/vcpkg.git $VcpkgRoot 2>&1 | ForEach-Object {
                    Write-Host "    $_" -ForegroundColor DarkGray
                    Add-Content -Path $LogFile -Value $_
                }
                
                if ($LASTEXITCODE -ne 0) {
                    throw "Git clone failed"
                }
            }
            catch {
                Write-Host "    " -NoNewline
                Write-ErrorMessage "Failed to clone vcpkg repository"
                Add-Content -Path $LogFile -Value "ERROR: Failed to clone vcpkg repository: $_"
                Write-Host "    Make sure git is installed and you have internet connection`n"
                Write-Host "    Press any key to exit..."
                $null = $Host.UI.RawUI.ReadKey("NoEcho,IncludeKeyDown")
                exit 1
            }
        }
        
        # Bootstrap vcpkg
        Write-Host "`nBootstrapping vcpkg..."
        Add-Content -Path $LogFile -Value "`nBootstrapping vcpkg..."
        Write-Warning "Downloading and bootstrapping vcpkg. Progress will be shown below.`n"
        
        Push-Location $VcpkgRoot
        try {
            $bootstrapScript = Join-Path $VcpkgRoot "bootstrap-vcpkg.bat"
            & $bootstrapScript 2>&1 | ForEach-Object {
                Write-Host $_
                Add-Content -Path $LogFile -Value $_
            }
            
            if ($LASTEXITCODE -ne 0) {
                throw "Bootstrap failed"
            }
        }
        catch {
            Write-ErrorMessage "Failed to bootstrap vcpkg"
            Add-Content -Path $LogFile -Value "ERROR: Failed to bootstrap vcpkg: $_"
            Pop-Location
            Write-Host "`nPress any key to exit..."
            $null = $Host.UI.RawUI.ReadKey("NoEcho,IncludeKeyDown")
            exit 1
        }
        finally {
            Pop-Location
        }
        
        # Integrate vcpkg with Visual Studio
        Write-Host "`nIntegrating vcpkg with Visual Studio..."
        Add-Content -Path $LogFile -Value "`nIntegrating vcpkg with Visual Studio..."
        
        try {
            & $VcpkgExe integrate install 2>&1 | ForEach-Object {
                Write-Host $_
                Add-Content -Path $LogFile -Value $_
            }
        }
        catch {
            Write-Warning "vcpkg integration had issues, but continuing..."
            Add-Content -Path $LogFile -Value "WARNING: vcpkg integration had issues: $_"
        }
        
        Write-Host ""
        Write-Success "vcpkg installed successfully!"
        Add-Content -Path $LogFile -Value "vcpkg installed successfully!"
    } else {
        Write-Host "Skipping vcpkg installation."
        Write-Host "You will need to install dependencies manually."
        Add-Content -Path $LogFile -Value "Skipping vcpkg installation."
        Add-Content -Path $LogFile -Value "You will need to install dependencies manually."
    }
}

Write-Host "`nPress any key to continue..."
$null = $Host.UI.RawUI.ReadKey("NoEcho,IncludeKeyDown")

#endregion

#region Step 3: Install Dependencies
try {
    Show-SectionBanner -Title "STEP 3/6: Configuring Dependencies" -Icon "🔧"
    Add-Content -Path $LogFile -Value "[Step 3/6] Installing dependencies...`n"
    
    Write-Host "Checking for vcpkg executable..." -ForegroundColor DarkGray
    Add-Content -Path $LogFile -Value "Checking for vcpkg executable..."
    
    if (Test-Path $VcpkgExe) {
        Write-Host "vcpkg found at: $VcpkgExe" -ForegroundColor Green
        Add-Content -Path $LogFile -Value "vcpkg found at: $VcpkgExe"
        
        Write-Host "This project uses vcpkg manifest mode (vcpkg.json)."
        Write-Host "Dependencies will be installed automatically during CMake configuration.`n"
        Add-Content -Path $LogFile -Value @"
This project uses vcpkg manifest mode (vcpkg.json).
Dependencies will be installed automatically during CMake configuration.

"@
        
        Write-Host "Dependencies defined in vcpkg.json:"
        Write-Host "  - GLFW3 (window management)"
        Write-Host "  - GLM (math library)"
        Write-Host "  - ImGui (editor UI)`n"
        
        Add-Content -Path $LogFile -Value @"
Dependencies defined in vcpkg.json:
  - GLFW3 (window management)
  - GLM (math library)
  - ImGui (editor UI)

"@
        
        Write-Success "vcpkg is configured for manifest mode"
        Write-Host "  Dependencies will be installed in the next step during CMake configuration.`n"
        Add-Content -Path $LogFile -Value "vcpkg is configured for manifest mode"
        Add-Content -Path $LogFile -Value "Dependencies will be installed in the next step during CMake configuration.`n"
    } else {
        Write-Warning "vcpkg not available at: $VcpkgExe"
        Write-Warning "Dependencies must be installed manually.`n"
        Add-Content -Path $LogFile -Value "vcpkg not available at: $VcpkgExe"
        Add-Content -Path $LogFile -Value "Dependencies must be installed manually.`n"
        
        Write-Host "The following libraries are required:"
        Write-Host "  - GLFW 3.3+"
        Write-Host "  - GLM"
        Write-Host "  - ImGui (optional, for editor UI)`n"
        
        Add-Content -Path $LogFile -Value @"
The following libraries are required:
  - GLFW 3.3+
  - GLM
  - ImGui (optional, for editor UI)

"@
        
        Write-Host "See $RepoRoot\DEVELOPER_SETUP.md for manual installation instructions.`n"
        Add-Content -Path $LogFile -Value "See DEVELOPER_SETUP.md for manual installation instructions.`n"
    }
}
catch {
    Write-ErrorMessage "Error in Step 3 (Installing Dependencies): $_"
    Add-Content -Path $LogFile -Value "ERROR in Step 3: $_"
    Add-Content -Path $LogFile -Value "Stack Trace: $($_.ScriptStackTrace)"
    Write-Host "`nPress any key to exit..."
    $null = $Host.UI.RawUI.ReadKey("NoEcho,IncludeKeyDown")
    exit 1
}

Write-Host "Press any key to continue..."
try {
    $null = $Host.UI.RawUI.ReadKey("NoEcho,IncludeKeyDown")
    Write-Host "Key pressed, continuing to next step..."
    Add-Content -Path $LogFile -Value "User pressed key, continuing to Step 4..."
}
catch {
    Write-ErrorMessage "Error reading key input: $_"
    Add-Content -Path $LogFile -Value "ERROR: Failed to read key input: $_"
    Write-Host "Continuing anyway...`n"
}

#endregion

#region Step 4: Generate Visual Studio Project Files
try {
    Show-SectionBanner -Title "STEP 4/6: Generating Visual Studio Project Files" -Icon "⚙️"
    Add-Content -Path $LogFile -Value "[Step 4/6] Generating Visual Studio project files...`n"
}
catch {
    Write-ErrorMessage "Error starting Step 4: $_"
    Add-Content -Path $LogFile -Value "ERROR: Failed to start Step 4: $_"
    Write-Host "`nPress any key to exit..."
    $null = $Host.UI.RawUI.ReadKey("NoEcho,IncludeKeyDown")
    exit 1
}

Write-Host "NOTE: This step will automatically install dependencies from vcpkg.json"
Write-Host "      if vcpkg is available. This may take several minutes on first run.`n"
Add-Content -Path $LogFile -Value @"
NOTE: This step will automatically install dependencies from vcpkg.json
      if vcpkg is available. This may take several minutes on first run.

"@

# Create build directory if it doesn't exist
$BuildDir = Join-Path $RepoRoot "build"
if (-not (Test-Path $BuildDir)) {
    New-Item -ItemType Directory -Path $BuildDir | Out-Null
}

Push-Location $BuildDir

Write-Host "Running CMake..."
Add-Content -Path $LogFile -Value "Running CMake..."

$VcpkgToolchain = Join-Path $VcpkgRoot "scripts\buildsystems\vcpkg.cmake"

try {
    if (Test-Path $VcpkgToolchain) {
        Write-Host "Using vcpkg toolchain file (manifest mode)..."
        Write-Host "Dependencies will be installed automatically from vcpkg.json...`n"
        Add-Content -Path $LogFile -Value "Using vcpkg toolchain file (manifest mode)..."
        Add-Content -Path $LogFile -Value "Dependencies will be installed automatically from vcpkg.json...`n"
        
        Write-Warning "NOTE: vcpkg will now install dependencies. This may take 10-15 minutes on first run."
        Write-Warning "      You will see download and build progress below. Please be patient.`n"
        
        & cmake -G "Visual Studio 17 2022" -A x64 "-DCMAKE_TOOLCHAIN_FILE=$VcpkgToolchain" .. 2>&1 | ForEach-Object {
            Write-Host $_
            Add-Content -Path $LogFile -Value $_
        }
    } else {
        Write-Host "Using system-installed dependencies..."
        Add-Content -Path $LogFile -Value "Using system-installed dependencies..."
        
        & cmake -G "Visual Studio 17 2022" -A x64 .. 2>&1 | ForEach-Object {
            Write-Host $_
            Add-Content -Path $LogFile -Value $_
        }
    }
    
    if ($LASTEXITCODE -ne 0) {
        throw "CMake generation failed with exit code $LASTEXITCODE"
    }
}
catch {
    Write-Host ""
    Write-ErrorMessage "CMake project generation failed"
    Add-Content -Path $LogFile -Value "ERROR: CMake project generation failed: $_"
    Write-Host "Please check the error messages above`n"
    Write-Host "Full CMake output has been saved to: $LogFile`n"
    Pop-Location
    Write-Host "Press any key to exit..."
    $null = $Host.UI.RawUI.ReadKey("NoEcho,IncludeKeyDown")
    exit 1
}
finally {
    Pop-Location
}

Write-Host ""
Write-Success "Visual Studio project files generated successfully!"
Write-Host "  Location: $BuildDir\FreshVoxelEngine.sln`n"
Add-Content -Path $LogFile -Value "Visual Studio project files generated successfully!"
Add-Content -Path $LogFile -Value "Location: $BuildDir\FreshVoxelEngine.sln`n"

Write-Host "Press any key to continue..."
$null = $Host.UI.RawUI.ReadKey("NoEcho,IncludeKeyDown")

#endregion

#region Step 5: Build the Project
Show-SectionBanner -Title "STEP 5/6: Building the Project" -Icon "🔨"
Add-Content -Path $LogFile -Value "[Step 5/6] Building the project...`n"

Write-Host "This may take several minutes depending on your system...`n"
Add-Content -Path $LogFile -Value "This may take several minutes depending on your system...`n"

Write-Host "Building Release configuration..."
Add-Content -Path $LogFile -Value "Building Release configuration...`n"
Write-Warning "Building... This may take several minutes. Progress will be shown below.`n"

try {
    & cmake --build $BuildDir --config Release 2>&1 | ForEach-Object {
        Write-Host $_
        Add-Content -Path $LogFile -Value $_
    }
    
    if ($LASTEXITCODE -ne 0) {
        throw "Build failed with exit code $LASTEXITCODE"
    }
}
catch {
    Write-Host ""
    Write-ErrorMessage "Build failed"
    Add-Content -Path $LogFile -Value "ERROR: Build failed: $_"
    Write-Host "Please check the error messages above`n"
    Write-Host "Full build output has been saved to: $LogFile`n"
    Write-Host "You can try building manually by opening:"
    Write-Host "  $BuildDir\FreshVoxelEngine.sln`n"
    Add-Content -Path $LogFile -Value "You can try building manually by opening:"
    Add-Content -Path $LogFile -Value "  $BuildDir\FreshVoxelEngine.sln`n"
    Write-Host "Press any key to exit..."
    $null = $Host.UI.RawUI.ReadKey("NoEcho,IncludeKeyDown")
    exit 1
}

Write-Host ""
Write-Success "Build completed successfully!`n"
Add-Content -Path $LogFile -Value "Build completed successfully!`n"

Write-Host "Press any key to continue..."
$null = $Host.UI.RawUI.ReadKey("NoEcho,IncludeKeyDown")

#endregion

#region Step 6: Create Shortcuts
Show-SectionBanner -Title "STEP 6/6: Creating Shortcuts" -Icon "🔗"
Add-Content -Path $LogFile -Value "[Step 6/6] Creating shortcuts...`n"

$createShortcuts = Read-Host "Would you like to create desktop shortcuts? (Y/N)"

if ($createShortcuts -eq "Y" -or $createShortcuts -eq "y") {
    Write-Host "Creating shortcuts..."
    Add-Content -Path $LogFile -Value "Creating shortcuts..."
    
    # Create batch file shortcut to open Visual Studio solution
    $openSolutionBat = Join-Path $RepoRoot "Open_Solution.bat"
    Set-Content -Path $openSolutionBat -Value @"
@echo off
start "" "build\FreshVoxelEngine.sln"
"@
    
    Write-Success "Created Open_Solution.bat"
    Add-Content -Path $LogFile -Value "Created Open_Solution.bat"
    
    # Create batch file shortcut to run the engine
    $engineExe = Join-Path $BuildDir "Release\FreshVoxelEngine.exe"
    if (Test-Path $engineExe) {
        $runEngineBat = Join-Path $RepoRoot "Run_Engine.bat"
        Set-Content -Path $runEngineBat -Value @"
@echo off
cd /d "$RepoRoot"
start "" "build\Release\FreshVoxelEngine.exe"
"@
        Write-Success "Created Run_Engine.bat"
        Add-Content -Path $LogFile -Value "Created Run_Engine.bat"
    }
}

Write-Host ""

#endregion

#region Installation Complete

# Show completion banner
Write-Host ""
Write-Host ""
Write-Host "    ╔═══════════════════════════════════════════════════════════════════════════╗" -ForegroundColor Green
Write-Host "    ║                                                                           ║" -ForegroundColor Green
Write-Host "    ║                  " -ForegroundColor Green -NoNewline
Write-Host "✅ INSTALLATION COMPLETED SUCCESSFULLY! ✅" -ForegroundColor White -NoNewline
Write-Host "                  ║" -ForegroundColor Green
Write-Host "    ║                                                                           ║" -ForegroundColor Green
Write-Host "    ╚═══════════════════════════════════════════════════════════════════════════╝" -ForegroundColor Green
Write-Host ""

$completionTime = Get-Date -Format "yyyy-MM-dd HH:mm:ss"
Write-Host "    🎉 Installation finished: " -ForegroundColor Cyan -NoNewline
Write-Host $completionTime -ForegroundColor White
Write-Host ""

$completionMessage = @"

Installation finished: $completionTime

Next steps:

  1. Open the solution:
     - Double-click: Open_Solution.bat
     - Or manually: build\FreshVoxelEngine.sln

  2. Run the engine:
"@

Add-Content -Path $LogFile -Value $completionMessage

Write-Host "    " -NoNewline
Write-ColorOutput "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━ NEXT STEPS ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━" -ForegroundColor Cyan
Write-Host ""
Write-Host "    📂 1. Open the solution:" -ForegroundColor Yellow
Write-Host "       • Double-click: " -ForegroundColor Gray -NoNewline
Write-Host "Open_Solution.bat" -ForegroundColor White
Write-Host "       • Or manually: " -ForegroundColor Gray -NoNewline
Write-Host "build\FreshVoxelEngine.sln" -ForegroundColor White
Write-Host ""
Write-Host "    🎮 2. Run the engine:" -ForegroundColor Yellow

$engineExe = Join-Path $BuildDir "Release\FreshVoxelEngine.exe"
if (Test-Path $engineExe) {
    $runMessage = @"
     - Double-click: Run_Engine.bat
     - Or manually: build\Release\FreshVoxelEngine.exe
"@
    Write-Host "       • Double-click: " -ForegroundColor Gray -NoNewline
    Write-Host "Run_Engine.bat" -ForegroundColor White
    Write-Host "       • Or manually: " -ForegroundColor Gray -NoNewline
    Write-Host "build\Release\FreshVoxelEngine.exe" -ForegroundColor White
    Add-Content -Path $LogFile -Value $runMessage
} else {
    $buildMessage = @"
     - Build the project first in Visual Studio
     - Then run: build\Release\FreshVoxelEngine.exe
"@
    Write-Host "       • Build the project first in Visual Studio" -ForegroundColor Gray
    Write-Host "       • Then run: " -ForegroundColor Gray -NoNewline
    Write-Host "build\Release\FreshVoxelEngine.exe" -ForegroundColor White
    Add-Content -Path $LogFile -Value $buildMessage
}

$finalMessage = @"

  3. Learn more:
     - README.md - Project overview
     - GETTING_STARTED.md - Usage guide
     - DEVELOPER_SETUP.md - Development setup
     - docs/EDITOR_INTEGRATION.md - Editor guide

"@

Write-Host ""
Write-Host "    📚 3. Learn more:" -ForegroundColor Yellow
Write-Host "       • README.md - Project overview" -ForegroundColor Gray
Write-Host "       • GETTING_STARTED.md - Usage guide" -ForegroundColor Gray
Write-Host "       • DEVELOPER_SETUP.md - Development setup" -ForegroundColor Gray
Write-Host "       • docs/EDITOR_INTEGRATION.md - Editor guide" -ForegroundColor Gray
Write-Host ""

Write-Host $finalMessage
Add-Content -Path $LogFile -Value $finalMessage

Write-Host ""
Write-Host "    ▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓" -ForegroundColor DarkCyan
Write-Host ""
Write-Host "    " -NoNewline
Write-ColorOutput "🚀 Thank you for installing Fresh Voxel Engine! 🚀" -ForegroundColor Cyan
Write-Host ""
Write-Host "    📝 Complete installation log saved to:" -ForegroundColor Gray
Write-Host "       $LogFile" -ForegroundColor White
Write-Host ""
Write-Host "    ▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓" -ForegroundColor DarkCyan
Write-Host "          ░▒▓█► I N S T A L L E R   D E S I G N E D   B Y:  S H I F T Y ◄█▓▒░          " -ForegroundColor Magenta
Write-Host "    ▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓" -ForegroundColor DarkCyan
Add-Content -Path $LogFile -Value "Complete installation log saved to: $LogFile"

Write-Host ""
Write-Host "    Press any key to exit..." -ForegroundColor Gray
$null = $Host.UI.RawUI.ReadKey("NoEcho,IncludeKeyDown")

#endregion

#endregion
