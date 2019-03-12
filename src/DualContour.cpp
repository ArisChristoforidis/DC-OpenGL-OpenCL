#include "DualContour.h"


DualContour::DualContour() {
}


DualContour::~DualContour() {
}

Mesh DualContour::ExtractSurface(float(*function)(float, float, float)) {
	//TODO:Octree should be created here.

	const float halfStep = STEP / 2.0;
	const glm::fvec3 cornerOffset[8] = {
		glm::fvec3(0,0,0),
		glm::fvec3(0,0,1),
		glm::fvec3(0,1,0),
		glm::fvec3(0,1,1),
		glm::fvec3(1,0,0),
		glm::fvec3(1,0,1),
		glm::fvec3(1,1,0),
		glm::fvec3(1,1,1)
	};


	const int edgeMap[12][2] = {
		{0,4},{1,5},{2,6},{3,7},	// x-axis 
		{0,2},{1,3},{4,6},{5,7},	// y-axis
		{0,1},{2,3},{4,5},{6,7}		// z-axis
	};

	const int size = MAX - MIN;
	TreeNode verts[size][size][size];

	for (int x = MIN; x < MAX; x++) {
		for (int y = MIN; y < MAX; y++) {
			for (int z = MIN; z < MAX; z++) {

				/*
				The variable cornerSum is an 8 bit unsigned int(takes values on the range [0,255]).We use this
				in the place of a bool array in order to determine whether a corner of a cube has a value or not 
				based on the function threshold.Each bit corresponds to one corner.
				*/
				uint8_t cornerHasSurface = 0;
				float cornerDensity[8];
				for(int i=0;i<8;i++){
					glm::fvec3 cornerPosition = glm::fvec3(x, y, z) + cornerOffset[i];
					cornerDensity[i] = function(cornerPosition.x,cornerPosition.y,cornerPosition.z);
					//std::cout << "[" << cornerPosition.x << "," << cornerPosition.y << "," << cornerPosition.z << "] : " << cornerDensity[i] << "\n";
					//If the density is below the threshold, the corner is outside the shape.
					int hasSurface = cornerDensity[i] < THRESHOLD ? 0 : 1;
					cornerHasSurface |= (hasSurface << i);
				}


				if(cornerHasSurface == 0 || cornerHasSurface == 255){
					//Delete the node because it is either fully outside the function or fully inside it.
					continue;
				}

				std::vector<glm::fvec3> edgeList;
				for (unsigned int i = 0; i < 12; i++) {
					//Get the two vertices(the corners) of the edge.
					int v0 = (cornerHasSurface >> edgeMap[i][0]) & 1;
					int v1 = (cornerHasSurface >> edgeMap[i][1]) & 1;

					//If they are not equal,means that there was a sign change somewhere on this edge.
					if(v0 != v1){

						//We could get the other corner as well but its not necessary.
						glm::fvec3 edge = glm::fvec3(x, y, z) + cornerOffset[edgeMap[i][0]];

						float c0Value = cornerDensity[edgeMap[i][0]];
						float c1Value = cornerDensity[edgeMap[i][1]];

						//TODO:These are not optimal.
						if(i<4){
							edge.x = x + Adapt(c0Value, c1Value);
						}else if(i<8){
							edge.y = y + Adapt(c0Value, c1Value);
						} else {
							edge.z = z + Adapt(c0Value, c1Value);
						}

						//Add it to the list.
						edgeList.push_back(edge);
					}
				}

				//Calculate edge normal and add it to a list.
				std::vector<glm::fvec3> edgeNormals;
				for(unsigned int i=0;i<edgeList.size();i++){
					glm::fvec3 normalValue = CalculateNormal(function, edgeList[i]);
					edgeNormals.push_back(normalValue);
				}
				
				//TODO:Solve QEF.
				glm::fvec3 pos = Qef::Solve(glm::fvec3(x, y, z), edgeList, edgeNormals);


			}
		}
	}
	VertexArray va;
	IndexBuffer ib(0,0);
	Mesh mesh(va, ib);
	return mesh;

}

float DualContour::Adapt(float v0Val, float v1Val) {
	return -v0Val /(v1Val - v0Val);
}

