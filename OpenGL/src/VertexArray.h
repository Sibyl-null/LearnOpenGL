#pragma once
#include "VertexBuffer.h";

class VertexBufferLayout;	// 避免include循环包含

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