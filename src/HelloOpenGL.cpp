#include "HelloOpenGL.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <Windows.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "CommonFunction.h"
#include "Shader.h"
#include "stb_imageImplement.h"

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

unsigned int GenerateVAO(float * vertices, int verticeslen, unsigned int * indices, int indicesLen, int iStride) {
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
    char pointerIndex = 0;
    //链接顶点属性
    glVertexAttribPointer(pointerIndex, 3, GL_FLOAT, GL_FALSE, iStride * sizeof(float)/*步长,连续的顶点属性组之间的间隔*/, (void*)0);
    glEnableVertexAttribArray(pointerIndex); //启用指定属性, 在顶点着色器中访问逐顶点的属性数据
    // 颜色属性
    //glVertexAttribPointer(++pointerIndex, 3, GL_FLOAT, GL_FALSE, iStride * sizeof(float), (void*)(pointerIndex * 3 * sizeof(float)));
    //glEnableVertexAttribArray(pointerIndex);
    //纹理属性
    glVertexAttribPointer(++pointerIndex, 2, GL_FLOAT, GL_FALSE, iStride * sizeof(float), (void*)(pointerIndex * 3 * sizeof(float)));
    glEnableVertexAttribArray(pointerIndex);
    //解除绑定
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    return VAO;
}

unsigned int GenerateTexture(std::string sTexturePath) {
    unsigned int texture = 0;
    glGenTextures(1/*生成的纹理数目*/, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    //加载图片
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true); //OpenGL要求y轴0.0坐标是在图片的底部的，但是图片的y轴0.0坐标通常在顶部
    unsigned char* data = stbi_load(sTexturePath.c_str(), &width, &height, &nrChannels, 0);
    if (data == nullptr)
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    else {
        //载入的图片数据
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D); //为当前绑定的纹理自动生成所有需要的多级渐远纹理
        //释放图像的内存是一个很好的习惯
        stbi_image_free(data);
    }
    return texture;
}

int main()
{
    //-----------------------------------
    // glfw 窗口创建
    //-----------------------------------
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

    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
    glEnable(GL_DEPTH_TEST);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    //----------------------------------
    // shader
    //----------------------------------
    std::string vertexPath = "shader/vertexShader.txt";
    std::string fragmentPath = "shader/fragmentShader.txt";
    Shader * shaderClass = new Shader(vertexPath.c_str(), fragmentPath.c_str());

    //----------------------------------
    //设置顶点数据(和缓冲区)和配置顶点属性
    //----------------------------------
    //float vertices[] = {
    //    //     ---- 位置 ----     ---- 颜色 ----      - 纹理坐标 -
    //     0.5f,  0.5f, 0.0f,         1.0f, 0.0f, 0.0f,       1.0f, 1.0f,   // 右上
    //     0.5f, -0.5f, 0.0f,         0.0f, 1.0f, 0.0f,       1.0f, 0.0f,   // 右下
    //    -0.5f, -0.5f, 0.0f,         0.0f, 0.0f, 1.0f,       0.0f, 0.0f,   // 左下
    //    -0.5f,  0.5f, 0.0f,         1.0f, 1.0f, 0.0f,       0.0f, 1.0f    // 左上
    //};
    float vertices[] = {
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };
    unsigned int indices[] = {
    0, 1, 2,
    2, 3, 0
    };
    unsigned int VAO = GenerateVAO(vertices, sizeof(vertices), indices, sizeof(indices), 5);

    //----------------------------------
    //生成纹理
    //----------------------------------
    //创建第一个纹理
    unsigned int texture1 = GenerateTexture("res/container.jpg");
    //创建第二个纹理
    unsigned int texture2 = GenerateTexture("res/awesomeface.jpg");
    //设置采样
    shaderClass->use(); // 不要忘记在设置uniform变量之前激活着色器程序！
    shaderClass->setInt("texture1", 0);
    shaderClass->setInt("texture2", 1);

    //渲染循环
    while (!glfwWindowShouldClose(window))
    {
        //检查输入
        processInput(window);

        //清屏
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //清除颜色和深度缓冲

        //---------------------------------
        //draw
        //---------------------------------
        glActiveTexture(GL_TEXTURE0); // 在绑定纹理之前先激活纹理单元, 0默认总是被激活
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);

        //模型矩阵
        glm::mat4 model;
        model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
        //观察矩阵
        glm::mat4 view;
        view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f)); //我们将矩阵向我们要进行移动场景的反方向移动
        //投影矩阵
        glm::mat4 projection;
        projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        unsigned int transformLoc = glGetUniformLocation(shaderClass->ID, "model");
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE/*是否交换矩阵的行和列*/, glm::value_ptr(model));
        transformLoc = glGetUniformLocation(shaderClass->ID, "view");
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(view));
        transformLoc = glGetUniformLocation(shaderClass->ID, "projection");
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(projection));

        shaderClass->use();
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        glfwSwapBuffers(window); //交换缓冲
        glfwPollEvents();
    }

    //删除分配的所有资源
    glfwTerminate();
    return 0;
}