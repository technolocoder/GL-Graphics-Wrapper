#include "Texture.hpp"

void Texture::initialize(const char *filename ,void additional_parameters() ,int channel_count,bool gamma_corrected){
    int width,height,channels;
    unsigned char *image = stbi_load(filename,&width,&height,&channels,channel_count);

    if(image==NULL){
        std::cerr << "Error opening file named: " << filename << '\n';
        exit(1);
    }

    glGenTextures(1,&texture_id);
    glBindTexture(GL_TEXTURE_2D,texture_id);
    additional_parameters();

    int internal_format,format;
    if(gamma_corrected){
        if(channel_count == 1){
            internal_format = GL_R;
            format = GL_R;
        }else if(channel_count == 3){
            internal_format = GL_SRGB;
            format = GL_RGB;
        }else if(channel_count == 4){
            internal_format = GL_SRGB_ALPHA;
            format = GL_RGBA;
        }else{
            std::cerr << "Invalid channel count\n";
            exit(1);
        }
    }else{
        if(channel_count == 1){
            internal_format = GL_R;
            format = GL_R;
        }else if(channel_count == 3){
            internal_format = GL_RGB;
            format = GL_RGB;
        }else if(channel_count == 4){
            internal_format = GL_RGBA;
            format = GL_RGBA;
        }else{
            std::cerr << "Invalid channel count\n";
            exit(1);
        }
    }

    glTexImage2D(GL_TEXTURE_2D,0,internal_format,width,height,0,format,GL_UNSIGNED_BYTE,image);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(image);
}

Texture::Texture(const char *filename ,void additional_parameters() ,int channel_count,bool gamma_corrected){
    initialize(filename,additional_parameters,channel_count,gamma_corrected);
}

Texture::Texture(const char *filename ,void additional_parameters() ,int channel_count){
    initialize(filename,additional_parameters,channel_count,false);
}

Texture::Texture(const char *filename ,void additional_parameters()){
    initialize(filename,additional_parameters,3,false);
}

Texture::Texture(const char *filename){
    initialize(filename,[](){
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    },3,false);
}

GLuint Texture::operator()(){
    return texture_id;
}

int Texture::get_texture_type(){
    return texture_type;
}

void Texture::set_texture_type(int _texture_type){
    texture_type = _texture_type;
}

void Texture::bind(){
    glBindTexture(GL_TEXTURE_2D,texture_id);
}