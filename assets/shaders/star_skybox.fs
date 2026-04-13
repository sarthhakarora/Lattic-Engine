#version 330

in vec3 dir;
out vec4 fragColor;

// hash function (public-domain style)
float hash(vec3 p)
{
    p = fract(p * 0.3183099 + vec3(0.1, 0.2, 0.3));
    p *= 17.0;
    return fract(p.x * p.y * p.z * (p.x + p.y + p.z));
}

void main()
{
    vec3 d = normalize(dir);

    // star density control
    float density = 0.995; // higher = fewer stars
    float h = hash(d * 500.0);

    float star = step(density, h);

    // brightness variation
    float brightness = hash(d * 2000.0);
    star *= brightness * 1.5;

    // subtle blue tint
    vec3 color = vec3(1.0, 1.0, 1.0) * star;
    color += vec3(0.05, 0.07, 0.1) * star;

    fragColor = vec4(color, 1.0);
}