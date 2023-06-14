#pragma once

class IndexBuffer
{
private:
	unsigned int _renderId;
	unsigned int _count;
public:
	IndexBuffer(const unsigned int* data, unsigned int count);
	~IndexBuffer();

	void Bind() const;
	void UnBind() const;
	
	unsigned int GetCount() const { return _count; }
};