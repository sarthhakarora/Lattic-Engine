create_world()

-- central body
add_planet{
    name = "sun",
    texturepath = "assets/graphics/sun.jpg",
    radius = 30,
    position = {0, 0, 0},
    rings = 48,
    slices = 48,
    mass = 1000,
    has_gravity = true
}

-- asteroid belt
local asteroid_count = 10
local belt_radius = 150
local belt_thickness = 40

for i = 1, asteroid_count do
    local angle = math.random() * math.pi * 2
    local radius = belt_radius + math.random(-belt_thickness, belt_thickness)

    local x = math.cos(angle) * radius
    local z = math.sin(angle) * radius
    local y = math.random(-5, 5)

    add_planet{
        name = "asteroid_" .. i,
        texturepath = "assets/graphics/earth.jpg",
        radius = math.random(5, 10),
        position = { x, y, z },
        rings = 8,
        slices = 8,
        mass = 1,
        has_gravity = false
    }
end