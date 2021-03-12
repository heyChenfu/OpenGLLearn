#include "HelloBloom.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <Windows.h>

#include "Common.h"
#include "Shader.h"

namespace HELLO_BLOOM {
    bool firstMouse = true;

    void framebuffer_size_callback(GLFWwindow* window, int width, int height)
    {
        glViewport(0, 0, width, height);
    }

    void mouse_callback(GLFWwindow* window, double xpos, double ypos)
    {
        if (firstMouse) // ���bool������ʼʱ���趨Ϊtrue��
        {
            Common::lastX = xpos;
            Common::lastY = ypos;
            firstMouse = false;
        }
        float xoffset = xpos - Common::lastX;
        float yoffset = Common::lastY - ypos; // ע���������෴�ģ���Ϊy�����Ǵӵײ����������������
        Common::lastX = xpos;
        Common::lastY = ypos;

        Camera::getInstence()->ProcessMouseMovement(xoffset, yoffset, Common::deltaTime);
    }

    void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
    {
        Camera::getInstence()->ProcessMouseScroll(yoffset);
    }

    HelloBloom::HelloBloom()
    {
        //-----------------------------------
        // glfw ���ڴ���
        //-----------------------------------
        GLFWwindow* window = Common::CreateGLFWWindow();
        glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        glfwSetCursorPosCallback(window, mouse_callback);
        glfwSetScrollCallback(window, scroll_callback);

        //----------------------------------
        // ���������
        //----------------------------------
        Camera::getInstence()->CameraInit(glm::vec3(0.0f, 0.0f, 5.0f), glm::vec3(0.0f, 1.0f, 0.0f));

        //----------------------------------
        //����֡����
        //----------------------------------
        //����������Ⱦ�����ͳ�������ɫ�ﵽһ����ֵ�Ļ���
        unsigned int hdrFBO;
        glGenFramebuffers(1, &hdrFBO);
        glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);
        //����������ɫ����, ��һ����Ⱦ����������ɫ������
        unsigned int colorBuffer[2];
        glGenTextures(2, colorBuffer);
        GLuint attachments[2];
        for (int i = 0; i < 2; i++) {
            glBindTexture(GL_TEXTURE_2D, colorBuffer[i]);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_FLOAT, NULL);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            //���ӵ�֡����
            attachments[i] = GL_COLOR_ATTACHMENT0 + i;
            glFramebufferTexture2D(
                GL_FRAMEBUFFER, attachments[i]/*һ��֡������Ӷ����ɫ����*/, GL_TEXTURE_2D, colorBuffer[i], 0);
        }
        //���ݶ����ɫ������ö����ʽ��֪OpenGL��������ͨ��glDrawBuffers��Ⱦ�������ɫ����
        glDrawBuffers(2, attachments);
        //������Ȼ���
        unsigned int depthBuff;
        glGenRenderbuffers(1, &depthBuff);
        glBindRenderbuffer(GL_RENDERBUFFER, depthBuff);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, SCR_WIDTH, SCR_HEIGHT); //ָ���洢��renderbuffer��ͼ��Ŀ���Լ���ɫ��ʽ
        //���ӵ�֡����
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuff);

        //���֡�����Ƿ����óɹ�
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            std::cout << "֡��������ʧ�ܣ�������룡" << std::endl;
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        //ʹ������֡�������������˹ģ��
        unsigned int gaussFBO[2];
        unsigned int gaussBuffer[2];
        glGenFramebuffers(2, gaussFBO);
        glGenTextures(2, gaussBuffer);
        for (int i = 0; i < 2; i++) {
            glBindFramebuffer(GL_FRAMEBUFFER, gaussFBO[i]);
            glBindTexture(GL_TEXTURE_2D, gaussBuffer[i]);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_FLOAT, NULL);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gaussBuffer[i], 0);
        }
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        //----------------------------------
        //Shader
        //----------------------------------
        Shader shader("shader/7.bloomTwoColorBufferVertex.txt", "shader/7.bloomTwoColorBufferFragment.txt");
        Shader lightShader("shader/7.bloomTwoColorBufferVertex.txt", "shader/7.bloomTwoColorLightFragment.txt");
        Shader blurShader("shader/7.gaussBlurVertex.txt", "shader/7.gaussBlurFragment.txt");
        Shader blurFinalShader("shader/7.blurFinalVertex.txt", "shader/7.blurFinalFragment.txt");

        //----------------------------------
        // ��������
        //----------------------------------
        unsigned int woodTexture = Common::GenerateTexture("res/wood.jpg");
        unsigned int containerTexture = Common::GenerateTexture("res/container2.jpg");

        //����shader����
        shader.use();
        shader.setInt("diffuseTexture", 0);
        blurShader.use();
        blurShader.setInt("image", 0);
        blurFinalShader.use();
        blurFinalShader.setInt("scene", 0);
        blurFinalShader.setInt("bloomBlur", 1);

        //----------------------------------
        //��Դ
        //----------------------------------
        // positions
        std::vector<glm::vec3> lightPositions;
        lightPositions.push_back(glm::vec3(0.0f, 0.5f, 1.5f));
        lightPositions.push_back(glm::vec3(-4.0f, 0.5f, -3.0f));
        lightPositions.push_back(glm::vec3(3.0f, 0.5f, 1.0f));
        lightPositions.push_back(glm::vec3(-.8f, 2.4f, -1.0f));
        // colors
        std::vector<glm::vec3> lightColors;
        lightColors.push_back(glm::vec3(5.0f, 5.0f, 5.0f));
        lightColors.push_back(glm::vec3(10.0f, 0.0f, 0.0f));
        lightColors.push_back(glm::vec3(0.0f, 0.0f, 15.0f));
        lightColors.push_back(glm::vec3(0.0f, 5.0f, 0.0f));

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

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, woodTexture);

            //---------------------------------
            //draw
            //---------------------------------
            glm::mat4 cameraView = Camera::getInstence()->GetViewMatrix(); //�۲����
            glm::mat4 projection = 
                glm::perspective(glm::radians(Camera::getInstence()->Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f); //ͶӰ����
            glm::mat4 model = glm::mat4(1.0f); 

            glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //�����ǰ֡��������
            shader.use();
            shader.setMat4("projection", projection);
            shader.setMat4("view", cameraView);
            // set lighting uniforms
            for (unsigned int i = 0; i < lightPositions.size(); i++)
            {
                shader.setVec3("lights[" + std::to_string(i) + "].Position", lightPositions[i]);
                shader.setVec3("lights[" + std::to_string(i) + "].Color", lightColors[i]);
            }
            // create multiple cubes
            glBindTexture(GL_TEXTURE_2D, containerTexture);
            model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(0.0f, 1.5f, 0.0));
            model = glm::scale(model, glm::vec3(0.5f));
            shader.setMat4("model", model);
            renderCube();
            model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(2.0f, 0.0f, 1.0));
            model = glm::scale(model, glm::vec3(0.5f));
            shader.setMat4("model", model);
            renderCube();
            model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(-1.0f, -1.0f, 2.0));
            model = glm::rotate(model, glm::radians(60.0f), glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
            shader.setMat4("model", model);
            renderCube();
            model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(0.0f, 2.7f, 4.0));
            model = glm::rotate(model, glm::radians(23.0f), glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
            model = glm::scale(model, glm::vec3(1.25));
            shader.setMat4("model", model);
            renderCube();
            model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(-2.0f, 1.0f, -3.0));
            model = glm::rotate(model, glm::radians(124.0f), glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
            shader.setMat4("model", model);
            renderCube();
            model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(-3.0f, 0.0f, 0.0));
            model = glm::scale(model, glm::vec3(0.5f));
            shader.setMat4("model", model);
            renderCube();

            //��Դ����
            lightShader.use();
            lightShader.setMat4("projection", projection);
            lightShader.setMat4("view", cameraView);
            for (unsigned int i = 0; i < lightPositions.size(); i++)
            {
                model = glm::mat4(1.0f);
                model = glm::translate(model, glm::vec3(lightPositions[i]));
                model = glm::scale(model, glm::vec3(0.25f));
                lightShader.setMat4("model", model);
                lightShader.setVec3("lightColor", lightColors[i]);
                renderCube();
            }
            glBindFramebuffer(GL_FRAMEBUFFER, 0);

            //two pass gauss blur
            bool bHorizontal = true;
            bool bFirstLoop = true; //��һ��ѭ��ʹ����һ��֡�������ɫ����
            int iBlurCount = 10;
            blurShader.use();
            for (int i = 0; i < iBlurCount; i++) {
                glBindFramebuffer(GL_FRAMEBUFFER, gaussFBO[bHorizontal]);
                glBindTexture(GL_TEXTURE_2D, bFirstLoop ? colorBuffer[1] : gaussBuffer[!bHorizontal]);
                blurShader.setBool("horizontal", bHorizontal);
                blurShader.setInt("image", 0);
                renderQuad();
                bHorizontal = !bHorizontal;
                if (bFirstLoop) bFirstLoop = false;
            }
            glBindFramebuffer(GL_FRAMEBUFFER, 0);

            //�ϲ����
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            blurFinalShader.use();
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, colorBuffer[0]);
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, gaussBuffer[!bHorizontal]);
            blurFinalShader.setFloat("exposure", 1.0f);
            renderQuad();

            glfwSwapBuffers(window); //��������
            glfwPollEvents();
        }

        //ɾ�������������Դ
        glfwTerminate();
        return;
    }

    void HelloBloom::renderCube()
    {
        // initialize (if necessary)
        if (cubeVAO == 0)
        {
            float vertices[] = {
                // back face
                -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
                 1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
                 1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f, // bottom-right         
                 1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
                -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
                -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f, // top-left
                // front face
                -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
                 1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f, // bottom-right
                 1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
                 1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
                -1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f, // top-left
                -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
                // left face
                -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
                -1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-left
                -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
                -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
                -1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-right
                -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
                // right face
                 1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
                 1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
                 1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-right         
                 1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
                 1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
                 1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-left     
                // bottom face
                -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
                 1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f, // top-left
                 1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
                 1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
                -1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f, // bottom-right
                -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
                // top face
                -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
                 1.0f,  1.0f , 1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
                 1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f, // top-right     
                 1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
                -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
                -1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f  // bottom-left        
            };
            glGenVertexArrays(1, &cubeVAO);
            glGenBuffers(1, &cubeVBO);
            // fill buffer
            glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
            // link vertex attributes
            glBindVertexArray(cubeVAO);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
            glEnableVertexAttribArray(2);
            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindVertexArray(0);
        }
        // render Cube
        glBindVertexArray(cubeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
    }

    void HelloBloom::renderQuad()
    {
        if (quadVAO == 0)
        {
            float quadVertices[] = {
                // positions        // texture Coords
                -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
                -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
                 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
                 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
            };
            // setup plane VAO
            glGenVertexArrays(1, &quadVAO);
            glGenBuffers(1, &quadVBO);
            glBindVertexArray(quadVAO);
            glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        }
        glBindVertexArray(quadVAO);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        glBindVertexArray(0);
    }

}