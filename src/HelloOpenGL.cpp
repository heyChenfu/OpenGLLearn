#include "HelloOpenGL.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <Windows.h>

#include "Common.h"
#include "Shader.h"

bool firstMouse = true;

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) 
{
    if (firstMouse) // 这个bool变量初始时是设定为true的
    {
        Common::lastX = xpos;
        Common::lastY = ypos;
        firstMouse = false;
    }
    float xoffset = xpos - Common::lastX;
    float yoffset = Common::lastY - ypos; // 注意这里是相反的，因为y坐标是从底部往顶部依次增大的
    Common::lastX = xpos;
    Common::lastY = ypos;

    Camera::getInstence()->ProcessMouseMovement(xoffset, yoffset, Common::deltaTime);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    Camera::getInstence()->ProcessMouseScroll(yoffset);
}

unsigned int GenerateVAO(float * vertices, int verticeslen, unsigned int * indices, int indicesLen, int iStride) {
    //顶点缓冲对象(Vertex Buffer Objects, VBO)，它会在GPU内存(通常被称为显存)中储存大量顶点
    //顶点数组对象(Vertex Array Object, VAO)可以像顶点缓冲对象那样被绑定，任何随后的顶点属性调用都会储存在这个VAO中
    //索引缓冲对象 (Element Buffer Objects) 储存顶点的索引, 设定绘制这些顶点的顺序。使用glDrawElements时, 会使用当前绑定的索引缓冲对象中的索引进行绘制
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

HelloOpenGL::HelloOpenGL()
{
    //-----------------------------------
    // glfw 窗口创建
    //-----------------------------------
    GLFWwindow* window = Common::CreateGLFWWindow();
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    //----------------------------------
    // 创建摄像机
    //----------------------------------
    Camera::getInstence()->CameraInit(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 1.0f, 0.0f));

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
    unsigned int texture1 = Common::GenerateTexture("res/container.jpg");
    //创建第二个纹理
    unsigned int texture2 = Common::GenerateTexture("res/awesomeface.jpg");
    //设置采样
    shaderClass->use(); // 不要忘记在设置uniform变量之前激活着色器程序！
    shaderClass->setInt("texture1", 0);
    shaderClass->setInt("texture2", 1);

    //渲染循环
    while (!glfwWindowShouldClose(window))
    {
        //刷新每帧耗时
        float currentFrame = glfwGetTime();
        Common::deltaTime = currentFrame - Common::lastFrame;
        Common::lastFrame = currentFrame;

        //检查输入
        Common::ProcessInput(Camera::getInstence(), window);

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
        //model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f)/*旋转角度*/, glm::vec3(0.5f, 1.0f, 0.0f)/*旋转轴*/);
        //观察矩阵
        glm::mat4 cameraView;
        //创建一个LookAt矩阵，我们可以把它当作我们的观察矩阵
        cameraView = glm::lookAt(
            Camera::getInstence()->Position, Camera::getInstence()->Position + Camera::getInstence()->Front, Camera::getInstence()->Up);
        //投影矩阵
        glm::mat4 projection;
        projection = glm::perspective(glm::radians(Camera::getInstence()->Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        shaderClass->setMat4("model", model);
        shaderClass->setMat4("view", cameraView);
        shaderClass->setMat4("projection", projection);

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
    return;
}