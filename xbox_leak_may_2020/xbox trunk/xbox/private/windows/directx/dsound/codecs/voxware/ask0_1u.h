/******************************************************************************
*                        Voxware Proprietary Material
*                        Copyright 1996, Voxware, Inc
*                            All Rights Reserved
*
*                       DISTRIBUTION PROHIBITED without
*                      written authorization from Voxware
******************************************************************************/
 
/******************************************************************************
* Filename:    ASK0_1U.h
*
* Purpose:     codebooks for 1st ASK pair, which include 5 bits stage 1 codebook,
*              5 bit rotation-scaling matrix, 5 bit stage 2 inner cell codebook
*              and 5 bit stage 2 outer cell codebook. Those codebooks are designed 
*              for unvoiced frame.
*
* $Header:   /export/phobos/pvcs/r_and_d/archives/scaleable/tables/ASK0_1U.h_v   1.0   06 Feb 1998 10:12:18   johnp  $
*
******************************************************************************/
#ifndef _ASK0_1UV_H_
#define _ASK0_1UV_H_
5.346080e-001F, -2.664800e-002F,  /*    Stage 1 codebook   (5 bits) */
-7.315710e-001F, -1.067840e-001F,
5.545200e-001F, -8.732390e-001F,
-1.388620e-001F, -6.173690e-001F,
-1.145860e-001F, -2.426080e-001F,
-1.646715e+000F, -5.555210e-001F,
1.810100e-001F, -7.415380e-001F,
-1.080797e+000F, -3.857420e-001F,
2.757710e-001F, 6.284650e-001F,
-1.022302e+000F, 4.740630e-001F,
2.691220e-001F, 1.512120e-001F,
-6.456890e-001F, 2.273390e-001F,
-5.576760e-001F, 5.870270e-001F,
-1.588281e+000F, 5.492110e-001F,
-3.897670e-001F, -3.216600e-002F,
-1.783118e+000F, 2.189600e-002F,
5.327120e-001F, 1.616290e-001F,
-6.619200e-002F, 1.006160e-001F,
4.889410e-001F, -3.056130e-001F,
1.933000e-001F, -4.359960e-001F,
2.243300e-001F, -1.296420e-001F,
-9.830650e-001F, 6.724300e-002F,
5.011230e-001F, -5.915750e-001F,
-5.559490e-001F, -4.871650e-001F,
9.395000e-002F, 1.015221e+000F,
-4.693910e-001F, 1.040591e+000F,
4.457890e-001F, 3.434880e-001F,
-2.884950e-001F, 3.371480e-001F,
-1.120130e-001F, 6.302960e-001F,
-1.092578e+000F, 9.320080e-001F,
9.762100e-002F, 3.421660e-001F,
-1.359483e+000F, 3.691600e-002F, 
1.197622e+001F, 0.000000e+000F,       /* scaling and rotation codebook (5 bits) */
-5.473183e+000F, -5.473183e+000F,
0.000000e+000F, -9.567114e+000F,
-2.346564e+000F, -6.113009e+000F,
-4.737762e+000F, -6.064059e+000F,
-2.067132e+000F, -4.056975e+000F,
-2.778737e+000F, -7.238858e+000F,
4.621751e-001F, -5.282686e+000F,
6.674952e+000F, 2.971880e+000F,
-5.738080e+000F, -1.011778e+000F,
8.560416e+000F, 4.942359e+000F,
7.563557e+000F, 2.026649e+000F,
6.284680e+000F, 1.683975e+000F,
-3.669446e+000F, 2.765126e+000F,
-4.314700e+000F, -6.396806e+000F,
-4.815127e+000F, 0.000000e+000F,
1.434556e+001F, 2.272114e+000F,
-6.034344e+000F, 6.034344e+000F,
9.383263e+000F, -4.917560e-001F,
8.508529e+000F, -1.485169e-001F,
8.639927e+000F, -6.041626e-001F,
-2.960525e+000F, -6.649448e+000F,
9.357885e+000F, 3.267846e-001F,
-1.127190e+000F, -5.303013e+000F,
3.026769e+000F, 4.322675e+000F,
-2.462139e-001F, 4.698040e+000F,
8.993511e+000F, 5.192406e+000F,
-5.437171e-001F, 7.775517e+000F,
-2.584510e-001F, 7.401068e+000F,
-1.340366e+000F, 4.125221e+000F,
6.355361e+000F, 6.355361e+000F,
0.000000e+000F, 5.716511e+000F,
-3.689370e-001F, 7.284960e-001F,    /* stage 2 inner cell codebook (5 bits) */
9.890650e-001F, 4.882320e-001F,
-1.312897e+000F, 7.767640e-001F,
6.278100e-002F, 8.835180e-001F,
-4.261960e-001F, -2.561540e-001F,
6.963430e-001F, -6.519790e-001F,
-1.289014e+000F, -3.634020e-001F,
-2.468930e-001F, -7.322210e-001F,
-2.804180e-001F, 1.260715e+000F,
9.564890e-001F, 1.136400e+000F,
-8.134410e-001F, 1.074187e+000F,
3.471590e-001F, 1.257091e+000F,
8.138800e-002F, 4.041290e-001F,
1.385320e+000F, 7.117900e-002F,
-8.851230e-001F, -1.982800e-002F,
4.031270e-001F, -2.622580e-001F,
-4.036960e-001F, 2.359050e-001F,
8.979540e-001F, -1.419460e-001F,
-1.418555e+000F, 1.852740e-001F,
1.182600e-002F, -1.382330e-001F,
-5.792290e-001F, -1.135800e+000F,
9.723100e-001F, -1.100621e+000F,
-1.162367e+000F, -9.777620e-001F,
-3.641100e-002F, -1.222577e+000F,
5.397330e-001F, 7.543810e-001F,
1.555048e+000F, 7.269100e-001F,
-8.615620e-001F, 5.074220e-001F,
5.429300e-001F, 2.184310e-001F,
1.909680e-001F, -7.088340e-001F,
1.247367e+000F, -5.702140e-001F,
-7.811680e-001F, -6.135670e-001F,
4.776820e-001F, -1.171449e+000F,
9.467470e-001F, -7.964760e-001F,  /* stage 2 outer cell codebook */
2.056197e+000F, 2.331590e-001F,
-5.249660e-001F, -3.631910e-001F,
8.569580e-001F, 7.814150e-001F,
-2.489300e-001F, 8.470100e-002F,
1.229369e+000F, 1.196587e+000F,
-9.227100e-001F, 7.431310e-001F,
-2.944800e-002F, 1.190333e+000F,
3.529100e-002F, -3.254440e-001F,
1.815008e+000F, 1.154408e+000F,
-7.212640e-001F, 4.833700e-002F,
3.625540e-001F, 7.354990e-001F,
-5.801710e-001F, 4.654920e-001F,
5.559260e-001F, 1.350522e+000F,
-1.113965e+000F, 2.980620e-001F,
-5.201220e-001F, 1.048562e+000F,
1.356565e+000F, -1.229959e+000F,
2.212666e+000F, -9.018570e-001F,
-4.786700e-002F, -1.185486e+000F,
1.086926e+000F, -1.770570e-001F,
3.773470e-001F, -8.436830e-001F,
1.351700e+000F, 3.794640e-001F,
-8.050260e-001F, -7.061850e-001F,
2.128770e-001F, 2.517420e-001F,
6.001950e-001F, -1.308107e+000F,
1.542591e+000F, -5.610400e-001F,
-5.539760e-001F, -1.111319e+000F,
5.145440e-001F, -3.403320e-001F,
-2.330810e-001F, -7.583730e-001F,
7.282810e-001F, 2.160780e-001F,
-1.057276e+000F, -2.468670e-001F,
-1.821850e-001F, 6.986970e-001F
#endif