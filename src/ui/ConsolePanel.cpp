#include "ui/ConsolePanel.h"
#include "core/Logger.h"
#include <imgui.h>
#include <chrono>
#include <iomanip>
#include <sstream>

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
    clipper.Begin(m_messages.size());
    
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
            
            // TODO: Process command
            // For now, just echo back
            addMessage(ConsoleMessageType::Warning, "Command execution not yet implemented");
            
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
    localtime_s(&tm, &time);
    
    std::ostringstream oss;
    oss << std::put_time(&tm, "%H:%M:%S") << "." 
        << std::setfill('0') << std::setw(3) << ms.count();
    
    return oss.str();
}

} // namespace fresh
