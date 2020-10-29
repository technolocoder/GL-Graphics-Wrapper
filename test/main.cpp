#include "GL_Wrapper.hpp"
#include <GLFW/glfw3.h>

float vertices[] = {
    -0.5,-0.5,1.0,0.0,0.0,
    -0.5, 0.5,0.0,1.0,0.0,
     0.5, 0.5,0.0,0.0,1.0,
     0.5,-0.5,1.0,1.0,1.0
};

unsigned int indices[] = {
    0,1,2,
    2,3,0
};

int main(){
    glfwInit();
    GLFWwindow *window = glfwCreateWindow(500,500,"Window",NULL,NULL);
    glfwMakeContextCurrent(window);

    glewExperimental = true;
    glewInit();

    GLuint vbo,vao,ebo;
    glGenBuffers(1,&vbo);
    glBindBuffer(GL_ARRAY_BUFFER,vbo);
    glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);

    glGenVertexArrays(1,&vao);
    glBindVertexArray(vao);

    glGenBuffers(1,&ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(indices),indices,GL_STATIC_DRAW);

    glVertexAttribPointer(0,2,GL_FLOAT,GL_FALSE,sizeof(float)*5,(const void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,sizeof(float)*5,(const void*)(sizeof(float)*2));
    glEnableVertexAttribArray(1);

    Program program;
    program.initialize();
    program.attachShader("./test/Shader/VertexShader.glsl",GL_VERTEX_SHADER);
    program.attachShader("./test/Shader/FragmentShader.glsl",GL_FRAGMENT_SHADER);
    program.link();

    while(!glfwWindowShouldClose(window)){
        glClear(GL_COLOR_BUFFER_BIT);

        program.use();
        glBindVertexArray(vao);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,ebo);
        glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,(const void*)0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}