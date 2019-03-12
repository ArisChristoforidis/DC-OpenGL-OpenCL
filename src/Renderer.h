#ifndef RENDERER_H
#define RENDERER_H

#include <glad/glad.h>
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"

class Renderer {
public:
	Renderer();
	~Renderer();

	void ClearScreen() const;
	void Draw(VertexArray &vertexArray,IndexBuffer &indexBuffer,Shader &shader) const;
};
#endif
