#!/usr/bin/env python3
"""
Generate test WAV files for the Fresh Voxel Engine audio system.
Creates simple sine wave test tones in WAV format.
"""

import wave
import struct
import math
import sys

def generate_sine_wave(frequency, duration, sample_rate=44100, amplitude=0.5):
    """Generate a sine wave as PCM data."""
    num_samples = int(duration * sample_rate)
    samples = []
    
    for i in range(num_samples):
        t = float(i) / sample_rate
        sample = amplitude * math.sin(2 * math.pi * frequency * t)
        # Convert to 16-bit PCM
        sample_int = int(sample * 32767)
        samples.append(sample_int)
    
    return samples

def write_wav_file(filename, samples, sample_rate=44100, num_channels=1):
    """Write samples to a WAV file."""
    with wave.open(filename, 'w') as wav_file:
        # Set parameters: channels, sample width (bytes), sample rate, frames, compression type, compression name
        wav_file.setnchannels(num_channels)
        wav_file.setsampwidth(2)  # 16-bit = 2 bytes
        wav_file.setframerate(sample_rate)
        wav_file.setnframes(len(samples))
        wav_file.setcomptype('NONE', 'not compressed')
        
        # Write the samples
        for sample in samples:
            wav_file.writeframes(struct.pack('<h', sample))

def main():
    print("Generating test WAV files...")
    
    # Generate test tone (440 Hz A note, 1 second)
    print("  Creating test_tone.wav (440 Hz, 1 sec)...")
    samples = generate_sine_wave(440, 1.0)
    write_wav_file('sounds/test_tone.wav', samples)
    
    # Generate short beep (880 Hz, 0.2 seconds) - could be used for UI sounds
    print("  Creating beep.wav (880 Hz, 0.2 sec)...")
    samples = generate_sine_wave(880, 0.2)
    write_wav_file('sounds/beep.wav', samples)
    
    # Generate low thud (220 Hz, 0.3 seconds) - could simulate block placement
    print("  Creating thud.wav (220 Hz, 0.3 sec)...")
    samples = generate_sine_wave(220, 0.3)
    write_wav_file('sounds/thud.wav', samples)
    
    print("WAV files generated successfully!")
    print("  sounds/test_tone.wav")
    print("  sounds/beep.wav")
    print("  sounds/thud.wav")

if __name__ == '__main__':
    main()
