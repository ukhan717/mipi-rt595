/* ------------------------------------------------------------------------ */
/* Copyright (c) 2016 by Cadence Design Systems, Inc. ALL RIGHTS RESERVED.  */
/* These coded instructions, statements, and computer programs (�Cadence    */
/* Libraries�) are the copyrighted works of Cadence Design Systems Inc.	    */
/* Cadence IP is licensed for use with Cadence processor cores only and     */
/* must not be used for any other processors and platforms. Your use of the */
/* Cadence Libraries is subject to the terms of the license agreement you   */
/* have entered into with Cadence Design Systems, or a sublicense granted   */
/* to you by a direct Cadence licensee.                                     */
/* ------------------------------------------------------------------------ */
/*  IntegrIT, Ltd.   www.integrIT.com, info@integrIT.com                    */
/*                                                                          */
/* DSP Library                                                              */
/*                                                                          */
/* This library contains copyrighted materials, trade secrets and other     */
/* proprietary information of IntegrIT, Ltd. This software is licensed for  */
/* use with Cadence processor cores only and must not be used for any other */
/* processors and platforms. The license to use these sources was given to  */
/* Cadence, Inc. under Terms and Condition of a Software License Agreement  */
/* between Cadence, Inc. and IntegrIT, Ltd.                                 */
/* ------------------------------------------------------------------------ */
/*          Copyright (C) 2015-2016 IntegrIT, Limited.                      */
/*                      All Rights Reserved.                                */
/* ------------------------------------------------------------------------ */

/*
  NatureDSP Signal Processing Library. FIR part
    Real data circular cross-correlation, 32x16-bit
    C code optimized for Fusion
*/

/*-------------------------------------------------------------------------
  Circular Correlation
  Estimates the circular cross-correlation between vectors x (of length N) 
  and y (of length M)  resulting in vector r of length N. It is a similar 
  to correlation but x is read in opposite direction.

  Precision: 
  16x16     16x16-bit data, 16-bit outputs
  24x24     24x24-bit data, 24-bit outputs
  32x16     32x16-bit data, 32-bit outputs
  32x32     32x32-bit data, 32-bit outputs
  f         floating point (both real and complex data)

  Input:
  x[N]          input data Q31,Q15 or floating point
  y[M]          input data Q31,Q15 or floating point
  N             length of x
  M             length of y
  Output:
  r[N]          output data,Q31,Q15 or floating point

  Restriction:
  x,y,r should not overlap
  x,y,r - aligned on an 8-bytes boundary
  N,M   - multiples of 4 and >0
-------------------------------------------------------------------------*/

/* Portable data types. */
#include "NatureDSP_types.h"
/* Signal Processing Library API. */
#include "NatureDSP_Signal.h"
/* Common utility and macros declarations. */
#include "common.h"

void fir_xcorr32x16( int32_t * restrict r,
               const int32_t * restrict x,
               const int16_t * restrict y,
               int N, int M )
{
  //
  // Circular cross-correlation algorithm:
  //
  //   r[n] = sum( x[mod(n+m,N)]*y[m] )
  //        m=0..M-1
  //
  //   where n = 0..N-1
  //

    const ae_int32x2 * restrict pX0;
    const ae_int32x2 * restrict pX1;
    const int32_t * restrict pY;
          ae_f32x2 * restrict pR;
    ae_int32x2 X01,X12;
    ae_f16x4 Y01;
    ae_f64 a0,a1,a2,a3;
    ae_valign ax0;
    int n, m;

    NASSERT(r);
    NASSERT(x);
    NASSERT(y);
    NASSERT_ALIGN(r,8);
    NASSERT_ALIGN(x,8);
    NASSERT_ALIGN(y,8);
    NASSERT(M>0 && M%4==0);
    NASSERT(N>0 && N%4==0);
    WUR_AE_CBEGIN0( (uintptr_t)( x     ) );
    WUR_AE_CEND0  ( (uintptr_t)( x + N ) );
    pR=(ae_f32x2*)r;

    for ( n=0; n<N; n+=4 )
    {
        a0=a1=a2=a3=AE_ZERO();
        pY=(const int32_t*)y;
        pX0=(const ae_int32x2*)(x+n);
        pX1=(const ae_int32x2*)(x+n+1);
        AE_LA32X2POS_PC(ax0,pX1);
        AE_L32X2_XC (X01,pX0,sizeof(int32_t)*2);
        AE_LA32X2_IC(X12,ax0,pX1);
        __Pragma("loop_count min=2")
        for ( m=0; m<M; m+=2 )
        {
            ae_int32x2 t;
            AE_L32_IP(t, castxcc(ae_int32, pY), sizeof(int32_t)); Y01 = AE_MOVF16X4_FROMINT32X2(t);
            AE_MULAAFD32X16_H2_L3(a0,X01,Y01);
            AE_MULAAFD32X16_H2_L3(a1,X12,Y01);
            AE_L32X2_XC (X01,pX0,sizeof(int32_t)*2);
            AE_LA32X2_IC(X12,ax0,pX1);
            AE_MULAAFD32X16_H2_L3(a2,X01,Y01);
            AE_MULAAFD32X16_H2_L3(a3,X12,Y01);
        }
        AE_S32X2RA64S_IP(a0,a1,pR);
        AE_S32X2RA64S_IP(a2,a3,pR);
    }
} // fir_xcorr32x16()
