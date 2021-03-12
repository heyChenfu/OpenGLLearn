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

    HelloBloom::HelloBloom()
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
        Camera::getInstence()->CameraInit(glm::vec3(0.0f, 0.0f, 5.0f), glm::vec3(0.0f, 1.0f, 0.0f));

        //----------------------------------
        //创建帧缓存
        //----------------------------------
        //保存正常渲染场景和场景中颜色达到一定阈值的缓冲
        unsigned int hdrFBO;
        glGenFramebuffers(1, &hdrFBO);
        glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);
        //创建两个颜色缓存, 第一次渲染到这两个颜色缓存中
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
            //附加到帧缓存
            attachments[i] = GL_COLOR_ATTACHMENT0 + i;
            glFramebufferTexture2D(
                GL_FRAMEBUFFER, attachments[i]/*一个帧缓存添加多个颜色缓存*/, GL_TEXTURE_2D, colorBuffer[i], 0);
        }
        //传递多个颜色附件的枚举显式告知OpenGL我们正在通过glDrawBuffers渲染到多个颜色缓冲
        glDrawBuffers(2, attachments);
        //创建深度缓存
        unsigned int depthBuff;
        glGenRenderbuffers(1, &depthBuff);
        glBindRenderbuffer(GL_RENDERBUFFER, depthBuff);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, SCR_WIDTH, SCR_HEIGHT); //指定存储在renderbuffer中图像的宽高以及颜色格式
        //附加到帧缓存
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuff);

        //检查帧缓存是否设置成功
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            std::cout << "帧缓存设置失败，请检查代码！" << std::endl;
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        //使用两个帧缓冲进行两步高斯模糊
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
        // 加载纹理
        //----------------------------------
        unsigned int woodTexture = Common::GenerateTexture("res/wood.jpg");
        unsigned int containerTexture = Common::GenerateTexture("res/container2.jpg");

        //设置shader纹理
        shader.use();
        shader.setInt("diffuseTexture", 0);
        blurShader.use();
        blurShader.setInt("image", 0);
        blurFinalShader.use();
        blurFinalShader.setInt("scene", 0);
        blurFinalShader.setInt("bloomBlur", 1);

        //----------------------------------
        //光源
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
            glBindTexture(GL_TEXTURE_2D, woodTexture);

            //---------------------------------
            //draw
            //---------------------------------
            glm::mat4 cameraView = Camera::getInstence()->GetViewMatrix(); //观察矩阵
            glm::mat4 projection = 
                glm::perspective(glm::radians(Camera::getInstence()->Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f); //投影矩阵
            glm::mat4 model = glm::mat4(1.0f); 

            glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //清除当前帧缓冲数据
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

            //光源物体
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
            bool bFirstLoop = true; //第一次循环使用上一个帧缓冲的颜色缓冲
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

            //合并输出
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            blurFinalShader.use();
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, colorBuffer[0]);
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, gaussBuffer[!bHorizontal]);
            blurFinalShader.setFloat("exposure", 1.0f);
            renderQuad();

            glfwSwapBuffers(window); //交换缓冲
            glfwPollEvents();
        }

        //删除分配的所有资源
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