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

#ifndef EML_H
#define EML_H

#include <math.h>
#include <cmath>
#include <string.h>
#include "complex.h"
#include "vector.h"
#include <iostream>

using namespace std;

namespace eml {

	double SMLNUM = 1e-50;
	double BIGNUM = 1e50;

	Complex zlangeM(Vector<Vector<Complex> > x) {
		Complex y;

		throw "NOT IMPLMENTED: DO NOT USE ZLANGEM";
		if(x.Size()) {
			for(int i=0,l=x.Size();i<l;++i) {
				for(int j=0,m=x[i].Size();j<m;++j) {
					Complex absxk = complex_number::abs(x[i][j]);
					if(absxk > y) {
						y = absxk;
					}
				}
			}
		}

		return y; 
	}

	Vector<Vector<Complex> > zlascl(Complex from, Complex to, Vector<Vector<Complex> > A) {
		throw "NOT IMPLMENTED: DO NOT USE ZLASCL";
		return A;
	}

	
	void zggbal_eigsearch_rows(const Vector<Vector<Complex> > &A, const Vector<Vector<Complex> > &B,int ihi, int &i, int &j, bool &found) {

		throw "NOT IMPLMENTED: DO NOT USE ZGGBAL_EIGSEARCH_ROWS";

		i = 0;
		j = 0;
		found = false;
		bool isgen = !((bool)B.Size());

		int ii = ihi-1;
		while(ii >= 0) {
			int nzcount = 0;
			i = ii;
			j = ihi-1;
			for(int jj=0;jj<ihi;++j) {
				if((A[ii][jj] != 0.0) || (isgen && (B[ii][jj] != 0.0)) || (!isgen && (ii == jj))) {
					if(nzcount == 0) {
						j = jj;
						nzcount = 1;
					} else {
						nzcount = 2;
						break;
					}
				}
			}
			if(nzcount < 2) {
				found = true;
				break;
			}
			--ii;
		}
	}


	void zggbal_eigsearch_cols(Vector<Vector<Complex> > A, Vector<Vector<Complex> > B,int ilo, int ihi, int &i, int &j, bool &found) {

		throw "NOT IMPLMENTED: DO NOT USE ZGGBAL_EIGSEARCH_COLS";

		i = 0;
		j = 0;
		found = false;
		bool isgen = !((bool)B.Size());

		for(int jj=ilo;jj<ihi;++jj) {
			int nzcount = 0;
			i = ihi-1;
			j = jj;
			for(int ii=ilo;ii<ihi;++ii) {
				if((A[ii][jj] != 0.0) || (isgen && (B[ii][jj] != 0.0)) || (!isgen && (ii == jj))) {
					if(nzcount == 0) {
						i = ii;
						nzcount = 1;
					} else {
						nzcount = 2;
						break;
					}
				}
			}
			if(nzcount < 2) {
				found = true;
				break;
			}
		}
	}


	void zggbal_simtran(Vector<Vector<Complex> > &A,Vector<Vector<Complex> > &B,int m,int i,int j,int ilo, int ihi) {

		throw "NOT IMPLMENTED: DO NOT USE ZGGBAL_SIMTRAN";

		int n = A.Size();
		bool isgen = !((bool)B.Size());

		if(i != m) {
			for(int k=ilo;k<n;++k) {
				Complex atmp = A[i][k];
				A[i][k] = A[m][k];
				A[m][k] = atmp;
			}
			if(isgen) {
				for(int k=ilo;k<n;++k) {
					Complex btmp = B[i][k];
					B[i][k] = B[m][k];
					B[m][k] = btmp;
				}
			}
		}
		if(j != m) {
			for(int k=0;k<ihi;++k) {
				Complex atmp = A[k][j];
				A[k][j] = A[k][m];
				A[k][m] = atmp;
			}
			if(isgen) {
				for(int k=0;k<ihi;++k) {
					Complex btmp = B[k][j];
					B[k][j] = B[k][m];
					B[k][m] = btmp;
				}
			}
		}

	}

