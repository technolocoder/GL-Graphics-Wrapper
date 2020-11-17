#version 440 core

in VS_OUT{
    vec2 texture_coords;
} fs_in;

struct Material{
    sampler2D diffuse1;
};
uniform Material material;

out vec4 color;
void main(){
    color = texture(material.diffuse1,fs_in.texture_coords);
}