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
    Real data circular convolution, 24x24-bit
    C code optimized for Fusion
*/

/*-------------------------------------------------------------------------
  Circular Convolution
  Performs circular convolution between vectors x (of length N) and y (of 
  length M)  resulting in vector r of length N.

  Precision: 
  16x16  16x16-bit data, 16-bit outputs
  24x24  24x24-bit data, 24-bit outputs
  32x16  32x16-bit data, 32-bit outputs (both real and complex)
  32x32  32x32-bit data, 32-bit outputs 
  f      floating point 

  Input:
  x[N]          input data (Q31,Q15 or floating point)
  y[M]          input data (Q31,Q15 or floating point)
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

void fir_convol24x24( int32_t * restrict r,
                const f24     * restrict x,
                const f24     * restrict y,
                int N, int M )
{
    const ae_f24x2 * restrict pX0;
    const ae_f24x2 * restrict pX1;
    const ae_f24x2 * restrict pY;
          ae_f24x2 * restrict pR;
    ae_f24x2 X10,X21,X32,X43,Y01,d0;
    ae_f64 a0,a1,a2,a3;
    ae_valign ax0;
    int n,m;
    // Circular convolution algorithm:
    //   r[n] = sum( x[mod(n-m,N)]*y[m] )
    //        m=0..M-1
    //   where n = 0..N-1

    NASSERT( r && x && y && N > 0 && M > 0 );
    NASSERT(N%4==0 && M%4==0);
    NASSERT_ALIGN(x,8);
    NASSERT_ALIGN(y,8);
    NASSERT_ALIGN(r,8);
    WUR_AE_CBEGIN0( (uintptr_t)( x     ) );
    WUR_AE_CEND0  ( (uintptr_t)( x + N ) );
    pR=(ae_f24x2*)r;
    for ( n=0; n<N; n+=4 )
    {
        a0=a1=a2=a3=AE_ZERO();
        pY=(const ae_f24x2*)y;
        pX0=pX1=(const ae_f24x2*)(x+n+2);
        AE_LA32X2F24NEG_PC(ax0,pX1);
        AE_LA32X2F24_RIC(X32,ax0,pX1);
        AE_L32X2F24_RIC(X43,pX0);
        __Pragma ("loop_count factor=2")
        for ( m=0; m<M; m+=2 )
        {
            AE_LA32X2F24_RIC(X10,ax0,pX1);
            AE_L32X2F24_RIC (X21,pX0);
            AE_L32X2F24_IP(Y01,pY,sizeof(int32_t)*2);
            AE_MULAAFD24_HH_LL(a0,Y01,X10);
            AE_MULAAFD24_HH_LL(a1,Y01,X21);
            AE_MULAAFD24_HH_LL(a2,Y01,X32);
            AE_MULAAFD24_HH_LL(a3,Y01,X43);
            X32=X10;
            X43=X21;
        }
        // Q(23+8) <- [ Q16.47 - 24 w/ rounding and saturation ] + 8
        d0 = AE_ROUND24X2F48SASYM( a0, a1 ); AE_S32X2F24_IP( d0, pR, sizeof(int32_t)*2);
        d0 = AE_ROUND24X2F48SASYM( a2, a3 ); AE_S32X2F24_IP( d0, pR, sizeof(int32_t)*2);
    }
} /* fir_convol24x24() */
