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
    Bi-quad Real Block IIR, 32x32-bit, Direct Form II
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
#define MAGIC     0x417cc78a

/* Reserve memory for alignment. */
#define ALIGNED_SIZE( size, align ) \
      ( (size_t)(size) + (align) - 1 )

/* Align address on a specified boundary. */
#define ALIGNED_ADDR( addr, align ) \
      (void*)( ( (uintptr_t)(addr) + ( (align) - 1 ) ) & ~( (align) - 1 ) )

#define sz_i16 sizeof(int16_t)
#define sz_i32 sizeof(int32_t)

/* Filter instance structure. */
typedef struct tag_bq3iir32x32_df2_t
{
  uint32_t        magic;    // Instance pointer validation number
  int             M;        // Number of sections
  int16_t         gain;     // Gain shift amount applied after the last section
  const int32_t * coef_sos; // Num/den coefs for each of M biquads, Q30
  const int16_t * coef_g;   // Per-section scaling gain coefficients, Q15
  int32_t *       state;    // 3*2 state elements per section, Q31

} bq3iir32x32_df2_t, *bq3iir32x32_df2_ptr_t;

// Determine the memory area size for a filter instance.
size_t bq3iir32x32_df2_alloc( int M )
{
  ASSERT( M >= 0 );
  return ( ALIGNED_SIZE( sizeof(bq3iir32x32_df2_t), 4 )
           + // 2 state elements for each of M DFII sections
           ALIGNED_SIZE( 3*2*M*sz_i32, 2*sz_i32 )
           + // 6 SOS coefficients for each of M sections
           ALIGNED_SIZE( 6*M*sz_i32, 2*sz_i32 )
           + // g coefficient for each of M sections 
           ALIGNED_SIZE( M*sz_i16, sz_i16 ) );

} // bq3iir32x32_df2_alloc()

// Given a memory area of sufficient size, initialize a filter instance and 
// return a handle to it.
bq3iir32x32_df2_handle_t bq3iir32x32_df2_init( void * objmem, int M, 
                                               const int32_t * coef_sos,
                                               const int16_t * coef_g,
                                               int16_t         gain )
{
  bq3iir32x32_df2_ptr_t bq3iir;
  ae_int32x2 * restrict PsectState;
  int32_t * sectState;
  int32_t * sectCoef_sos;
  int16_t * sectCoef_g;
  void *    ptr;
  ae_int32x2 _0;
  int m;

  ASSERT( objmem && M >= 0 && coef_sos && coef_g );
  ASSERT( -48 <= gain && gain <= 15 );

  //
  // Partition the memory block
  //

  ptr = objmem;

  bq3iir       = (bq3iir32x32_df2_ptr_t)ALIGNED_ADDR( ptr, 4 );
  ptr          = bq3iir + 1;
  sectState    = (int32_t*)ALIGNED_ADDR( ptr, 2*sz_i32 );
  ptr          = sectState + 3*2*M;
  sectCoef_sos = (int32_t*)ALIGNED_ADDR( ptr, 2*sz_i32 );
  ptr          = sectCoef_sos + 6*M;
  sectCoef_g   = (int16_t*)ALIGNED_ADDR( ptr, sz_i16 );
  ptr          = sectCoef_g + M;

  ASSERT( (int8_t*)ptr - (int8_t*)objmem <= (int)bq3iir32x32_df2_alloc( M ) );

  //
  // Copy filter coefficients for M sections, zero the state elements.
  //
  PsectState = (ae_int32x2 *)sectState;
  _0 = 0;

  for ( m=0; m<M; m++ )
  {
    sectCoef_sos[6*m+0] = coef_sos[5*m+0]; // b0, Q30
    sectCoef_sos[6*m+1] = coef_sos[5*m+0]; // b0, Q30
    sectCoef_sos[6*m+2] = coef_sos[5*m+1]; // b1, Q30
    sectCoef_sos[6*m+3] = coef_sos[5*m+2]; // b2, Q30
    sectCoef_sos[6*m+4] = coef_sos[5*m+3]; // a1, Q30
    sectCoef_sos[6*m+5] = coef_sos[5*m+4]; // a2, Q30

    sectCoef_g[m] = coef_g[m]; // Q15

    AE_S32X2_IP(_0, PsectState, sz_i32*2);
    AE_S32X2_IP(_0, PsectState, sz_i32*2);
    AE_S32X2_IP(_0, PsectState, sz_i32*2);
  }

  //
  // Initialize the filter instance.
  //

  bq3iir->magic    = MAGIC;
  bq3iir->M        = M;
  bq3iir->gain     = gain;
  bq3iir->coef_sos = sectCoef_sos;
  bq3iir->coef_g   = sectCoef_g;
  bq3iir->state    = sectState;

  return (bq3iir);

} // bq3iir32x32_df2_init()

