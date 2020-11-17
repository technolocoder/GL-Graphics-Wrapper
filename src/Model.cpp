 #include "Model.hpp"

void Model::draw(Program &program){
    program.use();

    for(int i = 0; i < meshes.size(); ++i){
        meshes[i].set_texture_uniforms(program);
        meshes[i].draw(program);
    }
}

void Model::set_texture_uniforms(Program &program){
    program.use();
    for(int i = 0; i < meshes.size(); ++i){
        meshes[i].set_texture_uniforms(program);
    }
}

void Model::draw_mesh(Program &program,int mesh_index){
    meshes[mesh_index].set_texture_uniforms(program);
    meshes[mesh_index].draw(program);
}

Model::Model() {}

Model::Model(const std::string &path){
    initialize(path,false);
}

Model::Model(const std::string &path ,bool gamma_corrected){
    initialize(path,gamma_corrected);
}

void Model::initialize(const std::string &path){
    initialize(path,false);
}

void Model::initialize(const std::string &path ,bool gamma_corrected){
    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(path,aiProcess_Triangulate|aiProcess_FlipUVs);

    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode){
        std::cerr << "Error loading assimp: " << importer.GetErrorString() << '\n';
        exit(1);
    }
    directory = path.substr(0,path.find_last_of('/'));
    process_node(scene->mRootNode,scene,gamma_corrected);
}

void Model::process_node(aiNode *node ,const aiScene *scene ,bool gamma_corrected){
    for(int i = 0; i < node->mNumMeshes; ++i){
        meshes.push_back(process_mesh(scene->mMeshes[node->mMeshes[i]],scene,gamma_corrected));
    }

    for(int i = 0; i < node->mNumChildren; ++i){
        process_node(node->mChildren[i],scene,gamma_corrected);
    }

}

Mesh Model::process_mesh(aiMesh *mesh ,const aiScene *scene ,bool gamma_corrected){   
    std::vector<Vertex> vertices(mesh->mNumVertices);
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    
    if(mesh->mTextureCoords[0]){
        for(int i = 0; i < mesh->mNumVertices; ++i){
            vertices[i].position = glm::vec3(mesh->mVertices[i].x,mesh->mVertices[i].y,mesh->mVertices[i].z); 
            vertices[i].normal = glm::vec3(mesh->mNormals[i].x,mesh->mNormals[i].y,mesh->mNormals[i].z);
            vertices[i].texture_coords = glm::vec2(mesh->mTextureCoords[0][i].x,mesh->mTextureCoords[0][i].y);
        }
    }else{
        for(int i = 0; i < mesh->mNumVertices; ++i){
            vertices[i].position = glm::vec3(mesh->mVertices[i].x,mesh->mVertices[i].y,mesh->mVertices[i].z); 
            vertices[i].normal = glm::vec3(mesh->mNormals[i].x,mesh->mNormals[i].y,mesh->mNormals[i].z);
            vertices[i].texture_coords = glm::vec2(0.0f,0.0f);
        }
    }

    for(int i = 0; i < mesh->mNumFaces; ++i){
        aiFace face = mesh->mFaces[i];
        for(int j = 0; j < face.mNumIndices; ++j){
            indices.push_back(face.mIndices[j]);
        }
    }

    if(mesh->mMaterialIndex >= 0){
        aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
        load_material_textures(material,aiTextureType_DIFFUSE,TEXTURE_DIFFUSE,textures,false);
        //load_material_textures(material,aiTextureType_SPECULAR,TEXTURE_SPECULAR,textures,gamma_corrected);
    }
    
    return Mesh(vertices,textures,indices);
}

void Model::load_material_textures(aiMaterial *material ,aiTextureType texture_type,int texture_type_name , std::vector<Texture> &textures,bool gamma_corrected){
    for(int i = 0; i < material->GetTextureCount(texture_type); ++i){
        aiString str;
        material->GetTexture(texture_type,i,&str);

        std::string texture_path = directory + '/' + str.C_Str();
        if(loaded_textures.find(texture_path)==loaded_textures.end()){
            Texture tex(texture_path.c_str(),[](){
                glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
                glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
                glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
            },3,gamma_corrected);
            tex.set_texture_type(texture_type_name);
            loaded_textures[texture_path] = tex;
            textures.push_back(tex);
        }else{
            textures.push_back(loaded_textures[texture_path]);
        }
    }
}
