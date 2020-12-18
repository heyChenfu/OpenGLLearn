#include "HelloOpenGL.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <Windows.h>

#include "Common.h"
#include "Shader.h"

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

unsigned int GenerateVAO(float * vertices, int verticeslen, unsigned int * indices, int indicesLen, int iStride) {
    //���㻺�����(Vertex Buffer Objects, VBO)��������GPU�ڴ�(ͨ������Ϊ�Դ�)�д����������
    //�����������(Vertex Array Object, VAO)�����񶥵㻺������������󶨣��κ����Ķ������Ե��ö��ᴢ�������VAO��
    //����������� (Element Buffer Objects) ���涥�������, �趨������Щ�����˳��ʹ��glDrawElementsʱ, ��ʹ�õ�ǰ�󶨵�������������е��������л���
    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO); //����һ��VAO����
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO); //����һ��EBO����
    glBindVertexArray(VAO);
    //�����ǵĶ������鸴�Ƶ�һ�����㻺����, ��OpenGLʹ��
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, verticeslen, vertices, GL_STATIC_DRAW);
    //�������ǵ��������鵽һ������������, ��OpenGLʹ��
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesLen, indices, GL_STATIC_DRAW);
    // ���ö�������ָ��
    char pointerIndex = 0;
    //���Ӷ�������
    glVertexAttribPointer(pointerIndex, 3, GL_FLOAT, GL_FALSE, iStride * sizeof(float)/*����,�����Ķ���������֮��ļ��*/, (void*)0);
    glEnableVertexAttribArray(pointerIndex); //����ָ������, �ڶ�����ɫ���з����𶥵����������
    // ��ɫ����
    //glVertexAttribPointer(++pointerIndex, 3, GL_FLOAT, GL_FALSE, iStride * sizeof(float), (void*)(pointerIndex * 3 * sizeof(float)));
    //glEnableVertexAttribArray(pointerIndex);
    //��������
    glVertexAttribPointer(++pointerIndex, 2, GL_FLOAT, GL_FALSE, iStride * sizeof(float), (void*)(pointerIndex * 3 * sizeof(float)));
    glEnableVertexAttribArray(pointerIndex);
    //�����
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    return VAO;
}

HelloOpenGL::HelloOpenGL()
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
    Camera::getInstence()->CameraInit(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    //----------------------------------
    // shader
    //----------------------------------
    std::string vertexPath = "shader/vertexShader.txt";
    std::string fragmentPath = "shader/fragmentShader.txt";
    Shader * shaderClass = new Shader(vertexPath.c_str(), fragmentPath.c_str());

    //----------------------------------
    //���ö�������(�ͻ�����)�����ö�������
    //----------------------------------
    //float vertices[] = {
    //    //     ---- λ�� ----     ---- ��ɫ ----      - �������� -
    //     0.5f,  0.5f, 0.0f,         1.0f, 0.0f, 0.0f,       1.0f, 1.0f,   // ����
    //     0.5f, -0.5f, 0.0f,         0.0f, 1.0f, 0.0f,       1.0f, 0.0f,   // ����
    //    -0.5f, -0.5f, 0.0f,         0.0f, 0.0f, 1.0f,       0.0f, 0.0f,   // ����
    //    -0.5f,  0.5f, 0.0f,         1.0f, 1.0f, 0.0f,       0.0f, 1.0f    // ����
    //};
    float vertices[] = {
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };
    unsigned int indices[] = {
    0, 1, 2,
    2, 3, 0
    };
    unsigned int VAO = GenerateVAO(vertices, sizeof(vertices), indices, sizeof(indices), 5);

    //----------------------------------
    //��������
    //----------------------------------
    //������һ������
    unsigned int texture1 = Common::GenerateTexture("res/container.jpg");
    //�����ڶ�������
    unsigned int texture2 = Common::GenerateTexture("res/awesomeface.jpg");
    //���ò���
    shaderClass->use(); // ��Ҫ����������uniform����֮ǰ������ɫ������
    shaderClass->setInt("texture1", 0);
    shaderClass->setInt("texture2", 1);

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
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);

        //ģ�;���
        glm::mat4 model;
        //model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f)/*��ת�Ƕ�*/, glm::vec3(0.5f, 1.0f, 0.0f)/*��ת��*/);
        //�۲����
        glm::mat4 cameraView;
        //����һ��LookAt�������ǿ��԰����������ǵĹ۲����
        cameraView = glm::lookAt(
            Camera::getInstence()->Position, Camera::getInstence()->Position + Camera::getInstence()->Front, Camera::getInstence()->Up);
        //ͶӰ����
        glm::mat4 projection;
        projection = glm::perspective(glm::radians(Camera::getInstence()->Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        shaderClass->setMat4("model", model);
        shaderClass->setMat4("view", cameraView);
        shaderClass->setMat4("projection", projection);

        shaderClass->use();
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        glfwSwapBuffers(window); //��������
        glfwPollEvents();
    }

    //ɾ�������������Դ
    glfwTerminate();
    return;
}