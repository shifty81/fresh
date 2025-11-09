#!/usr/bin/env python3
"""
Fresh Voxel Engine - Texture Atlas Generator

Generates texture atlases from individual block texture files.
Helps optimize rendering by combining multiple textures into a single atlas.
"""

import os
import sys
from pathlib import Path
from PIL import Image


class TextureAtlasGenerator:
    """Generates texture atlases for the voxel engine."""
    
    def __init__(self, input_dir, output_file, tile_size=16, atlas_size=4096):
        """
        Initialize the atlas generator.
        
        Args:
            input_dir: Directory containing input texture files
            output_file: Output atlas filename
            tile_size: Size of each texture tile (default 16x16)
            atlas_size: Size of the output atlas (default 4096x4096)
        """
        self.input_dir = Path(input_dir)
        self.output_file = Path(output_file)
        self.tile_size = tile_size
        self.atlas_size = atlas_size
        self.tiles_per_row = atlas_size // tile_size
    
    def generate(self):
        """Generate the texture atlas from input files."""
        print(f"Generating texture atlas...")
        print(f"  Input directory: {self.input_dir}")
        print(f"  Output file: {self.output_file}")
        print(f"  Tile size: {self.tile_size}x{self.tile_size}")
        print(f"  Atlas size: {self.atlas_size}x{self.atlas_size}")
        
        if not self.input_dir.exists():
            print(f"Error: Input directory not found: {self.input_dir}")
            return False
        
        # Find all PNG files in input directory
        texture_files = sorted(list(self.input_dir.glob("*.png")))
        if not texture_files:
            print(f"Error: No PNG files found in {self.input_dir}")
            return False
        
        print(f"\nFound {len(texture_files)} texture files")
        max_tiles = self.tiles_per_row * self.tiles_per_row
        
        if len(texture_files) > max_tiles:
            print(f"Warning: Too many textures ({len(texture_files)}), maximum is {max_tiles}")
            texture_files = texture_files[:max_tiles]
        
        # Create atlas image
        try:
            atlas = Image.new('RGBA', (self.atlas_size, self.atlas_size), (0, 0, 0, 0))
            
            for idx, texture_file in enumerate(texture_files):
                row = idx // self.tiles_per_row
                col = idx % self.tiles_per_row
                x = col * self.tile_size
                y = row * self.tile_size
                
                # Load and resize texture
                texture = Image.open(texture_file)
                if texture.size != (self.tile_size, self.tile_size):
                    texture = texture.resize((self.tile_size, self.tile_size), Image.LANCZOS)
                
                # Paste into atlas
                atlas.paste(texture, (x, y))
                print(f"  [{idx+1}/{len(texture_files)}] {texture_file.name} -> ({x}, {y})")
            
            # Save atlas
            self.output_file.parent.mkdir(parents=True, exist_ok=True)
            atlas.save(self.output_file)
            print(f"\nAtlas saved: {self.output_file}")
            
            # Generate metadata file
            metadata_file = self.output_file.with_suffix('.txt')
            self._generate_metadata(metadata_file, texture_files)
            print(f"Metadata saved: {metadata_file}")
            
            return True
            
        except Exception as e:
            print(f"Error generating atlas: {e}")
            return False
    
    def _generate_metadata(self, metadata_file, texture_files):
        """Generate metadata file with texture coordinates."""
        with open(metadata_file, 'w') as f:
            f.write("# Fresh Voxel Engine Texture Atlas Metadata\n")
            f.write(f"# Atlas size: {self.atlas_size}x{self.atlas_size}\n")
            f.write(f"# Tile size: {self.tile_size}x{self.tile_size}\n")
            f.write(f"# Total textures: {len(texture_files)}\n\n")
            
            for idx, texture_file in enumerate(texture_files):
                row = idx // self.tiles_per_row
                col = idx % self.tiles_per_row
                u = col / self.tiles_per_row
                v = row / self.tiles_per_row
                
                f.write(f"{texture_file.stem},{idx},{u:.6f},{v:.6f}\n")


def main():
    """Main entry point for the texture atlas generator."""
    print("Fresh Voxel Engine - Texture Atlas Generator")
    print("=" * 60)
    
    # Check for PIL/Pillow
    try:
        import PIL
    except ImportError:
        print("\nError: Pillow library not found!")
        print("Install with: pip install Pillow")
        return 1
    
    if len(sys.argv) < 3:
        print("\nUsage: python texture_atlas_generator.py <input_dir> <output_file>")
        print("\nExample:")
        print("  python texture_atlas_generator.py textures/blocks textures/atlas.png")
        print("\nOptional arguments:")
        print("  --tile-size <size>    Size of each tile (default: 16)")
        print("  --atlas-size <size>   Size of output atlas (default: 4096)")
        return 1
    
    input_dir = sys.argv[1]
    output_file = sys.argv[2]
    tile_size = 16
    atlas_size = 4096
    
    # Parse optional arguments
    for i in range(3, len(sys.argv), 2):
        if sys.argv[i] == '--tile-size' and i+1 < len(sys.argv):
            tile_size = int(sys.argv[i+1])
        elif sys.argv[i] == '--atlas-size' and i+1 < len(sys.argv):
            atlas_size = int(sys.argv[i+1])
    
    generator = TextureAtlasGenerator(input_dir, output_file, tile_size, atlas_size)
    
    if generator.generate():
        print("\nTexture atlas generated successfully!")
        return 0
    else:
        print("\nFailed to generate texture atlas.")
        return 1


if __name__ == '__main__':
    sys.exit(main())
