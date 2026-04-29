function init()
	create_world()

	set_world_gravity_constant(0.15)

	-- MULTIPLE MASSIVE CORES (creates chaos)
	for i = 1, 3 do
		local angle = (i / 3) * math.pi * 2
		add_planet({
			name = "core_" .. i,
			texturepath = "assets/graphics/sun.jpg",
			radius = 25,
			position = {
				math.cos(angle) * 100,
				0,
				math.sin(angle) * 100,
			},
			rings = 32,
			slices = 32,
			mass = 20000,
			has_gravity = true,
		})
	end

	-- SPIRAL STREAMS (this is the sauce)
	local count = 8000

	for i = 1, count do
		local t = i / count * math.pi * 6 -- spiral twist
		local r = 50 + i * 0.1

		local x = math.cos(t) * r
		local z = math.sin(t) * r
		local y = math.sin(t * 0.5) * 10

		add_planet({
			name = "particle_" .. i,
			texturepath = "assets/graphics/makemake.jpg",
			radius = 1.5,
			position = { x, y, z },
			rings = 4,
			slices = 4,
			mass = 1,
			has_gravity = false,
		})
	end
end

function draw() end

function update()
	if is_key_pressed(KEY.L) then
		pause()
	end
	if is_key_pressed(KEY.P) then
		resume()
	end
end
