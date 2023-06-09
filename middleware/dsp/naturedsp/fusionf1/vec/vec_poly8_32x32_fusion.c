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
#include "common.h"
#include "NatureDSP_types.h"

/*-------------------------------------------------------------------------
  Polynomial approximation
  Functions calculate polynomial approximation for all values from given 
  vector. Fixed point functions take polynomial coefficients in Q15 or 
  Q31 precision. 
  NOTE:
  approximation is calculated like Taylor series that is why overflow may 
  potentially occur if cumulative sum of coefficients given from the last 
  to the first coefficient is bigger than 1. To avoid this negative effect,
  all the coefficients may be scaled down and result will be shifted left 
  after all intermediate computations.

  Precision: 
  16x16  16-bit inputs, 16-bit coefficients, 16-bit output. 
  24x24  24-bit inputs, 24-bit coefficients, 24-bit output. 
  32x32  32-bit inputs, 32-bit coefficients, 32-bit output.
  f      floating point

  Input:
  x[N]    input data, Q15, Q31 or floating point
  N       length of vector
  lsh     additional left shift for result
  c[M+1]  coefficients (5 coefficients for vec_poly4_xxx 
          and 9 coefficients for vec_poly8_xxx), Q15, Q31 
          or floating point
  Output:
  z[N]    result, Q15, Q31 or floating point

  Restriction:
  x,c,z should not overlap
  lsh   should be in range 0...31
-------------------------------------------------------------------------*/
void vec_poly8_32x32 (int32_t * restrict z, const int32_t * restrict x, const int32_t * restrict c, int lsh,int N )
#if 0
{
  int         n;
        const ae_int32x2 * restrict px = (const ae_int32x2 *)x;
        const ae_int32   * restrict pc = (const ae_int32   *)c;
              ae_int32x2 * restrict pz = (      ae_int32x2 *)z;
  ae_valign      x_align, z_align;

  ae_int32x2  vyw,vxw;
  ae_f32x2    vxf;
  ae_int32x2  c0w,c1w,c2w,c3w,c4w,c5w,c6w,c7w,c8w;
  ae_f32x2    t0f,t1f,t2f,t3f,t4f,t5f,t6f,t7f,t8f;

  NASSERT(x);
  NASSERT(c);
  NASSERT(z);
  if (N <= 0) return;

  x_align = AE_LA64_PP(px);
  z_align = AE_ZALIGN64();
  
  AE_L32_IP(c0w, pc, sizeof(*pc));
  AE_L32_IP(c1w, pc, sizeof(*pc));
  AE_L32_IP(c2w, pc, sizeof(*pc));
  AE_L32_IP(c3w, pc, sizeof(*pc));
  AE_L32_IP(c4w, pc, sizeof(*pc));
  AE_L32_IP(c5w, pc, sizeof(*pc));
  AE_L32_IP(c6w, pc, sizeof(*pc));
  AE_L32_IP(c7w, pc, sizeof(*pc));
  AE_L32_IP(c8w, pc, sizeof(*pc));

  t0f = (c0w);
  t1f = (c1w);
  t2f = (c2w);
  t3f = (c3w);
  t4f = (c4w);
  t5f = (c5w);
  t6f = (c6w);
  t7f = (c7w);
  t8f = (c8w);

  AE_LA32X2_IP(vxw, x_align, px); 

  for (n=0;n<N-1;n+=2)
  {
    vxf = (vxw); 
    AE_MULAFP32X2RAS(t7f, vxf, t8f);
    AE_MULAFP32X2RAS(t6f, vxf, t7f);
    AE_MULAFP32X2RAS(t5f, vxf, t6f);
    AE_MULAFP32X2RAS(t4f, vxf, t5f);
    AE_MULAFP32X2RAS(t3f, vxf, t4f);
    AE_MULAFP32X2RAS(t2f, vxf, t3f);
    AE_MULAFP32X2RAS(t1f, vxf, t2f);
    AE_MULAFP32X2RAS(t0f, vxf, t1f);
    vyw = (t0f);
    
    
    vyw = AE_SLAA32S(vyw,lsh);
    AE_SA32X2_IP(vyw, z_align, pz);//put answer
    AE_LA32X2_IP(vxw, x_align, px);

    t0f = (c0w);
    t1f = (c1w);
    t2f = (c2w);
    t3f = (c3w);
    t4f = (c4w);
    t5f = (c5w);
    t6f = (c6w);
    t7f = (c7w);

   
  }
  AE_SA64POS_FP(z_align, pz);
  if (N&1)
  {
    vxf = (vxw); 
    AE_MULAFP32X2RAS(t7f, vxf, t8f);
    AE_MULAFP32X2RAS(t6f, vxf, t7f);
    AE_MULAFP32X2RAS(t5f, vxf, t6f);
    AE_MULAFP32X2RAS(t4f, vxf, t5f);
    AE_MULAFP32X2RAS(t3f, vxf, t4f);
    AE_MULAFP32X2RAS(t2f, vxf, t3f);
    AE_MULAFP32X2RAS(t1f, vxf, t2f);
    AE_MULAFP32X2RAS(t0f, vxf, t1f);
    vyw = (t0f);    
    
    vyw = AE_SLAA32S(vyw,lsh);
    vyw = AE_SEL32_HH(vyw,vyw);
    AE_S32_L_I(vyw, (ae_int32 *)pz, 0);//put answer
  }
}
#else
{
    int         n;
    const ae_int32x2 * restrict px   = (const ae_int32x2 *)x;
    const ae_int32   * restrict pc   = (const ae_int32   *)c;
          ae_f32x2   * restrict pz   = (      ae_f32x2   *)z;

    ae_valign   x_align, z_align;
    ae_int32x2   c0f, c1f, c2f, c3f, c4f, c5f, c6f, c7f, c8f, s, vxf;
    ae_f32x2 t;

    NASSERT(x);
    NASSERT(c);
    NASSERT(z);
    NASSERT(lsh>=0 && lsh<=31);

    if (N <= 0) return;
  
    x_align = AE_LA64_PP(px);
    z_align = AE_ZALIGN64();
    c8f=AE_L32_X (pc, 8*sizeof(*pc));
    WUR_AE_SAR(lsh);
    for (n=0; n<(N&~1); n+=2)
    { 
        AE_LA32X2_IP(vxf, x_align, px); 
        c7f=AE_L32_I (pc, 7*sizeof(*pc)); t=c7f; AE_MULAFP32X2RAS(t, vxf, c8f); s=(t);
        c6f=AE_L32_I (pc, 6*sizeof(*pc)); t=c6f; AE_MULAFP32X2RAS(t, vxf, s  ); s=(t);
        c5f=AE_L32_I (pc, 5*sizeof(*pc)); t=c5f; AE_MULAFP32X2RAS(t, vxf, s  ); s=(t);
        c4f=AE_L32_I (pc, 4*sizeof(*pc)); t=c4f; AE_MULAFP32X2RAS(t, vxf, s  ); s=(t);
        c3f=AE_L32_I (pc, 3*sizeof(*pc)); t=c3f; AE_MULAFP32X2RAS(t, vxf, s  ); s=(t);
        c2f=AE_L32_I (pc, 2*sizeof(*pc)); t=c2f; AE_MULAFP32X2RAS(t, vxf, s  ); s=(t);
        c1f=AE_L32_I (pc, 1*sizeof(*pc)); t=c1f; AE_MULAFP32X2RAS(t, vxf, s  ); s=(t);
        AE_L32_XP(c0f,pc, 0*sizeof(*pc)); t=c0f; AE_MULAFP32X2RAS(t, vxf, s  ); 
        t=AE_SLAS32S(t);
        s=(t);
        AE_SA32X2_IP(s, z_align, pz);//put answer
    }
    AE_SA64POS_FP(z_align, pz);
    if (N&1)
    { 
        AE_LA32X2_IP(vxf, x_align, px); 
        c7f=AE_L32_I (pc, 7*sizeof(*pc)); t=c7f; AE_MULAFP32X2RAS(t, vxf, c8f); s=(t);
        c6f=AE_L32_I (pc, 6*sizeof(*pc)); t=c6f; AE_MULAFP32X2RAS(t, vxf, s  ); s=(t);
        c5f=AE_L32_I (pc, 5*sizeof(*pc)); t=c5f; AE_MULAFP32X2RAS(t, vxf, s  ); s=(t);
        c4f=AE_L32_I (pc, 4*sizeof(*pc)); t=c4f; AE_MULAFP32X2RAS(t, vxf, s  ); s=(t);
        c3f=AE_L32_I (pc, 3*sizeof(*pc)); t=c3f; AE_MULAFP32X2RAS(t, vxf, s  ); s=(t);
        c2f=AE_L32_I (pc, 2*sizeof(*pc)); t=c2f; AE_MULAFP32X2RAS(t, vxf, s  ); s=(t);
        c1f=AE_L32_I (pc, 1*sizeof(*pc)); t=c1f; AE_MULAFP32X2RAS(t, vxf, s  ); s=(t);
        AE_L32_XP(c0f,pc, 0*sizeof(*pc)); t=c0f; AE_MULAFP32X2RAS(t, vxf, s  ); 
        t=AE_SLAS32S(t);
        s=(t);
        s=AE_SEL32_HH(s,s);
        AE_S32_L_I(s, (ae_int32 *)pz, 0);//put answer
    }
}
#endif
