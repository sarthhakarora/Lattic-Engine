function init()
    create_world()

    -- CENTRAL BLACK HOLE (static gravity)
    add_planet{
        name = "sagittarius_a",
        texturepath = "assets/graphics/sun.jpg", -- placeholder visual
        radius = 35,
        position = {0, 0, 0},
        rings = 64,
        slices = 64,
        mass = 50000,
        has_gravity = true
    }

    -- INNER BULGE (dense core stars)
    local bulge_count = 1200
    local bulge_radius = 120

    for i = 1, bulge_count do
        local a = math.random() * math.pi * 2
        local r = math.random() * bulge_radius
        local y = math.random(-15, 15)

        add_planet{
            name = "bulge_" .. i,
            texturepath = "assets/graphics/ceres.jpg",
            radius = 2,
            position = {
                math.cos(a) * r,
                y,
                math.sin(a) * r
            },
            rings = 6,
            slices = 6,
            mass = 1,
            has_gravity = false
        }
    end

    -- GALACTIC DISK
    local disk_count = 5000
    local disk_inner = 150
    local disk_outer = 900
    local disk_thickness = 25

    for i = 1, disk_count do
        local a = math.random() * math.pi * 2

        -- bias radius outward for flat disk look
        local t = math.random()
        local r = disk_inner + (disk_outer - disk_inner) * (t * t)

        local y = math.random(-disk_thickness, disk_thickness)

        add_planet{
            name = "star_" .. i,
            texturepath = "assets/graphics/makemake.jpg",
            radius = 1.8,
            position = {
                math.cos(a) * r,
                y,
                math.sin(a) * r
            },
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