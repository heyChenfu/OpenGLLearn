#include "Common.h"
#include "stb_imageImplement.h"

float Common::deltaTime = 0.0f; //时间差变量, 储存渲染上一帧所用的时间, 速度乘以deltaTime值会相应平衡而不受帧率影响
float Common::lastFrame = 0.0f; // 上一帧的时间
float Common::lastX = SCR_WIDTH / 2;
float Common::lastY = SCR_HEIGHT / 2;

//@desc: glfw 窗口创建
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
    glGenTextures(1/*生成的纹理数目*/, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    //加载图片
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true); //OpenGL要求y轴0.0坐标是在图片的底部的，但是图片的y轴0.0坐标通常在顶部
    unsigned char* data = stbi_load(sTexturePath.c_str(), &width, &height, &nrChannels, 0);
    if (data == nullptr)
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    else {
        //载入的图片数据
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D); //为当前绑定的纹理自动生成所有需要的多级渐远纹理
        //释放图像的内存是一个很好的习惯
        stbi_image_free(data);
    }
    return texture;
}

//@desc: 输入监听
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