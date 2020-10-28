#include "Program.hpp"

Program::Program() {}

void Program::initialize(){
    program_id = glCreateProgram();
}

void Program::attachShader(const char *filepath ,GLuint shader_type){
    Shader shader(filepath,shader_type);
    glAttachShader(program_id,shader());
}

void Program::attachShader(Shader &shader){
    glAttachShader(program_id,shader());
}

void Program::link(){
    glLinkProgram(program_id);
}

void Program::use(){
    glUseProgram(program_id);
}

GLuint Program::operator()(){
    return program_id;
}

void Program::set_float(const char *var_name ,const float value){
    glUniform1f(get_uniform_location(var_name),value);
}

void Program::set_double(const char *var_name ,const double value){
    glUniform1d(get_uniform_location(var_name),value);
}

void Program::set_int(const char *var_name ,const int value){
    glUniform1i(get_uniform_location(var_name),value);
}

void Program::set_vec2(const char *var_name ,const glm::vec2 value){
    glUniform2fv(get_uniform_location(var_name),1,glm::value_ptr(value));
}

void Program::set_vec3(const char *var_name ,const glm::vec3 value){
    glUniform3fv(get_uniform_location(var_name),1,glm::value_ptr(value));
}

void Program::set_vec4(const char *var_name ,const glm::vec4 value){
    glUniform4fv(get_uniform_location(var_name),1,glm::value_ptr(value));
}

void Program::set_mat2(const char *var_name ,const glm::mat2 value){
    glUniformMatrix2fv(get_uniform_location(var_name),1,GL_FALSE,glm::value_ptr(value));
}

void Program::set_mat3(const char *var_name ,const glm::mat3 value){
    glUniformMatrix3fv(get_uniform_location(var_name),1,GL_FALSE,glm::value_ptr(value));
}

void Program::set_mat4(const char *var_name ,const glm::mat4 value){
    glUniformMatrix4fv(get_uniform_location(var_name),1,GL_FALSE,glm::value_ptr(value));
}

GLuint Program::get_uniform_location(const char *var_name){
    if(uniform_map.find(var_name) == uniform_map.end()){
        uniform_map[var_name] = glGetUniformLocation(program_id,var_name);
        return uniform_map[var_name];
    }
    return uniform_map[var_name];
}