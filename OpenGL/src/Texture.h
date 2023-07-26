#pragma once
#include <string>

enum class TextureType{
	texture_diffuse,
	texture_specular,
	texture_normal,
	texture_height
};

class Texture{
private:
	unsigned int _renderId;
	int _width;
	int _height;
	int _BPP;		// 像素深度
	unsigned int _slot;
public:
	TextureType type;
	std::string path;

	Texture(const std::string& texturePath, TextureType t);
	~Texture();

	void Bind(unsigned int slot = 0);
	void UnBind() const;

	int GetWidth() const { return _width; }
	int GetHeight() const { return _height; }
	unsigned int GetSlot() const { return _slot; }
};