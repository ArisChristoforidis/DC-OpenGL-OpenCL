#ifndef VERTEXBUFFERLAYOUT_H
#define VERTEXBUFFERLAYOUT_H

#include <glad/glad.h>
#include <vector>

struct VertexBufferElement{
	unsigned int type;
	unsigned int count;
	unsigned int isNormalized;

	static unsigned int GetSizeOfType(){
		return 4;
	}
};

class VertexBufferLayout {
public:
	VertexBufferLayout();
	~VertexBufferLayout();

	template<typename T>
	void Push(unsigned int count, unsigned int isNormalized){
		static_assert(false);
	}

	template<>
	void Push<float>(unsigned int count, unsigned int isNormalized){
		elements.push_back({GL_FLOAT,count,isNormalized});
		stride += count * VertexBufferElement::GetSizeOfType();
	}

	template<>
	void Push<unsigned int>(unsigned int count, unsigned int isNormalized) {
		elements.push_back({ GL_UNSIGNED_INT,count,isNormalized });
		stride += count * VertexBufferElement::GetSizeOfType();

	}


	std::vector<VertexBufferElement> GetElements() const;
	unsigned int GetStride() const;

private:
	std::vector<VertexBufferElement> elements;
	unsigned int stride;
};
#endif


