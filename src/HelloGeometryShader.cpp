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

    HelloGeometryShader::HelloGeometryShader() {
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
        //设置顶点数据(和缓冲区)和配置顶点属性
        //----------------------------------
        //普通盒子
        float points[] = {
        -0.5f,  0.5f, 1.0f, 0.0f, 0.0f, // 左上
         0.5f,  0.5f, 0.0f, 1.0f, 0.0f, // 右上
         0.5f, -0.5f, 0.0f, 0.0f, 1.0f, // 右下
        -0.5f, -0.5f, 1.0f, 1.0f, 0.0f  // 左下
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
        glGenVertexArrays(1, &VAO); //生成一个VAO对象
        glGenBuffers(1, &VBO);
        glBindVertexArray(VAO);
        //把我们的顶点数组复制到一个顶点缓冲中, 供OpenGL使用
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(points), &points, GL_STATIC_DRAW);
        // 设置顶点属性指针
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));
        glBindBuffer(GL_ARRAY_BUFFER, 0); //解除绑定

        //cube
        unsigned int cubeVBO, cubeVAO;
        glGenVertexArrays(1, &cubeVAO); //生成一个VAO对象
        glGenBuffers(1, &cubeVBO);
        glBindVertexArray(cubeVAO);
        //把我们的顶点数组复制到一个顶点缓冲中, 供OpenGL使用
        glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
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

        //----------------------------------
        //生成纹理
        //----------------------------------
        //创建第一个纹理
        unsigned int texture1 = Common::GenerateTexture("res/container2.jpg");
        //设置采样
        cubeShader->use(); // 不要忘记在设置uniform变量之前激活着色器程序！
        cubeShader->setInt("texture1", 0);

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

            glm::mat4 model; //模型矩阵
            glm::mat4 cameraView = Camera::getInstence()->GetViewMatrix(); //观察矩阵
            glm::mat4 projection = glm::perspective(glm::radians(Camera::getInstence()->Zoom), 
                (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f); //投影矩阵

            //通过geometryShader绘制更多的点和效果
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
            //法线可视化
            cubeNormalExplodeShader->use();
            cubeNormalExplodeShader->setMat4("model", model);
            cubeNormalExplodeShader->setMat4("view", cameraView);
            cubeNormalExplodeShader->setMat4("projection", projection);
            glBindVertexArray(cubeVAO);
            glDrawArrays(GL_TRIANGLES, 0, 36);
            glBindVertexArray(0);

            glfwSwapBuffers(window); //交换缓冲
            glfwPollEvents();
        }

        //删除分配的所有资源
        glfwTerminate();
        return;
    }

}