#pragma once

#include <string>
#include <vector>
#include <deque>

namespace fresh {

/**
 * @brief Console message types
 */
enum class ConsoleMessageType {
    Info,
    Warning,
    Error,
    Success
};

/**
 * @brief Console message structure
 */
struct ConsoleMessage {
    ConsoleMessageType type;
    std::string message;
    std::string timestamp;
    
    ConsoleMessage(ConsoleMessageType t, const std::string& msg, const std::string& time)
        : type(t), message(msg), timestamp(time) {}
};

/**
 * @brief Console / Status Bar Panel
 * 
 * Displays engine status, error messages, and debug information.
 * Acts as both a console for logging and a status bar for quick info.
 */
class ConsolePanel {
public:
    ConsolePanel();
    ~ConsolePanel();

    /**
     * @brief Initialize the panel
     * @return true if successful
     */
    bool initialize();

    /**
     * @brief Render the panel UI
     */
    void render();

    /**
     * @brief Set panel visibility
     * @param visible true to show, false to hide
     */
    void setVisible(bool visible) { m_visible = visible; }

    /**
     * @brief Check if panel is visible
     * @return true if visible
     */
    bool isVisible() const { return m_visible; }

    /**
     * @brief Add a message to the console
     * @param type Message type
     * @param message Message text
     */
    void addMessage(ConsoleMessageType type, const std::string& message);

    /**
     * @brief Clear all messages
     */
    void clear();

private:
    void renderMessages();
    void renderCommandInput();
    std::string getTimestamp();

private:
    bool m_visible;
    std::deque<ConsoleMessage> m_messages;
    size_t m_maxMessages;
    char m_commandBuffer[512];
    bool m_autoScroll;
    bool m_filterInfo;
    bool m_filterWarning;
    bool m_filterError;
};

} // namespace fresh
