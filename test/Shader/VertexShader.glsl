#version 420 core

layout(location=0) in vec3 position;
layout(location=1) in vec3 normal;
layout(location=2) in vec2 texture_coords;

out VS_OUT{
    vec2 texture_coords;
} vs_out;

uniform mat4 projection;
uniform mat4 view;

void main(){
    gl_Position = projection * view * vec4(position,1.0);
    vs_out.texture_coords = texture_coords;
}