/*
* Copyright (c) 2012 Joey Yore
*
* Permission is hereby granted, free of charge, to any person obtaining a 
* copy of this software and associated documentation files (the "Software"),
* to deal in the Software without restriction, including without limitation 
* the rights to use, copy, modify, merge, publish, distribute, sublicense, 
* and/or sell copies of the Software, and to permit persons to whom the 
* Software is furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included 
* in all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL 
* THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR 
* OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, 
* ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR 
* OTHER DEALINGS IN THE SOFTWARE.
*/
#ifndef MATRIX_H
#define MATRIX_H

#include <iostream>
#include <cmath>
#include <string.h>
#include "vector.h"

class Matrix {

	private:
		unsigned int nrow;
		unsigned int ncol;
		double **elements;

		friend Matrix operator*(const double &lhs,const Matrix &rhs);
		friend Matrix operator/(const double &lhs,const Matrix &rhs);
		friend std::ostream& operator<<(std::ostream&, const Matrix&);	
	public:

		Matrix();
		Matrix(unsigned int r, unsigned int c);
		template <typename T> Matrix(const Vector<Vector<T> > &v);
		~Matrix();
		Matrix(const Matrix &cpy);

		unsigned int getNumRows() {return nrow;}
		unsigned int getNumCols() {return ncol;}
		unsigned int getSize() {return ncol*nrow;}
		void set(double *vals);
		void setRow(int r, double *vals);
		void setCol(int c, double *vals);
		void setElement(int r, int c, double val);
		Matrix transpose();
		double norm();
		double determinant();
		Matrix cofactor();
		Matrix adjoint();
		Matrix inverse();
		void swap_rows(int r1,int r2);

		//Add Another Matrix
		Matrix operator+(const Matrix &rhs) {
			if(nrow != rhs.nrow || ncol != rhs.ncol) {
				return *this;
			}

			Matrix newM(nrow,ncol);

			for(int i=0;i<nrow;++i) {
				for(int j=0;j<ncol;++j) {
					newM.elements[i][j] = elements[i][j] + rhs.elements[i][j];
				}
			}
			return newM;
		}

		//Inline Add Matrix
		Matrix& operator+=(const Matrix &rhs) {
			if(nrow != rhs.nrow || ncol != rhs.ncol) {
				return *this;
			}

			for(int i=0;i<nrow;++i) {
				for(int j=0;j<ncol;++j) {
					elements[i][j] += rhs.elements[i][j];
				}
			}

			return *this;
		}

		//Subtract Matrix
		Matrix operator-(const Matrix &rhs) {
			if(nrow != rhs.nrow || ncol != rhs.ncol) {
				return *this;
			}

			Matrix newM(nrow,ncol);

			for(int i=0;i<nrow;++i) {
				for(int j=0;j<ncol;++j) {
					newM.elements[i][j] = elements[i][j] - rhs.elements[i][j];
				}
			}
			return newM;
		}

		//Inline Subtract Matrix
		Matrix& operator-=(const Matrix &rhs) {
			if(nrow != rhs.nrow || ncol != rhs.ncol) {
				return *this;
			}

			for(int i=0;i<nrow;++i) {
				for(int j=0;j<ncol;++j) {
					elements[i][j] -= rhs.elements[i][j];
				}
			}

			return *this;
		}


		//Scalar Multiplication
		Matrix operator*(const double &rhs) {
			Matrix m(*this);
			for(int i=0;i<nrow;++i) {
				for(int j=0;j<ncol;++j) {
					m.elements[i][j] *= rhs;
				}
			}
			return m;
		}	

		//Inline Scalar Multiplication
		Matrix& operator*=(const double &rhs) {
			for(int i=0;i<nrow;++i) {
				for(int j=0;j<ncol;++j) {
					elements[i][j] *= rhs;
				}
			}
			return *this;
		}

