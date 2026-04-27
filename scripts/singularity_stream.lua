function init()
	create_world()
	set_world_gravity_constant(0.0)

	time = 0

	--------------------------------------------------
	-- CORE
	--------------------------------------------------
	add_planet({
		name = "core",
		texturepath = "assets/graphics/sun.jpg",
		radius = 18,
		position = { 0, 0, 0 },
		rings = 32,
		slices = 32,
		mass = 50000,
		has_gravity = false,
	})

	--------------------------------------------------
	-- STORAGE
	--------------------------------------------------
	inner = {}
	disk = {}
	jets = {}

	--------------------------------------------------
	-- INNER RING
	--------------------------------------------------
	for i = 1, 1000 do
		local a = math.random() * math.pi * 2
		local r = 50 + math.random() * 10

		local id = add_planet({
			name = "inner_" .. i,
			texturepath = "assets/graphics/sun.jpg",
			radius = 1.2,
			position = { math.cos(a) * r, 0, math.sin(a) * r },
			rings = 4,
			slices = 4,
			mass = 1,
			has_gravity = false,
		})

		inner[i] = { id = id, angle = a, r = r }
	end

	--------------------------------------------------
	-- DISK
	--------------------------------------------------
	local count = 3000
	for i = 1, count do
		local a = math.random() * math.pi * 2
		local t = math.random()
		local r = 80 + (500 - 80) * (t * t)

		local id = add_planet({
			name = "disk_" .. i,
			texturepath = "assets/graphics/mars.jpg",
			radius = 1.5,
			position = { math.cos(a) * r, 0, math.sin(a) * r },
			rings = 4,
			slices = 4,
			mass = 1,
			has_gravity = false,
		})

		disk[i] = { id = id, angle = a, r = r }
	end

	--------------------------------------------------
	-- JETS
	--------------------------------------------------
	for i = 1, 1000 do
		local h = math.random() * 800
		local spread = math.random() * 10

		local id1 = add_planet({
			name = "jet_up_" .. i,
			texturepath = "assets/graphics/venus.jpg",
			radius = 1.2,
			position = { 0, h, 0 },
			rings = 4,
			slices = 4,
			mass = 1,
			has_gravity = false,
		})

		local id2 = add_planet({
			name = "jet_down_" .. i,
			texturepath = "assets/graphics/venus.jpg",
			radius = 1.2,
			position = { 0, -h, 0 },
			rings = 4,
			slices = 4,
			mass = 1,
			has_gravity = false,
		})

		jets[i] = { id1 = id1, id2 = id2, h = h, spread = spread }
	end
end

function update()
	begin_blend_mode(1)
	if is_key_pressed(KEY.L) then
		pause()
	end
	if is_key_pressed(KEY.P) then
		resume()
	end

	time = time + 0.01

	--------------------------------------------------
	-- INNER RING (FAST)
	--------------------------------------------------
	for i = 1, #inner do
		local p = inner[i]
		p.angle = p.angle + 0.01

		set_planet_position(p.id, {
			math.cos(p.angle) * p.r,
			math.sin(time * 2) * 2,
			math.sin(p.angle) * p.r,
		})
	end

	--------------------------------------------------
	-- DISK (SLOWER)
	--------------------------------------------------
	for i = 1, #disk do
		local p = disk[i]
		p.angle = p.angle + 0.002

		set_planet_position(p.id, {
			math.cos(p.angle) * p.r,
			math.sin(p.angle * 3 + time) * 3,
			math.sin(p.angle) * p.r,
		})
	end

	--------------------------------------------------
	-- JETS (WAVY MOTION)
	--------------------------------------------------
	for i = 1, #jets do
		local j = jets[i]

		local x = math.sin(time * 2 + i) * j.spread
		local z = math.cos(time * 2 + i) * j.spread

		set_planet_position(j.id1, { x, j.h, z })
		set_planet_position(j.id2, { x, -j.h, z })
	end
	end_blend_mode()
end
