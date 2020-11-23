#include "Application.hpp"

int main(){
    App::initialize();
    App::main_loop();
    App::terminate();
    
    return 0;
}