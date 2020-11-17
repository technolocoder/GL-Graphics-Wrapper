#ifndef GLW_CAMERA
#define GLW_CAMERA

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera{
public:
    glm::vec3 position;
    float yaw,pitch;
    
    Camera(glm::vec3 _position,float yaw ,float pitch);
    void intialize(glm::vec3 _position ,float yaw, float pitch);

    glm::mat4 get_view_matrix();
    glm::mat4 get_view_matrix_compute();
    void compute_view_matrix();

    glm::vec3 get_front();
    glm::vec3 get_up();
    glm::vec3 get_right();
private:
    glm::vec3 front,up,right;
    glm::mat4 view_matrix;
};

#endif 