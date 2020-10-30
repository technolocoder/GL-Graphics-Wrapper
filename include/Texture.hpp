#ifndef GLW_TEXTURE
#define GLW_TEXTURE

#include "stb_image.h"
#include <GL/glew.h>
#include <iostream>

enum TEXTURE_TYPES {TEXTURE_DIFFUSE ,TEXTURE_SPECULAR};

class Texture{
public:
    void initialize(const char *filename ,void additional_parameters() ,int channel_count ,bool gamma_corrected);

    Texture(const char *filename ,void additional_parameters() ,int channel_count,bool gamma_corrected);
    Texture(const char *filename ,void additional_parameters() ,int channel_count);
    Texture(const char *filename ,void additional_parameters());
    Texture(const char *filename);

    GLuint operator()();
    
    void set_texture_type(int _texture_type);
    int get_texture_type();

    void bind();
private:
    GLuint texture_id;
    int texture_type;
};

#endif