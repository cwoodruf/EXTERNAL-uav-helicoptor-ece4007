#ifndef EML_H
#define EML_H

#include <math.h>
#include <cmath>
#include "complex.h"
#include "vector.h"
#include <iostream>


namespace eml {

	double SMLNUM = 1e-50;
	double BIGNUM = 1e50;

	Complex zlangeM(Vector<Vector<Complex> > x) {
		Complex y;

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
		std::cout << "WARNING: zlascl not implemented" << std::endl;
		return A;
	}
/*
	void zggbal_eigsearch_rows(Vector<Vector<Complex> > A, Vector<Vector<Complex> > B,int ihi, int &i, int &j, bool &found) {
		i = 0;
		j = 0;
		found = false;
		isgen = !((bool)B.Size());
		for(int ii=ihi;ii>=0;++ii) {
			for(int jj=0;jj<ihi;++jj) {
				if(A[ii][jj] != 0 || (isgen && (B[])))
			}
		}
	}
*/
	void zggbal(Vector<Vector<Complex> > &A,Vector<Vector<Complex> > &B,int &ilo,int &ihi,Vector<Complex> &rscale) {
		int n = A.Size();
		rscale = Vector<Complex>(n);
		ilo = 0;
		ihi = n;

		if(n <= 1) {
			ilo = 0;
			ihi = 0;
			rscale[0] = 1;
			return;
		}
/*
		while(1) {
			int i,j;
			bool found;
			zggbal_eigsearch_rows(A,B,ihi,i,j,found);
			if(!found) {
				break;
			}
			zggbal_simtran(A,B,ihi,i,j,ilo,ihi);
			rscale[ihi] = j;
			ihi = index_minus(ihi,0);
			if(ihi == 1) {
				rscale[ihi] = ihi;
				return;
			}
		}
*/
	}

	void zggev(Vector<Vector<Complex> > Ac, Vector<Vector<Complex> > Bc, Vector<Complex> &a1, Vector<Complex> &b1) {
	
		if(Ac.Size() == 0) {
			a1 = Vector<Complex>();
			b1 = Vector<Complex>();
			return;
		}

		bool compv = false;
		bool isgen = !((bool)Bc.Size());

		int n = Ac[0].Size();
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

			if(bnrm > 0 && bnrm < SMLNUM) {
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
	}
}

#endif
