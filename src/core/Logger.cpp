#include "core/Logger.h"
#include <iostream>
#include <ctime>
#include <sys/stat.h>
#include <sys/types.h>

#ifdef _WIN32
    #include <direct.h>
    #define MKDIR(path) _mkdir(path)
    #define PATH_SEPARATOR "\\"
#else
    #include <unistd.h>
    #define MKDIR(path) mkdir(path, 0755)
    #define PATH_SEPARATOR "/"
#endif

namespace fresh {

Logger::Logger()
    : m_initialized(false)
    , m_consoleOutput(true)
{
}

Logger::~Logger() {
    shutdown();
}

Logger& Logger::getInstance() {
    static Logger instance;
    return instance;
}

bool Logger::initialize(const std::string& logDir) {
    std::lock_guard<std::mutex> lock(m_mutex);
    
    if (m_initialized) {
        return true;
    }
    
    m_logDirectory = logDir;
    
    // Ensure base log directory exists
    ensureDirectoryExists(m_logDirectory);
    
    // Ensure Environment directory exists
    std::string envDir = m_logDirectory + PATH_SEPARATOR + "Environment";
    ensureDirectoryExists(envDir);
    
    // Ensure platform-specific directory exists
    std::string platformDir = envDir + PATH_SEPARATOR + getPlatformName();
    ensureDirectoryExists(platformDir);
    
    // Create timestamped log files
    std::string timestamp = getTimestamp();
    
    // Application log path
    m_applicationLogPath = m_logDirectory + PATH_SEPARATOR + "application_errors_" + timestamp + ".txt";
    
    // Environment-specific log path
    m_environmentLogPath = platformDir + PATH_SEPARATOR + "errors_" + timestamp + ".txt";
    
    // Open log files
    m_applicationLog.open(m_applicationLogPath, std::ios::out | std::ios::app);
    m_environmentLog.open(m_environmentLogPath, std::ios::out | std::ios::app);
    
    if (!m_applicationLog.is_open() || !m_environmentLog.is_open()) {
        std::cerr << "Failed to open log files" << std::endl;
        return false;
    }
    
    // Write header to log files
    std::string header = "=== Fresh Voxel Engine - Log Started at " + timestamp + " ===\n";
    header += "Platform: " + getPlatformName() + "\n";
    header += "========================================\n\n";
    
    writeToFile(m_applicationLog, header);
    writeToFile(m_environmentLog, header);
    
    m_initialized = true;
    
    info("Logger initialized successfully", "Logger");
    
    return true;
}

void Logger::shutdown() {
    std::lock_guard<std::mutex> lock(m_mutex);
    
    if (!m_initialized) {
        return;
    }
    
    std::string footer = "\n========================================\n";
    footer += "=== Log Ended at " + getTimestamp() + " ===\n";
    
    writeToFile(m_applicationLog, footer);
    writeToFile(m_environmentLog, footer);
    
    if (m_applicationLog.is_open()) {
        m_applicationLog.close();
    }
    
    if (m_environmentLog.is_open()) {
        m_environmentLog.close();
    }
    
    m_initialized = false;
}

void Logger::log(LogLevel level, const std::string& message, const std::string& component) {
    std::lock_guard<std::mutex> lock(m_mutex);
    
    if (!m_initialized) {
        // If not initialized, just output to console
        if (m_consoleOutput) {
            std::cerr << "[" << getLevelString(level) << "] " << message << std::endl;
        }
        return;
    }
    
    // Format log message
    std::stringstream ss;
    ss << "[" << getTimestamp() << "]";
    ss << "[" << getLevelString(level) << "]";
    
    if (!component.empty()) {
        ss << "[" << component << "]";
    }
    
    ss << " " << message << "\n";
    
    std::string logMessage = ss.str();
    
    // Write to application log
    writeToFile(m_applicationLog, logMessage);
    
    // Write errors and fatal messages to environment log
    if (level == LogLevel::ERROR || level == LogLevel::FATAL) {
        writeToFile(m_environmentLog, logMessage);
    }
    
    // Output to console if enabled
    if (m_consoleOutput) {
        if (level == LogLevel::ERROR || level == LogLevel::FATAL) {
            std::cerr << logMessage;
        } else {
            std::cout << logMessage;
        }
    }
}

void Logger::info(const std::string& message, const std::string& component) {
    log(LogLevel::INFO, message, component);
}

void Logger::warning(const std::string& message, const std::string& component) {
    log(LogLevel::WARNING, message, component);
}

void Logger::error(const std::string& message, const std::string& component) {
    log(LogLevel::ERROR, message, component);
}

void Logger::fatal(const std::string& message, const std::string& component) {
    log(LogLevel::FATAL, message, component);
}

std::string Logger::getTimestamp() const {
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
        now.time_since_epoch()) % 1000;
    
    std::stringstream ss;
    ss << std::put_time(std::localtime(&time), "%Y-%m-%d_%H-%M-%S");
    ss << "." << std::setfill('0') << std::setw(3) << ms.count();
    
    return ss.str();
}

std::string Logger::getLevelString(LogLevel level) const {
    switch (level) {
        case LogLevel::INFO:    return "INFO";
        case LogLevel::WARNING: return "WARNING";
        case LogLevel::ERROR:   return "ERROR";
        case LogLevel::FATAL:   return "FATAL";
        default:                return "UNKNOWN";
    }
}

std::string Logger::getPlatformName() const {
#ifdef _WIN32
    return "Windows";
#elif defined(__APPLE__) || defined(__MACH__)
    return "macOS";
#elif defined(__linux__)
    return "Linux";
#else
    return "Other";
#endif
}

std::string Logger::getEnvironmentLogPath() const {
    return m_environmentLogPath;
}

void Logger::ensureDirectoryExists(const std::string& path) {
    struct stat info;
    
    if (stat(path.c_str(), &info) != 0) {
        // Directory doesn't exist, create it
        MKDIR(path.c_str());
    }
}

void Logger::writeToFile(std::ofstream& file, const std::string& message) {
    if (file.is_open()) {
        file << message;
        file.flush(); // Ensure immediate write for crash scenarios
    }
}

} // namespace fresh
