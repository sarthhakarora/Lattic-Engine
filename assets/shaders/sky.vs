#version 330

in vec3 vertexPosition;

uniform mat4 mvp;

out vec3 fragPosition;

void main()
{
    fragPosition = vertexPosition;
    gl_Position = mvp * vec4(vertexPosition, 1.0);
}
