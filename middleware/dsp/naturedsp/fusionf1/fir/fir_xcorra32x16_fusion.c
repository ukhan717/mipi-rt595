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
    Real data circular cross-correlation, 32x16-bit, no requirements on vectors 
    length and alignment.
    C code optimized for Fusion
*/

/*-------------------------------------------------------------------------
  Circular Correlation
  Estimates the circular cross-correlation between vectors x (of length N) 
  and y (of length M)  resulting in vector r of length N. It is a similar 
  to correlation but x is read in opposite direction.
  These functions implement the circular correlation algorithm described in
  the previous chapter with no limitations on x and y vectors length and
  alignment at the cost of increased processing complexity. In addition, this
  implementation variant requires scratch memory area.

  Precision: 
  16x16     16x16-bit data, 16-bit outputs
  24x24     24x24-bit data, 24-bit outputs
  32x16     32x16-bit data, 32-bit outputs
  32x32     32x32-bit data, 32-bit outputs
  f         floating point (both real and complex data)

  Input:
  s[]           Scratch memory, 
                FIR_XCORRA16X16_SCRATCH_SIZE( N, M ) 
                FIR_XCORRA24X24_SCRATCH_SIZE( N, M ) 
                FIR_XCORRA32X16_SCRATCH_SIZE( N, M ) 
                FIR_XCORRA32X32_SCRATCH_SIZE( N, M ) 
                FIR_XCORRAF_SCRATCH_SIZE( N, M )     
                CXFIR_XCORRAF_SCRATCH_SIZE( N, M )   
                bytes
  x[N]          input data Q31, Q15  or floating point
  y[M]          input data Q31, Q15 or floating point
  N             length of x
  M             length of y
  Output:
  r[N]          output data, Q31, Q15  or floating point

  Restrictions:
  x,y,r should not overlap
  s        - must be aligned on an 8-bytes boundary
  N,M      - must be >0
  N >= M-1 - minimum allowed length of vector x is the length of y minus one
-------------------------------------------------------------------------*/
/* Portable data types. */
#include "NatureDSP_types.h"
/* Signal Processing Library API. */
#include "NatureDSP_Signal.h"
/* Common utility and macros declarations. */
#include "common.h"
#include "raw_corr32x16.h"

/* Align address on a specified boundary. */
#define ALIGNED_ADDR( addr, align ) \
      (void*)( ( (uintptr_t)(addr) + ( (align) - 1 ) ) & ~( (align) - 1 ) )

void fir_xcorra32x16( void    * restrict s,
                      int32_t * restrict r,
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

    void    * s_ptr;
    int32_t * x_buf;
    int16_t * y_buf;

    const ae_int32x2 *          S;
        ae_int32x2 * restrict D;
    const ae_int16x4 *          S16;
        ae_int16x4 * restrict D16;
        ae_int16   * restrict D16_s;
    ae_valign S_va, D_va;

    ae_int32x2 t;
    ae_int16x4 p;

    int n, m;

    NASSERT(s);
    NASSERT(r);
    NASSERT(x);
    NASSERT(y);
    NASSERT_ALIGN(s,8);
    NASSERT(N>0 && M>0);
    NASSERT(N>=M-1);

    //----------------------------------------------------------------------------
    // Partition the scratch memory area.

    s_ptr = s;
    x_buf = (int32_t*)ALIGNED_ADDR( s_ptr, 8 );
    s_ptr = x_buf + N + M-1 + 3;
    y_buf = (int16_t*)ALIGNED_ADDR( s_ptr, 8 );
    s_ptr = y_buf + M + 3;

    ASSERT( (int8_t *)s_ptr - (int8_t *)s <= 
            (int)FIR_XCORRA32X16_SCRATCH_SIZE( N, M ) );

    //----------------------------------------------------------------------------
    // Copy x[N] data into the aligned buffer in a way that simplifies the
    // correlation calculation:
    //  x_buf[N+M-1+3] = { x[0]..x[N-1] x[0]..x[M-2] X X X }
    // Three X locations are reserved to allow for block-4 processing.
    // Copy x[N].
    S = (const ae_int32x2 *)x;
    D = (      ae_int32x2 *)x_buf;
    S_va = AE_LA64_PP( S );
    for ( n=0; n<((N+1)>>1); n++ )
    {
        AE_LA32X2_IP( t, S_va, S );
        AE_S32X2_IP( t, D, +8 );
    }
  // Copy first M-1 entries of x{N].
  S = (const ae_int32x2 *)x;
  D = (      ae_int32x2 *)( (int32_t *)D - ( N & 1 ) );
  S_va = AE_LA64_PP( S );
  D_va = AE_ZALIGN64();
#ifdef COMPILER_XTENSA
  #pragma concurrent
#endif
    for ( m=0; m<(M>>1); m++ )
    {
        AE_LA32X2_IP( t, S_va, S );
        AE_SA32X2_IP( t, D_va, D );
    }
    AE_SA64POS_FP( D_va, D );
    D = (      ae_int32x2 *)(x_buf+N + M-1);
    AE_S32_L_I( AE_ZERO32(),(ae_int32*) D, 0*sizeof(int32_t) );
    AE_S32_L_I( AE_ZERO32(),(ae_int32*) D, 1*sizeof(int32_t) );
    AE_S32_L_I( AE_ZERO32(),(ae_int32*) D, 2*sizeof(int32_t) );
    //----------------------------------------------------------------------------
    // Copy y[M] data into the aligned buffer and append 3 zeros:
    //  y_buf[M+3] = { y[0]..y[M-1] 0 0 0 }
    S16 = (const ae_int16x4 *)y;
    D16 = (      ae_int16x4 *)y_buf;
    S_va = AE_LA64_PP( S16 );
    for ( m=0; m<((M+3)>>2); m++ )
    {
        AE_LA16X4_IP( p, S_va, S16 );
        AE_S16X4_IP( p, D16, +8 );
    }
  // Append three zeros to allow for block-4 processing.
    D16_s = (ae_int16 *)( (int16_t *)D16 - ( -M & 3 ) );
    p = AE_ZERO16();
    for ( m=0; m<3; m++ )
    {
        AE_S16_0_IP( p, D16_s, +2 );
    }
    raw_corr32x16(r, x_buf, y_buf,N, (M+1)&~1);
} // fir_xcorra32x16()