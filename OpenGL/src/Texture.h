#pragma once
#include <string>

class Texture{
private:
	unsigned int _renderId;
	int _width;
	int _height;
	int _BPP;		// 像素深度
public:
	Texture(const std::string& texturePath);
	~Texture();

	void Bind(unsigned int slot = 0) const;
	void UnBind() const;

	int GetWidth() const { return _width; }
	int GetHeight() const { return _height; }
};