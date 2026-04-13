-- ============================================================
-- Textured Dual Galaxy Merger
-- Heavy CPU + Render Stress
-- ============================================================

local TOTAL_PER_GALAXY = 12000
local GALAXY_RADIUS = 1500
local CORE_MASS = 300000
local STAR_BASE_MASS = 1.0

local GALAXY_OFFSET = 3000
local GALAXY_APPROACH_SPEED = 80

local TEXTURES = {
    "assets/graphics/mercury.jpg",
    "assets/graphics/venus.jpg",
    "assets/graphics/earth.jpg",
    "assets/graphics/mars.jpg",
    "assets/graphics/jupiter.jpg",
    "assets/graphics/saturn.jpg",
    "assets/graphics/uranus.jpg",
    "assets/graphics/neptune.jpg",
    "assets/graphics/moon.jpg",
    "assets/graphics/makemake.jpg",
    "assets/graphics/haumea.jpg",
    "assets/graphics/ceres.jpg"
}

local function randf(a, b)
    return a + (b - a) * math.random()
end

local function random_texture()
    return TEXTURES[math.random(#TEXTURES)]
end

local function spawn_galaxy(center_x, direction)

    -- Supermassive core (Sagittarius A*)
    add_planet{
        name = "core",
        texturepath = "assets/graphics/sun.jpg",
        radius = 60,
        position = {center_x, 0, 0},
        rings = 48,
        slices = 48,
        mass = CORE_MASS,
        has_gravity = true
    }

    for i = 1, TOTAL_PER_GALAXY do

        -- Spiral distribution
        local r = math.sqrt(math.random()) * GALAXY_RADIUS
        local angle = r * 0.002 + randf(0, math.pi * 2)

        local x = center_x + math.cos(angle) * r
        local y = randf(-40, 40)
        local z = math.sin(angle) * r

        local radius = randf(1.5, 4.0)
        local mass = STAR_BASE_MASS * randf(0.8, 1.2)

        add_planet{
            name = "star_" .. i,
            texturepath = random_texture(),
            radius = radius,
            position = {x, y, z},
            rings = 8,
            slices = 8,
            mass = mass,
            has_gravity = false
        }
    end
end

function init()
    create_world()

    set_world_gravity_constant(60.0)
    math.randomseed(os.time())

    spawn_galaxy(-GALAXY_OFFSET, 1)
    spawn_galaxy(GALAXY_OFFSET, -1)
end

function update()
    if is_key_pressed(KEY.L) then pause() end
    if is_key_pressed(KEY.P) then resume() end
end