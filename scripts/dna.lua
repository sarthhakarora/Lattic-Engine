function init()
    create_world()

    -- single gravity core
    add_planet{
        name = "core",
        texturepath = "assets/graphics/sun.jpg",
        radius = 25,
        position = {0, 0, 0},
        rings = 48,
        slices = 48,
        mass = 3000,
        has_gravity = true
    }

    local count = 1000
    local base_radius = 220
    local radius_offset = 8
    local height_step = 0.6
    local twist = 0.45

    for i = 1, count do
        local angle = i * twist
        local y = (i - count/2) * height_step

        -- strand A
        add_planet{
            name = "dna_a_" .. i,
            texturepath = "assets/graphics/eris.jpg",
            radius = 2.5,
            position = {
                math.cos(angle) * (base_radius + radius_offset),
                y,
                math.sin(angle) * (base_radius + radius_offset)
            },
            rings = 8,
            slices = 8,
            mass = 1,
            has_gravity = false
        }

        -- strand B (phase shifted)
        add_planet{
            name = "dna_b_" .. i,
            texturepath = "assets/graphics/neptune.jpg",
            radius = 2.5,
            position = {
                math.cos(angle + math.pi) * (base_radius - radius_offset),
                y,
                math.sin(angle + math.pi) * (base_radius - radius_offset)
            },
            rings = 8,
            slices = 8,
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