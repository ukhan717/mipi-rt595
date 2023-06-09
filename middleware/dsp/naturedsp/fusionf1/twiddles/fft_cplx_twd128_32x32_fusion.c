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
#include "fft_cplx_twiddles_32x32.h"
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
ALIGN(8) static const int32_t fft_twiddle_table[192] =
{

    (int32_t)0x7FFFFFFF, (int32_t)0x00000000, (int32_t)0x7FFFFFFF, (int32_t)0x00000000,
    (int32_t)0x7FFFFFFF, (int32_t)0x00000000, (int32_t)0x7FD8878E, (int32_t)0xF9B82684,
    (int32_t)0x7F62368F, (int32_t)0xF3742CA2, (int32_t)0x7E9D55FC, (int32_t)0xED37EF91,
    (int32_t)0x7F62368F, (int32_t)0xF3742CA2, (int32_t)0x7D8A5F40, (int32_t)0xE70747C4,
    (int32_t)0x7A7D055B, (int32_t)0xDAD7F3A2, (int32_t)0x7E9D55FC, (int32_t)0xED37EF91,
    (int32_t)0x7A7D055B, (int32_t)0xDAD7F3A2, (int32_t)0x73B5EBD1, (int32_t)0xC945DFEC,
    (int32_t)0x7D8A5F40, (int32_t)0xE70747C4, (int32_t)0x7641AF3D, (int32_t)0xCF043AB3,
    (int32_t)0x6A6D98A4, (int32_t)0xB8E31319, (int32_t)0x7C29FBEE, (int32_t)0xE0E60685,
    (int32_t)0x70E2CBC6, (int32_t)0xC3A94590, (int32_t)0x5ED77C8A, (int32_t)0xAA0A5B2E,
    (int32_t)0x7A7D055B, (int32_t)0xDAD7F3A2, (int32_t)0x6A6D98A4, (int32_t)0xB8E31319,
    (int32_t)0x5133CC94, (int32_t)0x9D0DFE54, (int32_t)0x78848414, (int32_t)0xD4E0CB15,
    (int32_t)0x62F201AC, (int32_t)0xAECC336C, (int32_t)0x41CE1E65, (int32_t)0x9235F2EC,
    (int32_t)0x7641AF3D, (int32_t)0xCF043AB3, (int32_t)0x5A82799A, (int32_t)0xA57D8666,
    (int32_t)0x30FBC54D, (int32_t)0x89BE50C3, (int32_t)0x73B5EBD1, (int32_t)0xC945DFEC,
    (int32_t)0x5133CC94, (int32_t)0x9D0DFE54, (int32_t)0x1F19F97B, (int32_t)0x83D60412,
    (int32_t)0x70E2CBC6, (int32_t)0xC3A94590, (int32_t)0x471CECE7, (int32_t)0x9592675C,
    (int32_t)0x0C8BD35E, (int32_t)0x809DC971, (int32_t)0x6DCA0D14, (int32_t)0xBE31E19B,
    (int32_t)0x3C56BA70, (int32_t)0x8F1D343A, (int32_t)0xF9B82684, (int32_t)0x80277872,
    (int32_t)0x6A6D98A4, (int32_t)0xB8E31319, (int32_t)0x30FBC54D, (int32_t)0x89BE50C3,
    (int32_t)0xE70747C4, (int32_t)0x8275A0C0, (int32_t)0x66CF8120, (int32_t)0xB3C0200C,
    (int32_t)0x25280C5E, (int32_t)0x8582FAA5, (int32_t)0xD4E0CB15, (int32_t)0x877B7BEC,
    (int32_t)0x62F201AC, (int32_t)0xAECC336C, (int32_t)0x18F8B83C, (int32_t)0x8275A0C0,
    (int32_t)0xC3A94590, (int32_t)0x8F1D343A, (int32_t)0x5ED77C8A, (int32_t)0xAA0A5B2E,
    (int32_t)0x0C8BD35E, (int32_t)0x809DC971, (int32_t)0xB3C0200C, (int32_t)0x99307EE0,
    (int32_t)0x5A82799A, (int32_t)0xA57D8666, (int32_t)0x00000000, (int32_t)0x80000000,
    (int32_t)0xA57D8666, (int32_t)0xA57D8666, (int32_t)0x55F5A4D2, (int32_t)0xA1288376,
    (int32_t)0xF3742CA2, (int32_t)0x809DC971, (int32_t)0x99307EE0, (int32_t)0xB3C0200C,
    (int32_t)0x5133CC94, (int32_t)0x9D0DFE54, (int32_t)0xE70747C4, (int32_t)0x8275A0C0,
    (int32_t)0x8F1D343A, (int32_t)0xC3A94590, (int32_t)0x4C3FDFF4, (int32_t)0x99307EE0,
    (int32_t)0xDAD7F3A2, (int32_t)0x8582FAA5, (int32_t)0x877B7BEC, (int32_t)0xD4E0CB15,
    (int32_t)0x471CECE7, (int32_t)0x9592675C, (int32_t)0xCF043AB3, (int32_t)0x89BE50C3,
    (int32_t)0x8275A0C0, (int32_t)0xE70747C4, (int32_t)0x41CE1E65, (int32_t)0x9235F2EC,
    (int32_t)0xC3A94590, (int32_t)0x8F1D343A, (int32_t)0x80277872, (int32_t)0xF9B82684,
    (int32_t)0x3C56BA70, (int32_t)0x8F1D343A, (int32_t)0xB8E31319, (int32_t)0x9592675C,
    (int32_t)0x809DC971, (int32_t)0x0C8BD35E, (int32_t)0x36BA2014, (int32_t)0x8C4A142F,
    (int32_t)0xAECC336C, (int32_t)0x9D0DFE54, (int32_t)0x83D60412, (int32_t)0x1F19F97B,
    (int32_t)0x30FBC54D, (int32_t)0x89BE50C3, (int32_t)0xA57D8666, (int32_t)0xA57D8666,
    (int32_t)0x89BE50C3, (int32_t)0x30FBC54D, (int32_t)0x2B1F34EB, (int32_t)0x877B7BEC,
    (int32_t)0x9D0DFE54, (int32_t)0xAECC336C, (int32_t)0x9235F2EC, (int32_t)0x41CE1E65,
    (int32_t)0x25280C5E, (int32_t)0x8582FAA5, (int32_t)0x9592675C, (int32_t)0xB8E31319,
    (int32_t)0x9D0DFE54, (int32_t)0x5133CC94, (int32_t)0x1F19F97B, (int32_t)0x83D60412,
    (int32_t)0x8F1D343A, (int32_t)0xC3A94590, (int32_t)0xAA0A5B2E, (int32_t)0x5ED77C8A,
    (int32_t)0x18F8B83C, (int32_t)0x8275A0C0, (int32_t)0x89BE50C3, (int32_t)0xCF043AB3,
    (int32_t)0xB8E31319, (int32_t)0x6A6D98A4, (int32_t)0x12C8106F, (int32_t)0x8162AA04,
    (int32_t)0x8582FAA5, (int32_t)0xDAD7F3A2, (int32_t)0xC945DFEC, (int32_t)0x73B5EBD1,
    (int32_t)0x0C8BD35E, (int32_t)0x809DC971, (int32_t)0x8275A0C0, (int32_t)0xE70747C4,
    (int32_t)0xDAD7F3A2, (int32_t)0x7A7D055B, (int32_t)0x0647D97C, (int32_t)0x80277872,
    (int32_t)0x809DC971, (int32_t)0xF3742CA2, (int32_t)0xED37EF91, (int32_t)0x7E9D55FC, };


static const tFftDescr descr = { 128, NULL, (cint32_ptr)fft_twiddle_table };
const fft_handle_t cfft32_128=(const void*)&descr;
const fft_handle_t rfft32_256=(const void*)&descr;
const fft_handle_t cifft32_128=(const void*)&descr;
const fft_handle_t rifft32_256=(const void*)&descr;