	void zggbal(Vector<Vector<Complex> > &A,Vector<Vector<Complex> > &B,int &ilo,int &ihi,Vector<Complex> &rscale) {

		throw "NOT IMPLMENTED: DO NOT USE ZGGBAL";

		int n = A.Size();
		rscale = Vector<Complex>(n);
		ilo = 0;
		ihi = n;

		if(n <= 1) {
			ilo = 0;
			ihi = 0;
			rscale[0] = 0;
			return;
		}

		while(1) {
			int i,j;
			bool found;
			zggbal_eigsearch_rows(A,B,ihi,i,j,found);
			if(!found) {
				break;
			}
			zggbal_simtran(A,B,ihi,i,j,ilo,ihi);
			rscale[ihi] = j;
			if(ihi == 1) {
				rscale[ihi] = ihi;
				return;
			}
		}
		while(1) {
			int i,j;
			bool found;
			zggbal_eigsearch_cols(A,B,ilo,ihi,i,j,found);
			if(!found) {
				break;
			}
			zggbal_simtran(A,B,ihi,i,j,ilo,ihi);
			rscale[ilo] = j;
			ilo = ++ilo;
			if(ilo == ihi) {
				rscale[ilo] = ilo;
				return;
			}
		}
	}

	double sub2norm(Matrix x, int start, int end) {

		double y = 0.0, scale = 0.0;

		for(int k=start;k<end;++k) {
			double absx = abs(x(k));
			if(absx > 0) {
				if(scale < absx) {
					double tmp = scale/absx;
					y = 1 + y*tmp*tmp;
					scale = absx;
				} else {
					double tmp = absx / scale;
					y = y + tmp*tmp;
				}
			}	
		}
		return (scale * sqrt(y));
	}	

	bool isreal(const Vector<Vector<Complex> > &v) {
		for(int i=0,il=v.Size();i<il;++i) {
			for(int j=0,jl=v[i].Size();j<jl;++j) {
				if(!v[i][j].isReal()) {
					return false;
				}
			}
		}
		return true;
	}

	void zgeqr2(Vector<Vector<Complex> > &A, int ijmin, int imax, int jmax,Vector<Complex> &tau) {

		throw "NOT IMPLMENTED: DO NOT USE ZQEQR2";

		double SAFMIN = 2 * SMLNUM;
		double RSAFMN = 1/SAFMIN;
		int m = A.Size();
		int n = A[0].Size();
		int mmn = m < n ? m : n;
		int ijm = imax < jmax ? imax : jmax;
		int mn = mmn < ijm ? mmn : ijm;
cout << m << endl;
cout << n << endl;
		for(int i=ijmin;i<mn;++i) {
			int itau = i - ijmin + 1;
			int i2 = i*m;
			int i1 = i2 + 1 - (imax - i);
			double xnrm = sub2norm(Matrix(A),i1,i2);
			double alphar = A[i][i].getReal();
			double alphai = A[i][i].getImag();

			if(xnrm == 0 && alphai == 0) {
				tau[itau] = 0.0;
			} else {
				double beta1;
				if(isreal(A)) {
					beta1 = sqrt(pow(abs(alphar),2) + pow(abs(xnrm),2));
				} else {
					beta1 = sqrt(pow(abs(alphar),2) + pow(abs(alphai),2) + pow(abs(xnrm),2));
				} 
				if(alphar >= 0) {
					beta1 = -beta1;
				}
				int knt = 0;
				while(abs(beta1) < SAFMIN) {
					++knt;
					for(int k=i1;k<i2;++k) {
						A[k] = A[k] * RSAFMN;
					}
					beta1 *= RSAFMN;
					alphai *= RSAFMN;
					alphar *= RSAFMN;
				}
				if(knt > 0) {
					xnrm = sub2norm(Matrix(A),i1,i2);
					if(isreal(A)) {
						beta1 = sqrt(pow(abs(alphar),2) + pow(abs(xnrm),2));
					} else {
						beta1 = sqrt(pow(abs(alphar),2) + pow(abs(alphai),2) + pow(abs(xnrm),2));
					}
					if(alphar >= 0) {
						beta1 = -beta1;
					}
				}

				double alpha1;
				if(isreal(A)) {
					tau[itau] = (beta1 - alphar)/beta1;
					alpha1 = 1/(alphar - beta1);
				} else {
					tau[itau] = Complex((beta1-alphar)/beta1,-alphai/beta1);
					alpha1 = 1/static_cast<double>(Complex(alphar-beta1,alphai));
				}
				for(int k=i1;k<i2;++k) {
					A[k] = A[k] * alpha1;
				}
				for(int k=i1;k<i2;++k) {
					beta1 *= SAFMIN;
				}
				A[i][i] = beta1;
			}
			if(i < jmax) {
				Complex conjtaui = complex_number::conj(tau[itau]);
				if(conjtaui != 0.0) {
					int ip1 = i+1;
					for(int j=ip1;j<jmax;++j) {
						Complex wj = complex_number::conj(A[i][j]);
						for(int k=ip1;k<imax;++k) {
							wj += A[k][j] * complex_number::conj(A[k][j]);
						}
						wj = conjtaui * complex_number::conj(wj);
						if(wj != 0.0) {
							A[i][j] -= wj;
							for(int k=ip1;k<imax;++k) {
								A[k][j] -= A[k][i]*wj;
							}
						}
					}
				}
			}
		}
	}

