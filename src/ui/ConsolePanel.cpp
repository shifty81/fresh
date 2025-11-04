#include "ui/ConsolePanel.h"
#include "core/Logger.h"
#include <imgui.h>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <algorithm>
#include <ctime>

namespace fresh {

ConsolePanel::ConsolePanel()
    : m_visible(true)
    , m_maxMessages(1000)
    , m_autoScroll(true)
    , m_filterInfo(true)
    , m_filterWarning(true)
    , m_filterError(true)
{
    m_commandBuffer[0] = '\0';
}

ConsolePanel::~ConsolePanel() {
}

bool ConsolePanel::initialize() {
    registerDefaultCommands();
    addMessage(ConsoleMessageType::Info, "Console initialized");
    LOG_INFO_C("Console Panel initialized", "ConsolePanel");
    return true;
}

void ConsolePanel::render() {
    if (!m_visible) {
        return;
    }

    ImGui::Begin("Console", &m_visible);

    // Toolbar
    if (ImGui::Button("Clear")) {
        clear();
    }
    ImGui::SameLine();
    
    ImGui::Checkbox("Auto-scroll", &m_autoScroll);
    ImGui::SameLine();
    
    ImGui::Text("Filter:");
    ImGui::SameLine();
    ImGui::Checkbox("Info", &m_filterInfo);
    ImGui::SameLine();
    ImGui::Checkbox("Warning", &m_filterWarning);
    ImGui::SameLine();
    ImGui::Checkbox("Error", &m_filterError);

    ImGui::Separator();

    // Message display area
    ImGui::BeginChild("ScrollingRegion", ImVec2(0, -30), false, ImGuiWindowFlags_HorizontalScrollbar);
    
    renderMessages();
    
    if (m_autoScroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY()) {
        ImGui::SetScrollHereY(1.0f);
    }
    
    ImGui::EndChild();

    ImGui::Separator();

    // Command input
    renderCommandInput();

    ImGui::End();
}

void ConsolePanel::renderMessages() {
    ImGuiListClipper clipper;
    clipper.Begin(static_cast<int>(m_messages.size()));
    
    while (clipper.Step()) {
        for (int i = clipper.DisplayStart; i < clipper.DisplayEnd; i++) {
            const ConsoleMessage& msg = m_messages[i];
            
            // Filter messages
            if ((msg.type == ConsoleMessageType::Info && !m_filterInfo) ||
                (msg.type == ConsoleMessageType::Warning && !m_filterWarning) ||
                (msg.type == ConsoleMessageType::Error && !m_filterError)) {
                continue;
            }
            
            // Color based on message type
            ImVec4 color;
            const char* prefix;
            switch (msg.type) {
                case ConsoleMessageType::Info:
                    color = ImVec4(0.7f, 0.7f, 0.7f, 1.0f);
                    prefix = "[INFO]";
                    break;
                case ConsoleMessageType::Warning:
                    color = ImVec4(1.0f, 0.8f, 0.0f, 1.0f);
                    prefix = "[WARN]";
                    break;
                case ConsoleMessageType::Error:
                    color = ImVec4(1.0f, 0.3f, 0.3f, 1.0f);
                    prefix = "[ERROR]";
                    break;
                case ConsoleMessageType::Success:
                    color = ImVec4(0.3f, 1.0f, 0.3f, 1.0f);
                    prefix = "[SUCCESS]";
                    break;
                default:
                    color = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
                    prefix = "[LOG]";
                    break;
            }
            
            ImGui::PushStyleColor(ImGuiCol_Text, color);
            ImGui::TextWrapped("[%s] %s: %s", 
                msg.timestamp.c_str(), 
                prefix, 
                msg.message.c_str());
            ImGui::PopStyleColor();
        }
    }
}

void ConsolePanel::renderCommandInput() {
    ImGui::Text("Command:");
    ImGui::SameLine();
    
    ImGui::SetNextItemWidth(-80);
    bool enterPressed = ImGui::InputText("##command", m_commandBuffer, sizeof(m_commandBuffer), 
                                         ImGuiInputTextFlags_EnterReturnsTrue);
    
    ImGui::SameLine();
    bool executeClicked = ImGui::Button("Execute");
    
    if (enterPressed || executeClicked) {
        if (m_commandBuffer[0] != '\0') {
            std::string command(m_commandBuffer);
            addMessage(ConsoleMessageType::Info, "> " + command);
            
            // Process command
            executeCommand(command);
            
            m_commandBuffer[0] = '\0';
        }
    }
}

void ConsolePanel::addMessage(ConsoleMessageType type, const std::string& message) {
    std::string timestamp = getTimestamp();
    m_messages.emplace_back(type, message, timestamp);
    
    // Limit message history
    while (m_messages.size() > m_maxMessages) {
        m_messages.pop_front();
    }
}

void ConsolePanel::clear() {
    m_messages.clear();
    addMessage(ConsoleMessageType::Info, "Console cleared");
}

std::string ConsolePanel::getTimestamp() {
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
        now.time_since_epoch()) % 1000;
    
