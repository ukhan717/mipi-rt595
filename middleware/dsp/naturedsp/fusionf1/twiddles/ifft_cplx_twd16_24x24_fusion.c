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
	NatureDSP_Signal library. FFT part
    C code with generic optimization

    twiddle factors for 24x24 real fwd/rev FFT transforms
*/

#include "NatureDSP_Signal.h"
#include "fft_cplx_twiddles_24x24.h"
#include "common.h"

/*
    Twiddle tables are received by sequential writing twiddle factors used in the original fft loops
    However, last radix2/radix4 stage is omitted because they are processed separately and combined 
    with bitreversing
    Higher word of each item is a cosine of a twiddle angle and lower word is a sine. All numbers are
    represented in Q31 with cleared 8 LSBs. Twiddle angle is always an integral number multiplied by 
    2*pi/N where N is FFT length. 
    For example, the sequence of twiddle angles is:
    16-pts transform 
    [0 0 0 1 2 3 2 4 6 3 6 -7]/16
    32-pts transform 
    [0 0 0 1 2 3 2 4 6 3 6 9 4 8 12 5 10 15 6 12 -14 7 14 -11 0 0 0 4 8 12 0 0 0 4 8 12 0 0 0 4 8 12 0 0 0 4 8 12 ]/32
    ....
*/
const int32_t MSC_ALIGNED ifft24x24_twd16[12*2] GCC_ALIGNED = {
(int32_t)0x7fffff00, (int32_t)0x00000000, (int32_t)0x7fffff00, (int32_t)0x00000000, (int32_t)0x7fffff00, (int32_t)0x00000000,(int32_t)0x7641af00, (int32_t)0x30fbc500, (int32_t)0x5a827900, (int32_t)0x5a827900, (int32_t)0x30fbc500, (int32_t)0x7641af00,
(int32_t)0x5a827900, (int32_t)0x5a827900, (int32_t)0x00000000, (int32_t)0x7fffff00, (int32_t)0xa57d8600, (int32_t)0x5a827900,(int32_t)0x30fbc500, (int32_t)0x7641af00, (int32_t)0xa57d8600, (int32_t)0x5a827900, (int32_t)0x89be5000, (int32_t)0xcf043a00,
};
static const tFftDescr descr={  16,NULL,(cint32_ptr)ifft24x24_twd16};
const fft_handle_t cifft24_16=(const void*)&descr;
const fft_handle_t rifft24_32=(const void*)&descr;
