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
#if !(XCHAL_HAVE_FUSION_FP)
DISCARD_FUN(void, cmtx_mul3x3f, (complex_float   * restrict z, const complex_float   *restrict x, const complex_float   *restrict y, int L))
#else
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
void cmtx_mul3x3f(complex_float   * restrict z, const complex_float   *restrict x, const complex_float   *restrict y, int L)
{
  const xtfloatx2    *  restrict py0 = (const xtfloatx2    *)y;
  const xtfloatx2    *  restrict py1 = (const xtfloatx2    *)y;
  const xtfloatx2    *  restrict py2 = (const xtfloatx2    *)y;
        float32_t    *  pz = (float32_t      *)z;
  const xtfloatx2    *  restrict px = (const xtfloatx2      *)x;
  int l, m;
  xtfloatx2 vx0, vy0, vx1, vy1;
  xtfloatx2 vx2, vy2;


  xtfloat A0, A1, A2, A3;
  NASSERT_ALIGN(x, sizeof(*x));
  NASSERT_ALIGN(y, sizeof(*y));
  NASSERT_ALIGN(z, sizeof(*z));

  for (l = 0; l<L; l++)
  {
    py0 = (const xtfloatx2    *)(y + 0 + 9 * l);
    py1 = (const xtfloatx2    *)(y + 3 + 9 * l);
    py2 = (const xtfloatx2    *)(y + 6 + 9 * l);

    for (m = 0; m<3; m++)
    {

      XT_LSX2IP(vx0, px, 8);
      XT_LSX2IP(vx1, px, 8);
      XT_LSX2IP(vx2, px, 8);

      XT_LSX2IP(vy0, py0, 8);
      XT_LSX2IP(vy1, py1, 8);
      XT_LSX2IP(vy2, py2, 8);

      A0 = XT_MUL_LHH_S(vx0, vy0);
      XT_MSUB_LLL_S(A0, vx0, vy0);
      XT_MADD_LHH_S(A0, vx1, vy1);
      XT_MSUB_LLL_S(A0, vx1, vy1);
      XT_MADD_LHH_S(A0, vx2, vy2);
      XT_MSUB_LLL_S(A0, vx2, vy2); 
      A1 = XT_MUL_LHL_S(vx0, vy0);
      XT_MADD_LLH_S(A1, vx0, vy0);
      XT_MADD_LHL_S(A1, vx1, vy1);
      XT_MADD_LLH_S(A1, vx1, vy1);
      XT_MADD_LHL_S(A1, vx2, vy2);
      XT_MADD_LLH_S(A1, vx2, vy2);
   
      XT_SSIP(A0, pz, 4);
      XT_SSIP(A1, pz, 4);

      XT_LSX2XP(vy0, py0, -8);
      XT_LSX2XP(vy1, py1, -8);
      XT_LSX2XP(vy2, py2, -8);

      A0 = XT_MUL_LHH_S(vx0, vy0);
      XT_MSUB_LLL_S(A0, vx0, vy0);
      XT_MADD_LHH_S(A0, vx1, vy1);
      XT_MSUB_LLL_S(A0, vx1, vy1);
      XT_MADD_LHH_S(A0, vx2, vy2);
      XT_MSUB_LLL_S(A0, vx2, vy2); 
      A1 = XT_MUL_LHL_S(vx0, vy0);
      XT_MADD_LLH_S(A1, vx0, vy0);
      XT_MADD_LHL_S(A1, vx1, vy1);
      XT_MADD_LLH_S(A1, vx1, vy1);
      XT_MADD_LHL_S(A1, vx2, vy2);
      XT_MADD_LLH_S(A1, vx2, vy2);

      XT_SSIP(A0, pz, 4);
      XT_SSIP(A1, pz, 8*2-4);
    }
  }
  __Pragma("no_reorder");
  px = (const xtfloatx2      *)x;
  pz = (float32_t      *)(z + 2);
  py0 = (const xtfloatx2    *)(y + 2);
  py1 = (const xtfloatx2    *)(y + 5);
  py2 = (const xtfloatx2    *)(y + 8);
  for (l = 0; l<L; l++)
  {
    XT_LSX2IP(vx0, px, 8);
    XT_LSX2IP(vx1, px, 8);
    XT_LSX2IP(vx2, px, 8);

    XT_LSX2XP(vy0, py0, 8*9);
    XT_LSX2XP(vy1, py1, 8*9);
    XT_LSX2XP(vy2, py2, 8*9);

    A0 = XT_MUL_LHH_S(vx0, vy0);
    A1 = XT_MUL_LHL_S(vx0, vy0);     
    XT_MSUB_LLL_S(A0, vx0, vy0);
    XT_MADD_LHH_S(A0, vx1, vy1);
    XT_MSUB_LLL_S(A0, vx1, vy1);
    XT_MADD_LHH_S(A0, vx2, vy2);
    XT_MSUB_LLL_S(A0, vx2, vy2); 
 
    XT_MADD_LLH_S(A1, vx0, vy0);
    XT_MADD_LHL_S(A1, vx1, vy1);
    XT_MADD_LLH_S(A1, vx1, vy1);
    XT_MADD_LHL_S(A1, vx2, vy2);
    XT_MADD_LLH_S(A1, vx2, vy2);

    XT_SSIP(A0, pz, 4);
    XT_SSIP(A1, pz, 3*8-4);

    XT_LSX2IP(vx0, px, 8);
    XT_LSX2IP(vx1, px, 8);
    XT_LSX2IP(vx2, px, 8*4);

    A2 = XT_MUL_LHH_S(vx0, vy0);
    A3 = XT_MUL_LHL_S(vx0, vy0);
    XT_MSUB_LLL_S(A2, vx0, vy0);
    XT_MADD_LHH_S(A2, vx1, vy1);
    XT_MSUB_LLL_S(A2, vx1, vy1);
    XT_MADD_LHH_S(A2, vx2, vy2);
    XT_MSUB_LLL_S(A2, vx2, vy2);

    XT_MADD_LLH_S(A3, vx0, vy0);
    XT_MADD_LHL_S(A3, vx1, vy1);
    XT_MADD_LLH_S(A3, vx1, vy1);
    XT_MADD_LHL_S(A3, vx2, vy2);
    XT_MADD_LLH_S(A3, vx2, vy2);

    XT_SSIP(A2, pz, 4);
    XT_SSXP(A3, pz, 6* 8 - 4);
  }
  __Pragma("no_reorder");
  px = (const xtfloatx2      *)(x+6);
  pz = (float32_t      *)(z + 8);
  py0 = (const xtfloatx2    *)(y + 2);
  py1 = (const xtfloatx2    *)(y + 5);
  py2 = (const xtfloatx2    *)(y + 8);
  for (l = 0; l<L; l++)
  {
    XT_LSX2IP(vx0, px, 8);
    XT_LSX2IP(vx1, px, 8);
    XT_LSX2IP(vx2, px, 7*8);

    XT_LSX2XP(vy0, py0, 8 * 9);
    XT_LSX2XP(vy1, py1, 8 * 9);
    XT_LSX2XP(vy2, py2, 8 * 9);

    A0 = XT_MUL_LHH_S(vx0, vy0);
    A1 = XT_MUL_LHL_S(vx0, vy0);
    A2 = XT_MUL_LLL_S(vx0, vy0);
    A3 = XT_MUL_LLH_S(vx0, vy0);
    // XT_MSUB_LLL_S(A0, vx0, vy0);
    XT_MADD_LHH_S(A0, vx1, vy1);
    XT_MADD_LLL_S(A2, vx1, vy1);
    XT_MADD_LHH_S(A0, vx2, vy2);
    XT_MADD_LLL_S(A2, vx2, vy2);

    XT_MADD_LHL_S(A1, vx1, vy1);
    XT_MADD_LLH_S(A3, vx1, vy1);
    XT_MADD_LHL_S(A1, vx2, vy2);
    XT_MADD_LLH_S(A3, vx2, vy2);

    A0 = XT_SUB_S(A0,A2);
    A1 = XT_ADD_S(A1,A3);
    XT_SSIP(A0, pz, 4);
    XT_SSXP(A1, pz, 9 * 8 - 4);
  }
} /* cmtx_mul3x3f() */

#endif


