Earth = 0

function init()
    create_world()

    -- =========================
    -- CENTRAL STAR
    -- =========================
    add_planet{
        name = "sun",
        texturepath = "assets/graphics/sun.jpg",
        radius = 35,
        position = {0, 0, 0},
        rings = 64,
        slices = 64,
        mass = 20000,
        has_gravity = true
    }

    -- =========================
    -- PLANETS (orbit the Sun)
    -- order matters visually only
    -- =========================

    -- Mercury
    add_planet{
        name = "mercury",
        texturepath = "assets/graphics/mercury.jpg",
        radius = 4,
        position = {70, 0, 0},
        rings = 24,
        slices = 24,
        mass = 2,
        has_gravity = false
    }

    -- Venus
    add_planet{
        name = "venus",
        texturepath = "assets/graphics/venus.jpg",
        radius = 6,
        position = {110, 0, 0},
        rings = 28,
        slices = 28,
        mass = 1,
        has_gravity = false
    }

    -- Earth
    Earth = add_planet{
        name = "earth",
        texturepath = "assets/graphics/earth.jpg",
        radius = 6.5,
        position = {160, 0, 0},
        rings = 32,
        slices = 32,
        mass = 1,
        has_gravity = false
    }

    -- Mars
    add_planet{
        name = "mars",
        texturepath = "assets/graphics/mars.jpg",
        radius = 5,
        position = {210, 0, 0},
        rings = 28,
        slices = 28,
        mass = 1,
        has_gravity = false
    }

    -- Jupiter
    add_planet{
        name = "jupiter",
        texturepath = "assets/graphics/jupiter.jpg",
        radius = 18,
        position = {300, 0, 0},
        rings = 48,
        slices = 48,
        mass = 1,
        has_gravity = false
    }

    -- Saturn
    add_planet{
        name = "saturn",
        texturepath = "assets/graphics/saturn.jpg",
        radius = 16,
        position = {380, 0, 0},
        rings = 48,
        slices = 48,
        mass = 1,
        has_gravity = false
    }

    -- Uranus
    add_planet{
        name = "uranus",
        texturepath = "assets/graphics/uranus.jpg",
        radius = 12,
        position = {460, 0, 0},
        rings = 40,
        slices = 40,
        mass = 1,
        has_gravity = false
    }

    -- Neptune
    add_planet{
        name = "neptune",
        texturepath = "assets/graphics/neptune.jpg",
        radius = 12,
        position = {540, 0, 0},
        rings = 40,
        slices = 40,
        mass = 1,
        has_gravity = false
    }

end

function update()
    if is_key_pressed(KEY.L) then
        pause()
    end
    if is_key_pressed(KEY.P) then
        resume()
    end
end 