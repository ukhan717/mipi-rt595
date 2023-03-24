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
    Bi-quad Real Block IIR, 16x16-bit, Direct Form II
    Optimized code for Fusion
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

/* Instance pointer validation number. */
#define MAGIC     0x830B87CF

/* Reserve memory for alignment. */
#define ALIGNED_SIZE( size, align ) \
      ( (size_t)(size) + (align) - 1 )

/* Align address on a specified boundary. */
#define ALIGNED_ADDR( addr, align ) \
      (void*)( ( (uintptr_t)(addr) + ( (align) - 1 ) ) & ~( (align) - 1 ) )

#define sz_i16 sizeof(int16_t)

/* Filter instance structure. */
typedef struct tag_bqriir16x16_df2_t
{
  uint32_t        magic;  // Instance pointer validation number
  int             M;      // Number of sections
  int16_t         gain;   // Gain shift amount applied after the last section
  const int16_t * coef;   // Num/den coefs (Q14) and gain (Q15) for each biquad
  int16_t *       state;  // 2 state elements per section, Q15

} bqriir16x16_df2_t, *bqriir16x16_df2_ptr_t;

// Determine the memory area size for a filter instance.
size_t bqriir16x16_df2_alloc( int M )
{
  ASSERT( M >= 0 );

  return ( ALIGNED_SIZE( sizeof(bqriir16x16_df2_t), 4 )
           + // 2 state elements for each of M DFII sections.
           ALIGNED_SIZE( 2*M*sz_i16,   sz_i16 )
           + // 6+2 coefficients for each of M sections
           ALIGNED_SIZE( 8*M*sz_i16, 4*sz_i16 ) );

} // bqriir16x16_df2_alloc()

// Given a memory area of sufficient size, initialize a filter instance and 
// return a handle to it.
bqriir16x16_df2_handle_t bqriir16x16_df2_init( void * objmem, int M,
                                               const int16_t * coef_sos,
                                               const int16_t * coef_g,
                                               int16_t         gain )
{
  bqriir16x16_df2_ptr_t bqriir;

  int16_t * sectState;
  int16_t * sectCoef;
  void *    ptr;

  int m;

  ASSERT( objmem && M >= 0 && coef_sos && coef_g );

  ASSERT( -48 <= gain && gain <= 15 );

  //
  // Partition the memory block
  //

  ptr = objmem;

  bqriir    = (bqriir16x16_df2_ptr_t)ALIGNED_ADDR( ptr, 4 );
  ptr       = bqriir + 1;
  sectState = (int16_t *)ALIGNED_ADDR( ptr, sz_i16 );
  ptr       = sectState + 2*M;
  sectCoef  = (int16_t *)ALIGNED_ADDR( ptr, 4*sz_i16 );
  ptr       = sectCoef + 8*M;

  ASSERT( (int8_t*)ptr - (int8_t*)objmem <= (int)bqriir16x16_df2_alloc(M ) );

  //
  // Copy filter coefficients for M sections, zero the state elements.
  //
  
#if (XCHAL_HAVE_FUSION_16BIT_BASEBAND)
  for ( m=0; m<M; m++ )
  {
    sectCoef[8*m+0] = coef_sos[5*m+0];                  //  b0, Q14
    sectCoef[8*m+1] = coef_sos[5*m+1];                  //  b1, Q14
    sectCoef[8*m+2] = coef_sos[5*m+2];                  //  b2, Q14
    sectCoef[8*m+3] = 0;
    sectCoef[8*m+4] = AE_NEG16S_scalar(coef_sos[5*m+3]);// -a1, Q14
    sectCoef[8*m+5] = AE_NEG16S_scalar(coef_sos[5*m+4]);// -a2, Q14
    sectCoef[8*m+6] = 0;
    sectCoef[8*m+7] = coef_g[m]>>1;                     //   g, Q14

    sectState[2*m+0] = 0;
    sectState[2*m+1] = 0;
  }
#else
  for ( m=0; m<M; m++ )
  {
    sectCoef[8*m+0] = coef_g[m]>>1;    // g  [Q14]
    sectCoef[8*m+1] = coef_g[m]>>1;    // g  [Q14]
    sectCoef[8*m+2] = coef_sos[5*m+0]; // b0 [Q14]
    sectCoef[8*m+3] = coef_sos[5*m+0]; // b0 [Q14]
    sectCoef[8*m+4] = coef_sos[5*m+1]; // b1 [Q14]
    sectCoef[8*m+5] = coef_sos[5*m+2]; // b2 [Q14]
    sectCoef[8*m+6] = coef_sos[5*m+3]; // a1 [Q14]
    sectCoef[8*m+7] = coef_sos[5*m+4]; // a2 [Q14]
  }
  for ( m=0; m<2*M; m++ )
  {
    sectState[m] = 0;
  }
#endif

  //
  // Initialize the filter instance.
  //

  bqriir->magic = MAGIC;
  bqriir->M     = M;
  bqriir->gain  = gain;
  bqriir->coef  = sectCoef;
  bqriir->state = sectState;

  return (bqriir);

} // bqriir16x16_df2_init()

