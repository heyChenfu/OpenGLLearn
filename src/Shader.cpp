#include "Shader.h"
#include <fstream>
#include <sstream>
#include <iostream>

Shader::Shader(const GLchar* vertexPath, const GLchar* fragmentPath, const GLchar* geometryPath){
    // 1. ���ļ�·���л�ȡ����/Ƭ����ɫ��
    std::string vertexCode;
    std::string fragmentCode;
    std::string geometryCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    std::ifstream gShaderFile;
    // ��֤ifstream��������׳��쳣��
    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    gShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        std::stringstream vShaderStream, fShaderStream, gShaderStream;
        
        vShaderFile.open(vertexPath); // ���ļ�
        vShaderStream << vShaderFile.rdbuf(); // ��ȡ�ļ��Ļ������ݵ���������
        vShaderFile.close(); // �ر��ļ�������
        fShaderFile.open(fragmentPath);
        fShaderStream << fShaderFile.rdbuf();
        fShaderFile.close();
        if (geometryPath != "") {
            gShaderFile.open(geometryPath);
            gShaderStream << gShaderFile.rdbuf();
            gShaderFile.close();
        }
        // ת����������string
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
        geometryCode = gShaderStream.str();
    }
    catch (std::ifstream::failure e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ " << e.what() << std::endl;
    }
    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();
    const char* gShaderCode = geometryCode.c_str();

    // 2. ������ɫ��
    unsigned int vertexShader, fragmentShader, geometryShader;
    int success;
    char infoLog[512];
    //------------------------------------
    //������ɫ��
    //------------------------------------
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    //����ɫ��Դ�븽�ӵ���ɫ��������
    glShaderSource(vertexShader, 1, &vShaderCode, NULL);
    glCompileShader(vertexShader);
    //�Ƿ�ɹ�����
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, sizeof(infoLog) / sizeof(infoLog[0]), NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    //------------------------------------
    //������ɫ��
    //------------------------------------
    if (geometryPath != "") {
        geometryShader = glCreateShader(GL_GEOMETRY_SHADER);
        glShaderSource(geometryShader, 1, &gShaderCode, NULL);
        glCompileShader(geometryShader);
        glGetShaderiv(geometryShader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(geometryShader, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::GEOMETRY::COMPILATION_FAILED\n" << infoLog << std::endl;
        }
    }
    //-------------------------------------
    //ƬԪ��ɫ��
    //-------------------------------------
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fShaderCode, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    //------------------------------------
    //��ɫ������
    //------------------------------------
    ID = glCreateProgram();
    glAttachShader(ID, vertexShader);
    glAttachShader(ID, fragmentShader);
    if (geometryPath != "")
        glAttachShader(ID, geometryShader);
    glLinkProgram(ID);
    //���������ɫ�������Ƿ�ʧ��
    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(ID, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER_PROGRAM::LINK_FAILED\n" << infoLog << std::endl;
    }
    //����ɫ���������ӵ���������Ժ�, ɾ����ɫ������, ������Ҫ������
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    if (geometryPath != "")
        glDeleteShader(geometryShader);

}

Shader::~Shader(){

}

void Shader::use() {
    glUseProgram(ID);
}

void Shader::setBool(const std::string& name, bool value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void Shader::setInt(const std::string& name, int value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setFloat(const std::string& name, float value) const
{
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setVec2(const std::string& name, float value1, float value2) const
{
    glUniform2f(glGetUniformLocation(ID, name.c_str()), value1, value2);
}

void Shader::setVec2(const std::string& name, glm::vec2 value) const
{
    glUniform2f(glGetUniformLocation(ID, name.c_str()), value.x, value.y);
}

void Shader::setVec3(const std::string& name, float value1, float value2, float value3) const
{
    glUniform3f(glGetUniformLocation(ID, name.c_str()), value1, value2, value3);
}

void Shader::setVec3(const std::string& name, glm::vec3 value) const
{
    glUniform3f(glGetUniformLocation(ID, name.c_str()), value.x, value.y, value.z);
}

//@desc: �ѱ任���󴫸���ɫ��
void Shader::setMat4(const std::string& name, glm::mat4 value) const
{
    //��һ������������uniform��λ��ֵ
    //�ڶ�����������OpenGL���ǽ�Ҫ���Ͷ��ٸ�����������1
    //����������ѯ�����������Ƿ�ϣ�������ǵľ�������û�(Transpose)
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE/*�Ƿ񽻻�������к���*/, glm::value_ptr(value));
}