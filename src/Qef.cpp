#include "Qef.h"


Qef::Qef() {
}


Qef::~Qef() {
}

glm::fvec3 Qef::Solve(glm::fvec3 pos,std::vector<glm::fvec3> positions, std::vector<glm::fvec3> normals) {
	float x[3];
	

	const unsigned int rows = normals.size() + 3;
	float b[15] = { 0 };
	float A[15][3];

	//Get the average of positions to find the point towards which we "pull".
	glm::fvec3 avgPoint = GetAverage(positions,rows-3);

	normals.push_back(glm::fvec3(BIAS_STRENGTH,0,0));
	positions.push_back(avgPoint);

	normals.push_back(glm::fvec3(0, BIAS_STRENGTH ,0));
	positions.push_back(avgPoint);
	
	normals.push_back(glm::fvec3(0 , 0, BIAS_STRENGTH));
	positions.push_back(avgPoint);

	for (int i = 0; i < 15; i++) {
		A[i][0] = 0;
		A[i][1] = 0;
		A[i][2] = 0;
	}


	for (int i = 0; i < normals.size(); i++) {
		A[i][0] = normals[i].x;
		A[i][1] = normals[i].y;
		A[i][2] = normals[i].z;
	}

	/*
	std::cout << "[";
	for (int i = 0; i < rows; i++) {
		std::cout << positions[i].x << " " << positions[i].y << " " << positions[i].z << std::endl;
	}
	std::cout << "]";
	*/

	//auto start = std::chrono::high_resolution_clock::now();


	//Initialize our A matrix.
	MatrixType ublas_A(rows, 3);
	//std::cout << ublas_A << std::endl;
	//Initialize our b vector.
	VectorType ublas_b(rows);
	
	//Fill A and b with data.
	for(int i=0;i<rows;i++){
		//Assign data to A.(No need to have a nested for-loop here obviously)
		ublas_A(i, 0) = normals[i].x;
		ublas_A(i, 1) = normals[i].y;
		ublas_A(i, 2) = normals[i].z;

		//Assign data to b.
		//std::cout << positions[i].x * normals[i].x + positions[i].y * normals[i].y + positions[i].z * normals[i].z << std::endl;
		b[i] =  positions[i].x * normals[i].x + positions[i].y * normals[i].y + positions[i].z * normals[i].z;
		ublas_b(i) = positions[i].x * normals[i].x + positions[i].y * normals[i].y + positions[i].z * normals[i].z;
	}

	
	for (int i = 0; i < 3; i++) {
		float s = 0;
		for (int j = 0; j < i; j++) {
			s += A[i][j] * x[j];
		}
		x[i] = (b[i] - s) / A[i][i];
	}
		
	
	//Create device matrix and vector for A and b.This will probably be removed down the line because this whole part will be run on the device.
	VCLMatrixType vcl_A(ublas_A.size1(), ublas_A.size2());
	VCLVectorType vcl_b(ublas_b.size());

	//Copy host to device.
	viennacl::copy(ublas_A, vcl_A);
	viennacl::copy(ublas_b, vcl_b);

	std::vector<ScalarType> hybrid_betas = viennacl::linalg::inplace_qr(vcl_A);
	//std::cout << "vcl_A:" << vcl_A << std::endl;
	viennacl::linalg::inplace_qr_apply_trans_Q(vcl_A, hybrid_betas, vcl_b);
	
	viennacl::range vcl_range(0, 3);
	viennacl::matrix_range<VCLMatrixType> vcl_R(vcl_A, vcl_range, vcl_range);
	viennacl::vector_range<VCLVectorType> vcl_b2(vcl_b, vcl_range);
	//std::cout << "VCL_R:" << std::endl;
	//std::cout << vcl_R << std::endl;
	//std::cout << "---------------" << std::endl;
	//std::cout << "VCL_b2:" << std::endl;
	//std::cout << vcl_b2 << std::endl;
	viennacl::linalg::inplace_solve(vcl_R, vcl_b2, viennacl::linalg::upper_tag());

	/*
	auto end = std::chrono::high_resolution_clock::now();
	auto dur = end - start;
	auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(dur).count();
	
	std::cout << "Finished least squares in " << ms << " milliseconds \n";
	*/
	//std::cout << "Result: " << vcl_b2 << std::endl;
	//std::cout << "[" << pos.x << "," << pos.y << "," << pos.z << "] : " << vcl_b2[0] << " " << vcl_b2[1] << " " << vcl_b2[2] << std::endl;
	glm::fvec3 result(vcl_b2[0], vcl_b2[1], vcl_b2[2]);
	
	//printf("(%d,%d,%d) : [%1.2f,%1.2f,%1.2f]\n", (int)pos.x, (int)pos.y, (int)pos.z, result.x, result.y, result.z);
	//glm::fvec3 result(x[0],x[1], x[2]);
	return result;
}

glm::fvec3 Qef::GetAverage(std::vector<glm::fvec3> vec,unsigned int size) {
	glm::fvec3 averagePos(0.0, 0.0, 0.0);
	for (unsigned int i = 0; i < size; i++) {
		averagePos += vec[i];
	}
	averagePos /= (float)size;

	return averagePos;
}

