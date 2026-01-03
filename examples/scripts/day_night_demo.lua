-- Day/Night Cycle Demo Script
-- Demonstrates the TimeManager Lua API

print("=== Day/Night Cycle Demo ===")
print("This script demonstrates time management and sun/moon queries")
print("")

-- Global state
local demoState = {
    timeAcceleration = 1.0,
    lastDay = -1,
    updateInterval = 2.0,  -- Update display every 2 seconds
    timeSinceUpdate = 0.0
}

-- Initialize function (called once when script loads)
function init()
    print("Initializing day/night demo...")
    
    -- Set initial time to dawn
    Time.setTimeOfDay(Time.TimeOfDay.Dawn)
    print("  ✓ Set time to Dawn (6:00 AM)")
    
    -- Set time rate for faster demo
    Time.setTimeRate(10.0)  -- 10x speed
    print("  ✓ Set time rate to 10x for demo purposes")
    
    -- Display current status
    displayTimeInfo()
    print("")
end

-- Update function (called every frame)
function on_update(dt)
    demoState.timeSinceUpdate = demoState.timeSinceUpdate + dt
    
    -- Update display periodically
    if demoState.timeSinceUpdate >= demoState.updateInterval then
        demoState.timeSinceUpdate = 0.0
        displayTimeInfo()
    end
    
    -- Check for day change
    local currentDay = Time.getCurrentDay()
    if currentDay ~= demoState.lastDay then
        print("")
        print("🌅 NEW DAY: Day " .. currentDay)
        print("")
        demoState.lastDay = currentDay
    end
end

-- Display current time information
function displayTimeInfo()
    local timeStr = Time.getFormattedTime()
    local hours = Time.getTimeInHours()
    local ticks = Time.getTime()
    local day = Time.getCurrentDay()
    
    print("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━")
    print("⏰ Current Time: " .. timeStr)
    print("   Day: " .. day .. " | Ticks: " .. ticks .. " | Hours: " .. string.format("%.2f", hours))
    
    -- Time of day
    if Time.isDaytime() then
        print("   Period: ☀️  Daytime")
    else
        print("   Period: 🌙 Nighttime")
    end
    
    if Time.isSunrise() then
        print("   Special: 🌅 Sunrise!")
    elseif Time.isSunset() then
        print("   Special: 🌇 Sunset!")
    end
    
    -- Sun information
    local sunDir = Time.getSunDirection()
    local sunElev = Time.getSunElevation()
    print(string.format("   Sun Elevation: %.1f° | Direction: (%.2f, %.2f, %.2f)", 
                       sunElev, sunDir.x, sunDir.y, sunDir.z))
    
    -- Lighting information
    local ambient = Time.getAmbientLightIntensity()
    local skyColor = Time.getSkyColor()
    local sunColor = Time.getSunLightColor()
    
    print(string.format("   Ambient Light: %.2f", ambient))
    print(string.format("   Sky Color: (%.2f, %.2f, %.2f)", skyColor.x, skyColor.y, skyColor.z))
    print(string.format("   Sun Color: (%.2f, %.2f, %.2f)", sunColor.x, sunColor.y, sunColor.z))
    print("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━")
end

-- Keyboard command examples
function onKeyPress(key)
    if key == "1" then
        print("\n🔹 Setting time to Dawn...")
        Time.setTimeOfDay(Time.TimeOfDay.Dawn)
        displayTimeInfo()
    elseif key == "2" then
        print("\n🔹 Setting time to Noon...")
        Time.setTimeOfDay(Time.TimeOfDay.Noon)
        displayTimeInfo()
    elseif key == "3" then
        print("\n🔹 Setting time to Dusk...")
        Time.setTimeOfDay(Time.TimeOfDay.Dusk)
        displayTimeInfo()
    elseif key == "4" then
        print("\n🔹 Setting time to Midnight...")
        Time.setTimeOfDay(Time.TimeOfDay.Midnight)
        displayTimeInfo()
    elseif key == "P" or key == "p" then
        Time.togglePause()
        if Time.isPaused() then
            print("\n⏸️  Time paused")
        else
            print("\n▶️  Time resumed")
        end
    elseif key == "+" or key == "=" then
        local rate = Time.getTimeRate()
        rate = math.min(rate * 2.0, 100.0)
        Time.setTimeRate(rate)
        print(string.format("\n⏩ Time rate increased to %.1fx", rate))
    elseif key == "-" or key == "_" then
        local rate = Time.getTimeRate()
        rate = math.max(rate / 2.0, 0.1)
        Time.setTimeRate(rate)
        print(string.format("\n⏪ Time rate decreased to %.1fx", rate))
    end
end

-- Display instructions
print("\n📋 Demo Controls:")
print("   1 - Set time to Dawn")
print("   2 - Set time to Noon")
print("   3 - Set time to Dusk")
print("   4 - Set time to Midnight")
print("   P - Pause/Resume time")
print("   + - Increase time rate")
print("   - - Decrease time rate")
print("")

-- Run initialization
init()
