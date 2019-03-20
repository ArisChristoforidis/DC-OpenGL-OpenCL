#ifndef VERTEXARRAY_H
#define VERTEXARRAY_H

#include <iostream>

#include<glad/glad.h>

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

class VertexArray {
public:
	VertexArray();
	~VertexArray();

	void AddBuffer(const VertexBuffer &vb, const VertexBufferLayout &layout);
	void Bind() const;
	void Unbind() const;
private:
	unsigned int rendererID;
};
#endif
