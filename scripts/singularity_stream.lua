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
	for i = 1, 2000 do
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
	local count = 6000
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
	for i = 1, 2000 do
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

function draw()
	begin_blend_mode(BLEND.ADDITIVE)
end

function update()
	if is_key_pressed(KEY.L) then
		pause()
	end
	if is_key_pressed(KEY.P) then
		resume()
	end

	time = time + 0.01

	--------------------------------------------------
	-- INNER CORE (TIGHT CHAOS)
	--------------------------------------------------
	for i = 1, #inner do
		local p = inner[i]

		local speed = 0.02 + (i % 7) * 0.001
		p.angle = p.angle + speed

		local r = p.r + math.sin(time * 6 + i) * 3
		local y = math.sin(time * 8 + i * 3) * 4

		set_planet_position(p.id, {
			math.cos(p.angle) * r,
			y,
			math.sin(p.angle) * r,
		})
	end

	--------------------------------------------------
	-- DISK (DRIFT + BEAM CAPTURE)
	--------------------------------------------------
	for i = 1, #disk do
		local p = disk[i]

		local drift = math.sin(time * 0.5 + i) * 0.001
		p.angle = p.angle + 0.002 + drift

		local noise = math.sin(time + i * 2) * 6

		local x = math.cos(p.angle) * (p.r + noise)
		local z = math.sin(p.angle) * (p.r + noise)
		local y = math.sin(time * 2 + i) * 4

		--------------------------------------------------
		-- BEAM CAPTURE ZONE
		--------------------------------------------------
		local dist = math.sqrt(x * x + z * z)

		if dist < 80 then
			local t = 1 - (dist / 80)

			-- inward pull
			x = x * (1 - 0.05 * t)
			z = z * (1 - 0.05 * t)

			-- spiral acceleration
			p.angle = p.angle + 0.015 * t

			-- vertical lift
			y = y + t * 15
		end

		set_planet_position(p.id, { x, y, z })
	end

	--------------------------------------------------
	-- BEAM (SPIRAL + COMPRESSION + FLOW)
	--------------------------------------------------
	for i = 1, #jets do
		local j = jets[i]

		local h = j.h
		local d = math.abs(h)

		-- spiral speed varies
		local a = time * (4 + (i % 5) * 0.4) + i

		-- cone shape (tight near center)
		local radius = 10 * (d / 800)

		-- spiral motion
		local x = math.cos(a) * radius
		local z = math.sin(a) * radius

		-- instability (important)
		x = x + math.sin(time * 5 + i) * 1.5
		z = z + math.cos(time * 5 + i) * 1.5

		-- compression toward axis
		x = x * 0.92
		z = z * 0.92

		--------------------------------------------------
		-- FLOW SPEED (acceleration near center)
		--------------------------------------------------
		local speed = 3 + (1 - d / 800) * 6

		local y_up = h + speed
		local y_down = -h - speed

		--------------------------------------------------
		-- EJECTION BURSTS
		--------------------------------------------------
		if i % 40 == 0 then
			x = x * 2.5
			z = z * 2.5
		end

		set_planet_position(j.id1, { x, y_up, z })
		set_planet_position(j.id2, { x, y_down, z })
	end
end
