#ifndef MESH_H
#define MESH_H

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "MeshData.h"

class Mesh {
public:
	Mesh(MeshData info);
	~Mesh();


	VertexArray &GetVertexArray();
	IndexBuffer &GetIndexBuffer();

	unsigned int GetVertexCount();
	unsigned int GetIndexCount();


private:
	VertexBuffer* vertexBuffer;
	VertexBufferLayout vertexBufferLayout;
	VertexArray* vertexArray;
	IndexBuffer* indexBuffer;

	std::vector<float> vertices;
	std::vector<unsigned int> indices;
	unsigned int vertexCount;
	unsigned int indexCount;

	void CreateVertexArray(std::vector<float> data);
	void CreateIndexBuffer(std::vector<unsigned int> data);


};

#endif


