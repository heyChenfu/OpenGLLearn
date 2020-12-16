#include "HelloOpenGL.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <Windows.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "CommonFunction.h"
#include "Shader.h"
#include "stb_imageImplement.h"
#include "Camera.h"

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

float deltaTime = 0.0f; //ʱ������, ������Ⱦ��һ֡���õ�ʱ��, �ٶȳ���deltaTimeֵ����Ӧƽ�������֡��Ӱ��
float lastFrame = 0.0f; // ��һ֡��ʱ��
float lastX = SCR_WIDTH / 2, lastY = SCR_HEIGHT / 2;

Camera* camera = nullptr;

bool firstMouse = true;

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

//@desc: �������
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        if (camera != nullptr) camera->ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        if (camera != nullptr) camera->ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        if (camera != nullptr) camera->ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        if (camera != nullptr) camera->ProcessKeyboard(RIGHT, deltaTime);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) 
{
    if (firstMouse) // ���bool������ʼʱ���趨Ϊtrue��
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // ע���������෴�ģ���Ϊy�����Ǵӵײ����������������
    lastX = xpos;
    lastY = ypos;

    camera->ProcessMouseMovement(xoffset, yoffset, deltaTime);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera->ProcessMouseScroll(yoffset);
}

unsigned int GenerateVAO(float * vertices, int verticeslen, unsigned int * indices, int indicesLen, int iStride) {
    //���㻺�����(Vertex Buffer Objects, VBO)��������GPU�ڴ�(ͨ������Ϊ�Դ�)�д����������
    //�����������(Vertex Array Object, VAO)�����񶥵㻺������������󶨣��κ����Ķ������Ե��ö��ᴢ�������VAO��
    //����������� (Element Buffer Objectss)
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

unsigned int GenerateTexture(std::string sTexturePath) {
    unsigned int texture = 0;
    glGenTextures(1/*���ɵ�������Ŀ*/, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    //����ͼƬ
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true); //OpenGLҪ��y��0.0��������ͼƬ�ĵײ��ģ�����ͼƬ��y��0.0����ͨ���ڶ���
    unsigned char* data = stbi_load(sTexturePath.c_str(), &width, &height, &nrChannels, 0);
    if (data == nullptr)
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    else {
        //�����ͼƬ����
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D); //Ϊ��ǰ�󶨵������Զ�����������Ҫ�Ķ༶��Զ����
        //�ͷ�ͼ����ڴ���һ���ܺõ�ϰ��
        stbi_image_free(data);
    }
    return texture;
}

int main()
{
    //-----------------------------------
    // glfw ���ڴ���
    //-----------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
    glEnable(GL_DEPTH_TEST);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    //----------------------------------
    // ���������
    //----------------------------------
    camera = new Camera(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 1.0f, 0.0f));

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
    unsigned int texture1 = GenerateTexture("res/container.jpg");
    //�����ڶ�������
    unsigned int texture2 = GenerateTexture("res/awesomeface.jpg");
    //���ò���
    shaderClass->use(); // ��Ҫ����������uniform����֮ǰ������ɫ������
    shaderClass->setInt("texture1", 0);
    shaderClass->setInt("texture2", 1);

    //��Ⱦѭ��
    while (!glfwWindowShouldClose(window))
    {
        //ˢ��ÿ֡��ʱ
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        //�������
        processInput(window);

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
        //model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
        //�۲����
        glm::mat4 cameraView;
        //����һ��LookAt�������ǿ��԰����������ǵĹ۲����
        cameraView = glm::lookAt(camera->Position, camera->Position + camera->Front, camera->Up);
        //ͶӰ����
        glm::mat4 projection;
        projection = glm::perspective(glm::radians(camera->Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
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
    return 0;
}