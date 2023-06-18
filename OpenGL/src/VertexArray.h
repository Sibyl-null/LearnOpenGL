#pragma once
#include "VertexBuffer.h";
#include "VertexBufferLayout.h"

class VertexArray {
private:
	unsigned int _renderId;
public:
	VertexArray();
	~VertexArray();

	void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);

	void Bind() const;
	void UnBind() const;
};