glm::fvec3 DualContour::CalculateNormal(float(*function)(float, float, float),glm::fvec3 pos, float d) {
	glm::fvec3 derivatives;

	derivatives.x = function(pos.x + d, pos.y, pos.z);
	derivatives.y = function(pos.x, pos.y + d, pos.z);
	derivatives.z = function(pos.x, pos.y, pos.z + d);

	return glm::normalize(derivatives);
}








//Old Implementation,doesn't work.

//Mesh DualContour::ExtractSurface(double(*function)(double, double, double)) {
//	std::vector<glm::dvec3> verts;
//	std::vector<int> vert_indices;
//	//std::unordered_map<glm::dvec3, int> vert_indices;
//
//	int vertLength = 0;
//	for(double i=XMIN;i < XMAX;i += STEP){
//		for (double j = YMIN; j < YMAX; j += STEP) {
//			for (double k = ZMIN; k < ZMAX; k += STEP) {
//				glm::dvec3 vert = FindBestVertex(function,i,j,k);
//				if (vert.length == 0) continue;
//				verts.push_back(vert);
//				vert_indices.push_back(vertLength);
//				vertLength += 1;
//			}
//		}
//	}
//
//	std::vector<int> indices;
//	for (double i = XMIN; i < XMAX; i += STEP) {
//		for (double j = YMIN; j < YMAX; j += STEP) {
//			for (double k = ZMIN; k < ZMAX; k += STEP) {
//				if(i > XMIN && j > YMIN){
//					bool solid1 = function(i, j, k) > 0;
//					bool solid2 = function(i, j, k + STEP) > 0;
//					if(solid1 != solid2){
//
//						
//						/*
//						indices.push_back(vert_indices.find(glm::dvec3(i, j, k))->second);
//						indices.push_back(vert_indices.find(glm::dvec3(i, j - STEP, k))->second);
//						indices.push_back(vert_indices.find(glm::dvec3(i - STEP, j - STEP, k))->second);
//
//						indices.push_back(vert_indices.find(glm::dvec3(i, j - STEP, k))->second);
//						indices.push_back(vert_indices.find(glm::dvec3(i - STEP, j - STEP, k))->second);
//						indices.push_back(vert_indices.find(glm::dvec3(i - STEP, j, k))->second);
//						*/
//					}
//				}
//				if (i > XMIN && k > ZMIN) {
//					bool solid1 = function(i, j, k) > 0;
//					bool solid2 = function(i, j + STEP, k) > 0;
//					if(solid1 != solid2){
//						/*
//						indices.push_back(vert_indices.find(glm::dvec3(i, j, k))->second);
//						indices.push_back(vert_indices.find(glm::dvec3(i, j, k - STEP))->second);
//						indices.push_back(vert_indices.find(glm::dvec3(i - STEP, j, k - STEP))->second);
//
//						indices.push_back(vert_indices.find(glm::dvec3(i, j, k - STEP))->second);
//						indices.push_back(vert_indices.find(glm::dvec3(i - STEP, j, k - STEP))->second);
//						indices.push_back(vert_indices.find(glm::dvec3(i - STEP, j, k))->second);
//						*/
//					}
//				}
//				if(j>YMIN && k>ZMIN){
//					bool solid1 = function(i, j, k) > 0;
//					bool solid2 = function(i + 1, j, k) > 0;
//					if(solid1 != solid2){
//						/*
//						indices.push_back(vert_indices.find(glm::dvec3(i, j, k))->second);
//						indices.push_back(vert_indices.find(glm::dvec3(i, j, k - STEP))->second);
//						indices.push_back(vert_indices.find(glm::dvec3(i, j - STEP, k - STEP))->second);
//
//						indices.push_back(vert_indices.find(glm::dvec3(i, j, k - STEP))->second);
//						indices.push_back(vert_indices.find(glm::dvec3(i, j - STEP, k - STEP))->second);
//						indices.push_back(vert_indices.find(glm::dvec3(i, j - STEP, k))->second);
//						*/
//					}
//
//				
//				}
//			}
//		}
//	}
//
//	//WARNING:Conversion from double to float here,be careful.Also all these are kinda temp.
//	float* vertArray = (float*)malloc(3*verts.size() * sizeof(float));
//	for (int i = 0; i < verts.size(); i+=3) {
//		vertArray[i] = verts[i].x;
//		vertArray[i+1] = verts[i].y;
//		vertArray[i+2] = verts[i].z;
//	}
//
//	VertexArray vArray;
//	VertexBuffer vBuffer(vertArray, 3 * verts.size() * sizeof(float));
//	VertexBufferLayout bufferLayout;
//	bufferLayout.Push<float>(3, true);
//	vArray.AddBuffer(vBuffer, bufferLayout);
//
//	unsigned int* indiceArray = (unsigned int*)malloc(indices.size() * sizeof(unsigned int));
//	IndexBuffer iBuffer(indiceArray, indices.size());
//
//
//	for (unsigned int i = 0; i < indices.size(); i++) {
//		std::cout << indices[i] << std::endl;
//	}
//
//	//TODO:Return the mesh.
//	return Mesh(vArray, iBuffer);
//}
//
//glm::dvec3 DualContour::FindBestVertex(double(*function)(double, double, double), double x, double y, double z) {
//	double v[2][2][2];
//
//	//TODO:Remove, this is temp.
//	if(1+1==2){
//		return glm::dvec3(x + 0.05f, y + 0.05f, z + 0.05f);
//	}
//
//	for(int dx=0;dx<1;dx++){
//		for (int dy = 0; dy < 1; dy++) {
//			for (int dz = 0; dz < 1; dz++) {
//				v[dx][dy][dz] = function(x + dx * STEP, y + dy * STEP, z + dz * STEP);
//			}
//		}
//	}
//
//	//Check if there is a sign change on each edge.
//	//TODO:Possible error here,those Adapts might need to be multiplied by STEP.
//	std::vector<glm::dvec3> changes;
//	for (int dx = 0; dx < 1; dx++) {
//		for (int dy = 0; dy < 1; dy++) {
//			if((v[dx][dy][0] > 0) != (v[dx][dy][1]>0)){
//				glm::dvec3 change(x + dx * STEP, y + dy * STEP, z + Adapt(v[dx][dy][0], v[dx][dy][1]));
//				changes.push_back(change);
//			}
//		}
//	}
//
//	for (int dx = 0; dx < 1; dx++) {
//		for (int dz = 0; dz < 1; dz++) {
//			if ((v[dx][0][dz] > 0) != (v[dx][1][dz] > 0)) {
//				glm::dvec3 change(x + dx * STEP, y + Adapt(v[dx][0][dz], v[dx][1][dz]), z + dz*STEP);
//				changes.push_back(change);
//			}
//		}
//	}
//
//	for (int dy = 0; dy < 1; dy++) {
//		for (int dz = 0; dz < 1; dz++) {
//			if ((v[0][dy][dz] > 0) != (v[1][dy][dz] > 0)) {
//				glm::dvec3 change(x + Adapt(v[0][dy][dz], v[1][dy][dz]), y + dy*STEP, z + dz * STEP);
//				changes.push_back(change);
//			}
//		}
//	}
//
//	//TODO:Not the best way to return empty.
//	if (changes.size() <= 1) return glm::dvec3(0,0,0);
//
//	std::vector<glm::dvec3> normals;
//	for (int i = 0; i < changes.size(); i++) {
//		glm::dvec3 change = changes[i];
//		glm::dvec3 normal = CalculateNormal(function,change.x, change.y, change.z);
//		normals.push_back(normal);
//	}
//
//
//}
//
//
//
//double DualContour::Adapt(double v0, double v1) {
//	//Assert that they have indeed a different sign.
//	assert((v1 > 0) != (v0 > 0));
//	double result = (0 - v0) / (v1 - v0);
//	return result;
//}
//
//glm::dvec3 DualContour::CalculateNormal(double(*function)(double, double, double),double x,double y,double z,double d) {
//
//	double nx = (function(x + d, y, z) - function(x - d, y, z)) / 2 / d;
//	double ny = (function(x, y + d, z) - function(x, y - d, z)) / 2 / d;
//	double nz = (function(x, y, z + d) - function(x, y , z - d)) / 2 / d;
//	glm::dvec3 result(nx, ny, nz);
//	return glm::normalize(result);
//}


