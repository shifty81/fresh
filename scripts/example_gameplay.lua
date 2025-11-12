-- example_gameplay.lua
-- Example Lua script demonstrating gameplay logic integration

print("=== Fresh Engine Lua Gameplay Script ===")
print("Initializing gameplay systems...")

-- Example: Player health management
PlayerHealth = {
    current = 100,
    max = 100,
    
    damage = function(self, amount)
        self.current = math.max(0, self.current - amount)
        print("Player took " .. amount .. " damage. Health: " .. self.current)
    end,
    
    heal = function(self, amount)
        self.current = math.min(self.max, self.current + amount)
        print("Player healed " .. amount .. ". Health: " .. self.current)
    end
}

-- Example: Enemy AI
Enemy = {}
Enemy.__index = Enemy

function Enemy:new(name, health)
    local enemy = {
        name = name or "Enemy",
        health = health or 50
    }
    setmetatable(enemy, self)
    return enemy
end

print("Lua gameplay script loaded successfully!")

return {
    PlayerHealth = PlayerHealth,
    Enemy = Enemy
}