	void zgghrd(const char *compq, const char *compz, int ilo, int ihi, const Vector<Vector<Complex> > &A, const Vector<Vector<Complex> > &B) {

		throw "NOT IMPLMENTED: DO NOT USE ZGGHRD";

		int n = A.Size();

		if(strcmp(compq,"N")) {
			throw "zgghrd: Not Implemnted";
		}
		if(strcmp(compz,"N")) {
			throw "zgghrd: Not Implemnted";
		}

		double ilq = false;
		double ilz = false;

		int ihim1 = ihi - 1;
		int jcol = ilo;
		//TODO: Finish
	}

	void zggev(Vector<Vector<Complex> > Ac, Vector<Vector<Complex> > Bc, Vector<Complex> &a1, Vector<Complex> &b1) {
	
		throw "NOT IMPLMENTED: DO NOT USE ZGGEV";

		if(Ac.Size() == 0) {
			a1 = Vector<Complex>();
			b1 = Vector<Complex>();
			return;
		}

		bool compv = false;
		bool isgen = !((bool)Bc.Size());

		int n = Ac.Size();
		Complex anrm = zlangeM(Ac);
		bool ilascl = false;
		Complex anrmto = anrm;

		if(anrm > 0.0 && anrm < SMLNUM) {
			anrmto = SMLNUM;
			ilascl = true;
		} else if(anrm > BIGNUM) {
			anrmto = BIGNUM;
			ilascl = true;
		}

		if(ilascl) {
			Ac = zlascl(anrm,anrmto,Ac);
		}

		if(isgen) {
			Complex bnrm = zlangeM(Bc);
			bool ilbscl = false;
			Complex bnrmto = bnrm;

			if(bnrm > 0.0 && bnrm < SMLNUM) {
				bnrmto = SMLNUM;
				ilbscl = true;
			} else if(bnrm > BIGNUM) {
				bnrmto = BIGNUM;
				ilbscl = true;
			}

			if(ilbscl) {
				Bc = zlascl(bnrm,bnrmto,Bc);
			}
		}

		int ilo,ihi;
		Vector<Complex> rscale;
		zggbal(Ac,Bc,ilo,ihi,rscale);

		int lastcol = ihi;
		if(isgen) {
			Vector<Complex> tau;
			//TODO: Implement those 2
			//zgeqr2(Bc,ilo,ihi,lastcol,tau);
			//Ac = zunmqr(Bc,tau,Ac,ilo,ihi,lastcol)
		}
		//TODO: Implement
		//zgghrd("N","N",ilo,ihi,Ac,Bc);
		//zhgeqz(Ac,Bc,ilo,ihi,a1,b1);
	}
}

#endif
