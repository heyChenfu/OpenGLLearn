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
        // glfw 窗口创建
        //-----------------------------------
        GLFWwindow* window = Common::CreateGLFWWindow();
        //----------------------------------
        // 创建摄像机
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
        //设置顶点数据(和缓冲区)和配置顶点属性
        //----------------------------------
        //初始随机位置
        unsigned int amount = 1000;
        glm::mat4* modelMatrices;
        modelMatrices = new glm::mat4[amount];
        srand(glfwGetTime()); // 初始化随机种子    
        float radius = 50.0;
        float offset = 2.5f;
        for (unsigned int i = 0; i < amount; i++)
        {
            glm::mat4 model;
            // 1. 位移：分布在半径为 'radius' 的圆形上，偏移的范围是 [-offset, offset]
            float angle = (float)i / (float)amount * 360.0f;
            float displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
            float x = sin(angle) * radius + displacement;
            displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
            float y = displacement * 0.4f; // 让行星带的高度比x和z的宽度要小
            displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
            float z = cos(angle) * radius + displacement;
            model = glm::translate(model, glm::vec3(x, y, z));

            // 2. 缩放：在 0.05 和 0.25f 之间缩放
            float scale = (rand() % 20) / 100.0f + 0.05;
            model = glm::scale(model, glm::vec3(scale));

            // 3. 旋转：绕着一个（半）随机选择的旋转轴向量进行随机的旋转
            float rotAngle = (rand() % 360);
            model = glm::rotate(model, rotAngle, glm::vec3(0.4f, 0.6f, 0.8f));

            // 4. 添加到矩阵的数组中
            modelMatrices[i] = model;
        }

        //----------------------------------
        // load models
        //----------------------------------
        string directionPath = _getcwd(NULL, 0);
        Model planetModel(directionPath + "/res/objects/planet/planet.obj");
        Model rockModel(directionPath + "/res/objects/rock/rock.obj");

        // 顶点缓冲对象
        unsigned int instancingVBO;
        glGenBuffers(1, &instancingVBO); //复制顶点数组到缓冲中供OpenGL使用
        glBindBuffer(GL_ARRAY_BUFFER, instancingVBO);
        glBufferData(GL_ARRAY_BUFFER, amount * sizeof(glm::mat4), &modelMatrices[0], GL_STATIC_DRAW);

        for (unsigned int i = 0; i < rockModel.GetMeshesCount(); i++)
        {
            unsigned int VAO = rockModel.GetVAO(i);
            glBindVertexArray(VAO);
            //顶点属性最大允许的数据大小等于一个vec4。因为一个mat4本质上是4个vec4，我们需要为这个矩阵预留4个顶点属性
            GLsizei vec4Size = sizeof(glm::vec4);
            glEnableVertexAttribArray(3);
            glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)0); //设置顶点属性指针
            glEnableVertexAttribArray(4);
            glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(vec4Size));
            glEnableVertexAttribArray(5);
            glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(2 * vec4Size));
            glEnableVertexAttribArray(6);
            glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(3 * vec4Size));

            /*
            使用glVertexAttribDivisor(GLuint index, GLuint divisor)将序数为index的属性指定为instance属性，
            每隔divisor个instance，vertex shader中注进buffer一个新的属性值
            */
            glVertexAttribDivisor(3, 1); //指定1表示每个实例更新一次数据
            glVertexAttribDivisor(4, 1);
            glVertexAttribDivisor(5, 1);
            glVertexAttribDivisor(6, 1);

            glBindVertexArray(0);
        }

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
            glm::mat4 cameraView = Camera::getInstence()->GetViewMatrix(); //观察矩阵
            glm::mat4 projection = glm::perspective(glm::radians(Camera::getInstence()->Zoom), 
                (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f); //投影矩阵

            // 绘制行星
            planetShader->use();
            glm::mat4 model;
            model = glm::translate(model, glm::vec3(0.0f, -3.0f, 0.0f));
            model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
            planetShader->setMat4("model", model);
            planetShader->setMat4("projection", projection);
            planetShader->setMat4("view", cameraView);
            planetModel.Draw(*planetShader);

            // 绘制小行星
            instancingShader->use();
            instancingShader->setMat4("projection", projection);
            instancingShader->setMat4("view", cameraView);
            rockModel.DrawInstanced(*instancingShader, amount);

            glfwSwapBuffers(window); //交换缓冲
            glfwPollEvents();
        }

        //删除分配的所有资源
        glfwTerminate();
        return;
    }

}