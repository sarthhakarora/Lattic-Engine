#version 330

in vec3 vertexPosition;

out vec3 dir;

uniform mat4 matProjection;
uniform mat4 matView;

void main()
{
    dir = vertexPosition;

    // remove translation from view matrix
    mat4 rotView = mat4(mat3(matView));
    vec4 pos = matProjection * rotView * vec4(vertexPosition, 1.0);

    gl_Position = pos.xyww; // force depth to far plane
}