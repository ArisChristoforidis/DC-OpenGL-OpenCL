#include "Mesh.h"



Mesh::Mesh(MeshData data) {
	this->vertexBufferLayout.Push<float>(3, true);
	
	CreateVertexArray(data.vertices);
	CreateIndexBuffer(data.indices);
}

Mesh::~Mesh() {
}



void Mesh::CreateVertexArray(std::vector<float> data){
	this->vertices = data;
	this->vertexBuffer = new VertexBuffer(&this->vertices[0], this->vertices.size() * sizeof(float));

	this->vertexArray = new VertexArray();
	this->vertexArray->AddBuffer(*vertexBuffer, this->vertexBufferLayout);
	this->vertexCount = this->vertices.size() / 3;
}

void Mesh::CreateIndexBuffer(std::vector<unsigned int> data) {
	this->indices = data;
	this->indexBuffer = new IndexBuffer(&this->indices[0], this->indices.size());
	this->indexCount = this->indices.size() / 3;
}

VertexArray &Mesh::GetVertexArray() {
	return *vertexArray;
}

IndexBuffer &Mesh::GetIndexBuffer() {
	return *indexBuffer;
}

unsigned int Mesh::GetVertexCount(){
	return this->vertexCount;
}

unsigned int Mesh::GetIndexCount(){
	return this->indexCount;
}
