#pragma once
#include "Renderer.h"

class CubeMap {
private:
	unsigned int _renderId;
public:
	CubeMap(std::vector<std::string>& faces);
	~CubeMap();

	void Bind() const;
	void UnBind() const;
};