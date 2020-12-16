#include "Shader.h"
#include <fstream>
#include <sstream>
#include <iostream>

Shader::Shader(const char* vertexPath, const char* fragmentPath){
    // 1. ���ļ�·���л�ȡ����/Ƭ����ɫ��
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    // ��֤ifstream��������׳��쳣��
    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        // ���ļ�
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);
        std::stringstream vShaderStream, fShaderStream;
        // ��ȡ�ļ��Ļ������ݵ���������
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();
        // �ر��ļ�������
        vShaderFile.close();
        fShaderFile.close();
        // ת����������string
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    }
    catch (std::ifstream::failure e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ " << e.what() << std::endl;
    }
    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();

    // 2. ������ɫ��
    unsigned int vertexShader, fragmentShader;
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

//@desc: �ѱ任���󴫸���ɫ��
void Shader::setMat4(const std::string& name, glm::mat4 value) const
{
    //��һ������������uniform��λ��ֵ
    //�ڶ�����������OpenGL���ǽ�Ҫ���Ͷ��ٸ�����������1
    //����������ѯ�����������Ƿ�ϣ�������ǵľ�������û�(Transpose)
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE/*�Ƿ񽻻�������к���*/, glm::value_ptr(value));
}