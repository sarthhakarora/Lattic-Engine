world_create {}

add_planet {
    name = "sun",
    mass = 1000,
    radius = 30,
    position = vec3(0, 0, 0),
    has_gravity = true
}

add_planet {
    name = "earth",
    mass = 1,
    radius = 10,
    position = vec3(200, 0, 0),
    has_gravity = false
}
