#include "HelloInstancing.h"
#include <iostream>
#include <Windows.h>
#include <direct.h> 
#include <functional>

#include "Common.h"
#include "Shader.h"
#include "Model.h"

namespace HELLO_INSTANCING {

    bool firstMouse = true;

    HelloInstancing::HelloInstancing() {
        //-----------------------------------
        // glfw ���ڴ���
        //-----------------------------------
        GLFWwindow* window = Common::CreateGLFWWindow();
        //----------------------------------
        // ���������
        //----------------------------------
        Camera::getInstence()->CameraInit(glm::vec3(0.0f, 0.0f, 100.0f), glm::vec3(0.0f, 1.0f, 0.0f));

        //----------------------------------
        // shader
        //----------------------------------
        std::string vertexPath = "shader/1.model_loadingVS.txt";
        std::string fragmentPath = "shader/1.model_loadingFS.txt";
        Shader* planetShader = new Shader(vertexPath.c_str(), fragmentPath.c_str());
        std::string vertexPath2 = "shader/instancingShaderVertex.txt";
        std::string fragmentPath2 = "shader/instancingShaderFragment.txt";
        Shader* instancingShader = new Shader(vertexPath2.c_str(), fragmentPath2.c_str());

        //----------------------------------
        //���ö�������(�ͻ�����)�����ö�������
        //----------------------------------
        //��ʼ���λ��
        unsigned int amount = 1000;
        glm::mat4* modelMatrices;
        modelMatrices = new glm::mat4[amount];
        srand(glfwGetTime()); // ��ʼ���������    
        float radius = 50.0;
        float offset = 2.5f;
        for (unsigned int i = 0; i < amount; i++)
        {
            glm::mat4 model;
            // 1. λ�ƣ��ֲ��ڰ뾶Ϊ 'radius' ��Բ���ϣ�ƫ�Ƶķ�Χ�� [-offset, offset]
            float angle = (float)i / (float)amount * 360.0f;
            float displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
            float x = sin(angle) * radius + displacement;
            displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
            float y = displacement * 0.4f; // �����Ǵ��ĸ߶ȱ�x��z�Ŀ��ҪС
            displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
            float z = cos(angle) * radius + displacement;
            model = glm::translate(model, glm::vec3(x, y, z));

            // 2. ���ţ��� 0.05 �� 0.25f ֮������
            float scale = (rand() % 20) / 100.0f + 0.05;
            model = glm::scale(model, glm::vec3(scale));

            // 3. ��ת������һ�����룩���ѡ�����ת�����������������ת
            float rotAngle = (rand() % 360);
            model = glm::rotate(model, rotAngle, glm::vec3(0.4f, 0.6f, 0.8f));

            // 4. ��ӵ������������
            modelMatrices[i] = model;
        }

        //----------------------------------
        // load models
        //----------------------------------
        string directionPath = _getcwd(NULL, 0);
        Model planetModel(directionPath + "/res/objects/planet/planet.obj");
        Model rockModel(directionPath + "/res/objects/rock/rock.obj");

        // ���㻺�����
        unsigned int instancingVBO;
        glGenBuffers(1, &instancingVBO); //���ƶ������鵽�����й�OpenGLʹ��
        glBindBuffer(GL_ARRAY_BUFFER, instancingVBO);
        glBufferData(GL_ARRAY_BUFFER, amount * sizeof(glm::mat4), &modelMatrices[0], GL_STATIC_DRAW);

        for (unsigned int i = 0; i < rockModel.GetMeshesCount(); i++)
        {
            unsigned int VAO = rockModel.GetVAO(i);
            glBindVertexArray(VAO);
            //�������������������ݴ�С����һ��vec4����Ϊһ��mat4��������4��vec4��������ҪΪ�������Ԥ��4����������
            GLsizei vec4Size = sizeof(glm::vec4);
            glEnableVertexAttribArray(3);
            glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)0); //���ö�������ָ��
            glEnableVertexAttribArray(4);
            glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(vec4Size));
            glEnableVertexAttribArray(5);
            glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(2 * vec4Size));
            glEnableVertexAttribArray(6);
            glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(3 * vec4Size));

            /*
            ʹ��glVertexAttribDivisor(GLuint index, GLuint divisor)������Ϊindex������ָ��Ϊinstance���ԣ�
            ÿ��divisor��instance��vertex shader��ע��bufferһ���µ�����ֵ
            */
            glVertexAttribDivisor(3, 1); //ָ��1��ʾÿ��ʵ������һ������
            glVertexAttribDivisor(4, 1);
            glVertexAttribDivisor(5, 1);
            glVertexAttribDivisor(6, 1);

            glBindVertexArray(0);
        }

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

            //---------------------------------
            //draw
            //---------------------------------
            glm::mat4 cameraView = Camera::getInstence()->GetViewMatrix(); //�۲����
            glm::mat4 projection = glm::perspective(glm::radians(Camera::getInstence()->Zoom), 
                (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f); //ͶӰ����

            // ��������
            planetShader->use();
            glm::mat4 model;
            model = glm::translate(model, glm::vec3(0.0f, -3.0f, 0.0f));
            model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
            planetShader->setMat4("model", model);
            planetShader->setMat4("projection", projection);
            planetShader->setMat4("view", cameraView);
            planetModel.Draw(*planetShader);

            // ����С����
            instancingShader->use();
            instancingShader->setMat4("projection", projection);
            instancingShader->setMat4("view", cameraView);
            rockModel.DrawInstanced(*instancingShader, amount);

            glfwSwapBuffers(window); //��������
            glfwPollEvents();
        }

        //ɾ�������������Դ
        glfwTerminate();
        return;
    }

}