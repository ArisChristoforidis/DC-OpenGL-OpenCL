#include "VertexArray.h"



VertexArray::VertexArray() {
	glGenVertexArrays(1, &rendererID);
	glBindVertexArray(rendererID);
}


VertexArray::~VertexArray() {
	glDeleteVertexArrays(1, &rendererID);

}

void VertexArray::AddBuffer(const VertexBuffer &vb, const VertexBufferLayout &layout) {
	Bind();
	vb.Bind();
	std::vector<VertexBufferElement> elements = layout.GetElements();
	unsigned int size = elements.size();
	unsigned int offset = 0;
	for (int i = 0; i < size;i++) {
		VertexBufferElement element = elements[i];
		//vb.Bind();
		glEnableVertexAttribArray(i);
		glVertexAttribPointer(i, element.count, element.type, element.isNormalized, layout.GetStride(), (void*)offset);
		offset += element.count;
	}
}

void VertexArray::Bind() const {
	glBindVertexArray(rendererID);
}

void VertexArray::Unbind() const {
	glBindVertexArray(0);

}
