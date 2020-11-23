#version 420 core

layout(location=0) in vec3 position;
layout(location=1) in vec3 normal;
layout(location=2) in vec2 texture_coords;
layout(location=3) in vec3 tangent;
layout(location=4) in vec3 bitangent;

out VS_OUT{
    vec3 normal,frag_coord;
    vec2 texture_coords;
    mat3 TBN;
} vs_out;

layout(std140,binding=0) uniform Matrices{
    mat4 projection,view;
};
uniform mat4 model;

void main(){
    gl_Position = projection * view * model * vec4(position,1.0);
    vs_out.texture_coords = texture_coords;
    vs_out.normal = mat3(transpose(inverse(model))) * normal;
    vs_out.frag_coord = vec3(model * vec4(position,1.0));

    vec3 T = normalize(vec3(model * vec4(tangent,0.0)));
    vec3 B = normalize(vec3(model * vec4(bitangent,0.0)));
    vec3 N = normalize(vec3(model * vec4(normal,0.0)));
    vs_out.TBN = mat3(T,B,N); 
}