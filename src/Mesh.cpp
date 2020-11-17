#include "Mesh.hpp"

Vertex::Vertex() {}
Vertex::Vertex(glm::vec3 _position ,glm::vec3 _normal ,glm::vec2 _texture_coords) {
    position = _position;
    normal = _normal;
    texture_coords = _texture_coords;
}

Mesh::Mesh(){}

void Mesh::initialize(const std::vector<Vertex> &_vertices ,const std::vector<Texture> &_textures ,const std::vector<unsigned int> &_indices){
    vertices = _vertices;
    indices = _indices;
    textures = _textures;
    
    glGenBuffers(1,&vbo);
    glBindBuffer(GL_ARRAY_BUFFER,vbo);
    glBufferData(GL_ARRAY_BUFFER,sizeof(Vertex)*vertices.size(),&vertices[0],GL_STATIC_DRAW);

    glGenVertexArrays(1,&vao);
    glBindVertexArray(vao);

    glGenBuffers(1,&ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(unsigned int)*indices.size(),&indices[0],GL_STATIC_DRAW);

    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,sizeof(Vertex),(const void*)0);
    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,sizeof(Vertex),(const void*)offsetof(Vertex,normal));
    glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE,sizeof(Vertex),(const void*)offsetof(Vertex,texture_coords));

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
}

Mesh::Mesh(const std::vector<Vertex> &_vertices ,const std::vector<Texture> &_textures ,const std::vector<unsigned int> &_indices):vertices(_vertices),textures(_textures),indices(_indices){
    glGenBuffers(1,&vbo);
    glBindBuffer(GL_ARRAY_BUFFER,vbo);
    glBufferData(GL_ARRAY_BUFFER,sizeof(Vertex)*vertices.size(),&vertices[0],GL_STATIC_DRAW);

    glGenVertexArrays(1,&vao);
    glBindVertexArray(vao);

    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,sizeof(Vertex),(const void*)0);
    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,sizeof(Vertex),(const void*)offsetof(Vertex,normal));
    glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE,sizeof(Vertex),(const void*)offsetof(Vertex,texture_coords));

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    glGenBuffers(1,&ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(unsigned int)*indices.size(),&indices[0],GL_STATIC_DRAW);
}

void Mesh::draw(Program program){
    program.use();
    glBindVertexArray(vao);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,ebo);

    glDrawElements(GL_TRIANGLES,indices.size(),GL_UNSIGNED_INT,(const void*)0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
    glBindVertexArray(0);
}

void Mesh::set_texture_uniforms(Program program){
    int specular_count=1,diffuse_count=1;
    for(int i = 0; i < textures.size(); ++i){
        glActiveTexture(GL_TEXTURE0+i);
        textures[i].bind(); 
        program.set_int((textures[i].get_texture_type()==TEXTURE_DIFFUSE?"material.diffuse"+std::to_string(diffuse_count++):"material.specular"+std::to_string(specular_count++)).c_str(),i);
    }
    glActiveTexture(GL_TEXTURE0);
}

Mesh::Mesh(const Mesh &cpy){
    vbo = cpy.vbo;
    ebo = cpy.ebo;
    vao = cpy.vao;

    vertices = cpy.vertices;
    textures = cpy.textures;
    indices = cpy.indices;
}