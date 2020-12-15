#include "Shader.hpp"

Shader::Shader() {}
Shader::Shader(const Shader &shader){
    shader_id = shader.shader_id;
}

Shader::Shader(const char *filepath ,const GLuint shader_type){
    initialize(filepath ,shader_type);
}

void Shader::initialize(const char *filepath ,const GLuint shader_type){
    shader_id = glCreateShader(shader_type);
    std::ifstream file(filepath);

    if(!file.is_open()){
        std::cerr << "Error opening file named: " << filepath << '\n';
        exit(1);
    }

    std::ostringstream oss;
    oss << file.rdbuf();
    std::string str = oss.str();
    const char *src_str = str.c_str();
    int str_size = str.size();

    glShaderSource(shader_id,1,(const char**)&src_str,&str_size);
    glCompileShader(shader_id);

    int compile_success;
    glGetShaderiv(shader_id,GL_COMPILE_STATUS,&compile_success);

    if(!compile_success){
        std::cerr << "Error compiling shader named: " << filepath << '\n';
        char log[2048];
        glGetShaderInfoLog(shader_id,2048*sizeof(char),NULL,log);
        std::cerr << "Error:\n" << log << '\n';
        exit(1);
    }
}

GLuint Shader::operator()(){
    return shader_id;
}