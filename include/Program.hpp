#ifndef GLW_PROGRAM
#define GLW_PROGRAM

#include "Shader.hpp"
#include <map>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class Program{
public:
    Program();
    
    void initialize();

    void attachShader(Shader &shader);
    void attachShader(const char *filepath ,const GLuint shader_type);

    void link();
    void use();

    GLuint operator()();

    void set_float(const char *var_name ,const float value);
    void set_double(const char *var_name ,const double value);
    void set_int(const char *var_name ,const int value);

    void set_vec2(const char *var_name ,const glm::vec2 value);
    void set_vec3(const char *var_name ,const glm::vec3 value);
    void set_vec4(const char *var_name ,const glm::vec4 value);

    void set_mat2(const char *var_name ,const glm::mat2 value);
    void set_mat3(const char *var_name ,const glm::mat3 value);
    void set_mat4(const char *var_name ,const glm::mat4 value);
private:
    GLuint get_uniform_location(const char *var_name);
    GLuint program_id;

    std::map<std::string,GLuint> uniform_map;
};

#endif