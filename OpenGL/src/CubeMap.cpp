#include "CubeMap.h"
#include "stb_image/stb_image.h"
#include <iostream>

CubeMap::CubeMap(std::vector<std::string>& faces)
{
    GLCall(glGenTextures(1, &_renderId));
    GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, _renderId));

    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); ++i) {
        stbi_set_flip_vertically_on_load(false);
        unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data == nullptr) {
            std::cout << stbi_failure_reason() << std::endl;
        }
        ASSERT(data != nullptr);
        std::cout << "load " << faces[i] << std::endl;

        GLCall(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB,
            width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data));
        GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
        GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
        GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
        GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
        GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE));

        stbi_image_free(data);
    }
}

CubeMap::~CubeMap()
{
}

void CubeMap::Bind() const
{
    GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, _renderId));
}

void CubeMap::UnBind() const
{
    GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, 0));
}