		//Matrix Multiplication
		Matrix operator*(const Matrix &rhs) {
			if(ncol != rhs.nrow) {
				return *this;
			}

			Matrix nm(nrow,rhs.ncol);

			for(int i=0;i<nm.nrow;++i) {
				for(int j=0;j<nm.ncol;++j) {
					nm.elements[i][j] = 0;
					for(int k=0;k<ncol;++k) {
						nm.elements[i][j] += elements[i][k] * rhs.elements[k][j];
					}
				}
			}

			return nm;
		}

		//Inline Matrix Multiplication
		Matrix& operator*=(const Matrix &rhs) {
			//Should support mxn * nxp = mxp
			if(ncol != rhs.nrow) {
				return *this;
			}

			Matrix cpy(*this);
			for(unsigned int i=0;i<nrow;++i) {
				delete[] elements[i];
			}
			delete[] elements;

			nrow = nrow;
			ncol = rhs.ncol;

			elements = new double*[nrow];
			for(unsigned int i=0;i<nrow;++i) {
				elements[i] = new double[ncol];
			}

			for(int i=0;i<nrow;++i) {
				for(int j=0;j<ncol;++j) {
					elements[i][j] = 0;
					for(int k=0;k<cpy.ncol;++k) {
						elements[i][j] += cpy.elements[i][k] * rhs.elements[k][j];
					}
				}
			}

			return *this;
		}

		//Component by component multiplication
		Matrix operator|(const Matrix &rhs) {
			Matrix m(*this);
	
			if(m.nrow == rhs.nrow && m.ncol == rhs.ncol) {
				for(int i=0;i<nrow;++i) {
					for(int j=0;j<ncol;++j) {
						m.elements[i][j] *= rhs.elements[i][j];
					}
				}
			}
			return m;
		}

		//Inline component by component multiplication
		Matrix& operator|=(const Matrix &rhs) {
	
			if(nrow == rhs.nrow && ncol == rhs.ncol) {
				for(int i=0;i<nrow;++i) {
					for(int j=0;j<ncol;++j) {
						elements[i][j] *= rhs.elements[i][j];
					}
				}
			}
			return *this;
		}

		//Component by scalar division
		Matrix operator/(const double &rhs) {
			Matrix m(*this);
	
			for(int i=0;i<nrow;++i) {
				for(int j=0;j<ncol;++j) {
					m.elements[i][j] /= rhs;
				}
			}
			
			return m;
		}

		//Inline component by scalar division
		Matrix& operator/=(const double &rhs) {
	
			for(int i=0;i<nrow;++i) {
				for(int j=0;j<ncol;++j) {
					elements[i][j] /= rhs;
				}
			}
			
			return *this;
		}


		//Component by component division
		Matrix operator/(const Matrix &rhs) {
			Matrix m(*this);
	
			if(m.nrow == rhs.nrow && m.ncol == rhs.ncol) {
				for(int i=0;i<nrow;++i) {
					for(int j=0;j<ncol;++j) {
						m.elements[i][j] /= rhs.elements[i][j];
					}
				}
			}
			return m;
		}

		//Inline component by component division
		Matrix& operator/=(const Matrix &rhs) {
	
			if(nrow == rhs.nrow && ncol == rhs.ncol) {
				for(int i=0;i<nrow;++i) {
					for(int j=0;j<ncol;++j) {
						elements[i][j] /= rhs.elements[i][j];
					}
				}
			}
			return *this;
		}

		//Equality Test
		bool operator==(const Matrix &rhs) {
			if(nrow != rhs.nrow || ncol != rhs.ncol) {
				return false;
			} 

			for(int i=0;i<nrow;++i) {
				for(int j=0;j<ncol;++j) {
					if(elements[i][j] != rhs.elements[i][j]) {
						return false;
					}
				}
			}

			return true;
		}

		//Inequality Test
		bool operator!=(const Matrix &rhs) {
			if(nrow != rhs.nrow || ncol != rhs.ncol) {
				return true;
			} 

			for(int i=0;i<nrow;++i) {
				for(int j=0;j<ncol;++j) {
					if(elements[i][j] != rhs.elements[i][j]) {
						return true;
					}
				}
			}

			return false;
		}

