#include "matrix_multiplication.h"
#include <iostream>
#include <vector>
#include <gtest/gtest.h>

/*

This file will be used for testing the correct implementation of a matrix multiplication using a black box method

We are assuming that the signature of the function is the following: (using a JML style language)

/*@ 

requires 

// Function arguments are consistent with the vector dimensions
A.size() == rowsA && A[0].size() == colsA && B[0].size() == colsB && 
// Dimensions are non negative
A.size() > 0 && A[0].size() > 0 && B.size() > 0 && B[0].size() > 0 &&
// Result matrix dimensions are consistent with the input arguments
C.size() == rowsA && C[0].size() == colsB

ensures

// NB: whatever is labelled as const i will ignore it, since the check is performed automatically

// The input dimensions remain constant
\old(rowsA) == rowsA && \old(colsA) == colsA && \old(colsB) == colsB &&
// The result matrix dimensions remain constant
\old(C.size()) == C.size() && \old(C[0].size()) == C[0].size()

// I will avoid writing the matrix multiplication logic (row*col) because honestly it is a PITA

void multiplyMatricesWithoutErrors(const std::vector<std::vector<int>> &A,
                      const std::vector<std::vector<int>> &B,
                      std::vector<std::vector<int>> &C, 
                      int rowsA, int colsA, int colsB) 
{ ... }

*/

using Matrix = std::vector<std::vector<int>>;

TEST(CorrectMatrixMutltiplicationTest, DegenerateMatMult){

    // Degenerate cases
    
    // multiplication by 1x1 matrices

    Matrix A = {

        {2}

    };

    Matrix B = {

        {1}

    };

    Matrix C(A.size(),std::vector<int>(B[0].size(),0));

    Matrix E = {

        {2}

    };

    multiplyMatrices(A,B,C,A.size(),A[0].size(),B[0].size());

	// Assert the expected dimensions
	ASSERT_EQ(C.size(),E.size());
	ASSERT_EQ(C[0].size(),E[0].size());
    // Assert the equality of the two matrices
	ASSERT_EQ(C,E);

    // Check commutativity <- careful: works only in one dimension
    A = {

        {1}

    };

    B = {

        {2}

    };

    multiplyMatrices(A,B,C,A.size(),A[0].size(),B[0].size());

	ASSERT_EQ(C.size(),E.size());
	ASSERT_EQ(C[0].size(),E[0].size());
	ASSERT_EQ(C,E);    

}

TEST(CorrectMatrixMutltiplicationTest, IdentityMatMult){

    // Multiplication by the identity for some matrix dimensions
    
    Matrix A;
    Matrix B;
    Matrix C;
    Matrix E;

    constexpr size_t MAX_DIM = 10;

    for(size_t i=1;i<=MAX_DIM;++i){

        // Clear the contents
        A.clear();
        B.clear();
        C.clear();
        E.clear();

        // Resize the matrix A -> we obtain a full matrix ixi with all elements equal to i
        A.resize(i);
        for(size_t j=0;j<i;++j) A[j].resize(i,i);

        // Resize B to be the identity
        B.resize(i);
        for(size_t j=0;j<i;++j) {B[j].resize(i,0); B[j][j]=1;}

        // Resize C to be a zero matrix
        C.resize(i);
        for(size_t j=0;j<i;++j) C[j].resize(i,0);

        // Matrix E shold be equal to matrix A
        E = A;

        multiplyMatrices(A,B,C,A.size(),A[0].size(),B[0].size());

        ASSERT_EQ(C.size(),E.size());
        ASSERT_EQ(C[0].size(),E[0].size());
        ASSERT_EQ(C,E);

    }

}

TEST(CorrectMatrixMutltiplicationTest, MatVectorMult){

    // Test what happens with a matrix-vector multiplication

    Matrix A = {

        {2,1,3,4,5,2}

    };

    Matrix B = {

        {1},
        {2},
        {4},
        {1},
        {3},
        {5}

    };

    Matrix C(A.size(),std::vector<int>(B[0].size(),0));

    Matrix E = {

        {45}

    };

    multiplyMatrices(A,B,C,A.size(),A[0].size(),B[0].size());

	// Assert the expected dimensions
	ASSERT_EQ(C.size(),E.size());
	ASSERT_EQ(C[0].size(),E[0].size());
    // Assert the equality of the two matrices
	ASSERT_EQ(C,E);

    // Check the non commutativity
    A = {

        {2},
        {1},
        {3},
        {4},
        {5},
        {2}

    };

    B = {

        {1,2,4,1,3,5}

    };

    // Resize C to be a zero matrix
    C.resize(A.size());
    for(size_t i=0;i<A.size();++i)C[i].resize(B[0].size(),0);

    // Now E is a different matrix
    E = {

        {2,4,8,2,6,10},
        {1,2,4,1,3,5},
        {3,6,12,3,9,15},
        {4,8,16,4,12,20},
        {5,10,20,5,15,25},
        {2,4,8,2,6,10}

    };

    multiplyMatrices(A,B,C,A.size(),A[0].size(),B[0].size());

	ASSERT_EQ(C.size(),E.size());
	ASSERT_EQ(C[0].size(),E[0].size());
	ASSERT_EQ(C,E);    

}

TEST(CorrectMatrixMutltiplicationTest, AssociativeMatMult){

    // Test associativity of multiplication, indeed A*B*C=(A*B)*C=A*(B*C)
    
    // multiplication by 1x1 matrices

    Matrix A = {

        {1,2},
        {3,4}

    };

    Matrix B = {

        {2},
        {3}

    };

    Matrix C = {

        {1,4}

    };

    Matrix E = {

        {8,32},
        {18,72}

    };

    // Store final and temporary result
    Matrix D(A.size(),std::vector<int>(C[0].size(),0));
    Matrix T(A.size(),std::vector<int>(B[0].size(),0));

    // (A*B)*C

    multiplyMatrices(A,B,T,A.size(),A[0].size(),B[0].size());
    multiplyMatrices(T,C,D,T.size(),T[0].size(),C[0].size());

	// Assert the expected dimensions
	ASSERT_EQ(D.size(),E.size());
	ASSERT_EQ(D[0].size(),E[0].size());
    // Assert the equality of the two matrices
	ASSERT_EQ(D,E);    

    // Resize the temporary matrix now
    T.clear();
    T.resize(B.size());
    for(size_t i=0;i<T.size();++i) T[i].resize(C[0].size(),0);
    // And also the result
    D.clear();
    D.resize(A.size());
    for(size_t i=0;i<D.size();++i) D[i].resize(C[0].size(),0);

    // A*(B*C)

    multiplyMatrices(B,C,T,B.size(),B[0].size(),C[0].size());
    multiplyMatrices(A,T,D,A.size(),A[0].size(),T[0].size());

	// Assert the expected dimensions
	ASSERT_EQ(D.size(),E.size());
	ASSERT_EQ(D[0].size(),E[0].size());
    // Assert the equality of the two matrices
	ASSERT_EQ(D,E);  

}

int main(int argc, char **argv) {
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
