create_world()

add_planet{
    name = "sun",
    texturepath = "../assets/graphics/sun.jpg",
    radius = 30,
    position = {0, 0, 0},
    rings = 32,
    slices = 32,
    mass = 1000,
    has_gravity = true
}

add_planet{
    name = "earth",
    texturepath = "../assets/graphics/earth.jpg",
    radius = 10,
    position = {200, 0, 0},
    rings = 32,
    slices = 32,
    mass = 1,
    has_gravity = false
}