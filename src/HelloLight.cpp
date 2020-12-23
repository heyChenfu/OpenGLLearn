#include "HelloLight.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <Windows.h>

#include "Common.h"
#include "Shader.h"

HelloLight::HelloLight() {
    //-----------------------------------
    // glfw 窗口创建
    //-----------------------------------
    GLFWwindow* window = Common::CreateGLFWWindow();
    //----------------------------------
    // 创建摄像机
    //----------------------------------
    Camera::getInstence()->CameraInit(glm::vec3(0.0f, 1.0f, 4.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    //----------------------------------
    // shader
    //----------------------------------
    std::string vertexPath = "shader/boxVertexShader.txt";
    std::string fragmentPath = "shader/boxFragmentShader.txt";
    Shader * shaderClass = new Shader(vertexPath.c_str(), fragmentPath.c_str());

    vertexPath = "shader/lightTestVertexShader.txt";
    fragmentPath = "shader/lightTestFragmentShader.txt";
    Shader * lightShaderClass = new Shader(vertexPath.c_str(), fragmentPath.c_str());

    //----------------------------------
    //设置顶点数据(和缓冲区)和配置顶点属性
    //----------------------------------
    float vertices[] = {
    //顶点                   //法线
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
    };
    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO); //生成一个VAO对象
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    //把我们的顶点数组复制到一个顶点缓冲中, 供OpenGL使用
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // 设置顶点属性指针
    char pointerIndex = 0;
    //链接顶点属性
    glVertexAttribPointer(pointerIndex, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float)/*步长,连续的顶点属性组之间的间隔*/, (void*)0);
    glEnableVertexAttribArray(pointerIndex); //启用指定属性, 在顶点着色器中访问逐顶点的属性数据
    //链接法线属性
    glVertexAttribPointer(++pointerIndex, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(pointerIndex * 3 * sizeof(float)));
    glEnableVertexAttribArray(pointerIndex);
    //解除绑定
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    //----------------------------------
    // 光源
    //----------------------------------
    //创建光源立方体
    glm::vec3 lightPos(0.5f, 0.5f, 2.0f);
    glm::vec3 lightColor(1.0f, 1.0f, 1.0f);
    unsigned int lightVAO;
    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);
    //使用现有的VBO数据
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // 设置灯立方体的顶点属性（对我们的灯来说仅仅只有位置数据）
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

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

        //变化的光源颜色
        lightColor.x = sin(glfwGetTime() * 2.0f);
        lightColor.y = sin(glfwGetTime() * 0.7f);
        lightColor.z = sin(glfwGetTime() * 1.3f);
        glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f); // 降低影响
        glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f); // 很低的影响

        //模型矩阵
        glm::mat4 model;
        //观察矩阵
        glm::mat4 cameraView = Camera::getInstence()->GetViewMatrix();
        //投影矩阵
        glm::mat4 projection = glm::perspective(glm::radians(Camera::getInstence()->Zoom),
            (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        shaderClass->use();
        shaderClass->setMat4("model", model);
        shaderClass->setMat4("view", cameraView);
        shaderClass->setMat4("projection", projection);
        shaderClass->setVec3("viewPos", Camera::getInstence()->Position);
        shaderClass->setVec3("lightPos", lightPos);
        shaderClass->setVec3("material.ambient", 1.0f, 0.5f, 0.31f);
        shaderClass->setVec3("material.diffuse", 1.0f, 0.5f, 0.31f);
        shaderClass->setVec3("material.specular", 0.5f, 0.5f, 0.5f);
        shaderClass->setFloat("material.shininess", 32.0f);
        shaderClass->setVec3("light.ambient", ambientColor);
        shaderClass->setVec3("light.diffuse", diffuseColor); // 将光照调暗了一些以搭配场景
        shaderClass->setVec3("light.specular", 1.0f, 1.0f, 1.0f);

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        //绘制光源
        glm::mat4 lightModel; //灯光位置
        lightModel = glm::translate(lightModel, lightPos);
        lightModel = glm::scale(lightModel, glm::vec3(0.2f));
        lightShaderClass->use();
        lightShaderClass->setMat4("model", lightModel);
        lightShaderClass->setMat4("view", cameraView);
        lightShaderClass->setMat4("projection", projection);
        lightShaderClass->setVec3("lightColor", lightColor);
        glBindVertexArray(lightVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glBindVertexArray(0);
        glfwSwapBuffers(window); //交换缓冲
        glfwPollEvents();
    }

    //删除分配的所有资源
    glfwTerminate();
    return;
}