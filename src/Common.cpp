#include "Common.h"
#include "stb_imageImplement.h"

float Common::deltaTime = 0.0f; //ʱ������, ������Ⱦ��һ֡���õ�ʱ��, �ٶȳ���deltaTimeֵ����Ӧƽ�������֡��Ӱ��
float Common::lastFrame = 0.0f; // ��һ֡��ʱ��
float Common::lastX = SCR_WIDTH / 2;
float Common::lastY = SCR_HEIGHT / 2;

//@desc: glfw ���ڴ���
GLFWwindow* Common::CreateGLFWWindow() {
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
        return NULL;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return NULL;
    }

    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
    glEnable(GL_DEPTH_TEST);

    return window;
}

GLenum Common::glCheckError_(const char* file, int line)
{
    GLenum errorCode;
    while ((errorCode = glGetError()) != GL_NO_ERROR)
    {
        std::string sError = "";
        char buffer[10];
        sprintf_s(buffer, "%d", errorCode);
        sError = buffer;
        switch (errorCode)
        {
        case GL_INVALID_ENUM:                  sError = "INVALID_ENUM"; break;
        case GL_INVALID_VALUE:                 sError = "INVALID_VALUE"; break;
        case GL_INVALID_OPERATION:             sError = "INVALID_OPERATION"; break;
        case GL_OUT_OF_MEMORY:                 sError = "OUT_OF_MEMORY"; break;
        case GL_INVALID_FRAMEBUFFER_OPERATION: sError = "INVALID_FRAMEBUFFER_OPERATION"; break;
        }
        std::cout << sError << " | " << file << " (" << line << ")" << std::endl;
    }
    return errorCode;
}

unsigned int Common::GenerateTexture(std::string sTexturePath) {
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

//@desc: �������
void Common::ProcessInput(Camera* cam, GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        if (cam != nullptr) cam->ProcessKeyboard(FORWARD, Common::deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        if (cam != nullptr) cam->ProcessKeyboard(BACKWARD, Common::deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        if (cam != nullptr) cam->ProcessKeyboard(LEFT, Common::deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        if (cam != nullptr) cam->ProcessKeyboard(RIGHT, Common::deltaTime);
}