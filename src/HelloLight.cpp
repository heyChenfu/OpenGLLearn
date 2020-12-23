#include "HelloLight.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <Windows.h>

#include "Common.h"
#include "Shader.h"

HelloLight::HelloLight() {
    //-----------------------------------
    // glfw ���ڴ���
    //-----------------------------------
    GLFWwindow* window = Common::CreateGLFWWindow();
    //----------------------------------
    // ���������
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
    //���ö�������(�ͻ�����)�����ö�������
    //----------------------------------
    float vertices[] = {
    //����                   //����
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
    glGenVertexArrays(1, &VAO); //����һ��VAO����
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    //�����ǵĶ������鸴�Ƶ�һ�����㻺����, ��OpenGLʹ��
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // ���ö�������ָ��
    char pointerIndex = 0;
    //���Ӷ�������
    glVertexAttribPointer(pointerIndex, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float)/*����,�����Ķ���������֮��ļ��*/, (void*)0);
    glEnableVertexAttribArray(pointerIndex); //����ָ������, �ڶ�����ɫ���з����𶥵����������
    //���ӷ�������
    glVertexAttribPointer(++pointerIndex, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(pointerIndex * 3 * sizeof(float)));
    glEnableVertexAttribArray(pointerIndex);
    //�����
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    //----------------------------------
    // ��Դ
    //----------------------------------
    //������Դ������
    glm::vec3 lightPos(0.5f, 0.5f, 2.0f);
    glm::vec3 lightColor(1.0f, 1.0f, 1.0f);
    unsigned int lightVAO;
    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);
    //ʹ�����е�VBO����
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // ���õ�������Ķ������ԣ������ǵĵ���˵����ֻ��λ�����ݣ�
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //��Ⱦѭ��
    while (!glfwWindowShouldClose(window))
    {
        //ˢ��ÿ֡��ʱ
        float currentFrame = glfwGetTime();
        Common::deltaTime = currentFrame - Common::lastFrame;
        Common::lastFrame = currentFrame;

        //�������
        Common::ProcessInput(Camera::getInstence(), window);

        //����
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //�����ɫ����Ȼ���

        //�仯�Ĺ�Դ��ɫ
        lightColor.x = sin(glfwGetTime() * 2.0f);
        lightColor.y = sin(glfwGetTime() * 0.7f);
        lightColor.z = sin(glfwGetTime() * 1.3f);
        glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f); // ����Ӱ��
        glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f); // �ܵ͵�Ӱ��

        //ģ�;���
        glm::mat4 model;
        //�۲����
        glm::mat4 cameraView = Camera::getInstence()->GetViewMatrix();
        //ͶӰ����
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
        shaderClass->setVec3("light.diffuse", diffuseColor); // �����յ�����һЩ�Դ��䳡��
        shaderClass->setVec3("light.specular", 1.0f, 1.0f, 1.0f);

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        //���ƹ�Դ
        glm::mat4 lightModel; //�ƹ�λ��
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
        glfwSwapBuffers(window); //��������
        glfwPollEvents();
    }

    //ɾ�������������Դ
    glfwTerminate();
    return;
}