		//Assignment
		Matrix& operator=(const Matrix &rhs) {

			if(ncol) {
				for(unsigned int i=0;i<nrow;++i) {
					delete[] elements[i];
				}
			}

			if(nrow) {
				delete[] elements;
			}

			nrow = rhs.nrow;
			ncol = rhs.ncol;

			//Allocate and assign new
			elements = new double*[nrow];
			for(unsigned int i=0;i<nrow;++i) {
				elements[i] = new double[ncol];
				for(unsigned int j=0;j<ncol;++j) {
					elements[i][j] = rhs.elements[i][j];
				}
			}

			return *this;
		}

		//Get a Row
		double* operator[](const int &ind) {
			return elements[ind];
		}

		//Get Element at index
		double& operator()(const int &ind) {
			int r = ind/ncol;
			int c = ind % nrow;
			return elements[r][c];
		}

		//Get Element at row r, column c
		double& operator()(const int &r, const int &c) {
			return elements[r][c];
		}

		//Transpose Matrix
		Matrix operator~() const {
			Matrix o(*this);
			return o.transpose();
		}	
	
		//Inverse Matrix
		Matrix operator!() const {
			Matrix o(*this);
			return o.inverse();
		}

		//Return a matrix of booleanesque values
		//
		// |1 2 3|         |0 0 0|
		// |4 0 2| > 4  =  |0 0 0|
		// |6 1 9|         |1 0 1|
		//
		Matrix operator>(const double &rhs) {
			Matrix m(nrow,ncol);

			for(int i=0;i<nrow;++i) {
				for(int j=0;j<ncol;++j) {
					m(i,j) = (double)(elements[i][j] > rhs);
				}
			}

			return m;
		}

		//Return a matrix of booleanesque values
		//
		// |1 2 3|          |0 0 0|
		// |4 0 2| >= 4  =  |1 0 0|
		// |6 1 9|          |1 0 1|
		//
		Matrix operator>=(const double &rhs) {
			Matrix m(nrow,ncol);

			for(int i=0;i<nrow;++i) {
				for(int j=0;j<ncol;++j) {
					m(i,j) = (double)(elements[i][j] >= rhs);
				}
			}

			return m;
		}

		//Return a matrix of booleanesque values
		//
		// |1 2 3|         |1 1 1|
		// |4 0 2| < 4  =  |0 1 1|
		// |6 1 9|         |0 1 0|
		//
		Matrix operator<(const double &rhs) {
			Matrix m(nrow,ncol);

			for(int i=0;i<nrow;++i) {
				for(int j=0;j<ncol;++j) {
					m(i,j) = (double)(elements[i][j] < rhs);
				}
			}

			return m;
		}

		//Return a matrix of booleanesque values
		//
		// |1 2 3|          |1 1 1|
		// |4 0 2| <= 4  =  |1 1 1|
		// |6 1 9|          |0 1 0|
		//
		Matrix operator<=(const double &rhs) {
			Matrix m(nrow,ncol);

			for(int i=0;i<nrow;++i) {
				for(int j=0;j<ncol;++j) {
					m(i,j) = (double)(elements[i][j] <= rhs);
				}
			}

			return m;
		}
};

namespace matrix {
	double epsilon = 1e-14;

	//Return a matrix of abs values
	//
	// |-1 0 1|    |1 0 1|
	// |2 -1 2| => |2 1 2|
	// |-2 1 0|    |2 1 0|
	//
	Matrix abs(Matrix A) {
		int r = A.getNumRows();
		int c = A.getNumCols();
		Matrix B(r,c);

		for(int i=0;i<r;++i) {
			for(int j=0;j<c;++j) {
				B(i,j) = (A(i,j) < 0) ? -(A(i,j)) : A(i,j);
			}
		}
		return B;
	}

