# Fresh Voxel Engine - Logging System

## Overview

The Fresh Voxel Engine includes a comprehensive logging system that captures all application events, errors, and debugging information. This system is designed to help developers understand application behavior and diagnose issues effectively.

## Log Directory Structure

```
logs/
├── .gitkeep                           # Preserves directory structure in git
├── application_errors_<timestamp>.txt # General application logs (all levels)
└── Environment/                       # Platform-specific error logs
    ├── Windows/
    │   └── errors_<timestamp>.txt     # Windows-specific error logs
    ├── Linux/
    │   └── errors_<timestamp>.txt     # Linux-specific error logs
    ├── macOS/
    │   └── errors_<timestamp>.txt     # macOS-specific error logs
    └── Other/
        └── errors_<timestamp>.txt     # Other platform error logs
```

## Log Files

### Application Logs
- **Location**: `logs/application_errors_<timestamp>.txt`
- **Content**: All log messages (INFO, WARNING, ERROR, FATAL)
- **Purpose**: Complete application activity tracking
- **Format**: `[timestamp][level][component] message`

### Environment Logs
- **Location**: `logs/Environment/<Platform>/errors_<timestamp>.txt`
- **Content**: Only ERROR and FATAL messages
- **Purpose**: Platform-specific error tracking for debugging
- **Format**: `[timestamp][level][component] message`

## Timestamp Format

Log files use the following timestamp format: `YYYY-MM-DD_HH-MM-SS.mmm`

Example: `application_errors_2025-11-03_21-03-23.467.txt`

## Using the Logger in C++

### Basic Usage

```cpp
#include "core/Logger.h"

int main() {
    // Initialize logger (call once at application startup)
    fresh::Logger::getInstance().initialize();
    
    // Log messages using convenience macros
    LOG_INFO("Application started");
    LOG_WARNING("Low memory warning");
    LOG_ERROR("Failed to load texture");
    LOG_FATAL("Critical system failure");
    
    // Log with component name
    LOG_INFO_C("Renderer initialized", "Renderer");
    LOG_ERROR_C("Failed to create window", "Window");
    
    // Shutdown logger (call once at application shutdown)
    fresh::Logger::getInstance().shutdown();
    
    return 0;
}
```

### Advanced Usage

```cpp
#include "core/Logger.h"

// Direct method calls
fresh::Logger& logger = fresh::Logger::getInstance();

// Initialize with custom log directory
logger.initialize("custom_logs");

// Control console output
logger.setConsoleOutput(false);  // Disable console output
logger.setConsoleOutput(true);   // Enable console output (default)

// Log with specific methods
logger.info("Info message", "Component");
logger.warning("Warning message", "Component");
logger.error("Error message", "Component");
logger.fatal("Fatal message", "Component");
```

## Log Levels

### INFO
- **Purpose**: General information about application state and progress
- **Logged to**: Application log only
- **Examples**: "Engine initialized", "World loaded successfully"

### WARNING
- **Purpose**: Potentially problematic situations that don't prevent operation
- **Logged to**: Application log only
- **Examples**: "Low memory", "Using fallback shader"

### ERROR
- **Purpose**: Error conditions that affect functionality but allow continuation
- **Logged to**: Application log AND Environment log
- **Examples**: "Failed to load texture", "Network connection lost"

### FATAL
- **Purpose**: Critical errors that require immediate attention or termination
- **Logged to**: Application log AND Environment log
- **Examples**: "Out of memory", "Critical system component failure"

## Batch File Error Logging

All build batch files automatically log errors to the `logs/` directory:

- `build.bat` → `logs/build_errors_<timestamp>.txt`
- `install.bat` → `logs/install_errors_<timestamp>.txt`
- `rebuild.bat` → `logs/rebuild_errors_<timestamp>.txt`
- `clean.bat` → `logs/clean_errors_<timestamp>.txt`

These logs capture:
- Build errors and warnings
- CMake configuration issues
- Dependency installation problems
- General script execution errors

## Thread Safety

The logging system is fully thread-safe and can be used from multiple threads simultaneously. All file operations are protected by mutexes.

## Performance Considerations

- Log files are flushed after each write to ensure data persistence in case of crashes
- File I/O is minimized through buffering
- Console output can be disabled for production builds to improve performance

## Uploading Logs for Development

The logging system is designed to produce human-readable, uploadable log files that can help steer development:

1. **Reproduce the issue** while the application is running
2. **Locate the log files** in the `logs/` directory
3. **Upload the relevant logs** to issue trackers or share with developers
4. **Include both application and environment logs** for complete context

The timestamped filenames make it easy to identify which logs correspond to specific test runs or issues.

## Best Practices

### DO:
- ✅ Initialize the logger at application startup
- ✅ Use appropriate log levels (don't log everything as ERROR)
- ✅ Include component names for better traceability
- ✅ Log both successes and failures for complete context
- ✅ Shutdown the logger gracefully before exit

### DON'T:
- ❌ Log in tight loops (can create massive log files)
- ❌ Log sensitive information (passwords, API keys, personal data)
- ❌ Use ERROR level for non-error situations
- ❌ Forget to initialize the logger (logs will go to stderr only)

## Example Log Output

```
=== Fresh Voxel Engine - Log Started at 2025-11-03_21-03-23.467 ===
Platform: Linux
========================================

[2025-11-03_21-03-23.467][INFO][Logger] Logger initialized successfully
[2025-11-03_21-03-23.500][INFO][Engine] Initializing Fresh Voxel Engine...
[2025-11-03_21-03-23.502][INFO][Window] Window created
[2025-11-03_21-03-23.550][INFO][Renderer] Renderer initialized with OpenGL
[2025-11-03_21-03-23.600][ERROR][Texture] Failed to load texture: missing_file.png
[2025-11-03_21-03-23.650][WARNING][Memory] Memory usage high: 85%
[2025-11-03_21-03-24.000][INFO][Engine] Shutting down...

========================================
=== Log Ended at 2025-11-03_21-03-24.100 ===
```

## Troubleshooting

### Logs not being created
- Ensure the `logs/` directory exists (it should be created automatically)
- Check file permissions in the logs directory
- Verify the logger is initialized before logging

### Console output not appearing
- Check if console output is enabled: `logger.setConsoleOutput(true);`
- Verify your terminal/console is capturing stdout/stderr

### Log files growing too large
- Review your logging frequency
- Consider implementing log rotation (not currently built-in)
- Disable verbose INFO logging in production builds

## Future Enhancements

Planned improvements to the logging system:
- Log rotation and file size limits
- Configurable log levels per component
- Remote logging capabilities
- Log file compression for archived logs
- Performance profiling integration
