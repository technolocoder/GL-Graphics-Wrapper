#version 420 core

in VS_OUT{
    vec2 texture_coords;
} fs_in;

uniform sampler2D tex;
out vec4 color;
void main(){
    color = texture(tex,fs_in.texture_coords);
}