#ifndef COMMON_H
#define COMMON_H

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Camera.h"

#define SCR_WIDTH 800
#define SCR_HEIGHT 600

//@desc: Common类
class Common
{
public:
    Common() {};
    ~Common() {};

    //@desc: glfw 窗口创建
    static GLFWwindow* CreateGLFWWindow();
    //@desc: 检查错误
    static GLenum glCheckError_(const char* file, int line);
    //生成纹理
    static unsigned int GenerateTexture(std::string sTexturePath);
    //输入监听
    static void ProcessInput(Camera* cam, GLFWwindow* window);

    static float deltaTime; //时间差变量, 储存渲染上一帧所用的时间, 速度乘以deltaTime值会相应平衡而不受帧率影响
    static float lastFrame; // 上一帧的时间
    static float lastX;
    static float lastY;

private:

};

#endif