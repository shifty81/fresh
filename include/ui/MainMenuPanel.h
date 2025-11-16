#pragma once

#include <functional>
#include <string>
#include <vector>

namespace fresh
{

/**
 * @brief ImGui-based Main Menu Panel
 *
 * Provides a graphical main menu interface for creating new worlds,
 * loading existing worlds, and accessing settings within the editor UI.
 */
class MainMenuPanel
{
public:
    MainMenuPanel();
    ~MainMenuPanel();

    /**
     * @brief Initialize the main menu panel
     * @return true if successful
     */
    bool initialize();

    /**
     * @brief Render the main menu panel UI
     */
    void render();

    /**
     * @brief Check if a new world should be created
     * @return true if user requested new world
     */
    bool shouldCreateNewWorld() const { return m_createNewWorld; }

    /**
     * @brief Check if a world should be loaded
     * @return true if user requested to load world
     */
    bool shouldLoadWorld() const { return m_loadWorld; }

    /**
     * @brief Get the world name for new world
     * @return World name
     */
    const std::string& getNewWorldName() const { return m_newWorldName; }

    /**
     * @brief Get the selected world to load
     * @return World filename
     */
    const std::string& getLoadWorldName() const { return m_loadWorldName; }

    /**
     * @brief Get the seed for new world generation
     * @return World seed
     */
    int getWorldSeed() const { return m_worldSeed; }

    /**
     * @brief Get the world type (2D or 3D)
     * @return true for 3D, false for 2D
     */
    bool isWorld3D() const { return m_isWorld3D; }

    /**
     * @brief Clear the create/load flags
     */
    void clearFlags();

    /**
     * @brief Scan for available world saves
     */
    void scanWorldSaves();

    /**
     * @brief Get list of available world saves
     * @return Vector of world filenames
     */
    const std::vector<std::string>& getWorldSaves() const { return m_worldSaves; }

    /**
     * @brief Check if in world selection mode (menu visible)
     * @return true if menu is active
     */
    bool isMenuActive() const { return m_menuActive; }

    /**
     * @brief Set menu active state
     * @param active true to show menu
     */
    void setMenuActive(bool active) { m_menuActive = active; }

    /**
     * @brief Check if exit was requested
     * @return true if user clicked exit button
     */
    bool shouldExit() const { return m_exitRequested; }

private:
    void renderWorldSelection();
    void renderNewWorldDialog();
    void renderLoadWorldDialog();

private:
    bool m_menuActive;
    bool m_createNewWorld;
    bool m_loadWorld;
    bool m_exitRequested;
    bool m_showNewWorldDialog;
    bool m_showLoadWorldDialog;
    bool m_isWorld3D; // World type: true = 3D, false = 2D
    
    std::string m_newWorldName;
    std::string m_loadWorldName;
    int m_worldSeed;
    int m_selectedWorldIndex;
    std::vector<std::string> m_worldSaves;

    // ImGui input buffers
    char m_worldNameBuffer[256];
    char m_seedBuffer[32];
};

} // namespace fresh
