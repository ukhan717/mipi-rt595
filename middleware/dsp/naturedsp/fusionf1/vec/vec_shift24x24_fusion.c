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
#include "NatureDSP_Signal.h"
#include "common.h"


/*===========================================================================
  Vector matematics:
  vec_shift,vec_scale  Vector Scaling with Saturation
===========================================================================*/

/*-------------------------------------------------------------------------
  Vector Scaling with Saturation
  These routines make shift with saturation of data values in the vector 
  by given scale factor (degree of 2). 24-bit routine works with f24 data 
  type and faster while 32-bit version keep all 32-bits and slower.
  Functions vec_scale() make multiplication of Q31 vector to Q31 coefficient 
  which is not a power of 2 forming Q31 result.
  Two versions of routines are available: regular versions (vec_shift24x24, 
  vec_shift32x32, vec_shift16x16, vec_shiftf, vec_scale32x24, vec_scale24x24, 
  vec_scale16x16, vec_scalef, vec_scale_sf) work with arbitrary arguments, faster 
  versions (vec_shift24x24_fast, vec_shift32x32_fast, vec_shift16x16_fast, 
  vec_scale32x24_fast, vec_scale24x24_fast, vec_scale16x16_fast) apply some 
  restrictions
For floating point:
  Fuction vec_shiftf() makes scaling without saturation of data values by given 
  scale factor (degree of 2). 
  Functions vec_scalef() and vec_scale_sf() make multiplication of input vector
  to coefficient which is not a power of 2.
  Two versions of routines are available: 
    without saturation - vec_scalef;
    with saturation - vec_scale_sf; 

Precision:
24x24 24-bit input, 32-bit output
32x32 32-bit input, 32-bit output
16x16 16-bit input, 16-bit output
f     single precision floating point

  Input:
  x[N]  input data , Q31,Q15 or floating point
  t     shift count. If positive, it shifts left with saturation, if
  negative it shifts right
  s     scale factor, Q31,Q15 or floating point
  N     length of vector
  fmin  minimum output value (only for vec_scale_sf)
  fmax  maximum output value (only for vec_scale_sf)

  Output:
  y[N]  output data, Q31 or Q15

  Restrictions:
  For regular versions (vec_shift24x24, vec_shift32x32, vec_shift16x16, 
  vec_shiftf, vec_scale32x24, vec_scale24x24, vec_scale16x16, vec_scalef, 
  vec_scale_sf):
  x,y should not overlap
  t   should be in range -31...31 for fixed-point functions and -129...146 
      for floating point

  For vec_scale_sf - fmin<=fmax;

  For faster versions (vec_shift24x24_fast, vec_shift32x32_fast, 
  vec_shift16x16_fast, vec_scale32x24_fast, vec_scale24x24_fast, 
  vec_scale16x16_fast):
  x,y should not overlap
  t should be in range -31...31 
  x,y - aligned on 8-byte boundary
  N   - multiple of 4 
-------------------------------------------------------------------------*/
void vec_shift24x24
(
  f24 * restrict y,
  const f24 * restrict x,
  int t,
  int N
)
{
  int n;
  ae_f24x2    vxf;
  ae_int32x2  vyw;
  const ae_f24x2   * restrict px = (const ae_f24x2   *)x;
        ae_int32x2 * restrict py = (      ae_int32x2 *)y;
  ae_valign x_align, y_align;

  NASSERT(x);
  NASSERT(y);
  NASSERT(t >= -31 && t <= 31);
  if (N <= 0) return;

  x_align = AE_LA64_PP(px);
  y_align = AE_ZALIGN64();

  t += 8;

  if (t>31) t=31; else if (t<-31) t=-31;
  n = RUR_AE_SAR();
  WUR_AE_SAR(t);
  t = n;

  for (n=0; n<(N>>1); n++)
  {
    AE_LA32X2F24_IP(vxf, x_align, px);
    vyw = AE_SLAS32S(vxf); 
    AE_SA32X2_IP(vyw, y_align, py);
  }

  AE_SA64POS_FP(y_align, py);

  if (N&1)
  {
    AE_L32F24_IP(vxf, castxcc(ae_f24, px), 0);
    vyw = AE_SLAS32S(vxf);
    vyw = AE_SEL32_HH(vyw, vyw);
    AE_S32_L_I(vyw, (ae_int32 *)py, 0);
  }
  WUR_AE_SAR(t);
}
