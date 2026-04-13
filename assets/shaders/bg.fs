#version 330

out vec4 fragColor;

uniform vec2 resolution;
uniform float time;

// simple hash
float hash(vec2 p)
{
    return fract(sin(dot(p, vec2(127.1,311.7))) * 43758.5453);
}

void main()
{
    vec2 uv = gl_FragCoord.xy / resolution;

    // -------------------------
    // BASE COLOR
    // -------------------------
    vec3 col = vec3(0.01, 0.0, 0.03);

    // -------------------------
    // BLOB (CENTERED ON TITLE)
    // -------------------------
    vec2 center = vec2(0.5, 0.75);
    vec2 p = uv - center;

    // -------------------------
    // ORGANIC BLOB (GOOD VERSION)
    // -------------------------

    // scale space
    vec2 bp = p * 2.0;

    // layered distortion
    float n = 0.0;
    n += sin(bp.x * 2.5 + time * 0.6) * 0.15;
    n += sin(bp.y * 2.0 - time * 0.5) * 0.15;
    n += sin((bp.x + bp.y) * 2.0 + time * 0.4) * 0.1;

    // warped distance
    float distBlob = length(bp + n);

    // soft irregular shape
    float blob = smoothstep(0.55 + n * 0.2, 0.2, distBlob);

    vec3 purple = vec3(0.4, 0.0, 0.7);
    col += blob * purple * 0.20;

    // -------------------------
    // STARS (NO RNG FAILURES)
    // -------------------------
    float stars = 0.0;

    vec2 grid = uv * 120.0;   // density
    vec2 cell = floor(grid);
    vec2 f = fract(grid);

    // random position inside each cell
    vec2 starPos = vec2(
        hash(cell),
        hash(cell + 1.0)
    );

    float d = length(f - starPos);

    // hard shape → guaranteed visible
    float star = smoothstep(0.02, 0.0, d);

    // twinkle
    float t = sin(time * 2.0 + hash(cell) * 20.0) * 0.5 + 0.5;

    stars = star * t;

    // strong add
    col += stars * vec3(1.8);

    // -------------------------
    // SOFT VIGNETTE
    // -------------------------
    float v = smoothstep(2.0, 0.5, length(p));
    col *= v;

    // -------------------------
    // FINAL
    // -------------------------
    fragColor = vec4(col, 1.0);
}
