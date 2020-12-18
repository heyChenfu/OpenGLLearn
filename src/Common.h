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

//@desc: Common��
class Common
{
public:
    Common() {};
    ~Common() {};

    //@desc: glfw ���ڴ���
    static GLFWwindow* CreateGLFWWindow();
    //@desc: ������
    static GLenum glCheckError_(const char* file, int line);
    //��������
    static unsigned int GenerateTexture(std::string sTexturePath);
    //�������
    static void ProcessInput(Camera* cam, GLFWwindow* window);

    static float deltaTime; //ʱ������, ������Ⱦ��һ֡���õ�ʱ��, �ٶȳ���deltaTimeֵ����Ӧƽ�������֡��Ӱ��
    static float lastFrame; // ��һ֡��ʱ��
    static float lastX;
    static float lastY;

private:

};

#endif