#if (XCHAL_HAVE_FUSION_16BIT_BASEBAND)
// Process data. The filter instance is identified with a handle returned by
// the initializing function.
void bqriir16x16_df2( bqriir16x16_df2_handle_t _bqriir,
                      void    * restrict       s,
                      int16_t * restrict       r,
                const int16_t *                x, int N )
{
  bqriir16x16_df2_ptr_t bqriir = (bqriir16x16_df2_ptr_t)_bqriir;
  
  const ae_int16x4 *  X;
        ae_int16x4 *  R;
        ae_int16x4 *restrict  St;
  const ae_int16x4 *restrict  Cf;

  ae_int16x4 inx;
  ae_int16x4 outy;
  ae_int16x4 d0, d1, d2, d1d2;
  ae_int16x4 b0b1b2_0, a1a2_0g;
  ae_int32x2 td, ty;
  ae_int64   accy;
  ae_f32x2   accd;
  int M, gain;
  int m, n;

  ASSERT( bqriir && bqriir->magic == MAGIC && r && x );
  NASSERT(N%2==0);
  if(N<=0) return;
  M = bqriir->M;

  St = (ae_int16x4 *)bqriir->state;
  Cf = (ae_int16x4 *)bqriir->coef;
  X = (const ae_int16x4 *)x;
  gain = bqriir->gain;

  // Perform data block processing for each section. Use the output array r[N]
  // for temporal storage of inter-section signal.
  for ( m=0; m<M-1; m++ )
  {
    //
    // Load 16-bit section coefficients.
    //
    ae_int16x4_loadip(b0b1b2_0, Cf, 4*sz_i16);
    ae_int16x4_loadip(a1a2_0g, Cf, 4*sz_i16);

    //
    // Load 16-bit section's state elements.
    //
    d2 = AE_L16_I((ae_int16 *)St, 0);
    d1 = AE_L16_I((ae_int16 *)St, sz_i16);
    d1d2 = AE_SEL16_4321(d1, d2);

    R = (ae_int16x4 *)r;
    
    //
    // Pass the block of input samples through the section. n-th sample at the
    // output of a biquad section r[n] and section state elements (d1,d2) update
    // are given by the following formulae:
    //   d0   = g*x[n] - a1*d1 - a2*d2
    //   r[n] = b0*d0 + b1*d1 + b2*d2;
    //   d2 = d1; d1 = d0;
    //
    
    // Load input sample.
    AE_L16_IP(inx, castxcc(const ae_int16, X), sz_i16);
    // d0 = g*x[n] - a1*d1 - a2*d2
    // Q30 <- Q15*Q14 + 1
    accd = AE_MULF16SS_00(inx, a1a2_0g);
    AE_MULAAFD16SS_33_22(accd, d1d2, a1a2_0g);
    // Q15 <- Q30 + 1 - 16 w/ asym. rounding and saturation
    td = AE_SLAI32S(accd, 1);
    d0 = AE_ROUND16X4F32SASYM(td, td);
    // Update the section's state elements.
    d1d2 = AE_SEL16_4321(d0, d1d2);
      
    // Load next input sample.
    AE_L16_IP(inx, castxcc(const ae_int16, X), sz_i16);
    // d0 = g*x[n+1] - a1*d1 - a2*d2
    // Q30 <- Q15*Q14 + 1
    accd = AE_MULF16SS_00(inx, a1a2_0g);
    AE_MULAAFD16SS_33_22(accd, d1d2, a1a2_0g);
    // Q15 <- Q30 + 1 - 16 w/ asym. rounding and saturation
    td = AE_SLAI32S(accd, 1);
    d0 = AE_ROUND16X4F32SASYM(td, td);
    
    // Processing loop;
    // loop is rotated for better scheduling
    for ( n=0; n<N-2; n++ )
    {
      // Calculate the output sample.
      // y[n] = b0*d0 + b1*d1 + b2*d2;
      // Q29 <- Q15*Q14
      accy = AE_MULZAAAAQ16(d1d2, b0b1b2_0);
      // Saturate the result.
      // Q31 <- Q29 + 2
      ty = AE_MOVINT32X2_FROMF64(accy);
      ty = AE_SLAI32S(ty, 2);
      
      // Update the section's state elements.
      d1d2 = AE_SEL16_4321(d0, d1d2);

      // Load input sample.
      AE_L16_IP(inx, castxcc(const ae_int16, X), sz_i16);
      // d0 = g*x[n+2] - a1*d1 - a2*d2
      // Q30 <- Q15*Q14 + 1
      accd = AE_MULF16SS_00(inx, a1a2_0g);
      AE_MULAAFD16SS_33_22(accd, d1d2, a1a2_0g);
      // Saturate the result.
      // Q31 <- Q30 + 1
      td = AE_SLAI32S(accd, 1);
      // Apply the rounding to both delay and output samples.
      // Q15 <- Q31 - 16 w/ asym. rounding
      d0 = AE_ROUND16X4F32SASYM(ty, td);
      outy = AE_MOVINT16X4_FROMINT32X2(AE_INTSWAP(AE_MOVINT32X2_FROMINT16X4(d0)));
      // Store output sample.
      // Q15
      AE_S16_0_IP(outy, castxcc(ae_int16, R), sz_i16);
    }
    // y[n+1] = b0*d0 + b1*d1 + b2*d2;
    // Q29 <- Q15*Q14
    accy = AE_MULZAAAAQ16(d1d2, b0b1b2_0);
    // Q15 <- Q29 + 2 - 16 w/ asym. rounding, saturation
    ty = AE_MOVINT32X2_FROMF64(accy);
    ty = AE_SLAI32S(ty, 2);
    outy = AE_ROUND16X4F32SASYM(ty, ty);
    // Store output sample.
    // Q15
    AE_S16_0_IP(outy, castxcc(ae_int16, R), sz_i16);
    // Update the section's state elements.
    d1d2 = AE_SEL16_4321(d0, d1d2);
    // y[n+2] = b0*d0 + b1*d1 + b2*d2;
    // Q29 <- Q15*Q14
    accy = AE_MULZAAAAQ16(d1d2, b0b1b2_0);
    // Q15 <- Q29 + 2 - 16 w/ asym. rounding, saturation
    ty = AE_MOVINT32X2_FROMF64(accy);
    ty = AE_SLAI32S(ty, 2);
    outy = AE_ROUND16X4F32SASYM(ty, ty);
    // Store output sample.
    // Q15
    AE_S16_0_IP(outy, castxcc(ae_int16, R), sz_i16);
    
    //
    // Save section's state elements.
    //
    d2 = AE_SEL16_5432(d1d2, d1d2);
    d1 = AE_SEL16_6543(d1d2, d1d2);
    AE_S16_0_IP(d2, castxcc(ae_int16, St), sz_i16);
    AE_S16_0_IP(d1, castxcc(ae_int16, St), sz_i16);

    //
    // 2nd to the last sections are fed with output samples of the preceding
    // biquad.
    //
    
    X = (const ae_int16x4 *)r;
  }
  //--------------------------------------------------------------
  // Pass signal through the last biquad and apply the total gain.
  //--------------------------------------------------------------
  {
    //
    // Load 16-bit section coefficients.
    //
    ae_int16x4_loadip(b0b1b2_0, Cf, 4*sz_i16);
    ae_int16x4_loadip(a1a2_0g, Cf, 4*sz_i16);

    //
    // Load 16-bit section's state elements.
    //
    d2 = AE_L16_I((ae_int16 *)St, 0);
    d1 = AE_L16_I((ae_int16 *)St, sz_i16);
    d1d2 = AE_SEL16_4321(d1, d2);
    
    R = (ae_int16x4 *)r;
    
    //
    // Pass the block of input samples through the section. n-th sample at the
    // output of a biquad section r[n] and section state elements (d1,d2) update
    // are given by the following formulae:
    //   d0   = g*x[n] - a1*d1 - a2*d2
    //   r[n] = b0*d0 + b1*d1 + b2*d2;
    //   d2 = d1; d1 = d0;
    //
    __Pragma("loop_count min=2, factor=2")
    for ( n=0; n<N; n++ )
    {
      // Load input sample.
      AE_L16_IP(inx, castxcc(const ae_int16, X), sz_i16);
      
      // d0 = g*x[n] - a1*d1 - a2*d2
      // Q30 <- Q15*Q14 + 1
      accd = AE_MULF16SS_00(inx, a1a2_0g);
      AE_MULAAFD16SS_33_22(accd, d1d2, a1a2_0g);
      // Q15 <- Q30 + 1 - 16 w/ asym. rounding and saturation
      td = AE_SLAI32S(accd, 1);
      d0 = AE_ROUND16X4F32SASYM(td, td);

      // Update the section's state elements.
      d1d2 = AE_SEL16_4321(d0, d1d2);
      
      // Compute the output sample.
      // Q29 <- Q15*Q14
      accy = AE_MULZAAAAQ16(d1d2, b0b1b2_0);
      // Apply total gain, format and store the output sample.
      // Q(15+gain) <- Q(29 + 2+gain - 16) w/ truncation and saturation
      ty = AE_MOVINT32X2_FROMF64(accy);
      ty = AE_SLAA32S(ty, 2+gain);
      ty = AE_SRAI32(ty, 16);
      outy = AE_MOVINT16X4_FROMINT32X2(ty);
      AE_S16_0_IP(outy, castxcc(ae_int16, R), sz_i16);
    }

    //
    // Save section's state elements.
    //
    d2 = AE_SEL16_5432(d1d2, d1d2);
    d1 = AE_SEL16_6543(d1d2, d1d2);
    AE_S16_0_IP(d2, castxcc(ae_int16, St), sz_i16);
    AE_S16_0_IP(d1, castxcc(ae_int16, St), sz_i16);
  }
} // bqriir16x16_df2()
#else
// Process data. The filter instance is identified with a handle returned by
// the initializing function.
void bqriir16x16_df2( bqriir16x16_df2_handle_t _bqriir,
                      void    * restrict       s,
                      int16_t * restrict       r,
                const int16_t *                x, int N )
{
  bqriir16x16_df2_ptr_t bqriir = (bqriir16x16_df2_ptr_t)_bqriir;

  const int16_t   * restrict X;
        ae_int16  * restrict R;
        int16_t   *          S;
  const ae_f16x4  *          C;
  ae_f32x2   x0;
  ae_f32x2   y0;
  ae_f32x2   d0_0;
  ae_f32x2   d1d2_0;
  ae_f64     q0;
  ae_f16x4   cf0, cf1;
  ae_f16x4   out0;
  int16_t    d1, d2, X0;
  int32_t gain;

  int n, m;
  int M;

  ASSERT( bqriir && bqriir->magic == MAGIC && r && x );
  ASSERT( !( N & 1 ) );

  if(N<=0) return;

  M = bqriir->M;
  gain = bqriir->gain;

  // Set the pointer to the filter state elements.
  S = (int16_t *)bqriir->state;
  // Set the pointer to the section coefficients.
  C = (const ae_f16x4 *)bqriir->coef;
  // When processing the first biquad section, use x[N] for the data input.
  X = (const int16_t *)(x);

  //----------------------------------------------------------------------------
  // Perform data block processing for each section but the last one.
  //
  for ( m=0; m<M-1; m++ )
  {
    // Output data of all biquads are stored to the output array.
    R = (ae_int16 *)(r);

    //
    // Load m-th section's SOS/g coefficients.
    //
                                   //          3      2      1      0
    ae_f16x4_loadip( cf0, C, +8 ); // cf0 <-  g_q15  g_q15 b0_q14 b0_q14
    ae_f16x4_loadip( cf1, C, +8 ); // cf1 <- b1_q14 b2_q14 a1_q14 a2_q14

    //
    // Load m-th section's state.
    // Q15
    d2 = S[0]; d1 = S[1];
    d1d2_0 = AE_MOVDA32X2(d1, d2);

    //
    // Pass the block of input samples through the section. n-th sample at the
    // output of a biquad section r[n] and section state elements (d1,d2) update
    // are given by the following formulae:
    //   d0   = g*x[n] - a1*d1 - a2*d2
    //   r[n] = b0*d0 + b1*d1 + b2*d2;
    //   d2 = d1; d1 = d0;
    //
    __Pragma("loop_count min=2, factor=2")
    for ( n=0; n<N; n++ )
    {
      // Load the input sample.
      // Q15
      X0 = (int32_t)*X++;
      x0 = AE_MOVDA32X2( X0, X0 );

      // q = g*x[n];
      // Q30 <- Q15*Q14 + 1
      q0 = AE_MULF32X16_L3( x0, cf0 );
      // q = q - d1*a1 - d2*a2;
      // Q30 <- Q30 - [ Q15*Q14 + 1 ] - [ Q15*Q14 + 1 ]
      AE_MULSSFD32X16_H1_L0( q0, d1d2_0, cf1 );
      // d0 = q;
      // Q15 <- Q30 + 1 - 16 w/ asym. rounding and saturation
      d0_0 = AE_MOVF32X2_FROMF64(AE_PKSR32_0( q0, 1 ));
      d0_0 = AE_MOVF32X2_FROMF16X4(AE_SAT16X4(d0_0, d0_0));
      // q = d0*b0;
      // Q30 <- Q15*Q14 + 1
      q0 = AE_MULF32X16_L0( d0_0, cf0 );
      // q = q + d1*b1 + d2*b2;
      // Q30 <- Q30 + [ Q15*Q14 + 1 ] + [ Q15*Q14 + 1 ]
      AE_MULAAFD32X16_H3_L2( q0, d1d2_0, cf1 );

      // y <- q
      // Q15 <- Q30 + 1 - 16 w/ asym. rounding and saturation
      y0 = AE_MOVF32X2_FROMF64(AE_PKSR32_0( q0, 1 ));
      // Save the output samples.
      out0 = AE_SAT16X4(y0, y0);
      AE_S16_0_IP(out0,R,sz_i16);

      // Update the delay lines:
      // d2 = d1; d1 = d0.
      d1d2_0 = AE_SEL32_LH( d0_0, d1d2_0 );
    }

    //
    // Save m-th section's state.
    //

    out0 = AE_SAT16X4(d1d2_0, d1d2_0);
    AE_S16_0_IP(out0, castxcc(ae_int16,S), sz_i16);
    out0 = AE_SEL16_4321(out0, out0);
    AE_S16_0_IP(out0, castxcc(ae_int16,S), sz_i16);

    //
    // From now on biquads are fed with outputs of preceding sections.
    //

    X = (const int16_t *)(r);
  }

  //----------------------------------------------------------------------------
  // Pass signal through the last biquad and apply the total gain.
  //

  // Last section's outputs will be stored to the output array.
  R = (ae_int16 *)(r);

  //
  // Load m-th section's SOS/g coefficients.
  //
                                 //          3      2      1      0
  ae_f16x4_loadip( cf0, C, +8 ); // cf0 <-  g_q15  g_q15 b0_q14 b0_q14
  ae_f16x4_loadip( cf1, C, +8 ); // cf1 <- b1_q14 b2_q14 a1_q14 a2_q14

  //
  // Load last section's state.
  // Q15
  d2 = S[0]; d1 = S[1];
  d1d2_0 = AE_MOVDA32X2(d1, d2);
    
  //
  // Pass the block of input samples through the section. n-th sample at the
  // output of a biquad section r[n] and section state elements (d1,d2) update
  // are given by the following formulae:
  //   d0   = g*x[n] - a1*d1 - a2*d2
  //   y[n] = b0*d0 + b1*d1 + b2*d2;
  //   d2 = d1; d1 = d0;
  // Final result is obtained by applying the total gain shift amount to
  // response of the last biquad:
  //   r[n] = 2^gain*y[n]
  //
  __Pragma("loop_count min=2, factor=2")
  for ( n=0; n<N; n++ )
  {
    // Load the input sample.
    // Q15
    X0 = (int32_t)*X++;
    x0 = AE_MOVDA32X2( X0, X0 );
      
    // q = g*x[n];
    // Q30 <- Q15*Q14 + 1
    q0 = AE_MULF32X16_L3( x0, cf0 );
    // q = q - d1*a1 - d2*a2;
    // Q30 <- Q30 - [ Q15*Q14 + 1 ] - [ Q15*Q14 + 1 ]
    AE_MULSSFD32X16_H1_L0( q0, d1d2_0, cf1 );
    // d0 = q;
    // Q15 <- Q30 + 1 - 16 w/ asym. rounding and saturation
    d0_0 = AE_MOVF32X2_FROMF64(AE_PKSR32_0( q0, 1 ));
    d0_0 = AE_MOVF32X2_FROMF16X4(AE_SAT16X4(d0_0, d0_0));
    // q = d0*b0;
    // Q30 <- Q15*Q14 + 1
    q0 = AE_MULF32X16_L0( d0_0, cf0 );
    // q = q + d1*b1 + d2*b2;
    // Q30 <- Q30 + [ Q15*Q14 + 1 ] + [ Q15*Q14 + 1 ]
    AE_MULAAFD32X16_H3_L2( q0, d1d2_0, cf1 );
    
    // Apply the total gain shift,
    // Format and save the output samples.
    // Q(15+gain) <- [ Q30 - 15 + gain ] w/ asym. rounding and saturation
    y0 = AE_MOVF32X2_FROMF64(q0);
    y0 = AE_SRAA32S(y0, 15-gain);
    out0 = AE_SAT16X4(y0, y0);

    AE_S16_0_IP(out0, R, sz_i16);

    // Update the delay lines:
    // d2 = d1; d1 = d0.
    d1d2_0 = AE_SEL32_LH( d0_0, d1d2_0 );
  }

  //
  // Save last section's state.
  //

  out0 = AE_SAT16X4(d1d2_0, d1d2_0);
  AE_S16_0_IP(out0, castxcc(ae_int16,S), sz_i16);
  out0 = AE_SEL16_4321(out0, out0);
  AE_S16_0_IP(out0, castxcc(ae_int16,S), sz_i16);

} // bqriir16x16_df2()
#endif