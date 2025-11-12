#!/usr/bin/env python3
"""
Asset Validator - Validates Fresh Engine asset packs and directories
"""

import os
import json
import sys
from pathlib import Path

# Try to import Fresh Engine bindings if available
try:
    import fresh_engine as fe
    BINDINGS_AVAILABLE = True
except ImportError:
    BINDINGS_AVAILABLE = False
    print("Warning: Fresh Engine Python bindings not available")
    print("Running in standalone mode")

class AssetValidator:
    """Validates asset files and directories"""
    
    SUPPORTED_TEXTURES = {'.png', '.jpg', '.jpeg', '.tga', '.bmp', '.dds'}
    SUPPORTED_MODELS = {'.obj', '.fbx', '.gltf', '.glb'}
    SUPPORTED_AUDIO = {'.wav', '.ogg', '.mp3'}
    
    def __init__(self):
        self.errors = []
        self.warnings = []
        self.stats = {
            'textures': 0,
            'models': 0,
            'audio': 0,
            'scripts': 0,
            'total': 0
        }
    
    def validate_directory(self, directory):
        """Validate all assets in a directory"""
        print(f"Validating assets in: {directory}")
        
        if not os.path.exists(directory):
            self.errors.append(f"Directory does not exist: {directory}")
            return False
        
        # Walk through directory
        for root, dirs, files in os.walk(directory):
            for file in files:
                filepath = os.path.join(root, file)
                self.validate_file(filepath)
        
        return len(self.errors) == 0
    
    def validate_file(self, filepath):
        """Validate a single asset file"""
        ext = Path(filepath).suffix.lower()
        
        # Check file exists and is readable
        if not os.path.isfile(filepath):
            self.errors.append(f"File not found: {filepath}")
            return False
        
        if not os.access(filepath, os.R_OK):
            self.errors.append(f"File not readable: {filepath}")
            return False
        
        # Validate by type
        if ext in self.SUPPORTED_TEXTURES:
            self.validate_texture(filepath)
            self.stats['textures'] += 1
        elif ext in self.SUPPORTED_MODELS:
            self.validate_model(filepath)
            self.stats['models'] += 1
        elif ext in self.SUPPORTED_AUDIO:
            self.validate_audio(filepath)
            self.stats['audio'] += 1
        elif ext == '.lua':
            self.validate_script(filepath)
            self.stats['scripts'] += 1
        
        self.stats['total'] += 1
        return True
    
    def validate_texture(self, filepath):
        """Validate texture file"""
        # Check file size
        size = os.path.getsize(filepath)
        if size > 10 * 1024 * 1024:  # 10 MB
            self.warnings.append(f"Large texture file ({size // 1024} KB): {filepath}")
        
        if size == 0:
            self.errors.append(f"Empty texture file: {filepath}")
        
        # Use Fresh Engine bindings if available
        if BINDINGS_AVAILABLE:
            try:
                fe.AssetProcessing.load_texture(filepath)
            except Exception as e:
                self.errors.append(f"Failed to load texture: {filepath} - {str(e)}")
    
    def validate_model(self, filepath):
        """Validate model file"""
        size = os.path.getsize(filepath)
        if size == 0:
            self.errors.append(f"Empty model file: {filepath}")
    
    def validate_audio(self, filepath):
        """Validate audio file"""
        size = os.path.getsize(filepath)
        if size == 0:
            self.errors.append(f"Empty audio file: {filepath}")
        elif size > 50 * 1024 * 1024:  # 50 MB
            self.warnings.append(f"Large audio file ({size // 1024 // 1024} MB): {filepath}")
    
    def validate_script(self, filepath):
        """Validate Lua script"""
        try:
            with open(filepath, 'r', encoding='utf-8') as f:
                content = f.read()
                if not content.strip():
                    self.errors.append(f"Empty script file: {filepath}")
        except UnicodeDecodeError:
            self.errors.append(f"Invalid UTF-8 encoding: {filepath}")
        except Exception as e:
            self.errors.append(f"Failed to read script: {filepath} - {str(e)}")
    
    def print_report(self):
        """Print validation report"""
        print("\n" + "="*60)
        print("ASSET VALIDATION REPORT")
        print("="*60)
        
        print("\nStatistics:")
        print(f"  Total files: {self.stats['total']}")
        print(f"  Textures: {self.stats['textures']}")
        print(f"  Models: {self.stats['models']}")
        print(f"  Audio: {self.stats['audio']}")
        print(f"  Scripts: {self.stats['scripts']}")
        
        if self.warnings:
            print(f"\nWarnings ({len(self.warnings)}):")
            for warning in self.warnings:
                print(f"  ⚠ {warning}")
        
        if self.errors:
            print(f"\nErrors ({len(self.errors)}):")
            for error in self.errors:
                print(f"  ✗ {error}")
        else:
            print("\n✓ All assets validated successfully!")
        
        print("="*60)

def main():
    if len(sys.argv) < 2:
        print("Usage: asset_validator.py <directory>")
        sys.exit(1)
    
    directory = sys.argv[1]
    validator = AssetValidator()
    
    success = validator.validate_directory(directory)
    validator.print_report()
    
    sys.exit(0 if success else 1)

if __name__ == "__main__":
    main()
