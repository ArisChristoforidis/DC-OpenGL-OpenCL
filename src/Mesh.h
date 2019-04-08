#ifndef MESH_H
#define MESH_H

#include "VertexArray.h"
#include "IndexBuffer.h"

class Mesh {
public:
	Mesh();
	~Mesh();

	void CreateVertexArray(std::vector<float> data);
	void CreateIndexBuffer(std::vector<unsigned int> data);

	VertexArray &GetVertexArray();
	IndexBuffer &GetIndexBuffer();

	unsigned int GetVertexCount();
	unsigned int GetIndexCount();
	VertexArray* vertexArray;
	IndexBuffer* indexBuffer;

	std::vector<float> vertices;
	std::vector<unsigned int> indices;

private:
	VertexBuffer* vertexBuffer;
	VertexBufferLayout vertexBufferLayout;

	unsigned int vertexCount;
	unsigned int indexCount;


};

#endif


