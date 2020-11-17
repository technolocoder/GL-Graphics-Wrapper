#include "Camera.hpp"

Camera::Camera(glm::vec3 _position ,float _yaw ,float _pitch) : position(_position),yaw(_yaw),pitch(_pitch){
    compute_view_matrix();
    up = glm::vec3(0.0,1.0,0.0);
}

void Camera::intialize(glm::vec3 _position ,float _yaw ,float _pitch){
    position = _position;
    yaw = _yaw;
    pitch = _pitch;

    compute_view_matrix();
    up = glm::vec3(0.0,1.0,0.0);
}

void Camera::compute_view_matrix(){
    front.x = glm::cos(glm::radians(yaw)) * glm::cos(glm::radians(pitch));
    front.y = glm::sin(glm::radians(pitch));
    front.z = glm::sin(glm::radians(yaw)) * glm::cos(glm::radians(pitch));

    glm::normalize(front);
    right = glm::normalize(glm::cross(front,up));
    view_matrix = glm::lookAt(position,position+front,up);
}

glm::mat4 Camera::get_view_matrix(){
    return view_matrix;
}

glm::mat4 Camera::get_view_matrix_compute(){
    compute_view_matrix();
    return view_matrix;
}

glm::vec3 Camera::get_front(){
    return front;
}
glm::vec3 Camera::get_up(){
    return up;
}
glm::vec3 Camera::get_right(){
    return right;
}