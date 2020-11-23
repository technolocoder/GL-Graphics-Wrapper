#version 440 core

in VS_OUT{
    vec3 normal,frag_coord;
    vec2 texture_coords;
    mat3 TBN;
} fs_in;

struct Material{
    sampler2D diffuse1,specular1,normal1;
};
uniform Material material;

layout(std140,binding=1) uniform light_data{
    vec3 ambient_strength,diffuse_strength,specular_strength;
    vec3 camera_position,light_position;
};

out vec4 color;
void main(){
    vec3 diffuse_texture = texture(material.diffuse1,fs_in.texture_coords).rgb;
    vec3 specular_texture = texture(material.specular1,fs_in.texture_coords).rgb;
    vec3 normal_texture = texture(material.normal1,fs_in.texture_coords).rgb;
    normal_texture = normal_texture * 2.0 - 1.0;
    vec3 norm;

    norm = normalize(fs_in.TBN * normal_texture);

    vec3 light_dir = normalize(light_position - fs_in.frag_coord);
    vec3 camera_dir = normalize(camera_position - fs_in.frag_coord);
    vec3 halfway_vec = normalize(light_dir + camera_dir);

    vec3 ambient = diffuse_texture * ambient_strength;

    float diff = max(dot(light_dir,norm),0.0);
    vec3 diffuse = diffuse_texture * diff * diffuse_strength;

    float spec = pow(max(dot(norm,halfway_vec),0.0),32.0);
    vec3 specular = spec * specular_texture * specular_strength;

    vec3 final_color = ambient+diffuse+specular;

    color = vec4(final_color,1.0);
}