	//Return true if any element of the matrix is non-zero
	bool any(Matrix A) {
		int r = A.getNumRows();
		int c = A.getNumCols();

		for(int i=0;i<r;++i) {
			for(int j=0;j<c;++j) {
				if(A(i,j) < -epsilon || A(i,j) > epsilon) {
					return true;
				}
			}
		}

		return false;
	}

	//Return the maximum element value
	double max(Matrix A) {
		int r = A.getNumRows();
		int c = A.getNumCols();
		double m = A(0,0);

		for(int i=0;i<r;++i) {
			for(int j=0;j<c;++j) {
				if(A(i,j) > m) {
					m = A(i,j);
				}
			}
		}

		return m;
	}

	//Return the minimum element value
	double min(Matrix A) {
		int r = A.getNumRows();
		int c = A.getNumCols();
		double m = A(0,0);

		for(int i=0;i<r;++i) {
			for(int j=0;j<c;++j) {
				if(A(i,j) < m) {
					m = A(i,j);
				}
			}
		}

		return m;
	}

	//Pivot the matrix
	int pivot(Matrix &A, int *order, int jcol) {

		int i, ipvt,n;
		double big,anext;
		n = A.getNumRows();
		ipvt = jcol;
		big = std::abs(A(ipvt,ipvt));
		for(i = ipvt+1;i<n;++i) {
			anext = std::abs(A(i,jcol));
			if(anext > big) {
				big = anext;
				ipvt = i;
			}
		}

		if(std::abs(big) < epsilon) {
			return 0;
		}

		if(ipvt==jcol) return 0;
		A.swap_rows(jcol,ipvt);
		i = order[jcol];
		order[jcol] = order[ipvt];
		order[ipvt] = i;
		return 1;
	}

	//LU Decomposition
	// A = LU
	//
	// | a11 a12 a13 |   | l11  0   0  | | u11 u12 u13 |
	// | a21 a22 a21 | = | l21 l22  0  | |  0  u22 u23 |
	// | a31 a32 a33 |   | l31 l32 l33 | |  0   0  u33 |
	//
	int lu(Matrix A,Matrix &LU) {
		int i,j,k,n,nm1;
		int flag = 1;
		double sum, diag;

		LU = Matrix(A);

		n = LU.getNumRows();
		if(n != LU.getNumCols()) {
			return 0;
		}

		int order[n];
		for(i=0;i<n;++i) {
			order[i] = i;
		}

		if(pivot(LU,order,0)) {
			flag = -flag;
		}

		diag = 1.0/LU(0,0);

		for(int i=1;i<n;++i) {
			LU(0,i) *= diag;
		}

		nm1 = n - 1;
		for(j=1;j<nm1;++j) {
			for(i=j;i<n;++i) {
				sum = 0.0;
				for(k=0;k<j;++k) {
					sum += LU(i,k)*LU(k,j);
				}
				LU(i,j) -= sum;
			}

			if(pivot(LU,order,j)) {
				flag = -flag;
			}

			diag = 1.0/LU(j,j);
			for(k=j+1;k<n;++k) {
				sum = 0.0;
				for(i=0;i<j;++i) {
					sum += LU(j,i)*LU(i,k);
				}
				LU(j,k) = (LU(j,k)-sum)*diag;
			}
		}

		sum = 0.0;
		for(k=0;k<nm1;++k) {
			sum += LU(nm1,k)*LU(k,nm1);
		}
		LU(nm1,nm1) -= sum;

		return flag;		
	}

	//Return the Upper Triangle Matrix
	// | u11 u12 u13 |
	// |  0  u22 u23 |
	// |  0   0  u33 |
	//
	Matrix upper_triangle(const Matrix &A) {
		Matrix u(A);

		int n = u.getNumRows();
		if(n == u.getNumCols()) {

			for(int i=0;i<n;++i) {
				for(int j=0;j<i;++j) {
					u(i,j) = 0.0;
				}
				u(i,i) = 0.0;
			}
		}
		return u;
	}

