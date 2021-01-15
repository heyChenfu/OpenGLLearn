#include "HelloModel.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <Windows.h>
#include <direct.h> 

#include "filesystem.h"
#include "Common.h"
#include "Shader.h"
#include "Model.h"

HelloModel::HelloModel() {
    //-----------------------------------
    // glfw ���ڴ���
    //-----------------------------------
    GLFWwindow* window = Common::CreateGLFWWindow();
    //----------------------------------
    // ���������
    //----------------------------------
    Camera::getInstence()->CameraInit(glm::vec3(2.0f, 2.0f, 10.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    //----------------------------------
    // shader
    //----------------------------------
    std::string vertexPath = "shader/1.model_loadingVS.txt";
    std::string fragmentPath = "shader/1.model_loadingFS.txt";
    Shader* shaderClass = new Shader(vertexPath.c_str(), fragmentPath.c_str());

    //----------------------------------
    // load models
    //----------------------------------
    string directionPath = _getcwd(NULL, 0);
    Model currModel(directionPath + "/res/objects/greymon/PM3D_Sphere3D7_1.obj");

    //��Ⱦѭ��
    while (!glfwWindowShouldClose(window)) {
        //ˢ��ÿ֡��ʱ
        float currentFrame = glfwGetTime();
        Common::deltaTime = currentFrame - Common::lastFrame;
        Common::lastFrame = currentFrame;

        //�������
        Common::ProcessInput(Camera::getInstence(), window);

        //����
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shaderClass->use();
        glm::mat4 projection = glm::perspective(glm::radians(Camera::getInstence()->Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = Camera::getInstence()->GetViewMatrix();
        shaderClass->setMat4("projection", projection);
        shaderClass->setMat4("view", view);
        // render the loaded model
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
        model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down
        shaderClass->setMat4("model", model);
        currModel.Draw(*shaderClass);

        glfwSwapBuffers(window); //��������
        glfwPollEvents();
    }

    glfwTerminate(); //ɾ�������������Դ
}