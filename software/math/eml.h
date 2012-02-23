#ifndef EML_H
#define EML_H

#include <math.h>
#include <cmath>
#include "complex.h"
#include "vector.h"
#include <iostream>

using namespace std;

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

	
	void zggbal_eigsearch_rows(const Vector<Vector<Complex> > &A, const Vector<Vector<Complex> > &B,int ihi, int &i, int &j, bool &found) {

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

	
	Vector<Vector<Complex> > zgeqr2(Vector<Vector<Complex> > Bc, int ijmin, int imax, int jmax,Vector<Vector<Complex> > &tau) {

		Vector<Vector<Complex> > out(Bc);

		
			
	}

	void zggev(Vector<Vector<Complex> > Ac, Vector<Vector<Complex> > Bc, Vector<Complex> &a1, Vector<Complex> &b1) {
	
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
			Vector<Vector<Complex> > tau;
			//Bc = zgeqr2(Bc,ilo,ihi,lascol,tau)
			//Ac = zunmqr(Bc,tau,Ac,ilo,ihi,lastcol)
		}
		//zgghrd("N","N",ilo,ihi,Ac,Bc);
		//zhgeqz(Ac,Bc,ilo,ihi,a1,b1);
	}
}

#endif
