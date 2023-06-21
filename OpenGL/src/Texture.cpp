#include "Texture.h"
#include "Renderer.h"
#include "vendor/stb_image/stb_image.h"

Texture::Texture(const std::string& texturePath)
{
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(texturePath.c_str(), &_width, &_height, &_BPP, 4);
    ASSERT(data != nullptr);

    GLCall(glGenTextures(1, &_renderId));
    GLCall(glBindTexture(GL_TEXTURE_2D, _renderId));

    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

    GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, _width, _height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data));
    GLCall(glGenerateMipmap(GL_TEXTURE_2D));
    
    stbi_image_free(data);
    UnBind();
}

Texture::~Texture()
{
    GLCall(glDeleteTextures(1, &_renderId));
}

void Texture::Bind(unsigned int slot) const
{
    GLCall(glActiveTexture(GL_TEXTURE0 + slot));
    GLCall(glBindTexture(GL_TEXTURE_2D, _renderId));
}

void Texture::UnBind() const
{
    GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}
