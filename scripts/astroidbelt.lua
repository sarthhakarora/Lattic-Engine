function init()
    create_world()

    -- STATIC GIANTS
    add_planet{
        name = "sun",
        texturepath = "assets/graphics/makemake.jpg",
        radius = 30,
        position = {0, 0, 0},
        rings = 64,
        slices = 64,
        mass = 12000,
        has_gravity = true
    }

    add_planet{
        name = "venus",
        texturepath = "assets/graphics/venus.jpg",
        radius = 20,
        position = {400, 0, 0},
        rings = 48,
        slices = 48,
        mass = 3000,
        has_gravity = true
    }

    -- PARTICLES ORBITING ONLY THE SUN INITIALLY
    local count = 5000
    local r0 = 220

    for i = 1, count do
        local a = math.random() * math.pi * 2
        local r = r0 + math.random(-10, 10)

        local x = math.cos(a) * r
        local z = math.sin(a) * r
        local y = math.random(-1, 1)

        add_planet{
            name = "dust_" .. i,
            texturepath = "assets/graphics/ceres.jpg", 
            radius = 2.5,
            position = {x, y, z},
            rings = 6,
            slices = 6,
            mass = 1,
            has_gravity = false
        }
    end
end

function update()
    if is_key_pressed(KEY.L) then
        pause()
    end

    if is_key_pressed(KEY.P) then
        resume()
    end
end 