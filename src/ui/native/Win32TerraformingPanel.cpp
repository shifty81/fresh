#ifdef _WIN32

#include "ui/native/Win32TerraformingPanel.h"
#include "editor/WorldEditor.h"
#include "core/Logger.h"
#include <commctrl.h>
#include <sstream>
#include <iomanip>

namespace fresh
{

// Command IDs for buttons
#define CMD_TOOL_SINGLE_BLOCK   2000
#define CMD_TOOL_BRUSH          2001
#define CMD_TOOL_SPHERE         2002
#define CMD_TOOL_FILLED_SPHERE  2003
#define CMD_TOOL_CUBE           2004
#define CMD_TOOL_FILLED_CUBE    2005
#define CMD_TOOL_LINE           2006
#define CMD_TOOL_FLATTEN        2007
#define CMD_TOOL_SMOOTH         2008
#define CMD_TOOL_PAINT          2009

#define CMD_MODE_PLACE          2100
#define CMD_MODE_REMOVE         2101
#define CMD_MODE_REPLACE        2102

#define CMD_SIZE_UP             2200
#define CMD_SIZE_DOWN           2201
#define CMD_UNDO                2202
#define CMD_REDO                2203

#define CMD_MATERIAL_BASE       3000  // Materials start at 3000

Win32TerraformingPanel::Win32TerraformingPanel()
    : Win32Panel()
    , m_worldEditor(nullptr)
    , m_terraformingSystem(nullptr)
    , m_currentTool(TerraformTool::SingleBlock)
    , m_currentMode(TerraformMode::Place)
    , m_currentMaterial(VoxelType::Stone)
    , m_currentSize(1)
    , m_canUndo(false)
    , m_canRedo(false)
    , m_toolGroup(nullptr)
    , m_modeGroup(nullptr)
    , m_materialGroup(nullptr)
    , m_sizeGroup(nullptr)
    , m_undoRedoGroup(nullptr)
    , m_sizeUpButton(nullptr)
    , m_sizeDownButton(nullptr)
    , m_sizeLabel(nullptr)
    , m_undoButton(nullptr)
    , m_redoButton(nullptr)
    , m_bgBrush(nullptr)
    , m_selectedBrush(nullptr)
    , m_buttonBrush(nullptr)
    , m_borderPen(nullptr)
    , m_titleFont(nullptr)
    , m_textFont(nullptr)
{
    for (int i = 0; i < 10; ++i) {
        m_toolButtons[i] = nullptr;
    }
    for (int i = 0; i < 3; ++i) {
        m_modeButtons[i] = nullptr;
    }
    for (int i = 0; i < 15; ++i) {
        m_materialButtons[i] = nullptr;
    }
}

Win32TerraformingPanel::~Win32TerraformingPanel()
{
    // Clean up GDI resources
    if (m_bgBrush) DeleteObject(m_bgBrush);
    if (m_selectedBrush) DeleteObject(m_selectedBrush);
    if (m_buttonBrush) DeleteObject(m_buttonBrush);
    if (m_borderPen) DeleteObject(m_borderPen);
    if (m_titleFont) DeleteObject(m_titleFont);
    if (m_textFont) DeleteObject(m_textFont);
}

bool Win32TerraformingPanel::initialize(HWND parent, WorldEditor* worldEditor)
{
    if (!worldEditor) {
        LOG_ERROR_C("Invalid world editor", "Win32TerraformingPanel");
        return false;
    }

    m_worldEditor = worldEditor;
    m_terraformingSystem = worldEditor->getTerraformingSystem();

    if (!m_terraformingSystem) {
        LOG_ERROR_C("No terraforming system available", "Win32TerraformingPanel");
        return false;
    }

    // Create the panel window
    if (!create(parent, 10, 100, 250, 600, L"Terraforming Tools")) {
        LOG_ERROR_C("Failed to create terraforming panel window", "Win32TerraformingPanel");
        return false;
    }

    // Create GDI resources
    m_bgBrush = CreateSolidBrush(UnrealStyleTheme::PanelBackground);
    m_selectedBrush = CreateSolidBrush(UnrealStyleTheme::AccentBlue);
    m_buttonBrush = CreateSolidBrush(UnrealStyleTheme::ButtonBackground);
    m_borderPen = CreatePen(PS_SOLID, 1, UnrealStyleTheme::Border);

    // Create fonts
    m_titleFont = CreateFont(16, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
                            DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                            CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Segoe UI");

    m_textFont = CreateFont(14, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
                           DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                           CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Segoe UI");

    LOG_INFO_C("Win32TerraformingPanel initialized", "Win32TerraformingPanel");
    return true;
}

void Win32TerraformingPanel::onCreate()
{
    createControls();
    update();
}

void Win32TerraformingPanel::createControls()
{
    // Create tool buttons
    int yPos = MARGIN;
    
    // Tool selection section
    createToolButtons();
    
    // Mode selection section
    createModeButtons();
    
    // Material picker section
    createMaterialPicker();
    
    // Size control section
    createSizeControl();
    
    // Undo/Redo section
    createUndoRedoDisplay();
}

void Win32TerraformingPanel::createToolButtons()
{
    // For simplicity, create a few key tool buttons
    // In a full implementation, you'd create all tool buttons with proper icons
    
    const wchar_t* toolNames[] = {
        L"Single Block",
        L"Brush",
        L"Sphere",
        L"Filled Sphere",
        L"Cube",
        L"Filled Cube",
        L"Line",
        L"Flatten",
        L"Smooth",
        L"Paint"
    };

    int cmdIds[] = {
        CMD_TOOL_SINGLE_BLOCK,
        CMD_TOOL_BRUSH,
        CMD_TOOL_SPHERE,
        CMD_TOOL_FILLED_SPHERE,
        CMD_TOOL_CUBE,
        CMD_TOOL_FILLED_CUBE,
        CMD_TOOL_LINE,
        CMD_TOOL_FLATTEN,
        CMD_TOOL_SMOOTH,
        CMD_TOOL_PAINT
    };

    int yPos = MARGIN + 30;  // After "Tools" header
    for (int i = 0; i < 10; ++i) {
        m_toolButtons[i] = CreateWindowW(
            L"BUTTON",
            toolNames[i],
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            MARGIN, yPos, m_width - 2 * MARGIN, BUTTON_HEIGHT,
            m_hwnd,
            (HMENU)(LONG_PTR)cmdIds[i],
            GetModuleHandle(nullptr),
            nullptr
        );
        
        if (m_toolButtons[i]) {
            SendMessage(m_toolButtons[i], WM_SETFONT, (WPARAM)m_textFont, TRUE);
        }
        
        yPos += BUTTON_HEIGHT + BUTTON_SPACING;
    }
}

void Win32TerraformingPanel::createModeButtons()
{
    const wchar_t* modeNames[] = { L"Place", L"Remove", L"Replace" };
    int cmdIds[] = { CMD_MODE_PLACE, CMD_MODE_REMOVE, CMD_MODE_REPLACE };

    int yPos = MARGIN + 30 + (BUTTON_HEIGHT + BUTTON_SPACING) * 10 + SECTION_SPACING + 30;
    int buttonWidth = (m_width - 3 * MARGIN - 2 * BUTTON_SPACING) / 3;

    for (int i = 0; i < 3; ++i) {
        m_modeButtons[i] = CreateWindowW(
            L"BUTTON",
            modeNames[i],
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            MARGIN + i * (buttonWidth + BUTTON_SPACING), yPos,
            buttonWidth, BUTTON_HEIGHT,
            m_hwnd,
            (HMENU)(LONG_PTR)cmdIds[i],
            GetModuleHandle(nullptr),
            nullptr
        );
        
        if (m_modeButtons[i]) {
            SendMessage(m_modeButtons[i], WM_SETFONT, (WPARAM)m_textFont, TRUE);
        }
    }
}

void Win32TerraformingPanel::createMaterialPicker()
{
    // Create a few material buttons
    // In a full implementation, use icons/colors for materials
    const wchar_t* materialNames[] = {
        L"Stone", L"Dirt", L"Grass", L"Sand", L"Water"
    };

    int yPos = MARGIN + 30 + (BUTTON_HEIGHT + BUTTON_SPACING) * 10 + 
               SECTION_SPACING + 30 + BUTTON_HEIGHT + SECTION_SPACING + 30;
    
    for (int i = 0; i < 5; ++i) {
        m_materialButtons[i] = CreateWindowW(
            L"BUTTON",
            materialNames[i],
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            MARGIN, yPos, m_width - 2 * MARGIN, BUTTON_HEIGHT,
            m_hwnd,
            (HMENU)(LONG_PTR)(CMD_MATERIAL_BASE + i),
            GetModuleHandle(nullptr),
            nullptr
        );
        
        if (m_materialButtons[i]) {
            SendMessage(m_materialButtons[i], WM_SETFONT, (WPARAM)m_textFont, TRUE);
        }
        
        yPos += BUTTON_HEIGHT + BUTTON_SPACING;
    }
}

void Win32TerraformingPanel::createSizeControl()
{
    int yPos = m_height - 150;

    // Size label
    m_sizeLabel = CreateWindowW(
        L"STATIC",
        L"Size: 1",
        WS_CHILD | WS_VISIBLE | SS_CENTER,
        MARGIN, yPos, m_width - 2 * MARGIN, 25,
        m_hwnd,
        nullptr,
        GetModuleHandle(nullptr),
        nullptr
    );

    if (m_sizeLabel) {
        SendMessage(m_sizeLabel, WM_SETFONT, (WPARAM)m_textFont, TRUE);
    }

    yPos += 30;

    // Size buttons
    int buttonWidth = (m_width - 3 * MARGIN) / 2;
    
    m_sizeDownButton = CreateWindowW(
        L"BUTTON",
        L"-",
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        MARGIN, yPos, buttonWidth, BUTTON_HEIGHT,
        m_hwnd,
        (HMENU)CMD_SIZE_DOWN,
        GetModuleHandle(nullptr),
        nullptr
    );

    m_sizeUpButton = CreateWindowW(
        L"BUTTON",
        L"+",
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        MARGIN + buttonWidth + BUTTON_SPACING, yPos, buttonWidth, BUTTON_HEIGHT,
        m_hwnd,
        (HMENU)CMD_SIZE_UP,
        GetModuleHandle(nullptr),
        nullptr
    );

    if (m_sizeDownButton) SendMessage(m_sizeDownButton, WM_SETFONT, (WPARAM)m_textFont, TRUE);
    if (m_sizeUpButton) SendMessage(m_sizeUpButton, WM_SETFONT, (WPARAM)m_textFont, TRUE);
}

void Win32TerraformingPanel::createUndoRedoDisplay()
{
    int yPos = m_height - 80;
    int buttonWidth = (m_width - 3 * MARGIN) / 2;

    m_undoButton = CreateWindowW(
        L"BUTTON",
        L"Undo (U)",
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        MARGIN, yPos, buttonWidth, BUTTON_HEIGHT,
        m_hwnd,
        (HMENU)CMD_UNDO,
        GetModuleHandle(nullptr),
        nullptr
    );

    m_redoButton = CreateWindowW(
        L"BUTTON",
        L"Redo (R)",
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        MARGIN + buttonWidth + BUTTON_SPACING, yPos, buttonWidth, BUTTON_HEIGHT,
        m_hwnd,
        (HMENU)CMD_REDO,
        GetModuleHandle(nullptr),
        nullptr
    );

    if (m_undoButton) SendMessage(m_undoButton, WM_SETFONT, (WPARAM)m_textFont, TRUE);
    if (m_redoButton) SendMessage(m_redoButton, WM_SETFONT, (WPARAM)m_textFont, TRUE);
}

void Win32TerraformingPanel::update()
{
    if (!m_terraformingSystem) {
        return;
    }

    // Update state from terraforming system
    m_currentTool = m_terraformingSystem->getTool();
    m_currentMode = m_terraformingSystem->getMode();
    m_currentMaterial = m_terraformingSystem->getVoxelType();
    m_currentSize = m_terraformingSystem->getToolSize();
    m_canUndo = m_terraformingSystem->canUndo();
    m_canRedo = m_terraformingSystem->canRedo();

    updateToolSelection();
    updateModeSelection();
    updateMaterialSelection();
    updateSizeDisplay();
    updateUndoRedoDisplay();

    // Redraw the panel
    InvalidateRect(m_hwnd, nullptr, TRUE);
}

void Win32TerraformingPanel::updateToolSelection()
{
    // Update visual selection state of tool buttons
    // In a full implementation, you'd change button styles or colors
}

void Win32TerraformingPanel::updateModeSelection()
{
    // Update visual selection state of mode buttons
}

void Win32TerraformingPanel::updateMaterialSelection()
{
    // Update visual selection state of material buttons
}

void Win32TerraformingPanel::updateSizeDisplay()
{
    if (m_sizeLabel) {
        std::wstringstream ss;
        ss << L"Size: " << m_currentSize;
        SetWindowTextW(m_sizeLabel, ss.str().c_str());
    }
}

void Win32TerraformingPanel::updateUndoRedoDisplay()
{
    if (m_undoButton) {
        EnableWindow(m_undoButton, m_canUndo ? TRUE : FALSE);
    }
    if (m_redoButton) {
        EnableWindow(m_redoButton, m_canRedo ? TRUE : FALSE);
    }
}

void Win32TerraformingPanel::onPaint(HDC hdc)
{
    RECT clientRect;
    GetClientRect(m_hwnd, &clientRect);

    // Fill background
    FillRect(hdc, &clientRect, m_bgBrush);

    // Set text properties
    SetBkMode(hdc, TRANSPARENT);
    SetTextColor(hdc, UnrealStyleTheme::TextNormal);
    SelectObject(hdc, m_titleFont);

    // Draw section headers
    int yPos = MARGIN;

    // Tools header
    RECT headerRect = { MARGIN, yPos, m_width - MARGIN, yPos + 25 };
    DrawText(hdc, L"Tools", -1, &headerRect, DT_LEFT | DT_VCENTER);
    yPos += 30 + (BUTTON_HEIGHT + BUTTON_SPACING) * 10 + SECTION_SPACING;

    // Mode header
    headerRect = { MARGIN, yPos, m_width - MARGIN, yPos + 25 };
    DrawText(hdc, L"Mode", -1, &headerRect, DT_LEFT | DT_VCENTER);
    yPos += 30 + BUTTON_HEIGHT + SECTION_SPACING;

    // Material header
    headerRect = { MARGIN, yPos, m_width - MARGIN, yPos + 25 };
    DrawText(hdc, L"Material", -1, &headerRect, DT_LEFT | DT_VCENTER);
}

void Win32TerraformingPanel::onResize(int width, int height)
{
    // Reposition controls on resize if needed
    // For simplicity, we'll keep controls at their original positions
}

bool Win32TerraformingPanel::handleMessage(UINT msg, WPARAM wParam, LPARAM lParam, LRESULT& result)
{
    if (msg == WM_COMMAND) {
        int cmdId = LOWORD(wParam);

        // Handle tool selection
        if (cmdId >= CMD_TOOL_SINGLE_BLOCK && cmdId <= CMD_TOOL_PAINT) {
            TerraformTool tool = static_cast<TerraformTool>(cmdId - CMD_TOOL_SINGLE_BLOCK);
            onToolButtonClick(tool);
            result = 0;
            return true;
        }

        // Handle mode selection
        if (cmdId >= CMD_MODE_PLACE && cmdId <= CMD_MODE_REPLACE) {
            TerraformMode mode = static_cast<TerraformMode>(cmdId - CMD_MODE_PLACE);
            onModeButtonClick(mode);
            result = 0;
            return true;
        }

        // Handle material selection
        if (cmdId >= CMD_MATERIAL_BASE && cmdId < CMD_MATERIAL_BASE + 15) {
            int materialIndex = cmdId - CMD_MATERIAL_BASE;
            VoxelType type = static_cast<VoxelType>(materialIndex + 1);  // +1 because Air is 0
            onMaterialSelect(type);
            result = 0;
            return true;
        }

        // Handle size controls
        switch (cmdId) {
            case CMD_SIZE_UP:
                onSizeChange(1);
                result = 0;
                return true;
            case CMD_SIZE_DOWN:
                onSizeChange(-1);
                result = 0;
                return true;
            case CMD_UNDO:
                onUndoClick();
                result = 0;
                return true;
            case CMD_REDO:
                onRedoClick();
                result = 0;
                return true;
        }
    }

    return false;
}

void Win32TerraformingPanel::onToolButtonClick(TerraformTool tool)
{
    if (m_terraformingSystem) {
        m_terraformingSystem->setTool(tool);
        update();
        LOG_INFO_C("Tool changed to: " + std::to_string(static_cast<int>(tool)), "Win32TerraformingPanel");
    }
}

void Win32TerraformingPanel::onModeButtonClick(TerraformMode mode)
{
    if (m_terraformingSystem) {
        m_terraformingSystem->setMode(mode);
        update();
        LOG_INFO_C("Mode changed to: " + std::to_string(static_cast<int>(mode)), "Win32TerraformingPanel");
    }
}

void Win32TerraformingPanel::onMaterialSelect(VoxelType type)
{
    if (m_terraformingSystem) {
        m_terraformingSystem->setVoxelType(type);
        update();
        LOG_INFO_C("Material changed to: " + std::to_string(static_cast<int>(type)), "Win32TerraformingPanel");
    }
}

void Win32TerraformingPanel::onSizeChange(int delta)
{
    if (m_terraformingSystem) {
        int newSize = m_currentSize + delta;
        if (newSize < 1) newSize = 1;
        if (newSize > 20) newSize = 20;  // Max size limit
        
        m_terraformingSystem->setToolSize(newSize);
        update();
        LOG_INFO_C("Tool size changed to: " + std::to_string(newSize), "Win32TerraformingPanel");
    }
}

void Win32TerraformingPanel::onUndoClick()
{
    if (m_terraformingSystem && m_terraformingSystem->canUndo()) {
        m_terraformingSystem->undo();
        update();
        LOG_INFO_C("Undo performed", "Win32TerraformingPanel");
    }
}

void Win32TerraformingPanel::onRedoClick()
{
    if (m_terraformingSystem && m_terraformingSystem->canRedo()) {
        m_terraformingSystem->redo();
        update();
        LOG_INFO_C("Redo performed", "Win32TerraformingPanel");
    }
}

std::wstring Win32TerraformingPanel::getToolName(TerraformTool tool) const
{
    switch (tool) {
        case TerraformTool::SingleBlock: return L"Single Block";
        case TerraformTool::Brush: return L"Brush";
        case TerraformTool::Sphere: return L"Sphere";
        case TerraformTool::FilledSphere: return L"Filled Sphere";
        case TerraformTool::Cube: return L"Cube";
        case TerraformTool::FilledCube: return L"Filled Cube";
        case TerraformTool::Line: return L"Line";
        case TerraformTool::Flatten: return L"Flatten";
        case TerraformTool::Smooth: return L"Smooth";
        case TerraformTool::Paint: return L"Paint";
        default: return L"Unknown";
    }
}

std::wstring Win32TerraformingPanel::getModeName(TerraformMode mode) const
{
    switch (mode) {
        case TerraformMode::Place: return L"Place";
        case TerraformMode::Remove: return L"Remove";
        case TerraformMode::Replace: return L"Replace";
        default: return L"Unknown";
    }
}

std::wstring Win32TerraformingPanel::getMaterialName(VoxelType type) const
{
    switch (type) {
        case VoxelType::Stone: return L"Stone";
        case VoxelType::Dirt: return L"Dirt";
        case VoxelType::Grass: return L"Grass";
        case VoxelType::Sand: return L"Sand";
        case VoxelType::Water: return L"Water";
        case VoxelType::Wood: return L"Wood";
        case VoxelType::Leaves: return L"Leaves";
        case VoxelType::Glass: return L"Glass";
        case VoxelType::Cobblestone: return L"Cobblestone";
        case VoxelType::Planks: return L"Planks";
        case VoxelType::Snow: return L"Snow";
        case VoxelType::Ice: return L"Ice";
        case VoxelType::Obsidian: return L"Obsidian";
        default: return L"Unknown";
    }
}

} // namespace fresh

#endif // _WIN32