	//Return the Lower Triangle Matrix
	// | l11  0   0  |
	// | l21 l22  0  |
	// | l31 l32 l33 |
	//
	Matrix lower_triangle(const Matrix &A) {
		Matrix l(A);

		int n = l.getNumRows();
		if(n == l.getNumCols()) {

			for(int i=0;i<n;++i) {
				for(int j=i+1;j<n;++j) {
					l(i,j) = 0.0;
				}
			}
		}
		return l;
	}

	//Return the Eigen values
	Matrix eig(Matrix A) {
		int n = A.getNumRows();
		if(n != A.getNumCols()) return A;

		Matrix eigens(n,1);
	}

	//Return a matrix with the product of all rows in that col
	Matrix prod(Matrix A) {
		int r = A.getNumRows();
		int c = A.getNumCols();
		Matrix B(1,c);

		B.setRow(0,A[0]);

		for(int j=0;j<c;++j) {
			for(int i=1;i<r;++i) {
				B(0,j) *= A(i,j);
			}
		}

		return B;
	}

	Matrix eye(int n) {
		Matrix o(n,n);
		for(int i=0;i<n;++i) {
			o(i,i) = 1.0;
		}
		return o;
	}

	template <typename T>
	Vector<Vector<T> > vectorize(Matrix m) {
		Vector<Vector<T> > o;
		for(int ii=0;ii<m.getNumRows();++ii) {
				Vector<T> v;
			for(int jj=0;jj<m.getNumCols();++jj) {
				v.Push_Back(T(m(ii,jj)));
			}
			o.Push_Back(v);
		}
		return o;
	}
}

//Scalar Multiplication
Matrix operator*(const double &lhs,const Matrix &rhs) {
	Matrix m(rhs);
	return m*lhs;
}

//Scalar Division
Matrix operator/(const double &lhs,const Matrix &rhs) {
	Matrix m(rhs);
	return m*(1/lhs);
}

//Default Constructor
//Creates a 0x0 matrix
Matrix::Matrix() : nrow(0), ncol(0) {
}

//Row,Column Contructor
//Creates a rxc matrix
Matrix::Matrix(unsigned int r, unsigned int c) : nrow(r), ncol(c) {
	elements = new double*[nrow];
	for(unsigned int i=0;i<nrow;++i) {
		elements[i] = new double[ncol];
		for(unsigned int j=0;j<ncol;++j) {
			elements[i][j] = 0.0;
		}
	}
}

template <typename T> 
Matrix::Matrix(const Vector<Vector<T> > &v) {
	nrow = v.Size();
	ncol = v[0].Size();
	elements = new double*[nrow];
	for(int i=0;i<nrow;++i) {
		elements[i] = new double[ncol];
		for(int j=0;j<ncol;++j) {
			elements[i][j] = static_cast<double>(v[i][j]);
		}
	}

}


//Destructor
Matrix::~Matrix() {
	for(unsigned int i=0;i<nrow;++i) {
		delete[] elements[i];
	}

	delete[] elements;
}

//Copy Constructor
Matrix::Matrix(const Matrix &cpy) {
	nrow = cpy.nrow;
	ncol = cpy.ncol;

	//Allocate and assign new
	elements = new double*[nrow];
	for(unsigned int i=0;i<nrow;++i) {
		elements[i] = new double[ncol];
		for(unsigned int j=0;j<ncol;++j) {
			elements[i][j] = cpy.elements[i][j];
		}
	}
}

//Set the matrix elements to the elements in the array
//
// vals = [0 1 2 3 4 5 6 7 8]
//
// For a 3x3 Matrix
//
// |0 1 2|
// |3 4 5|
// |6 7 8|
//
void Matrix::set(double *vals) {	
	for(int i=0;i<nrow;++i) {
		for(int j=0;j<ncol;++j) {
			elements[i][j] = vals[i*ncol+j];
		}
	}
}

//Set Row Values to values from an array
void Matrix::setRow(int r, double *vals) {
	for(int i=0;i<ncol;++i) {
		elements[r][i] = vals[i];
	}
}

