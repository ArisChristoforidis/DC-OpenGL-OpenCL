#include "VertexBufferLayout.h"



VertexBufferLayout::VertexBufferLayout() : stride(0){
}


VertexBufferLayout::~VertexBufferLayout() {
}

std::vector<VertexBufferElement> VertexBufferLayout::GetElements() const {
	return elements;
}

unsigned int VertexBufferLayout::GetStride() const {
	return stride;
}
