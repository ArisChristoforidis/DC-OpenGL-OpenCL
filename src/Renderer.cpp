#include "Renderer.h"



Renderer::Renderer() {

}


Renderer::~Renderer() {
}

void Renderer::ClearScreen() const {
	//glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClearColor(0.2f, 0.3f, 0.3f, 0.5f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::Draw(VertexArray &vertexArray, IndexBuffer &indexBuffer, Shader &shader) const {
	shader.use();
	vertexArray.Bind();
	indexBuffer.Bind();
	glDrawElements(GL_TRIANGLES, indexBuffer.GetCount(), GL_UNSIGNED_INT, nullptr);
}

void Renderer::Draw(Mesh &mesh,Shader &shader) const {
	shader.use();
	mesh.GetVertexArray().Bind();
	IndexBuffer &indexBuffer = mesh.GetIndexBuffer();
	indexBuffer.Bind();
	glDrawElements(GL_TRIANGLES, indexBuffer.GetCount(), GL_UNSIGNED_INT, nullptr);
}