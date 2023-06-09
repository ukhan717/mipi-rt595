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
  NatureDSP Signal Processing Library. IIR part
    Bi-quad Real Block IIR, floating point, Direct Form II
*/

/*-------------------------------------------------------------------------
  Bi-quad Block IIR
  Computes a IIR filter (cascaded IIR direct form I or II using 5 
  coefficients per bi-quad + gain term) . Input data are stored in vector x. 
  Filter output samples are stored in vector r. The filter calculates N output 
  samples using SOS and G matrices.
  Filters are able to process data in following formats:
  -  real (just array of samples)
  -  2-way or complex (interleaved real/imaginary samples)
  -  3-way (stream of interleaved samples from 3 channels)
  The same coefficients are used for filtering of multiple channels or 
  real/imaginary parts and they are processed independently. 
  The same format have to be used both for input and output streams.
  NOTES:
  1. Bi-quad coefficients may be derived from standard SOS and G matrices
     generated by MATLAB. However, typically biquad stages have big peaks 
     in their step response which may cause undesirable overflows at the 
     intermediate outputs. To avoid that the additional scale factors 
     coef_g[M] may be applied. These per-section scale factors may require 
     some tuning to find a compromise between quantization noise and possible
     overflows. Output of the last section is directed to an additional 
     multiplier, with the gain factor being a power of two, either negative 
     or non-negative. It is specified through the total gain shift amount 
     parameter gain of each filter initialization function.
  2. 16x16 filters may suffer more from accumulation of the roundoff errors, 
     so filters should be properly designed to match noise requirements

  Precision: 
  16x16  16-bit data, 16-bit coefficients, 16-bit intermediate stage outputs 
         (DF I, DF II), real data
  16x16  16-bit data, 16-bit coefficients, 16-bit intermediate stage outputs 
         (DF I, DF II), 3-way data
  24x24  32-bit data, 24-bit coefficients, 32-bit intermediate stage outputs 
         (DF I, DF II), real data
  32x16  32-bit data, 16-bit coefficients, 32-bit intermediate stage outputs 
         (DF I, DF II), real data
  32x16  32-bit data, 16-bit coefficients, 32-bit intermediate stage outputs 
         (DF I, DF II), 3-way data
  32x32  32-bit data, 32-bit coefficients, 32-bit intermediate stage outputs 
         (DF I, DF II)
  32x32  32-bit data, 32-bit coefficients, 32-bit intermediate stage outputs 
         (DF I, DF II) 3-way data
  f      floating point (DF I, DF II and DF IIt)
  f      floating point (DF I), 2-way (complex) data
  f      floating point (DF I, DF II) 3-way data

   ----------------+--------------------------------
   Functon         | Scratch memory, bytes
   ----------------+--------------------------------
   bqriir16x16_df1 | BQRIIR16X16_DF1_SCRATCH_SIZE(M)
   bqriir16x16_df2 | BQRIIR16X16_DF2_SCRATCH_SIZE(M)
   bq3iir16x16_df1 | BQ3IIR16X16_DF1_SCRATCH_SIZE(M)
   bq3iir16x16_df2 | BQ3IIR16X16_DF2_SCRATCH_SIZE(M)
   bqriir24x24_df1 | BQRIIR24X24_DF1_SCRATCH_SIZE(M)
   bqriir24x24_df2 | BQRIIR24X24_DF2_SCRATCH_SIZE(M)
   bqriir32x16_df1 | BQRIIR32X16_DF1_SCRATCH_SIZE(M)
   bqriir32x16_df2 | BQRIIR32X16_DF2_SCRATCH_SIZE(M)
   bq3iir32x16_df1 | BQ3IIR32X16_DF1_SCRATCH_SIZE(M)
   bq3iir32x16_df2 | BQ3IIR32X16_DF2_SCRATCH_SIZE(M)
   bqriir32x32_df1 | BQRIIR32X32_DF1_SCRATCH_SIZE(M)
   bqriir32x32_df2 | BQRIIR32X32_DF2_SCRATCH_SIZE(M)
   bq3iir32x32_df1 | BQ3IIR32X32_DF1_SCRATCH_SIZE(M)
   bq3iir32x32_df2 | BQ3IIR32X32_DF2_SCRATCH_SIZE(M)
   ----------------+--------------------------------
  Input:
  N             length of input sample block. For 3-way functions (bq3iirxxx), 
                N is a number of triplets
  M             number of bi-quad sections
  s[]           scratch memory area (for fixed-point functions only), Minimum 
                number of bytes depends on selected filter structure and 
                precision. see table above .If a particular macro returns zero, 
                then the corresponding IIR doesn't require a scratch area and 
                parameter s may hold zero

  coef_sos[M*5] filter coefficients stored in blocks of 5 numbers: 
                b0 b1 b2 a1 a2. 
                For fixed-point funcions, fixed point format of filter 
                coefficients is Q1.14 for 32x16, or Q1.30 for 32x16 and 
                24x24 (in the latter case 8 LSBs are actually ignored). 
  coef_g[M]     scale factor for each section, Q15 (for fixed-point 
                functions only). Please note that 24x24 DFI implementation 
                internally truncates scale factors to Q7 values.
  gain          total gain shift amount applied to output signal of the
                last section, -48..15
  x[N]          input samples, Q31, Q15 or floating point. For 3-way functions 
                (bq3iirxxx), N is a number of triplets, so array size should be 
                3*N.

  Output:
  r[N]          output data, Q31, Q15 or floating point. For 3-way functions 
                (bq3iirxxx), N is a number of triplets, so array size should be 
                3*N.

  Restriction:
  x,r,s,coef_g,coef_sos should not overlap
  N   - must be a multiple of 2
  s[] - whenever supplied must be aligned on an 8-bytes boundary

-------------------------------------------------------------------------*/

