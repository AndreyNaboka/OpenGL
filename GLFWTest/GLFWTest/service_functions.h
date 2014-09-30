//
//  service_functions.h
//  GLFWTest
//
//  Created by Andrey Naboka on 29/09/14.
//  Copyright (c) 2014 Andrey Naboka. All rights reserved.
//

#ifndef GLFWTest_service_functions_h
#define GLFWTest_service_functions_h

#include <OpenGL/gl3.h>
#include <string>
#include <iostream>
#include <fstream>
#include "png_loader/lodepng.h"

struct vector3d {
    float m_x, m_y, m_z;
    vector3d()
        :m_x(0.0f)
        ,m_y(0.0f)
        ,m_z(0.0f)
    {}
    vector3d(const float x, const float y, const float z)
        :m_x(x)
        ,m_y(y)
        ,m_z(z)
    {}
};

struct window_struct {
    GLFWwindow *m_wnd;
    int m_width, m_height;
    std::string m_caption;
    
    window_struct()
        :m_wnd(NULL)
        ,m_width(800)
        ,m_height(600)
        ,m_caption("GLFW")
    {}
};


void dump_debug_info(const window_struct& window);
void print_debug(const std::string& debug_string);
void image_loader(const std::string& filename, std::vector<unsigned char>& image_raw);
GLuint load_shaders(const char* vertex_file_path, const char* fragment_file_path);


GLuint load_shaders(const char* vertex_file_path, const char* fragment_file_path) {
    using namespace std;
    
    // Создаем шейдеры
    GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
    
    // Загружаем код Вершинного Шейдера из файла
    std::string VertexShaderCode;
    std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
    if(VertexShaderStream.is_open())
    {
        std::string Line = "";
        while(getline(VertexShaderStream, Line))
            VertexShaderCode += "\n" + Line;
        VertexShaderStream.close();
    }
    
    // Загружаем код Фрагментного шейдера из файла
    std::string FragmentShaderCode;
    std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
    if(FragmentShaderStream.is_open()){
        std::string Line = "";
        while(getline(FragmentShaderStream, Line))
            FragmentShaderCode += "\n" + Line;
        FragmentShaderStream.close();
    }
    
    GLint Result = GL_FALSE;
    int InfoLogLength;
    
    printf("Compile shader: %s\n", vertex_file_path);
    char const * VertexSourcePointer = VertexShaderCode.c_str();
    glShaderSource(VertexShaderID, 1, &VertexSourcePointer , NULL);
    glCompileShader(VertexShaderID);
    
    glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if (InfoLogLength) {
        std::vector<char> VertexShaderErrorMessage(InfoLogLength);
        glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
        fprintf(stdout, "%s\n", &VertexShaderErrorMessage[0]);
    }
    
    printf("Compile shader: %s\n", fragment_file_path);
    char const * FragmentSourcePointer = FragmentShaderCode.c_str();
    glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer , NULL);
    glCompileShader(FragmentShaderID);
    
    glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if (InfoLogLength) {
        std::vector<char> FragmentShaderErrorMessage(InfoLogLength);
        glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
        fprintf(stdout, "%s\n", &FragmentShaderErrorMessage[0]);
    }
    
    fprintf(stdout, "Create program\n");
    GLuint ProgramID = glCreateProgram();
    glAttachShader(ProgramID, VertexShaderID);
    glAttachShader(ProgramID, FragmentShaderID);
    glLinkProgram(ProgramID);
    
    glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
    glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if (InfoLogLength) {
        std::vector<char> ProgramErrorMessage( max(InfoLogLength, int(1)) );
        glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
        fprintf(stdout, "%s\n", &ProgramErrorMessage[0]);
    }
    glDeleteShader(VertexShaderID);
    glDeleteShader(FragmentShaderID);
    
    return ProgramID;
}

void image_loader(const std::string& filename, std::vector<unsigned char>& image_raw) {
    unsigned int width, height;
    unsigned int error = lodepng::decode(image_raw, width, height, filename.c_str());
    if (error) {
        print_debug("Failed loading texture : " + filename);
    }
}

void dump_debug_info(const window_struct& window) {
    int major, minor, rev;
    major = glfwGetWindowAttrib(window.m_wnd, GLFW_CONTEXT_VERSION_MAJOR);
    minor = glfwGetWindowAttrib(window.m_wnd, GLFW_CONTEXT_VERSION_MINOR);
    rev = glfwGetWindowAttrib(window.m_wnd, GLFW_CONTEXT_REVISION);
    printf("OpenGL version recieved: %d.%d.%d\n", major, minor, rev);
    printf("Supported OpenGL is %s\n", (const char*)glGetString(GL_VERSION));
    printf("Supported GLSL is %s\n", (const char*)glGetString(GL_SHADING_LANGUAGE_VERSION));
}

void print_debug(const std::string& debug_string) {
    std::cout << debug_string << std::endl;
}


#endif
