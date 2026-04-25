function init()
	create_world()

	set_world_gravity_constant(0.0)

	local count = 8000
	local radius = 400
	local thickness = 20

	for i = 1, count do
		local angle = (i / count) * math.pi * 2

		-- slight radial variation (ring thickness)
		local r = radius + (math.random() - 0.5) * thickness

		-- subtle vertical wobble (aura feel)
		local y = math.sin(angle * 10) * 5 + (math.random() - 0.5) * 3

		add_planet({
			name = "ring_" .. i,
			texturepath = "assets/graphics/venus.jpg",
			radius = 1.2,
			position = {
				math.cos(angle) * r,
				y,
				math.sin(angle) * r,
			},
			rings = 4,
			slices = 4,
			mass = 1,
			has_gravity = false,
		})
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