/* Portable data types. */
#include "NatureDSP_types.h"
/* Signal Processing Library API. */
#include "NatureDSP_Signal.h"
/* Common utility and macros declarations. */
#include "common.h"
#include "bqriirf_df2.h"

#if !(XCHAL_HAVE_FUSION_FP)
DISCARD_FUN(void, bqriirf_df2, ( bqriirf_df2_handle_t _bqriir,
                      float32_t * restrict       z,
                const float32_t *            x, int N))
#else
// Process data. The filter instance is identified with a handle returned by
// the initializing function.
void bqriirf_df2( bqriirf_df2_handle_t _bqriir,
                      float32_t * restrict       z,
                const float32_t *            x, int N)
{
    bqriirf_df2_t *state;
    const xtfloat    * restrict pX=(const xtfloat    *)x;
          xtfloat    * restrict pZ;
    const xtfloat  *pa1,*pa2,*pb0,*pb1,*pb2;
    const xtfloat* restrict pDrd;
          xtfloat* restrict pDwr;
    int n,m;
    int M;
    NASSERT(_bqriir);
    state=(bqriirf_df2_t*)(_bqriir);
//    if(N<=0) return;
    if (N<2) return;
    NASSERT(N>0);
    NASSERT(N%2==0);
    NASSERT(x);
    NASSERT(z);
    NASSERT(state);
    NASSERT(state->st);
    NASSERT(state->cf);
    M=state->M;
    NASSERT(M>0 && N>0);
    pb0 =( const xtfloat*)(state->cf+0*M);
    pb1 =( const xtfloat*)(state->cf+1*M);
    pb2 =( const xtfloat*)(state->cf+2*M);
    pa1 =( const xtfloat*)(state->cf+3*M);
    pa2 =( const xtfloat*)(state->cf+4*M);
    pDrd=( const xtfloat*)state->st;
    pDwr=(       xtfloat*)state->st;
    for(m=0;m<M;m++)
    {
        xtfloat dx0,dy0,dy1,dy2;
        xtfloat a1,a2,b0,b1,b2;
        pZ=(xtfloat    *)z;
        /* load delay lines and coefficients */
        XT_LSIP(dx0,pDrd,sizeof(float32_t)); 
        XT_LSIP(dy0,pDrd,sizeof(float32_t)); 
        XT_LSIP(dy1,pDrd,sizeof(float32_t)); 
        XT_LSIP(b0,pb0,sizeof(float32_t));
        XT_LSIP(b1,pb1,sizeof(float32_t));
        XT_LSIP(b2,pb2,sizeof(float32_t));
        XT_LSIP(a1,pa1,sizeof(float32_t));
        XT_LSIP(a2,pa2,sizeof(float32_t));
        /* processing loop  */
        for (n=0;n<(N>>1);n++)
        {
            XT_LSIP(dx0,pX,sizeof(x[0]));
            dy2=dy1;
            dy1=dy0;
            dy0=dx0;
            XT_MSUB_S(dy0,a2,dy2);
            XT_MSUB_S(dy0,a1,dy1);
            dx0=XT_MUL_S (b2,dy2);
            XT_MADD_S(dx0,b1,dy1);
            XT_MADD_S(dx0,b0,dy0);
            XT_SSIP(dx0,pZ,sizeof(x[0]));

            XT_LSIP(dx0,pX,sizeof(x[0]));
            dy2=dy1;
            dy1=dy0;
            dy0=dx0;
            XT_MSUB_S(dy0,a2,dy2);
            XT_MSUB_S(dy0,a1,dy1);
            dx0=XT_MUL_S (b2,dy2);
            XT_MADD_S(dx0,b1,dy1);
            XT_MADD_S(dx0,b0,dy0);
            XT_SSIP(dx0,pZ,sizeof(x[0]));
        }
        XT_SSIP(dx0,pDwr,sizeof(float32_t));
        XT_SSIP(dy0,pDwr,sizeof(float32_t));
        XT_SSIP(dy1,pDwr,sizeof(float32_t));
        pX=(const xtfloat*)z;    /* switch pointer to the input data to the pointer to the output */
    }
    // final scaling
    {
        xtfloat ft;
        int32_t s;
        s=state->gain;
        s=((s+127)&255)<<23;
        ft=XT_WFR(s);
        for (n=0;n<N;n++) z[n]=z[n]*(float32_t)ft;
    }
}
#endif
