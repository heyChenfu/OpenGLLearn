#include "HelloOpenGL.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "CommonFunction.h"

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

int main()
{
    //------------------------------------------------------
    // glfw ���ڴ���
    //------------------------------------------------------
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

    glViewport(0, 0, 800, 600);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    //------------------------------------------------------
    //������ɫ��
    //------------------------------------------------------
    const char* vertexShaderSource = "#version 330 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "void main()\n"
        "{\n"
        "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
        "}\0";
    //������ɫ������
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    //����ɫ��Դ�븽�ӵ���ɫ��������
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    //�Ƿ�ɹ�����
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    //------------------------------------------------------
    //ƬԪ��ɫ��
    //------------------------------------------------------
    const char* fragmentShaderSource = "#version 330 core\n"
        "out vec4 FragColor;\n"
        "void main()\n"
        "{\n"
        "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
        "}\n\0";
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    //------------------------------------------------------
    //��ɫ������
    //------------------------------------------------------
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    //���������ɫ�������Ƿ�ʧ��
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER_PROGRAM::LINK_FAILED\n" << infoLog << std::endl;
    }
    //��������������
    glUseProgram(shaderProgram);
    //����ɫ���������ӵ���������Ժ�, ɾ����ɫ������, ������Ҫ������
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    //------------------------------------------------------
    //���ö�������(�ͻ�����)�����ö�������
    //------------------------------------------------------
    float vertices[] = {
        0.5f, 0.5f, 0.0f,   // ���Ͻ�
        0.5f, -0.5f, 0.0f,  // ���½�
        -0.5f, -0.5f, 0.0f, // ���½�
        -0.5f, 0.5f, 0.0f   // ���Ͻ�
    };
    unsigned int indices[] = { // ע��������0��ʼ! 
    0, 1, 3, // ��һ��������
    1, 2, 3  // �ڶ���������
    };
    //���㻺�����(Vertex Buffer Objects, VBO)��������GPU�ڴ棨ͨ������Ϊ�Դ棩�д����������
    //����������� Element Buffer Objectss
    //�����������(Vertex Array Object, VAO)�����񶥵㻺������������󶨣��κ����Ķ������Ե��ö��ᴢ�������VAO��
    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO); //����һ��VAO����
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO); //����һ��EBO����
    glBindVertexArray(VAO);
    //�����ǵĶ������鸴�Ƶ�һ�����㻺����, ��OpenGLʹ��
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    //�������ǵ��������鵽һ������������, ��OpenGLʹ��
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    // ���ö�������ָ��
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    //�����
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    //��Ⱦѭ��
    while (!glfwWindowShouldClose(window))
    {
        //�������
        processInput(window);

        //����
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        //draw
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        //glDrawArrays(GL_TRIANGLES, 0, 6);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        glfwPollEvents();
        //��������
        glfwSwapBuffers(window);
    }

    //ɾ�������������Դ
    glfwTerminate();
    return 0;
}