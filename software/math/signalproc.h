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
#ifndef SIGNALPROC_H
#define SIGNALPROC_H

#include <math.h>
#include <cmath>
#include <string.h>


namespace signalproc {

	// Design a LP Butterworth Filter
	//
	// Returns filter coefficients ax and by
	// 
	// Process:
	// 1) Take the transfer function : H(s)
	// 2) Perform Bilinear Transform : H((z-1)/(z+1))
	// 3) Transpose fc to be in the bilinear transfer domain
	// 4) Express the Function as
	//
	//            B(z)       b0 + b1*z^-1 + b2z^-2 + ... + bN*z^-N
	//   H(z) = -------- = -----------------------------------------
	//            A(z)       1 + a1*z^-1 + a2*z^-2 + ... + aM*z^-M
	//
	void butter(double fs, double fc, double *ax, double *by) {
		double QcRaw = (2*M_PI*fc) / fs;
		double QcWarp = tan(QcRaw);

		double gain = 1 / (1+M_SQRT2/QcWarp + 2/(QcWarp^QcWarp));
		by[0] = 1;
		by[1] = (2 - 2*2/(QcWarp*QcWarp)) * gain;
		by[2] = (1 - M_SQRT2/QcWarp + 2/(QcWarp*QcWarp)) * gain;
		ax[0] = gain;
		ax[1] = 2 * gain;
		ax[2] = gain;
	}

	// Filter a signal with filter coeffs A and B
	//
	// Filter Response given as
	//
	//            N               M
	//           ___             ___
	//           \               \
	// y[n] =  - /__ ak*y[n-k] + /__ bk*x[n-k]
	//           k=1             k=0
	//
	void filter(double *ax, double *by, double *samples, int count) {
		double xv[3];
		double xy[3];

		for(int i=0;i<count;++i) {
			xv[2] = xv[1]; xv[1] = xv[0];
			xv[0] = samples[i];
			yv[2] = yv[1]; yc[1] = yv[0];

			yv[0] = (ax[0]*xv[0] + ax[1]*xv[1] + ax[2]*xv[2] - by[1]*yv[0] - by[2]*yv[1]);

			samples[i] = yv[0];
		}
	}
}

#endif
