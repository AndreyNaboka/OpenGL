#version 330 core
layout(location = 0) in vec3 vertexPosition_modelspace;
uniform mat4 mvp;
uniform float scale;
void main() {
	vec4 v = vec4(vertexPosition_modelspace.x * scale,
                  vertexPosition_modelspace.y * scale,
                  vertexPosition_modelspace.z,
                  1.0f);
    gl_Position = mvp * v;
}