// Process data. The filter instance is identified with a handle returned by
// the initializing function.
void bq3iir32x32_df2( bq3iir32x32_df2_handle_t _bq3iir,
                      void    * restrict       s,
                      int32_t * restrict       r,
                const int32_t *                x, int N )
{
  bq3iir32x32_df2_ptr_t bq3iir = (bq3iir32x32_df2_ptr_t)_bq3iir;

  const ae_int32   * restrict X;
        ae_int32x2 * restrict R;
  const ae_f32x2   * restrict C_sos;
  const ae_int16   * restrict C_g;
        ae_f32x2   * restrict S;

  ae_f32x2   cf_b0b0, cf_b1b2, cf_a1a2;
  ae_int16x4 cf_g;
  ae_f32x2   st, st0, st1, Y0, Y1;
  ae_int32x2 x0, x1;
  ae_int32   y0, y1;
  ae_f64     acc0, acc1;
  ae_f64     q0, q1;
  int M, gain;
  int m, n, l;

  ASSERT( bq3iir && bq3iir->magic == MAGIC && r && x );
  ASSERT( !( N & 1 ) );
  if(N<=0) return;

  M = bq3iir->M;
  gain = bq3iir->gain;
  S = (ae_f32x2 *)(bq3iir->state);
  
  // Process the input 3-way data by 1 stream
  for ( l=0; l<3; l++ )
  {
    //
    // Setup pointers.
    //
    C_sos = (const ae_f32x2 *)bq3iir->coef_sos;
    C_g   = (const ae_int16 *)bq3iir->coef_g;

    // When processing the first biquad section, use x[N] for the data input.
    X = (const ae_int32 *)(x+l);

    //----------------------------------------------------------------------------
    // Perform data block processing for each section but the last one.
    //
    for ( m=0; m<M-1; m++ )
    {
      // Output data of all biquads are stored to the
      // output array.
      R = (ae_int32x2 *)(r+l);

      //
      // Load m-th section's SOS/g coefficients.
      //
      // Q30
      ae_f32x2_loadip( cf_b0b0, C_sos, 2*sz_i32 );
      ae_f32x2_loadip( cf_b1b2, C_sos, 2*sz_i32 );
      ae_f32x2_loadip( cf_a1a2, C_sos, 2*sz_i32 );
      // Q15
      AE_L16_IP( cf_g, C_g, sz_i16 );

      //
      // Load m-th section's state.
      //
      // st[-2] st[-1]; Q31
      st = ae_f32x2_loadi( S, 0 );
      st0 = AE_SEL32_LL(st, st);
      st1 = AE_SEL32_HH(st, st);

      //
      // Pass the block of input samples through the section. Two outputs
      // y[n], y[n+1] are computed simultaneously:
      //   q0 = g*x[n]   - a1*st[-1] - a2*st[-2]
      //   q1 = g*x[n+1] - a1*q0     - a2*st[-1]
      //   y[n]   = b0*q0 + b1*st[-1] + b2*st[-2]
      //   y[n+1] = b0*q1 + b1*q0     + b2*st[-1]
      //   st[-2] = q0; st[-1] = q1
      //
      __Pragma("loop_count min=1")
      for ( n=0; n<(N>>1); n++ )
      {
        // Read x[n], x[n+1]; Q31
        AE_L32_IP( x0, X, 3*sz_i32 );
        AE_L32_IP( x1, X, 3*sz_i32 );

        // q0 = g*x[n]; q1 = g*x[n+1]
        // Q17.46 <- Q31*Q15
        q0 = AE_MUL32X16_L1( x0, cf_g );
        q1 = AE_MUL32X16_L1( x1, cf_g );

        // q0 -= a2*st[-2] + a1*st[-1]
        // Q17.46 <- Q17.46 + [ Q30*Q31 + 1 - 16 w/ sym. rounding ]
        AE_MULSF32R_LL( q0, cf_a1a2, st1 );
        AE_MULSF32R_HL( q0, cf_a1a2, st0 );

        // y0 = b2*st[-2] + b1*st[-1]
        // Q17.46 <- Q17.46 + [ Q30*Q31 + 1 - 16 w/ sym. rounding ]
        acc0 = AE_MULF32R_LL( cf_b1b2, st1 );
        AE_MULAF32R_HL( acc0, cf_b1b2, st0 );

        // Intermediate update of the delay line
        // st[-2] = st[-1]; st[-1] = q0
        // Q31 <- Q17.46 + 1 - 16 w/ asym. rounding and saturation
        st1=st0;
        st0=AE_MOVF32X2_FROMF64(AE_PKSR32_0(q0,1));

        // y0 += b0*q0
        // Q17.46 <- Q17.46 + [ Q30*Q31 + 1 - 16 w/ sym. rounding ]
        AE_MULAF32R_LL( acc0, cf_b0b0, st0 );

        //------------
        // q1 -= a2*st[-1]
        // q1 -= a1*st[0]
        // Q17.46 <- Q17.46 + [ Q30*Q31 + 1 - 16 w/ sym. rounding ]
        AE_MULSF32R_LL( q1, cf_a1a2, st1 );
        AE_MULSF32R_HL( q1, cf_a1a2, st0 );

        // y1 = b2*st[-1] + b1*q0
        // Q17.46 <- Q17.46 + [ Q30*Q31 + 1 - 16 w/ sym. rounding ]
        acc1 = AE_MULF32R_LL( cf_b1b2, st1 );
        AE_MULAF32R_HL( acc1, cf_b1b2, st0 );

        // Final update of the delay line
        // st[-2] = q0; st[-1] = q1
        // Q31 <- Q17.46 + 1 - 16 w/ asym. rounding and saturation
        st1=st0;
        st0=AE_MOVF32X2_FROMF64(AE_PKSR32_0(q1,1));

        //------------
        // y1 += b0*q1
        // Q17.46 <- Q17.46 + [ Q30*Q31 + 1 - 16 w/ sym. rounding ]
        AE_MULAF32R_LL( acc1, cf_b0b0, st0 );

        // Format and pack outputs y0, y1
        // Q31 <- Q17.46 + 17 - 32 w/ truncation and saturation
        y0=AE_MOVINT32_FROMF64(AE_PKSR32_0(acc0,1));
        y1=AE_MOVINT32_FROMF64(AE_PKSR32_0(acc1,1));

        // Store outputs to the scratch memory
        // y[n] = y0; y[n+1] = y1; Q31
        ae_int32_storeip(y0, castxcc(ae_int32, R), 3*sz_i32);
        ae_int32_storeip(y1, castxcc(ae_int32, R), 3*sz_i32);
      }

      //
      // Save m-th section's state.
      //

      // Q31
      st = AE_SEL32_LL(st1, st0);
      ae_f32x2_storeip( st, S, +8 );

      //
      // From now on biquads are fed with outputs of preceding sections.
      //

      X = (const ae_int32 *)(r+l);
    }

    //----------------------------------------------------------------------------
    // Pass signal through the last biquad and apply the total gain.
    //

    // Last section's outputs will be stored to the output array.
    R = (ae_int32x2 *)(r+l);

    //
    // Load last section's SOS/g coefficients.
    //

    // Q30
    ae_f32x2_loadip( cf_b0b0, C_sos, +8 );
    ae_f32x2_loadip( cf_b1b2, C_sos, +8 );
    ae_f32x2_loadip( cf_a1a2, C_sos, +8 );
    // Q15
    AE_L16_IP( cf_g, C_g, +2 );

    //
    // Load last section's state.
    //
    // st[-2] st[-1]; Q31
    st = ae_f32x2_loadi( S, 0 );
    st0 = AE_SEL32_LL(st, st);
    st1 = AE_SEL32_HH(st, st);

    //
    // Pass the block of input samples through the section. Two outputs
    // r[n], r[n+1] are computed simultaneously:
    //   q0 = g*x[n]   - a1*st[-1] - a2*st[-2]
    //   q1 = g*x[n+1] - a1*q0     - a2*st[-1]
    //   r[n]   = ( b0*q0 + b1*st[-1] + b2*st[-2] )*2^gain
    //   r[n+1] = ( b0*q1 + b1*q0     + b2*st[-1] )*2^gain
    //   st[-2] = q0; st[-1] = q1
    //
    __Pragma("loop_count min=1")
    for ( n=0; n<(N>>1); n++ )
    {
        // Read x[n], x[n+1]; Q31
        AE_L32_IP( x0, X, 3*sz_i32 );
        AE_L32_IP( x1, X, 3*sz_i32 );

        // q0 = g*x[n]; q1 = g*x[n+1]
        // Q17.46 <- Q31*Q15
        q0 = AE_MUL32X16_L1( x0, cf_g );
        q1 = AE_MUL32X16_L1( x1, cf_g );

        // q0 -= a2*st[-2] + a1*st[-1]
        // Q17.46 <- Q17.46 + [ Q30*Q31 + 1 - 16 w/ sym. rounding ]
        AE_MULSF32R_LL( q0, cf_a1a2, st1 );
        AE_MULSF32R_HL( q0, cf_a1a2, st0 );

        // y0 = b2*st[-2] + b1*st[-1]
        // Q17.46 <- Q17.46 + [ Q30*Q31 + 1 - 16 w/ sym. rounding ]
        acc0 = AE_MULF32R_LL( cf_b1b2, st1 );
        AE_MULAF32R_HL( acc0, cf_b1b2, st0 );

        // Intermediate update of the delay line
        // st[-2] = st[-1]; st[-1] = q0
        // Q31 <- Q17.46 + 1 - 16 w/ asym. rounding and saturation
        st1=st0;
        st0=AE_MOVF32X2_FROMF64(AE_PKSR32_0(q0,1));

        // y0 += b0*q0
        // Q17.46 <- Q17.46 + [ Q30*Q31 + 1 - 16 w/ sym. rounding ]
        AE_MULAF32R_LL( acc0, cf_b0b0, st0 );

        //------------
        // q1 -= a2*st[-1]
        // q1 -= a1*st[0]
        // Q17.46 <- Q17.46 + [ Q30*Q31 + 1 - 16 w/ sym. rounding ]
        AE_MULSF32R_LL( q1, cf_a1a2, st1 );
        AE_MULSF32R_HL( q1, cf_a1a2, st0 );

        // y1 = b2*st[-1] + b1*q0
        // Q17.46 <- Q17.46 + [ Q30*Q31 + 1 - 16 w/ sym. rounding ]
        acc1 = AE_MULF32R_LL( cf_b1b2, st1 );
        AE_MULAF32R_HL( acc1, cf_b1b2, st0 );

        // Final update of the delay line
        // st[-2] = q0; st[-1] = q1
        // Q31 <- Q17.46 + 1 - 16 w/ asym. rounding and saturation
        st1=st0;
        st0=AE_MOVF32X2_FROMF64(AE_PKSR32_0(q1,1));

        //------------
        // y1 += b0*q1
        // Q17.46 <- Q17.46 + [ Q30*Q31 + 1 - 16 w/ sym. rounding ]
        AE_MULAF32R_LL( acc1, cf_b0b0, st0 );

        // Format and pack outputs y0, y1
        // Q31 <- Q17.46 + 17 - 32 w/ truncation and saturation
        Y0=AE_MOVF32X2_FROMF64(AE_PKSR32_0(acc0,1));
        Y1=AE_MOVF32X2_FROMF64(AE_PKSR32_0(acc1,1));

        // Apply the total gain shift
        // Q(17.47+gain) <- [ Q17.46 + 1 + gain ] w/ saturation
        acc0 = AE_F64_SLAS(acc0, 1+gain);
        acc1 = AE_F64_SLAS(acc1, 1+gain);
      
        // Format and save the output samples.
        // Q31 <- Q17.47 - 16 w/ asym. rounding and saturation
        AE_S32RA64S_IP(acc0,castxcc(ae_f32, R),3*sz_i32);
        AE_S32RA64S_IP(acc1,castxcc(ae_f32, R),3*sz_i32);
    }

    //
    // Save last section's state.
    //
    st = AE_SEL32_LL(st1, st0);
    ae_f32x2_storeip( st, S, +8 );
  }
} // bq3iir32x32_df2()
