#ifndef TREENODE_H
#define TREENODE_H

#include <glm/glm.hpp>
#include <cstdint>

class TreeNode {
public:
	TreeNode();
	~TreeNode();

	//WARNING:If you have more than 4,294,967,295 verts in your mesh,this will be a problem.
	unsigned int index;
	uint8_t edgeHasSignChange = 0;
};

#endif



