#ifndef GLW_MODEL
#define GLW_MODEL

#include "Program.hpp"
#include "Mesh.hpp"
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

class Model{
public: 
    Model();

    Model(const std::string &filepath);
    Model(const std::string &filepath ,const bool gamma_corrected);

    void initialize(const std::string &filepath); 
    void initialize(const std::string &filepath ,bool gamma_corrected);

    void draw(Program &program);
    void draw_mesh(Program &program ,int mesh_index);

    void set_texture_uniforms(Program &program);
private:
    std::vector<Mesh> meshes;
    std::string directory;
    std::map<std::string,Texture> loaded_textures;

    void process_node(aiNode *node ,const aiScene *scene ,bool gamma_corrected);
    Mesh process_mesh(aiMesh *mesh ,const aiScene *scene ,bool gamma_corrected);
    void load_material_textures(aiMaterial *material ,aiTextureType texture_type,int texture_type_name , std::vector<Texture> &textures ,bool gamma_corrected);   
};

#endif