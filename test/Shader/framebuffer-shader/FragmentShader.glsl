#version 420
uniform sampler2D tex;
in VS_OUT{
    vec2 texture_coords;
} fs_in;

out vec4 color;
void main(){
    float exposure = 1.0;
    vec3 c = texture(tex,fs_in.texture_coords).rgb;
    c = vec3(1.0)-exp(-c*vec3(exposure));
    c = pow(c,vec3(1.0/2.2));
    color = vec4(c,1.0);
}