    std::tm tm;
#ifdef _WIN32
    localtime_s(&tm, &time);
#else
    localtime_r(&time, &tm);
#endif
    
    std::ostringstream oss;
    oss << std::put_time(&tm, "%H:%M:%S") << "." 
        << std::setfill('0') << std::setw(3) << ms.count();
    
    return oss.str();
}

void ConsolePanel::registerCommand(const std::string& name,
                                   std::function<void(const std::vector<std::string>&)> callback,
                                   const std::string& description) {
    CommandInfo info;
    info.callback = callback;
    info.description = description;
    m_commands[name] = info;
}

void ConsolePanel::executeCommand(const std::string& commandLine) {
    if (commandLine.empty()) {
        return;
    }

    // Parse command line into command and arguments
    std::vector<std::string> parts = parseCommandLine(commandLine);
    if (parts.empty()) {
        return;
    }

    std::string cmdName = parts[0];
    std::vector<std::string> args(parts.begin() + 1, parts.end());

    // Find and execute command
    auto it = m_commands.find(cmdName);
    if (it != m_commands.end()) {
        try {
            it->second.callback(args);
        } catch (const std::exception& e) {
            addMessage(ConsoleMessageType::Error, 
                      "Command error: " + std::string(e.what()));
        }
    } else {
        addMessage(ConsoleMessageType::Error, 
                  "Unknown command: " + cmdName + ". Type 'help' for available commands.");
    }
}

std::vector<std::string> ConsolePanel::parseCommandLine(const std::string& commandLine) {
    std::vector<std::string> result;
    std::string current;
    bool inQuotes = false;

    for (char c : commandLine) {
        if (c == '"') {
            inQuotes = !inQuotes;
        } else if (c == ' ' && !inQuotes) {
            if (!current.empty()) {
                result.push_back(current);
                current.clear();
            }
        } else {
            current += c;
        }
    }

    if (!current.empty()) {
        result.push_back(current);
    }

    return result;
}

void ConsolePanel::registerDefaultCommands() {
    // Help command
    registerCommand("help", [this](const std::vector<std::string>& args) {
        if (args.empty()) {
            addMessage(ConsoleMessageType::Info, "Available commands:");
            for (const auto& cmd : m_commands) {
                std::string desc = cmd.second.description.empty() ? 
                                  "No description" : cmd.second.description;
                addMessage(ConsoleMessageType::Info, "  " + cmd.first + " - " + desc);
            }
        } else {
            // Show help for specific command
            auto it = m_commands.find(args[0]);
            if (it != m_commands.end()) {
                addMessage(ConsoleMessageType::Info, 
                          args[0] + ": " + it->second.description);
            } else {
                addMessage(ConsoleMessageType::Error, "Unknown command: " + args[0]);
            }
        }
    }, "Display available commands or help for a specific command");

    // Clear command
    registerCommand("clear", [this](const std::vector<std::string>&) {
        clear();
    }, "Clear all console messages");

    // Echo command
    registerCommand("echo", [this](const std::vector<std::string>& args) {
        std::string message;
        for (size_t i = 0; i < args.size(); ++i) {
            if (i > 0) message += " ";
            message += args[i];
        }
        addMessage(ConsoleMessageType::Info, message);
    }, "Echo arguments to console");

    // Version command
    registerCommand("version", [this](const std::vector<std::string>&) {
        addMessage(ConsoleMessageType::Info, "Fresh Voxel Engine v0.1.0");
    }, "Display engine version");

    // Info command
    registerCommand("info", [this](const std::vector<std::string>&) {
        addMessage(ConsoleMessageType::Info, "=== Engine Information ===");
        addMessage(ConsoleMessageType::Info, "Engine: Fresh Voxel Engine");
        addMessage(ConsoleMessageType::Info, "Version: 0.1.0");
        addMessage(ConsoleMessageType::Info, "Graphics: OpenGL");
#ifdef _WIN32
        addMessage(ConsoleMessageType::Info, "Platform: Windows");
#elif __linux__
        addMessage(ConsoleMessageType::Info, "Platform: Linux");
#elif __APPLE__
        addMessage(ConsoleMessageType::Info, "Platform: macOS");
#else
        addMessage(ConsoleMessageType::Info, "Platform: Unknown");
#endif
    }, "Display engine information");
}

} // namespace fresh
