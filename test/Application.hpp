#include "GL_Wrapper.hpp"
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

namespace App{
    GLFWwindow *window = NULL;
    GLFWmonitor * monitor = NULL;
    const GLFWvidmode * video_mode = NULL;

    int window_width,window_height;
    double center_x,center_y;
    double prev_time=0,current_time=0,delta_time=0;

    double sensetivity = 0.05,movement_speed=1.0;
    GLuint ubo ,light_ubo;

    GLuint fbo,rbo,fbo_texture;
    Texture texture;

    Model backpack_model;
    Program program, fbo_program;
    Camera camera(glm::vec3(3.0,0.0,0.0),90.0f,0.0f);

    float vertices[] = {
        -1.0,-1.0, 0.0, 0.0,
        -1.0, 1.0, 0.0, 1.0,
         1.0, 1.0, 1.0, 1.0,
         1.0,-1.0, 1.0, 0.0
    };

    unsigned int indices[] = {
        0,1,2,
        2,3,0
    };
    GLuint vbo,ebo,vao;

    void process_inputs(){
        if(glfwGetKey(window,GLFW_KEY_ESCAPE)){
            glfwSetWindowShouldClose(window,true);
        }else{
            double _movement_speed = movement_speed * delta_time;
            bool moved = false;
            if(glfwGetKey(window,GLFW_KEY_LEFT_SHIFT)){
                _movement_speed *= 2;
            }
            if(glfwGetKey(window,GLFW_KEY_W)){
                camera.position += camera.get_front() * glm::vec3(_movement_speed);
                moved = true;
            }
            if(glfwGetKey(window,GLFW_KEY_S)){
                camera.position -= camera.get_front() * glm::vec3(_movement_speed);
                moved = true; 
            }

            if(glfwGetKey(window,GLFW_KEY_D)){
                camera.position += camera.get_right() * glm::vec3(_movement_speed);
                moved = true;
            }
            if(glfwGetKey(window,GLFW_KEY_A)){
                camera.position -= camera.get_right() * glm::vec3(_movement_speed);
                moved = true;
            }

            if(glfwGetKey(window,GLFW_KEY_SPACE)){
                camera.position += camera.get_up() * glm::vec3(_movement_speed);
                moved = true;
            }
            if(glfwGetKey(window,GLFW_KEY_LEFT_ALT)){
                camera.position -= camera.get_up() * glm::vec3(_movement_speed);
                moved = true;
            }

            if(moved){
                glBindBuffer(GL_UNIFORM_BUFFER,ubo);
                glBufferSubData(GL_UNIFORM_BUFFER,64,64,glm::value_ptr(camera.get_view_matrix_compute()));
                glBindBuffer(GL_UNIFORM_BUFFER,light_ubo);
                glBufferSubData(GL_UNIFORM_BUFFER,48,12,glm::value_ptr(camera.position));
            }
        }
    }

    void cursor_callback(GLFWwindow *window ,double x, double y){
        double delta_x = (center_x - x) * sensetivity;
        double delta_y = (center_y - y) * sensetivity;

        camera.pitch += delta_y;
        camera.yaw -= delta_x;

        if(camera.pitch < -89.0){
            camera.pitch = -89.0;
        }else if(camera.pitch > 89.0){
            camera.pitch = 89.0;
        }

        glBindBuffer(GL_UNIFORM_BUFFER,ubo);
        glBufferSubData(GL_UNIFORM_BUFFER,64,64,glm::value_ptr(camera.get_view_matrix_compute()));

        glfwSetCursorPos(window,center_x,center_y);
    }

    void initialize(){
        stbi_set_flip_vertically_on_load(true);
        glfwInit();

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR ,4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR ,5);
        glfwWindowHint(GLFW_OPENGL_PROFILE ,GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_SAMPLES,16);

        monitor = glfwGetPrimaryMonitor();
        video_mode = const_cast<GLFWvidmode*>(glfwGetVideoMode(monitor));

        window_width = video_mode->width;
        window_height = video_mode->height;

        center_x = window_width/2.0;
        center_y = window_height/2.0;

        window = glfwCreateWindow(window_width,window_height,"Test Window",monitor,NULL);
        glfwMakeContextCurrent(window);
    
        glViewport(0,0,window_width,window_height);
    
        glewExperimental = GL_TRUE;
        glewInit();

        glfwSetInputMode(window,GLFW_CURSOR,GLFW_CURSOR_HIDDEN);
        glfwSetCursorPosCallback(window,cursor_callback);
        glEnable(GL_DEPTH_TEST);

        backpack_model.initialize("test/Model/Backpack-Model/backpack.obj",true);

        program.initialize();
        program.attachShader("test/Shader/backpack-shader/VertexShader.glsl",GL_VERTEX_SHADER);
        program.attachShader("test/Shader/backpack-shader/FragmentShader.glsl",GL_FRAGMENT_SHADER);
        program.link();

        fbo_program.initialize();
        fbo_program.attachShader("test/Shader/framebuffer-shader/VertexShader.glsl",GL_VERTEX_SHADER);
        fbo_program.attachShader("test/Shader/framebuffer-shader/FragmentShader.glsl",GL_FRAGMENT_SHADER);
        fbo_program.link();

