#ifndef MESH_H
#define MESH_H

#include "VertexArray.h"
#include "IndexBuffer.h"

class Mesh {
public:
	Mesh(VertexArray vArray,IndexBuffer iBuffer) : vertexArray(vArray),indexBuffer(iBuffer){};
	~Mesh();

	VertexArray& GetVertexArray();
	IndexBuffer& GetIndexBuffer();
private:
	VertexArray vertexArray;
	IndexBuffer indexBuffer;
};

#endif


