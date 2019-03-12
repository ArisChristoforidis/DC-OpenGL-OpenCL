#include "Mesh.h"



Mesh::~Mesh() {
}

VertexArray& Mesh::GetVertexArray() {
	return vertexArray;
}

IndexBuffer& Mesh::GetIndexBuffer() {
	return indexBuffer;
}
