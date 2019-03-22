#ifndef QEF_H
#define QEF_H

//Activate ublas support for ViennaCL
#define VIENNACL_WITH_OPENMP
#define VIENNACL_WITH_UBLAS

//Boost headers.
#include <Boost/numeric/ublas/triangular.hpp>
#include <Boost/numeric/ublas/vector.hpp>
#include <Boost/numeric/ublas/vector_proxy.hpp>
#include <Boost/numeric/ublas/matrix.hpp>
#include <Boost/numeric/ublas/matrix_proxy.hpp>
#include <Boost/numeric/ublas/lu.hpp>
#include <Boost/numeric/ublas/io.hpp>

//ViennaCL headers.
#include <viennacl/vector.hpp>
#include <viennacl/matrix.hpp>
#include <viennacl/matrix_proxy.hpp>
#include <viennacl/linalg/qr.hpp>
#include <viennacl/linalg/lu.hpp>
#include <viennacl/linalg/direct_solve.hpp>

//GL Math headers.
#include <glm/glm.hpp>

#include <vector>
#include <iostream>
#include <chrono>

#define STEP 0.1 //TODO:Remove at some point, this is very temp,already defined in DualContour.h
#define BIAS_STRENGTH 0.01
class Qef {
public:
	Qef();
	~Qef();
	
	static glm::fvec3 Solve(glm::fvec3 pos, std::vector<glm::fvec3> positions, std::vector<glm::fvec3> normals);

private:


	typedef float ScalarType;
	typedef boost::numeric::ublas::matrix<ScalarType> MatrixType;
	typedef boost::numeric::ublas::vector<ScalarType> VectorType;
	typedef viennacl::matrix<ScalarType, viennacl::column_major> VCLMatrixType;
	typedef viennacl::vector<ScalarType> VCLVectorType;

	static glm::fvec3 GetAverage(std::vector<glm::fvec3> vec,unsigned int size);


};



#endif 


