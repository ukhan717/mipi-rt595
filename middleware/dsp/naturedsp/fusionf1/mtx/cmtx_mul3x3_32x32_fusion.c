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

/* Cross-platform data type definitions. */
#include "NatureDSP_types.h"
/* Signal Processing Library API. */
#include "NatureDSP_Signal.h"
/* Common helper macros. */
#include "common.h"
/*-------------------------------------------------------------------------
  Operations with Small Matrices
  These functions implement basic operations under the sequence of small 
  square matrices. Fixed point data are interpreted as Q15 or Q31 and 
  results might be saturated.
  NOTE: 
  Determinant is computed recursively via minors of submatrices. So, in 
  the fixed-point routines, intermediate results might be saturated 
  although final result is in range. To avoid this saturation, right shift 
  might be applied at the first stage of computations. It means that final 
  result would be represented in Q(15-rsh) or Q(31-rsh) respectively. 
  Ad-hoc formula for rsh is rsh>=N-2 for real matrices and rsh>=N-1 for 
  complex matrices.

  Precision: 
  16x16  16-bit input, 16-bit output (real and complex)
  32x32  32-bit input, 32-bit output (real and complex)
  f      floating point (real and complex)

  Matrix dimensions are 2x2, 3x3, 4x4

  Input:
  x[L][N*N]      L input matrices
  y[L][N*N]      L input matrices (for addition, subtraction, multiply 
                 functions)
  rsh            right shift for fixed-point multiply and determinant 
                 function
  L              number of matrices
  Output:
  z[L][N*N]      L output matrices (for addition, subtraction, multiply, 
                 transpose functions)
  d[L]           determinants for L matrices (for determinant functions)

  Restrictions:
  rsh should be in range 0..15
  x,y,z should not overlap
-------------------------------------------------------------------------*/
/* complex matrix multiply */
void cmtx_mul3x3_32x32(complex_fract32 * restrict z, const complex_fract32 *restrict x, const complex_fract32 *restrict y, int rsh, int L)
{
  const ae_int32x2    *  restrict py0 = (const ae_int32x2      *)y;
  const ae_int32x2    *  restrict py1 = (const ae_int32x2      *)y;
  const ae_int32x2    *  restrict py2 = (const ae_int32x2      *)y;
        ae_int32x2    *  restrict pz = (      ae_int32x2      *)z;
  const ae_int32x2    *  restrict px = (const ae_int32x2      *)x;
  int l, m;
  ae_int32x2 vx0, vy0, vx1, vy1, vx2, vy2;
  ae_int32x2 vz0;
  ae_f64 A0, A1;
  NASSERT_ALIGN(x, sizeof(*x));
  NASSERT_ALIGN(y, sizeof(*y));
  NASSERT_ALIGN(z, sizeof(*z));
  NASSERT(rsh >= 0 && rsh <= 15);
  px = (const ae_int32x2    *)(x);
  for (l = 0; l<L; l++)
  {
    py0 = (const ae_int32x2    *)(y + 0+9*l);
    py1 = (const ae_int32x2    *)(y + 3+9*l);
    py2 = (const ae_int32x2    *)(y + 6+9*l);
    for (m=0; m<3;m++)
    {
      AE_L32X2_IP(vx0, px, 8);
      AE_L32X2_IP(vx1, px, 8);
      AE_L32X2_IP(vx2, px, 8);

      AE_L32X2_IP(vy0, py0, 8);
      AE_L32X2_IP(vy1, py1, 8);
      AE_L32X2_IP(vy2, py2, 8);

      A0 = AE_MULF32R_HH(vx0, vy0);
      AE_MULSF32R_LL(A0, vx0, vy0);
      AE_MULAF32R_HH(A0, vx1, vy1);
      AE_MULSF32R_LL(A0, vx1, vy1);
      AE_MULAF32R_HH(A0, vx2, vy2);
      AE_MULSF32R_LL(A0, vx2, vy2);

      A1 = AE_MULF32R_HL(vx0, vy0);
      AE_MULAF32R_LH(A1, vx0, vy0);
      AE_MULAF32R_HL(A1, vx1 , vy1);
      AE_MULAF32R_LH(A1, vx1, vy1);
      AE_MULAF32R_HL(A1, vx2, vy2);
      AE_MULAF32R_LH(A1, vx2, vy2);

      A0 = AE_SRAA64(A0, rsh);
      A1 = AE_SRAA64(A1, rsh);
      vz0 = AE_ROUND32X2F48SASYM(A0, A1);
      AE_S32X2_IP(vz0, pz, 8); 

      AE_L32X2_XP(vy0, py0, -8);
      AE_L32X2_XP(vy1, py1, -8);
      AE_L32X2_XP(vy2, py2, -8);

      A0 = AE_MULF32R_HH(vx0, vy0);
      AE_MULSF32R_LL(A0, vx0, vy0);
      AE_MULAF32R_HH(A0, vx1, vy1);
      AE_MULSF32R_LL(A0, vx1, vy1);
      AE_MULAF32R_HH(A0, vx2, vy2);
      AE_MULSF32R_LL(A0, vx2, vy2);

      A1 = AE_MULF32R_HL(vx0, vy0);
      AE_MULAF32R_LH(A1, vx0, vy0);
      AE_MULAF32R_HL(A1, vx1, vy1);
      AE_MULAF32R_LH(A1, vx1, vy1);
      AE_MULAF32R_HL(A1, vx2, vy2);
      AE_MULAF32R_LH(A1, vx2, vy2);

      A0 = AE_SRAA64(A0, rsh);
      A1 = AE_SRAA64(A1, rsh);
      vz0 = AE_ROUND32X2F48SASYM(A0, A1);
      AE_S32X2_IP(vz0, pz, 8*2);

    }
  }
  px = (const ae_int32x2      *)x;
  pz = (ae_int32x2      *)(z +2);
  for (l = 0; l<L; l++, y += 9)
  {
    py0 = (const ae_int32x2    *)(y + 2);
    py1 = (const ae_int32x2    *)(y + 5);
    py2 = (const ae_int32x2    *)(y + 8);
    for (m=0; m<3;m++)
    {
      AE_L32X2_IP(vx0, px, 8);
      AE_L32X2_IP(vx1, px, 8);
      AE_L32X2_IP(vx2, px, 8);

      AE_L32X2_IP(vy0, py0, 0);
      AE_L32X2_IP(vy1, py1, 0);
      AE_L32X2_IP(vy2, py2, 0);

      A0 = AE_MULF32R_HH(vx0, vy0);
      AE_MULSF32R_LL(A0, vx0, vy0);
      AE_MULAF32R_HH(A0, vx1, vy1);
      AE_MULSF32R_LL(A0, vx1, vy1);
      AE_MULAF32R_HH(A0, vx2, vy2);
      AE_MULSF32R_LL(A0, vx2, vy2);

      A1 = AE_MULF32R_HL(vx0, vy0);
      AE_MULAF32R_LH(A1, vx0, vy0);
      AE_MULAF32R_HL(A1, vx1 , vy1);
      AE_MULAF32R_LH(A1, vx1, vy1);
      AE_MULAF32R_HL(A1, vx2, vy2);
      AE_MULAF32R_LH(A1, vx2, vy2);

      A0 = AE_SRAA64(A0, rsh);
      A1 = AE_SRAA64(A1, rsh);
      vz0 = AE_ROUND32X2F48SASYM(A0, A1);
      AE_S32X2_IP(vz0, pz, 3*8);
    }
  }
} /* cmtx_mul3x3_32x32() */



