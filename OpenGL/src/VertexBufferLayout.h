#pragma once
#include <vector>
#include "Renderer.h"

struct VertexBufferElement{
	unsigned int type;
	unsigned int count;
	unsigned char normalized;
	VertexBufferElement(unsigned int t = GL_FLOAT, unsigned int c = 0, bool n = GL_FALSE)
		: type(t), count(c), normalized(n)
	{}
	
	static unsigned int GetSizeOfType(unsigned int type) {
		switch (type){
			case GL_FLOAT:			return 4;
			case GL_UNSIGNED_INT:	return 4;
			case GL_UNSIGNED_BYTE:	return 1;
		}
		ASSERT(false);
	}
};

class VertexBufferLayout{
private:
	std::vector<VertexBufferElement> _elements;
	unsigned int _stride;
public:
	VertexBufferLayout()
		: _stride(0) {
	}

	template<typename T>
	void Push(unsigned int count) {
		static_assert(false);	// 必须以下面偏特化的形式调用Push函数
	}

	template<>
	void Push<float>(unsigned int count) {
		_elements.push_back({ GL_FLOAT, count, GL_FALSE });
		_stride += count * VertexBufferElement::GetSizeOfType(GL_FLOAT);
	}

	template<>
	void Push<unsigned int>(unsigned int count) {
		_elements.push_back({ GL_UNSIGNED_INT, count, GL_FALSE });
		_stride += count * VertexBufferElement::GetSizeOfType(GL_UNSIGNED_INT);
	}

	template<>
	void Push<unsigned char>(unsigned int count) {
		_elements.push_back({ GL_UNSIGNED_BYTE, count, GL_TRUE });
		_stride += count * VertexBufferElement::GetSizeOfType(GL_UNSIGNED_BYTE);
	}

	const std::vector<VertexBufferElement> GetElements() const { return _elements; }
	unsigned int GetStride() const { return _stride; }
};