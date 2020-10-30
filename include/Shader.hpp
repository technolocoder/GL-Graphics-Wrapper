#ifndef GLW_SHADER
#define GLW_SHADER

#include <GL/glew.h>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader{
public:
    Shader();
    Shader(const Shader &shader);
        
    Shader(const char *filepath ,const GLuint shader_type);
    void initialize(const char *filepath ,const GLuint shader_type);

    GLuint operator()();
private:
    GLuint shader_id;
};

#endif