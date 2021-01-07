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
        // positions          // normals           // texture coords
                -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
                 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
                 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
                 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
                -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
                -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

                -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
                 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
                 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
                 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
                -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
                -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

                -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
                -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
                -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
                -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
                -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
                -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

                 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
                 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
                 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
                 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
                 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
                 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

                -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
                 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
                 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
                 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
                -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
                -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

                -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
                 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
                 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
                 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
                -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
                -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
    };
    //所有几何体位置
    glm::vec3 cubePositions[] = {
        glm::vec3(0.0f,  0.0f,  0.0f),
        glm::vec3(2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3(2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3(1.3f, -2.0f, -2.5f),
        glm::vec3(1.5f,  2.0f, -2.5f),
        glm::vec3(1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f)
    };
    //所有点光源位置
    glm::vec3 pointLightPositions[] = {
        glm::vec3(0.7f,  0.2f,  2.0f),
        glm::vec3(2.3f, -3.3f, -4.0f),
        glm::vec3(-4.0f,  2.0f, -12.0f),
        glm::vec3(0.0f,  0.0f, -3.0f)
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
    char stride = 8; //步长
    //链接顶点属性
    glVertexAttribPointer(pointerIndex, 3, GL_FLOAT, GL_FALSE, stride * sizeof(float), 0);
    glEnableVertexAttribArray(pointerIndex); //启用指定属性, 在顶点着色器中访问逐顶点的属性数据
    //链接法线属性
    glVertexAttribPointer(++pointerIndex, 3, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(pointerIndex);
    //纹理属性
    glVertexAttribPointer(++pointerIndex, 2, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(pointerIndex);

    //解除绑定
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    //----------------------------------
    // 光源
    //----------------------------------
    //创建光源立方体
    //glm::vec3 lightPos(0.5f, 0.5f, 2.0f);
    glm::vec3 lightColor(1.0f, 1.0f, 1.0f);
    unsigned int lightVAO;
    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);
    //使用现有的VBO数据
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // 设置灯立方体的顶点属性（对我们的灯来说仅仅只有位置数据）
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //----------------------------------
    //纹理
    //----------------------------------
    //创建第一个纹理
    unsigned int texture1 = Common::GenerateTexture("res/container2.jpg");
    unsigned int texture2 = Common::GenerateTexture("res/container2_specular.jpg");
    shaderClass->use();
    shaderClass->setInt("material.diffuse", 0);
    shaderClass->setInt("material.specular", 1);

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

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);

        //变化的光源颜色
        //lightColor.x = sin(glfwGetTime() * 2.0f);
        //lightColor.y = sin(glfwGetTime() * 0.7f);
        //lightColor.z = sin(glfwGetTime() * 1.3f);
        glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f); // 降低影响
        glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f); // 很低的影响

        //模型矩阵
        //glm::mat4 model;
        //观察矩阵
        glm::mat4 cameraView = Camera::getInstence()->GetViewMatrix();
        //投影矩阵
        glm::mat4 projection = glm::perspective(glm::radians(Camera::getInstence()->Zoom),
            (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

        shaderClass->use();
        shaderClass->setMat4("view", cameraView);
        shaderClass->setMat4("projection", projection);
        shaderClass->setVec3("viewPos", Camera::getInstence()->Position);
        shaderClass->setVec3("material.specular", 0.5f, 0.5f, 0.5f);
        shaderClass->setFloat("material.shininess", 32.0f);
        //设置方向光
        shaderClass->setVec3("dirLight.ambient", ambientColor);
        shaderClass->setVec3("dirLight.diffuse", diffuseColor); // 将光照调暗了一些以搭配场景
        shaderClass->setVec3("dirLight.specular", 1.0f, 1.0f, 1.0f);
        shaderClass->setVec3("dirLight.direction", - 0.2f, -1.0f, -0.3f);
        //设置点光
        for (int i = 0; i < sizeof(pointLightPositions) / sizeof(pointLightPositions[0]); i++) {
            char str[20];
            sprintf_s(str, "pointLights[%d]", i);
            std::string variableName = std::string(str);
            shaderClass->setVec3(variableName + ".position", pointLightPositions[i]);
            shaderClass->setVec3(variableName + ".ambient", 0.05f, 0.05f, 0.05f);
            shaderClass->setVec3(variableName + ".diffuse", 0.8f, 0.8f, 0.8f);
            shaderClass->setVec3(variableName + ".specular", 1.0f, 1.0f, 1.0f);
            shaderClass->setFloat(variableName + ".constant", 1.0f);
            shaderClass->setFloat(variableName + ".linear", 0.09);
            shaderClass->setFloat(variableName + ".quadratic", 0.032);
        }

        glBindVertexArray(VAO);
        for (unsigned int i = 0; i < 10; i++)
        {
            glm::mat4 model;
            model = glm::translate(model, cubePositions[i]);
            float angle = 20.0f * i;
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            shaderClass->setMat4("model", model);

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        //绘制光源
        glm::mat4 lightModel; //灯光位置
        lightModel = glm::translate(lightModel, pointLightPositions[0]);
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