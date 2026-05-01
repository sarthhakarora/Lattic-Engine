#version 330

in vec3 fragPosition;
out vec4 fragColor;

uniform float time;

float hash(vec3 p)
{
    return fract(sin(dot(p, vec3(127.1,311.7,74.7))) * 43758.5453);
}

void main()
{
    vec3 dir = normalize(fragPosition);

    // -------------------------
    // BASE COLOR (dark + subtle gradient)
    // -------------------------
    float t = dir.y * 0.5 + 0.5;

    vec3 col = mix(
        vec3(0.003, 0.0, 0.01),   // deep purple
        vec3(0.0, 0.0, 0.01),     // dark blue
        t
    );

    // -------------------------
    // GALAXY BAND (structure)
    // -------------------------
    float band = pow(1.0 - abs(dir.y), 6.0);
    col += band * vec3(0.08, 0.03, 0.12) * 0.2;

    // -------------------------
    // STAR DENSITY FIELD (clustering)
    // -------------------------
    float density = hash(dir * 5.0);

    // -------------------------
    // MAIN STARS (rare + clustered)
    // -------------------------
    float n = hash(dir * 137.0);

    float threshold = 0.9992 - density * 0.001;  
    float star = step(threshold, n);

    float twinkle = 0.85 + 0.15 * sin(time * 1.5 + n * 40.0);

    // color variation
    vec3 starColor = mix(
        vec3(1.0, 0.9, 0.8),   // warm
        vec3(0.7, 0.8, 1.0),   // cool
        hash(dir * 50.0)
    );

    col += star * twinkle * starColor * 2.0;

    // -------------------------
    // SMALL STARS (very subtle fill)
    // -------------------------
    float n2 = hash(dir * 57.0);
    float smallStars = step(0.9987 - density * 0.0005, n2);

    col += smallStars * 0.25;

    // -------------------------
    // SUBTLE CENTER BIAS (ties to scene)
    // -------------------------
    vec3 forward = vec3(0.0, 0.0, 1.0); // adjust if needed
    float centerGlow = pow(max(dot(dir, forward), 0.0), 4.0);

    col += centerGlow * vec3(0.04, 0.015, 0.01);

    // -------------------------
    // FINAL
    // -------------------------
    fragColor = vec4(col, 1.0);
}
