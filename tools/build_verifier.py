#!/usr/bin/env python3
"""
Fresh Voxel Engine - Build Verification Tool

Verifies that all required dependencies and build tools are available.
Helps diagnose build environment issues before attempting to build.
"""

import os
import sys
import subprocess
from pathlib import Path


class BuildVerifier:
    """Verifies build environment for Fresh Voxel Engine."""
    
    def __init__(self):
        """Initialize the build verifier."""
        self.checks = []
        self.warnings = []
        self.errors = []
    
    def check_python_version(self):
        """Check Python version."""
        version = sys.version_info
        if version.major >= 3 and version.minor >= 8:
            self.checks.append(("Python version", f"{version.major}.{version.minor}.{version.micro}", True))
            return True
        else:
            self.checks.append(("Python version", f"{version.major}.{version.minor}.{version.micro}", False))
            self.errors.append("Python 3.8+ required")
            return False
    
    def check_cmake(self):
        """Check CMake installation."""
        try:
            result = subprocess.run(['cmake', '--version'], 
                                  capture_output=True, text=True, timeout=5)
            if result.returncode == 0:
                version = result.stdout.split('\n')[0].split()[-1]
                self.checks.append(("CMake", version, True))
                return True
        except (FileNotFoundError, subprocess.TimeoutExpired):
            pass
        
        self.checks.append(("CMake", "Not found", False))
        self.errors.append("CMake 3.20+ required")
        return False
    
    def check_git(self):
        """Check Git installation."""
        try:
            result = subprocess.run(['git', '--version'], 
                                  capture_output=True, text=True, timeout=5)
            if result.returncode == 0:
                version = result.stdout.split()[-1].strip()
                self.checks.append(("Git", version, True))
                return True
        except (FileNotFoundError, subprocess.TimeoutExpired):
            pass
        
        self.checks.append(("Git", "Not found", False))
        self.errors.append("Git required for vcpkg dependency management")
        return False
    
    def check_dotnet(self):
        """Check .NET SDK installation."""
        try:
            result = subprocess.run(['dotnet', '--version'], 
                                  capture_output=True, text=True, timeout=5)
            if result.returncode == 0:
                version = result.stdout.strip()
                self.checks.append((".NET SDK", version, True))
                if not version.startswith('9.'):
                    self.warnings.append(".NET 9 recommended for best compatibility")
                return True
        except (FileNotFoundError, subprocess.TimeoutExpired):
            pass
        
        self.checks.append((".NET SDK", "Not found", False))
        self.warnings.append(".NET 9 SDK recommended for C# bindings (optional)")
        return False
    
    def check_vs2022(self):
        """Check for Visual Studio 2022."""
        vs_paths = [
            r"C:\Program Files\Microsoft Visual Studio\2022\Community",
            r"C:\Program Files\Microsoft Visual Studio\2022\Professional",
            r"C:\Program Files\Microsoft Visual Studio\2022\Enterprise",
        ]
        
        for vs_path in vs_paths:
            if Path(vs_path).exists():
                self.checks.append(("Visual Studio 2022", vs_path.split('\\')[-1], True))
                return True
        
        self.checks.append(("Visual Studio 2022", "Not found", False))
        self.errors.append("Visual Studio 2022 with C++ workload required")
        return False
    
    def check_vcpkg(self):
        """Check for vcpkg."""
        # Check parent directory
        vcpkg_path = Path("../vcpkg")
        if vcpkg_path.exists():
            self.checks.append(("vcpkg", "Found in ../vcpkg", True))
            return True
        
        self.checks.append(("vcpkg", "Not found", False))
        self.warnings.append("vcpkg not found - run: git clone https://github.com/microsoft/vcpkg.git")
        return False
    
    def verify(self):
        """Run all verification checks."""
        print("Fresh Voxel Engine - Build Environment Verification")
        print("=" * 60)
        print()
        
        # Run all checks
        self.check_python_version()
        self.check_cmake()
        self.check_git()
        self.check_dotnet()
        self.check_vs2022()
        self.check_vcpkg()
        
        # Display results
        print("\nBuild Environment Checks:")
        print("-" * 60)
        for name, value, success in self.checks:
            status = "✓" if success else "✗"
            print(f"  {status} {name:<25} {value}")
        
        # Display warnings
        if self.warnings:
            print("\nWarnings:")
            print("-" * 60)
            for warning in self.warnings:
                print(f"  ⚠ {warning}")
        
        # Display errors
        if self.errors:
            print("\nErrors:")
            print("-" * 60)
            for error in self.errors:
                print(f"  ✗ {error}")
            print("\nPlease fix the above errors before building.")
            return False
        
        print("\n" + "=" * 60)
        print("Build environment verified successfully!")
        print("You can proceed with building the engine.")
        return True


def main():
    """Main entry point for the build verifier."""
    verifier = BuildVerifier()
    
    if verifier.verify():
        print("\nNext steps:")
        print("  1. Run: generate_vs2022.bat")
        print("  2. Open: build/FreshVoxelEngine.sln")
        print("  3. Build in Visual Studio (F7)")
        print("\nOr see BUILD.md for detailed instructions.")
        return 0
    else:
        print("\nSee BUILD.md for installation instructions.")
        return 1


if __name__ == '__main__':
    sys.exit(main())
