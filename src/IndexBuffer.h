#ifndef INDEXBUFFER_H
#define INDEXBUFFER_H

#include <glad/glad.h>


class IndexBuffer {
public:
	IndexBuffer(const unsigned int* data,unsigned int size);
	~IndexBuffer();

	void Bind() const;
	void Unbind() const;
	unsigned int GetCount();
private:
	unsigned int rendererID;
	unsigned int count;
};
#endif
