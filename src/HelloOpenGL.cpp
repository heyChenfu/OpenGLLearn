#include "HelloOpenGL.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "CommonFunction.h"
#include "Shader.h"
#include <Windows.h>

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

unsigned int GenerateVAO(float * vertices, int verticeslen, unsigned int * indices, int indicesLen) {
    //顶点缓冲对象(Vertex Buffer Objects, VBO)，它会在GPU内存(通常被称为显存)中储存大量顶点
    //顶点数组对象(Vertex Array Object, VAO)可以像顶点缓冲对象那样被绑定，任何随后的顶点属性调用都会储存在这个VAO中
    //索引缓冲对象 (Element Buffer Objectss)
    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO); //生成一个VAO对象
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO); //生成一个EBO对象
    glBindVertexArray(VAO);
    //把我们的顶点数组复制到一个顶点缓冲中, 供OpenGL使用
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, verticeslen, vertices, GL_STATIC_DRAW);
    //复制我们的索引数组到一个索引缓冲中, 供OpenGL使用
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesLen, indices, GL_STATIC_DRAW);
    // 设置顶点属性指针
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // 颜色属性
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    //解除绑定
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    return VAO;
}

int main()
{
    //------------------------------------------------------
    // glfw 窗口创建
    //------------------------------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glViewport(0, 0, 800, 600);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    std::string vertexPath = "shader/vertexShader.txt";
    std::string fragmentPath = "shader/fragmentShader.txt";
    Shader * shaderClass = new Shader(vertexPath.c_str(), fragmentPath.c_str());

    //------------------------------------------------------
    //设置顶点数据(和缓冲区)和配置顶点属性
    //------------------------------------------------------
    float vertices[] = {
        // 位置              // 颜色
         0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // 右下
        -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // 左下
         0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // 顶部
    };
    unsigned int indices[] = { // 注意索引从0开始! 
    0, 1, 2, // 第一个三角形
    };
    unsigned int VAO = GenerateVAO(vertices, sizeof(vertices), indices, sizeof(indices));

    //渲染循环
    while (!glfwWindowShouldClose(window))
    {
        //检查输入
        processInput(window);

        //清屏
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        //---------------------------------
        //draw
        //---------------------------------
        glUseProgram(shaderClass->ID);
        glBindVertexArray(VAO);
        //glDrawArrays(GL_TRIANGLES, 0, 6);
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        glfwPollEvents();
        //交换缓冲
        glfwSwapBuffers(window);
    }

    //删除分配的所有资源
    glfwTerminate();
    return 0;
}