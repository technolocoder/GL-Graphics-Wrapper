#ifndef GLW_MESH
#define GLW_MESH

#include "Texture.hpp"
#include "Program.hpp"
#include <glm/glm.hpp>
#include <vector>

struct Vertex{
    glm::vec3 position,normal;
    glm::vec2 texture_coords;
};

class Mesh{
public:
    Mesh(const std::vector<Vertex> &_vertices ,const std::vector<Texture> &_textures ,const std::vector<unsigned int> &_indices);    
    void draw(Program program);
    void set_texture_uniforms(Program program);
private:
    GLuint vbo,vao,ebo;    
    std::vector<Vertex> vertices;
    std::vector<Texture> textures;
    std::vector<unsigned int> indices;
};

#endif