//Set Col Values to values from an array
void Matrix::setCol(int c, double *vals) {
	for(int i=0;i<nrow;++i) {
		elements[i][c] = vals[i];
	}
}

//Set a specific element's value
void Matrix::setElement(int r, int c, double val) {
	elements[r][c] = val;
}

//Transpose the Matrix
//
//  |0 1 2|    |0 3 6|
//  |3 4 5| => |1 4 7|
//  |6 7 8|    |2 5 8|
//
Matrix Matrix::transpose() {
	Matrix t(ncol,nrow);

	for(int i=0;i<nrow;++i) {
		for(int j=0;j<ncol;++j) {
			t.elements[j][i] = elements[i][j];
		}
	}

	return t;
}

double Matrix::norm() {

	double n = 0.0;
	for(int i=0;i<nrow;++i) {
		for(int j=0;j<ncol;++j) {
			n += elements[i][j]*elements[i][j];
		}
	}
	return sqrt(n);
}

//Find the determinant
//
// Use LU Decomposition, and multiply return value 
// along diagonal
//
double Matrix::determinant() {
	if(nrow != ncol) return 0;

	Matrix m;
	double det = (double)matrix::lu(*this,m);
	for(int i=0;i<nrow;++i) {
		det *= m(i,i);
	}
	return (std::abs(det) > matrix::epsilon) ? det : 0;
}

//Find the cofactor matrix
Matrix Matrix::cofactor() {
	Matrix B(nrow,ncol);
	Matrix C(nrow-1,ncol-1);

	if(nrow == ncol) {
		for(int j=0;j<nrow;++j) {
			for(int i=0;i<nrow;++i) {
				int i1 = 0;
				for(int ii=0;ii<nrow;++ii) {
					if(ii == i) {
						continue;
					}
					int j1 = 0;
					for(int jj=0;jj<nrow;++jj) {
						if(jj == j) {
							continue;
						}
						C(i1,j1) = elements[ii][jj];
						++j1;
					}
					++i1;
				}

				double d = C.determinant();
				double v = pow(-1.0,i+j+2.0)*d;
				B(i,j) = (std::abs(v) > matrix::epsilon) ? v : 0;
			}
		}
	}

	return B;
}

//Find the adjoint matrix
Matrix Matrix::adjoint() {
	Matrix B = cofactor();
	return ~B;
}

//Find the inverse matrix
//
// AA^-1 = 1
// 
Matrix Matrix::inverse() {
	if(nrow != ncol || nrow < 2) {
		return *this;
	}

	if(nrow == 2) {
		Matrix iA(*this);
		iA(0,0) = elements[1][1];
		iA(1,1) = elements[0][0];
		iA(0,1) = -elements[0][1];
		iA(1,0) = -elements[1][0];
		double d = elements[0][0]*elements[1][1]-elements[0][1]*elements[1][0];

		if(std::abs(d) < matrix::epsilon) {
			//DNE
			return Matrix();
		}

		for(int i=0;i<nrow;++i) {
			for(int j=0;j<nrow;++j) {
				iA(i,j)/=d;
			}
		}
		return iA;
	}

	double d = determinant();

	if(std::abs(d) < matrix::epsilon) {
		return Matrix();
	}
	return adjoint()/d;
}


//Swap rows in the matrix
void Matrix::swap_rows(int r1, int r2) {
	double tmp;

	for(int i=0;i<ncol;++i) {
		tmp = elements[r1][i];
		elements[r1][i] = elements[r2][i];
		elements[r2][i] = tmp;
	}
}

//Global Overload for printing the matrix
std::ostream& operator<<(std::ostream &os, const Matrix &m) {
	for(int i=0;i<m.nrow;++i) {
		os << "|";
		for(int j=0;j<m.ncol-1;++j) {
			os << m.elements[i][j] << "\t";
		}
		os << m.elements[i][m.ncol-1] << "|" << std::endl;
	}

	return os;
}

#endif
