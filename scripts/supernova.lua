function init()
	create_world()

	set_world_gravity_constant(0.1)

	explosion_radius = 0
	spawn_rate = 250
	max_radius = 900

	id_counter = 0
	total_particles = 0
	max_particles = 15000

	-- CORE
	add_planet({
		name = "core",
		texturepath = "assets/graphics/sun.jpg",
		radius = 30,
		position = { 0, 0, 0 },
		rings = 32,
		slices = 32,
		mass = 60000,
		has_gravity = true,
	})
end

function draw() end

function update()
	if is_key_pressed(KEY.L) then
		pause()
	end
	if is_key_pressed(KEY.P) then
		resume()
	end

	if explosion_radius < max_radius and total_particles < max_particles then
		-- faster expansion
		explosion_radius = explosion_radius + 18

		for i = 1, spawn_rate do
			id_counter = id_counter + 1
			total_particles = total_particles + 1

			local theta = math.random() * math.pi * 2
			local phi = math.acos(2 * math.random() - 1)

			local dir = {
				math.sin(phi) * math.cos(theta),
				math.sin(phi) * math.sin(theta),
				math.cos(phi),
			}

			-- 🔥 asymmetric explosion (this makes it look real)
			local stretch = 1 + math.sin(theta * 3) * 0.5
			local r = explosion_radius * stretch + math.random() * 60

			-- 🔥 streak effect (elongated along direction)
			local offset = math.random() * 50

			local x = dir[1] * (r + offset)
			local y = dir[2] * (r * 0.6) -- flatten slightly
			local z = dir[3] * (r + offset)

			-- texture variety
			local textures = {
				"assets/graphics/sun.jpg",
				"assets/graphics/mars.jpg",
				"assets/graphics/venus.jpg",
				"assets/graphics/mercury.jpg",
				"assets/graphics/eris.jpg",
			}

			add_planet({
				name = "p_" .. id_counter,
				texturepath = textures[math.random(1, #textures)],
				radius = 1.0 + math.random() * 1.2,
				position = { x, y, z },
				rings = 4,
				slices = 4,
				mass = 1,
				has_gravity = false,
			})
		end
	end

	-- kill gravity early so explosion flies outward cleanly
	if explosion_radius > 120 then
		set_world_gravity_constant(0.0)
	end
end
