#include "GL_Wrapper.hpp"
#include <GLFW/glfw3.h>

namespace App{
    GLFWwindow *window;
    double current_time=0,delta_time=0,prev_time=0;

    Program program;
    Model model;
    
    int window_width,window_height;
    double center_x,center_y;
    
    GLFWmonitor *monitor;
    const GLFWvidmode *video_mode;

    const double sensetivity = 0.05;

    Camera camera(glm::vec3(1.0,0.0,1.0),-130.0f,0.0f);

    void cursor_callback(GLFWwindow *window ,double x,double y){
        double delta_x = center_x-x,delta_y = center_y-y;
        camera.yaw -= delta_x * sensetivity;
        camera.pitch += delta_y * sensetivity;

        camera.pitch = std::min(camera.pitch, 89.0f);
        camera.pitch = std::max(camera.pitch,-89.0f);

        program.use();
        program.set_mat4("view",camera.get_view_matrix_compute());

        glfwSetCursorPos(window,center_x,center_y);
    }

    void process_inputs(){
        if(glfwGetKey(window,GLFW_KEY_ESCAPE)){
            glfwSetWindowShouldClose(window,true);
        }else{
            double speed = delta_time;
            bool moved = false;
            if(glfwGetKey(window,GLFW_KEY_LEFT_SHIFT)) speed *= 2;
            if(glfwGetKey(window,GLFW_KEY_W)){
                camera.position += camera.get_front() * glm::vec3(speed); 
            }
            if(glfwGetKey(window,GLFW_KEY_S)){
                camera.position -= camera.get_front() * glm::vec3(speed); 
                moved = true;
            }
            if(glfwGetKey(window,GLFW_KEY_A)){
                camera.position -= camera.get_right() * glm::vec3(speed); 
                moved = true;
            }
            if(glfwGetKey(window,GLFW_KEY_D)){
                camera.position += camera.get_right() * glm::vec3(speed); 
                moved = true;
            }
            if(glfwGetKey(window,GLFW_KEY_SPACE)){
                camera.position += camera.get_up() * glm::vec3(speed); 
                moved = true;
            }
            if(glfwGetKey(window,GLFW_KEY_LEFT_ALT)){
                camera.position -= camera.get_up() * glm::vec3(speed); 
                moved = true;    
            }
            if(moved){
                program.use();
                program.set_mat4("view",camera.get_view_matrix_compute());
            }
        }
    }

    void initialize(){
        stbi_set_flip_vertically_on_load(true);
        glfwInit();

        monitor = glfwGetPrimaryMonitor();
        video_mode = const_cast<GLFWvidmode*>(glfwGetVideoMode(monitor));

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR ,4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR ,2);
        glfwWindowHint(GLFW_OPENGL_PROFILE ,GLFW_OPENGL_CORE_PROFILE);

        window = glfwCreateWindow(window_width,window_height,"Test Window",NULL,NULL);
        glfwMakeContextCurrent(window);

        glViewport(0,0,window_width,window_height);
        glfwSetInputMode(window,GLFW_CURSOR,GLFW_CURSOR_HIDDEN);

        glewExperimental = GL_TRUE;
        glewInit();

        glfwSetCursorPosCallback(window,cursor_callback);

        program.initialize();
        program.attachShader("test/Shader/VertexShader.glsl",GL_VERTEX_SHADER);
        program.attachShader("test/Shader/FragmentShader.glsl",GL_FRAGMENT_SHADER);
        program.link();

        program.use();
        program.set_mat4("projection",glm::perspective(glm::radians(45.0f),(float)window_width/(float)window_height,0.1f,100.0f));
        program.set_mat4("view",camera.get_view_matrix_compute());

        model.initialize("test/Model/Backpack-Model/backpack.obj");
        glEnable(GL_DEPTH_TEST);
    }

    void main_loop(){
        glClearColor(0.2,0.2,0.2,1.0);
        while(!glfwWindowShouldClose(window)){
            current_time = glfwGetTime();
            delta_time = current_time-prev_time;

            process_inputs();
            glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

            model.draw(program);

            glfwSwapBuffers(window);
            glfwPollEvents();

            prev_time = current_time;
        }
    }

    void terminate(){
        glfwDestroyWindow(window);
        glfwTerminate();
    }
};