        glGenBuffers(1,&ubo);
        glBindBuffer(GL_UNIFORM_BUFFER,ubo);
        glBufferData(GL_UNIFORM_BUFFER,128,NULL,GL_DYNAMIC_DRAW);
        glBindBufferBase(GL_UNIFORM_BUFFER,0,ubo);
        glBufferSubData(GL_UNIFORM_BUFFER,0,64,glm::value_ptr(glm::perspective(glm::radians(45.0f),(float)window_width/(float)window_height ,0.1f,100.0f)));
        glBufferSubData(GL_UNIFORM_BUFFER,64,64,glm::value_ptr(camera.get_view_matrix_compute()));
    
        glGenBuffers(1,&light_ubo);
        glBindBuffer(GL_UNIFORM_BUFFER,light_ubo);
        glBufferData(GL_UNIFORM_BUFFER,80,NULL,GL_DYNAMIC_DRAW);

        glm::vec3 light_color(1.0);
        glm::vec3 ambient_strength = light_color * glm::vec3(0.01) ,diffuse_strength = light_color * glm::vec3(1.0),specular_strength = light_color * glm::vec3(0.6);
        glm::vec3 light_position = glm::vec3(3.0,0.0,0.0);

        glBindBufferBase(GL_UNIFORM_BUFFER ,1 ,light_ubo);
        glBufferSubData(GL_UNIFORM_BUFFER,0,12,glm::value_ptr(ambient_strength));
        glBufferSubData(GL_UNIFORM_BUFFER,16,12,glm::value_ptr(diffuse_strength));
        glBufferSubData(GL_UNIFORM_BUFFER,32,12,glm::value_ptr(specular_strength));
        glBufferSubData(GL_UNIFORM_BUFFER,48,12,glm::value_ptr(camera.position));
        glBufferSubData(GL_UNIFORM_BUFFER,64,12,glm::value_ptr(light_position));

        glGenBuffers(1,&vbo);
        glBindBuffer(GL_ARRAY_BUFFER,vbo);
        glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);

        glGenVertexArrays(1,&vao);
        glBindVertexArray(vao);

        glGenBuffers(1,&ebo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(indices),indices,GL_STATIC_DRAW);
    
        glVertexAttribPointer(0,2,GL_FLOAT,GL_TRUE,sizeof(float)*4,(const void*)0);
        glEnableVertexAttribArray(0);    

        glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,sizeof(float)*4,(const void*)(sizeof(float)*2));
        glEnableVertexAttribArray(1);

        glGenFramebuffers(1,&fbo);
        glBindFramebuffer(GL_FRAMEBUFFER ,fbo);

        glGenTextures(1,&fbo_texture);
        glBindTexture(GL_TEXTURE_2D,fbo_texture);

        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

        glTexImage2D(GL_TEXTURE_2D,0,GL_RGB32F,window_width,window_height,0,GL_RGB,GL_FLOAT,NULL);
        glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D,fbo_texture,0);
        glBindTexture(GL_TEXTURE_2D,0);

        glGenRenderbuffers(1,&rbo);
        glBindRenderbuffer(GL_RENDERBUFFER,rbo);
        glRenderbufferStorage(GL_RENDERBUFFER,GL_DEPTH24_STENCIL8,window_width,window_height);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER,GL_DEPTH_STENCIL_ATTACHMENT,GL_RENDERBUFFER,rbo);
        glBindRenderbuffer(GL_RENDERBUFFER,0); 
    
        if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE){
            std::cerr << "Error setting up framebuffer\n";
            exit(1);
        }
        glBindFramebuffer(GL_FRAMEBUFFER,0);

        glEnable(GL_MULTISAMPLE);
        texture.initialize("test/Model/Backpack-Model/ao.jpg");
    }

    void main_loop(){
        while(!glfwWindowShouldClose(window)){
            current_time = glfwGetTime();
            delta_time = current_time-prev_time;
            prev_time = current_time;

            glBindFramebuffer(GL_FRAMEBUFFER,fbo);
            glEnable(GL_DEPTH_TEST);

            process_inputs();
            glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

            glm::mat4 model = glm::rotate(glm::mat4(1.0),(float)glfwGetTime(),glm::vec3(0.0,1.0,0.0));
            program.use();
            program.set_mat4("model",model);

            backpack_model.set_texture_uniforms(program);
            backpack_model.draw(program);

            glBindFramebuffer(GL_FRAMEBUFFER,0);
            glDisable(GL_DEPTH_TEST);

            glClear(GL_COLOR_BUFFER_BIT);

            glBindVertexArray(vao);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,ebo);
            fbo_program.use();
            glActiveTexture(GL_TEXTURE0);
            fbo_program.set_int("tex",0);
            glBindTexture(GL_TEXTURE_2D,fbo_texture);

            glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,(const void*)0);

            glfwSwapBuffers(window);
            glfwPollEvents();
        }
    }

    void terminate(){
        glfwDestroyWindow(window);
        glfwTerminate();
    }
};