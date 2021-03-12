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
    glfwWindowHint(GLFW_SAMPLES, 4);

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

    // Define the viewport dimensions
    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

    // Setup OpenGL options
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE); //���ز���

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
    //OpenGLҪ��y��0.0��������ͼƬ�ĵײ��ģ�����ͼƬ��y��0.0����ͨ���ڶ���
    stbi_set_flip_vertically_on_load(true);
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

unsigned int Common::loadCubemap(std::vector<std::string> faces)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            //������������Ŀ��
            //GL_TEXTURE_CUBE_MAP_POSITIVE_X 	��
            //GL_TEXTURE_CUBE_MAP_NEGATIVE_X 	��
            //GL_TEXTURE_CUBE_MAP_POSITIVE_Y 	��
            //GL_TEXTURE_CUBE_MAP_NEGATIVE_Y 	��
            //GL_TEXTURE_CUBE_MAP_POSITIVE_Z 	��
            //GL_TEXTURE_CUBE_MAP_NEGATIVE_Z 	ǰ
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
                );
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }
    //���������ƺ͹��˷�ʽ
    /*glTexParmeteri()������ȷ����ΰ���������ӳ�������(�����ƺ͹��˷�ʽ)��WRAP��������������������곬��[0,1]��Χʱ�����������ʽ*/
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
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
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        if (cam != nullptr) cam->ProcessKeyboard(LEFT_UP, Common::deltaTime);
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        if (cam != nullptr) cam->ProcessKeyboard(RIGHT_UP, Common::deltaTime);
}

unsigned int Common::TextureFromFile(const char* path, const std::string& directory, bool gamma)
{
    std::string filename = std::string(path);
    filename = directory + '/' + filename;

    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}