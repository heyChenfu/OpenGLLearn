#include "HelloParallaxMapping.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <Windows.h>

#include "Common.h"
#include "Shader.h"

namespace HELLO_PARALLAX_MAPPING {
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

    HelloParallaxMapping::HelloParallaxMapping()
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
        Camera::getInstence()->CameraInit(glm::vec3(0.0f, 0.0f, 4.0f), glm::vec3(0.0f, 1.0f, 0.0f));

        //----------------------------------
        // shader
        //----------------------------------
        Shader shader("shader/5.1.parallax_mappingVS.txt", "shader/5.1.parallax_mappingFS.txt");
        Shader steepParallaxShader("shader/5.2.steep_parallax_mappingVS.txt", "shader/5.2.steep_parallax_mappingFS.txt");
        Shader parallaxOcclusionShader("shader/5.2.parallax_occlusion_mappingVS.txt", "shader/5.2.parallax_occlusion_mappingFS.txt");

        //----------------------------------
        // ��������
        //----------------------------------
        unsigned int quadVAO = 0;
        unsigned int quadVBO;
        // positions
        glm::vec3 pos1(-1.0f, 1.0f, 0.0f);
        glm::vec3 pos2(-1.0f, -1.0f, 0.0f);
        glm::vec3 pos3(1.0f, -1.0f, 0.0f);
        glm::vec3 pos4(1.0f, 1.0f, 0.0f);
        // texture coordinates
        glm::vec2 uv1(0.0f, 1.0f);
        glm::vec2 uv2(0.0f, 0.0f);
        glm::vec2 uv3(1.0f, 0.0f);
        glm::vec2 uv4(1.0f, 1.0f);
        // normal vector
        glm::vec3 nm(0.0f, 0.0f, 1.0f);

        // �������ߺ͸�����
        glm::vec3 tangent1, bitangent1;
        glm::vec3 tangent2, bitangent2;
        // triangle 1
        // ----------
        glm::vec3 edge1 = pos2 - pos1;
        glm::vec3 edge2 = pos3 - pos1;
        glm::vec2 deltaUV1 = uv2 - uv1;
        glm::vec2 deltaUV2 = uv3 - uv1;

        float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

        tangent1.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
        tangent1.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
        tangent1.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);

        bitangent1.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
        bitangent1.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
        bitangent1.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);

        // triangle 2
        // ----------
        edge1 = pos3 - pos1;
        edge2 = pos4 - pos1;
        deltaUV1 = uv3 - uv1;
        deltaUV2 = uv4 - uv1;

        f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

        tangent2.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
        tangent2.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
        tangent2.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);

        bitangent2.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
        bitangent2.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
        bitangent2.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);

        float quadVertices[] = {
            // positions                // normal               // texcoords // tangent                                        // bitangent
            pos1.x, pos1.y, pos1.z, nm.x, nm.y, nm.z, uv1.x, uv1.y, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,
            pos2.x, pos2.y, pos2.z, nm.x, nm.y, nm.z, uv2.x, uv2.y, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,
            pos3.x, pos3.y, pos3.z, nm.x, nm.y, nm.z, uv3.x, uv3.y, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,

            pos1.x, pos1.y, pos1.z, nm.x, nm.y, nm.z, uv1.x, uv1.y, tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z,
            pos3.x, pos3.y, pos3.z, nm.x, nm.y, nm.z, uv3.x, uv3.y, tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z,
            pos4.x, pos4.y, pos4.z, nm.x, nm.y, nm.z, uv4.x, uv4.y, tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z
        };
        // configure plane VAO
        glGenVertexArrays(1, &quadVAO);
        glGenBuffers(1, &quadVBO);
        glBindVertexArray(quadVAO);
        glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(8 * sizeof(float)));
        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(11 * sizeof(float)));

        //----------------------------------
        //��������
        //----------------------------------
        //deffuse�ͷ�����ͼ
        unsigned int diffuseMap = Common::GenerateTexture("res/bricks2.jpg");
        unsigned int normalMap = Common::GenerateTexture("res/bricks2_normal.jpg");
        unsigned int depthMap = Common::GenerateTexture("res/bricks2_disp.jpg");
        //���ò���
        shader.use();
        shader.setInt("diffuseMap", 0);
        shader.setInt("normalMap", 1);
        shader.setInt("depthMap", 2);
        steepParallaxShader.use();
        steepParallaxShader.setInt("diffuseMap", 0);
        steepParallaxShader.setInt("normalMap", 1);
        steepParallaxShader.setInt("depthMap", 2);
        parallaxOcclusionShader.use();
        parallaxOcclusionShader.setInt("diffuseMap", 0);
        parallaxOcclusionShader.setInt("normalMap", 1);
        parallaxOcclusionShader.setInt("depthMap", 2);

        //----------------------------------
        //��Դ
        //----------------------------------
        glm::vec3 lightPos(0.5f, 1.0f, 0.3f);

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
            glActiveTexture(GL_TEXTURE0); // �ڰ�����֮ǰ�ȼ�������Ԫ, 0Ĭ�����Ǳ�����
            glBindTexture(GL_TEXTURE_2D, diffuseMap);
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, normalMap);
            glActiveTexture(GL_TEXTURE2);
            glBindTexture(GL_TEXTURE_2D, depthMap);

            //ģ�;���
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(-1.5f, 0.0f, 0.0f));
            //�۲����
            glm::mat4 cameraView = Camera::getInstence()->GetViewMatrix();
            //ͶӰ����
            glm::mat4 projection
                = glm::perspective(glm::radians(Camera::getInstence()->Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
            
            //��ͨ�Ӳ���ͼ��Ⱦ
            shader.use();
            shader.setMat4("model", model);
            shader.setMat4("projection", projection);
            shader.setMat4("view", cameraView);
            shader.setVec3("lightPos", lightPos);
            shader.setVec3("viewPos", Camera::getInstence()->Position);
            shader.setFloat("height_scale", 0.1);
            glBindVertexArray(quadVAO);
            glDrawArrays(GL_TRIANGLES, 0, 6);
            glBindVertexArray(0);

            //�Ӳ��ڱ���ͼ
            model = glm::translate(model, glm::vec3(2.2f, 0.0f, 0.0f));
            parallaxOcclusionShader.use();
            parallaxOcclusionShader.setMat4("model", model);
            parallaxOcclusionShader.setMat4("projection", projection);
            parallaxOcclusionShader.setMat4("view", cameraView);
            parallaxOcclusionShader.setVec3("lightPos", lightPos);
            parallaxOcclusionShader.setVec3("viewPos", Camera::getInstence()->Position);
            parallaxOcclusionShader.setFloat("height_scale", 0.1);
            glBindVertexArray(quadVAO);
            glDrawArrays(GL_TRIANGLES, 0, 6);
            glBindVertexArray(0);

            //�����Ӳ���ͼ
            model = glm::translate(model, glm::vec3(2.2f, 0.0f, 0.0f));
            steepParallaxShader.use();
            steepParallaxShader.setMat4("model", model);
            steepParallaxShader.setMat4("projection", projection);
            steepParallaxShader.setMat4("view", cameraView);
            steepParallaxShader.setVec3("lightPos", lightPos);
            steepParallaxShader.setVec3("viewPos", Camera::getInstence()->Position);
            steepParallaxShader.setFloat("height_scale", 0.1);
            glBindVertexArray(quadVAO);
            glDrawArrays(GL_TRIANGLES, 0, 6);
            glBindVertexArray(0);

            glfwSwapBuffers(window); //��������
            glfwPollEvents();
        }

        //ɾ�������������Դ
        glfwTerminate();
        return;
    }

}