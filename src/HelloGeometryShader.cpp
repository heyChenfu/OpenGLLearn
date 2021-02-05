#include "HelloGeometryShader.h"
#include <iostream>
#include <Windows.h>
#include <direct.h> 
#include <functional>

#include "Common.h"
#include "Shader.h"
#include "Model.h"

namespace HELLO_GEOMETRY_SHADER {

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

    HelloGeometryShader::HelloGeometryShader() {
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
        Camera::getInstence()->CameraInit(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 1.0f, 0.0f));

        //----------------------------------
        // shader
        //----------------------------------
        std::string vertexPath = "shader/9.1.geometry_shaderVertex.txt";
        std::string geometryPath = "shader/9.1.geometry_shaderGeometry.txt";
        std::string fragmentPath = "shader/9.1.geometry_shaderFragment.txt";
        Shader* pointShader = new Shader(vertexPath.c_str(), fragmentPath.c_str(), geometryPath.c_str());
        std::string vertexPath2 = "shader/normalExplodeShaderVertex.txt";
        std::string geometryPath2 = "shader/normalExplodeShaderGeometry.txt";
        std::string fragmentPath2 = "shader/normalExplodeShaderFragment.txt";
        Shader* cubeNormalExplodeShader = new Shader(vertexPath2.c_str(), fragmentPath2.c_str(), geometryPath2.c_str());
        std::string vertexPath3 = "shader/6.1.cubemapsVertex.txt";
        std::string fragmentPath3 = "shader/6.1.cubemapsFragment.txt";
        Shader* cubeShader = new Shader(vertexPath3.c_str(), fragmentPath3.c_str());

        //----------------------------------
        //���ö�������(�ͻ�����)�����ö�������
        //----------------------------------
        //��ͨ����
        float points[] = {
        -0.5f,  0.5f, 1.0f, 0.0f, 0.0f, // ����
         0.5f,  0.5f, 0.0f, 1.0f, 0.0f, // ����
         0.5f, -0.5f, 0.0f, 0.0f, 1.0f, // ����
        -0.5f, -0.5f, 1.0f, 1.0f, 0.0f  // ����
        };
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

        //draw point
        unsigned int VBO, VAO;
        glGenVertexArrays(1, &VAO); //����һ��VAO����
        glGenBuffers(1, &VBO);
        glBindVertexArray(VAO);
        //�����ǵĶ������鸴�Ƶ�һ�����㻺����, ��OpenGLʹ��
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(points), &points, GL_STATIC_DRAW);
        // ���ö�������ָ��
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));
        glBindBuffer(GL_ARRAY_BUFFER, 0); //�����

        //cube
        unsigned int cubeVBO, cubeVAO;
        glGenVertexArrays(1, &cubeVAO); //����һ��VAO����
        glGenBuffers(1, &cubeVBO);
        glBindVertexArray(cubeVAO);
        //�����ǵĶ������鸴�Ƶ�һ�����㻺����, ��OpenGLʹ��
        glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        // ���ö�������ָ��
        char pointerIndex = 0;
        char stride = 8; //����
        //���Ӷ�������
        glVertexAttribPointer(pointerIndex, 3, GL_FLOAT, GL_FALSE, stride * sizeof(float), 0);
        glEnableVertexAttribArray(pointerIndex); //����ָ������, �ڶ�����ɫ���з����𶥵����������
        //���ӷ�������
        glVertexAttribPointer(++pointerIndex, 3, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(pointerIndex);
        //��������
        glVertexAttribPointer(++pointerIndex, 2, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(pointerIndex);

        //----------------------------------
        //��������
        //----------------------------------
        //������һ������
        unsigned int texture1 = Common::GenerateTexture("res/container2.jpg");
        //���ò���
        cubeShader->use(); // ��Ҫ����������uniform����֮ǰ������ɫ������
        cubeShader->setInt("texture1", 0);

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

            glm::mat4 model; //ģ�;���
            glm::mat4 cameraView = Camera::getInstence()->GetViewMatrix(); //�۲����
            glm::mat4 projection = glm::perspective(glm::radians(Camera::getInstence()->Zoom), 
                (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f); //ͶӰ����

            //ͨ��geometryShader���Ƹ���ĵ��Ч��
            pointShader->use();
            glBindVertexArray(VAO);
            glDrawArrays(GL_POINTS, 0, 4);
            glBindVertexArray(0);

            //cube
            cubeShader->use();
            cubeShader->setMat4("model", model);
            cubeShader->setMat4("view", cameraView);
            cubeShader->setMat4("projection", projection);
            glBindVertexArray(cubeVAO);
            glDrawArrays(GL_TRIANGLES, 0, 36);
            glBindVertexArray(0);
            //���߿��ӻ�
            cubeNormalExplodeShader->use();
            cubeNormalExplodeShader->setMat4("model", model);
            cubeNormalExplodeShader->setMat4("view", cameraView);
            cubeNormalExplodeShader->setMat4("projection", projection);
            glBindVertexArray(cubeVAO);
            glDrawArrays(GL_TRIANGLES, 0, 36);
            glBindVertexArray(0);

            glfwSwapBuffers(window); //��������
            glfwPollEvents();
        }

        //ɾ�������������Դ
        glfwTerminate();
        return;
    }

}