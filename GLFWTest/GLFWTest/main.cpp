//
//  main.cpp
//  GLFWTest
//
//  Created by Andrey Naboka on 29/09/14.
//  Copyright (c) 2014 Andrey Naboka. All rights reserved.
//

#include <iostream>
#include <GLFW/glfw3.h>
#include <OpenGL/gl3.h>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "service_functions.h"

GLuint                     vbo;
GLuint                     vao;
GLuint                     program;
window_struct              m_window;
static const GLfloat       vertex_buffer_data[] { -1.0, -1.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f };
glm::mat4                  model_matrix;

void render(const double delta);
void init();




void init() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    
    model_matrix = glm::mat4(1.0f);
    
    program = load_shaders("/Users/andreynaboka/src/shaders/pass_only.vert", "/Users/andreynaboka/src/shaders/pass_only.frag");
  
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_buffer_data), vertex_buffer_data, GL_STATIC_DRAW);
}


void render(const double delta) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // triangle scale
    static float scale = 0.0f;
    scale += delta;
    if (scale >= M_PI) scale -= M_PI;
    GLuint scale_location = glGetUniformLocation(program, "scale");
    assert(scale_location != 0xFFFFFFFF);
    glUniform1f(scale_location, sinf(scale));
    
    // projection
    glm::mat4 projection_matrix = glm::perspective(45.0f, 4.0f/3.0f, 0.1f, 100.0f);
    
    // Camera
    glm::vec3 camera_position  = glm::vec3(0.0f, 0.0f, 3.0f);
    glm::vec3 camera_target    = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 camera_up_vector = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::mat4 view_matrix      = glm::lookAt(camera_position, camera_target, camera_up_vector);

    // mvp
    glm::mat4 mvp_matrix       = projection_matrix * view_matrix * model_matrix;
    
    GLuint matrix_id = glGetUniformLocation(program, "mvp");
    glUniformMatrix4fv(matrix_id, 1, GL_FALSE, &mvp_matrix[0][0]);
    
    
    
    // draw
    glUseProgram(program);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glDrawArrays(GL_TRIANGLES, 0, 3); // start from 0 vertex, 3 vertex all
    glDisableVertexAttribArray(0);
}


int main(int argc, const char * argv[]) {
    if (!glfwInit()) {
        print_debug("GLFW init failed");
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    m_window.m_wnd = glfwCreateWindow(m_window.m_width, m_window.m_height, m_window.m_caption.c_str(), NULL, NULL);
    if (!m_window.m_wnd) {
        glfwTerminate();
        print_debug("Can't create window");
        return -1;
    }
    
    glfwMakeContextCurrent(m_window.m_wnd);
    
    dump_debug_info(m_window);
    
    init();
    
    
    float previous = glfwGetTime();
    
    while (!glfwWindowShouldClose(m_window.m_wnd)) {
        const float current = glfwGetTime();
        const float delta_time = current - previous;
        render(delta_time);
        
        glfwSwapBuffers(m_window.m_wnd);
        glfwPollEvents();
        previous = current;
    }
    
    glfwTerminate();
    return 0;
}
