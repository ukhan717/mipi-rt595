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
const int32_t MSC_ALIGNED fft24x24_twd1024[12*128] GCC_ALIGNED = {
(int32_t)0x7fffff00,(int32_t)0x00000000,(int32_t)0x7fffff00,(int32_t)0x00000000,(int32_t)0x7fffff00,(int32_t)0x00000000,(int32_t)0x7fff6200,(int32_t)0xFF36F100,(int32_t)0x7ffd8800,(int32_t)0xFE6DE300,(int32_t)0x7ffa7200,(int32_t)0xFDA4DA00,
(int32_t)0x7ffd8800,(int32_t)0xFE6DE300,(int32_t)0x7ff62100,(int32_t)0xFCDBD600,(int32_t)0x7fe9cb00,(int32_t)0xFB49E700,(int32_t)0x7ffa7200,(int32_t)0xFDA4DA00,(int32_t)0x7fe9cb00,(int32_t)0xFB49E700,(int32_t)0x7fce0c00,(int32_t)0xF8EF5D00,
(int32_t)0x7ff62100,(int32_t)0xFCDBD600,(int32_t)0x7fd88700,(int32_t)0xF9B82700,(int32_t)0x7fa73600,(int32_t)0xF6957000,(int32_t)0x7ff09400,(int32_t)0xFC12DA00,(int32_t)0x7fc25500,(int32_t)0xF826A500,(int32_t)0x7f754e00,(int32_t)0xF43C5400,
(int32_t)0x7fe9cb00,(int32_t)0xFB49E700,(int32_t)0x7fa73600,(int32_t)0xF6957000,(int32_t)0x7f385700,(int32_t)0xF1E43E00,(int32_t)0x7fe1c700,(int32_t)0xFA810000,(int32_t)0x7f872b00,(int32_t)0xF5049800,(int32_t)0x7ef05800,(int32_t)0xEF8D6000,
(int32_t)0x7fd88700,(int32_t)0xF9B82700,(int32_t)0x7f623600,(int32_t)0xF3742D00,(int32_t)0x7e9d5500,(int32_t)0xED37F000,(int32_t)0x7fce0c00,(int32_t)0xF8EF5D00,(int32_t)0x7f385700,(int32_t)0xF1E43E00,(int32_t)0x7e3f5700,(int32_t)0xEAE42100,
(int32_t)0x7fc25500,(int32_t)0xF826A500,(int32_t)0x7f099100,(int32_t)0xF054D900,(int32_t)0x7dd66600,(int32_t)0xE8922700,(int32_t)0x7fb56300,(int32_t)0xF75E0000,(int32_t)0x7ed5e500,(int32_t)0xEEC61000,(int32_t)0x7d628a00,(int32_t)0xE6423500,
(int32_t)0x7fa73600,(int32_t)0xF6957000,(int32_t)0x7e9d5500,(int32_t)0xED37F000,(int32_t)0x7ce3ce00,(int32_t)0xE3F47E00,(int32_t)0x7f97ce00,(int32_t)0xF5CCF800,(int32_t)0x7e5fe400,(int32_t)0xEBAA8A00,(int32_t)0x7c5a3d00,(int32_t)0xE1A93600,
(int32_t)0x7f872b00,(int32_t)0xF5049800,(int32_t)0x7e1d9300,(int32_t)0xEA1DEC00,(int32_t)0x7bc5e200,(int32_t)0xDF609000,(int32_t)0x7f754e00,(int32_t)0xF43C5400,(int32_t)0x7dd66600,(int32_t)0xE8922700,(int32_t)0x7b26cb00,(int32_t)0xDD1ABF00,
(int32_t)0x7f623600,(int32_t)0xF3742D00,(int32_t)0x7d8a5f00,(int32_t)0xE7074800,(int32_t)0x7a7d0500,(int32_t)0xDAD7F400,(int32_t)0x7f4de400,(int32_t)0xF2AC2500,(int32_t)0x7d398000,(int32_t)0xE57D6000,(int32_t)0x79c89f00,(int32_t)0xD8986300,
(int32_t)0x7f385700,(int32_t)0xF1E43E00,(int32_t)0x7ce3ce00,(int32_t)0xE3F47E00,(int32_t)0x7909a900,(int32_t)0xD65C3C00,(int32_t)0x7f219100,(int32_t)0xF11C7900,(int32_t)0x7c894b00,(int32_t)0xE26CB100,(int32_t)0x78403300,(int32_t)0xD423B200,
(int32_t)0x7f099100,(int32_t)0xF054D900,(int32_t)0x7c29fb00,(int32_t)0xE0E60700,(int32_t)0x776c4e00,(int32_t)0xD1EEF600,(int32_t)0x7ef05800,(int32_t)0xEF8D6000,(int32_t)0x7bc5e200,(int32_t)0xDF609000,(int32_t)0x768e0e00,(int32_t)0xCFBE3900,
(int32_t)0x7ed5e500,(int32_t)0xEEC61000,(int32_t)0x7b5d0300,(int32_t)0xDDDC5C00,(int32_t)0x75a58500,(int32_t)0xCD91AC00,(int32_t)0x7eba3a00,(int32_t)0xEDFEEA00,(int32_t)0x7aef6300,(int32_t)0xDC597800,(int32_t)0x74b2c800,(int32_t)0xCB697E00,
(int32_t)0x7e9d5500,(int32_t)0xED37F000,(int32_t)0x7a7d0500,(int32_t)0xDAD7F400,(int32_t)0x73b5eb00,(int32_t)0xC945E000,(int32_t)0x7e7f3900,(int32_t)0xEC712500,(int32_t)0x7a05ee00,(int32_t)0xD957DF00,(int32_t)0x72af0500,(int32_t)0xC7270200,
(int32_t)0x7e5fe400,(int32_t)0xEBAA8A00,(int32_t)0x798a2300,(int32_t)0xD7D94700,(int32_t)0x719e2c00,(int32_t)0xC50D1200,(int32_t)0x7e3f5700,(int32_t)0xEAE42100,(int32_t)0x7909a900,(int32_t)0xD65C3C00,(int32_t)0x70837800,(int32_t)0xC2F83F00,
(int32_t)0x7e1d9300,(int32_t)0xEA1DEC00,(int32_t)0x78848400,(int32_t)0xD4E0CC00,(int32_t)0x6f5f0200,(int32_t)0xC0E8B700,(int32_t)0x7dfa9800,(int32_t)0xE957ED00,(int32_t)0x77fab900,(int32_t)0xD3670500,(int32_t)0x6e30e300,(int32_t)0xBEDEA800,
(int32_t)0x7dd66600,(int32_t)0xE8922700,(int32_t)0x776c4e00,(int32_t)0xD1EEF600,(int32_t)0x6cf93400,(int32_t)0xBCDA3F00,(int32_t)0x7db0fd00,(int32_t)0xE7CC9A00,(int32_t)0x76d94900,(int32_t)0xD078AE00,(int32_t)0x6bb81200,(int32_t)0xBADBAA00,
(int32_t)0x7d8a5f00,(int32_t)0xE7074800,(int32_t)0x7641af00,(int32_t)0xCF043B00,(int32_t)0x6a6d9800,(int32_t)0xB8E31400,(int32_t)0x7d628a00,(int32_t)0xE6423500,(int32_t)0x75a58500,(int32_t)0xCD91AC00,(int32_t)0x6919e300,(int32_t)0xB6F0A900,
(int32_t)0x7d398000,(int32_t)0xE57D6000,(int32_t)0x7504d300,(int32_t)0xCC210E00,(int32_t)0x67bd0f00,(int32_t)0xB5049400,(int32_t)0x7d0f4200,(int32_t)0xE4B8CE00,(int32_t)0x745f9d00,(int32_t)0xCAB27000,(int32_t)0x66573c00,(int32_t)0xB31F0000,
(int32_t)0x7ce3ce00,(int32_t)0xE3F47E00,(int32_t)0x73b5eb00,(int32_t)0xC945E000,(int32_t)0x64e88900,(int32_t)0xB1401800,(int32_t)0x7cb72700,(int32_t)0xE3307400,(int32_t)0x7307c300,(int32_t)0xC7DB6D00,(int32_t)0x63711400,(int32_t)0xAF680400,
(int32_t)0x7c894b00,(int32_t)0xE26CB100,(int32_t)0x72552c00,(int32_t)0xC6732300,(int32_t)0x61f10000,(int32_t)0xAD96EE00,(int32_t)0x7c5a3d00,(int32_t)0xE1A93600,(int32_t)0x719e2c00,(int32_t)0xC50D1200,(int32_t)0x60686c00,(int32_t)0xABCCFE00,
(int32_t)0x7c29fb00,(int32_t)0xE0E60700,(int32_t)0x70e2cb00,(int32_t)0xC3A94600,(int32_t)0x5ed77c00,(int32_t)0xAA0A5C00,(int32_t)0x7bf88800,(int32_t)0xE0232400,(int32_t)0x70231000,(int32_t)0xC247CE00,(int32_t)0x5d3e5200,(int32_t)0xA84F2E00,
(int32_t)0x7bc5e200,(int32_t)0xDF609000,(int32_t)0x6f5f0200,(int32_t)0xC0E8B700,(int32_t)0x5b9d1100,(int32_t)0xA69B9C00,(int32_t)0x7b920b00,(int32_t)0xDE9E4D00,(int32_t)0x6e96a900,(int32_t)0xBF8C0E00,(int32_t)0x59f3de00,(int32_t)0xA4EFCB00,
(int32_t)0x7b5d0300,(int32_t)0xDDDC5C00,(int32_t)0x6dca0d00,(int32_t)0xBE31E200,(int32_t)0x5842dd00,(int32_t)0xA34BE000,(int32_t)0x7b26cb00,(int32_t)0xDD1ABF00,(int32_t)0x6cf93400,(int32_t)0xBCDA3F00,(int32_t)0x568a3400,(int32_t)0xA1AFFF00,
(int32_t)0x7aef6300,(int32_t)0xDC597800,(int32_t)0x6c242900,(int32_t)0xBB853300,(int32_t)0x54ca0a00,(int32_t)0xA01C4D00,(int32_t)0x7ab6cb00,(int32_t)0xDB988900,(int32_t)0x6b4af200,(int32_t)0xBA32CB00,(int32_t)0x53028500,(int32_t)0x9E90EC00,
(int32_t)0x7a7d0500,(int32_t)0xDAD7F400,(int32_t)0x6a6d9800,(int32_t)0xB8E31400,(int32_t)0x5133cc00,(int32_t)0x9D0DFF00,(int32_t)0x7a421000,(int32_t)0xDA17BB00,(int32_t)0x698c2400,(int32_t)0xB7961A00,(int32_t)0x4f5e0800,(int32_t)0x9B93A700,
(int32_t)0x7a05ee00,(int32_t)0xD957DF00,(int32_t)0x68a69e00,(int32_t)0xB64BEB00,(int32_t)0x4d816200,(int32_t)0x9A220500,(int32_t)0x79c89f00,(int32_t)0xD8986300,(int32_t)0x67bd0f00,(int32_t)0xB5049400,(int32_t)0x4b9e0300,(int32_t)0x98B93900,
(int32_t)0x798a2300,(int32_t)0xD7D94700,(int32_t)0x66cf8100,(int32_t)0xB3C02100,(int32_t)0x49b41500,(int32_t)0x97596200,(int32_t)0x794a7c00,(int32_t)0xD71A8F00,(int32_t)0x65ddfb00,(int32_t)0xB27E9E00,(int32_t)0x47c3c200,(int32_t)0x96029F00,
(int32_t)0x7909a900,(int32_t)0xD65C3C00,(int32_t)0x64e88900,(int32_t)0xB1401800,(int32_t)0x45cd3500,(int32_t)0x94B50E00,(int32_t)0x78c7ab00,(int32_t)0xD59E4F00,(int32_t)0x63ef3200,(int32_t)0xB0049B00,(int32_t)0x43d09a00,(int32_t)0x9370CB00,
(int32_t)0x78848400,(int32_t)0xD4E0CC00,(int32_t)0x62f20100,(int32_t)0xAECC3400,(int32_t)0x41ce1e00,(int32_t)0x9235F300,(int32_t)0x78403300,(int32_t)0xD423B200,(int32_t)0x61f10000,(int32_t)0xAD96EE00,(int32_t)0x3fc5ec00,(int32_t)0x9104A100,
(int32_t)0x77fab900,(int32_t)0xD3670500,(int32_t)0x60ec3800,(int32_t)0xAC64D600,(int32_t)0x3db83200,(int32_t)0x8FDCF000,(int32_t)0x77b41700,(int32_t)0xD2AAC600,(int32_t)0x5fe3b300,(int32_t)0xAB35F600,(int32_t)0x3ba51e00,(int32_t)0x8EBEF800,
(int32_t)0x776c4e00,(int32_t)0xD1EEF600,(int32_t)0x5ed77c00,(int32_t)0xAA0A5C00,(int32_t)0x398cdd00,(int32_t)0x8DAAD400,(int32_t)0x77235f00,(int32_t)0xD1339800,(int32_t)0x5dc79d00,(int32_t)0xA8E21200,(int32_t)0x376f9e00,(int32_t)0x8CA09A00,
(int32_t)0x76d94900,(int32_t)0xD078AE00,(int32_t)0x5cb42000,(int32_t)0xA7BD2300,(int32_t)0x354d9000,(int32_t)0x8BA06300,(int32_t)0x768e0e00,(int32_t)0xCFBE3900,(int32_t)0x5b9d1100,(int32_t)0xA69B9C00,(int32_t)0x3326e200,(int32_t)0x8AAA4300,
(int32_t)0x7641af00,(int32_t)0xCF043B00,(int32_t)0x5a827900,(int32_t)0xA57D8700,(int32_t)0x30fbc500,(int32_t)0x89BE5100,(int32_t)0x75f42c00,(int32_t)0xCE4AB600,(int32_t)0x59646400,(int32_t)0xA462EF00,(int32_t)0x2ecc6800,(int32_t)0x88DCA100,
(int32_t)0x75a58500,(int32_t)0xCD91AC00,(int32_t)0x5842dd00,(int32_t)0xA34BE000,(int32_t)0x2c98fb00,(int32_t)0x88054700,(int32_t)0x7555bd00,(int32_t)0xCCD91E00,(int32_t)0x571dee00,(int32_t)0xA2386300,(int32_t)0x2a61b100,(int32_t)0x87385500,
(int32_t)0x7504d300,(int32_t)0xCC210E00,(int32_t)0x55f5a400,(int32_t)0xA1288400,(int32_t)0x2826b900,(int32_t)0x8675DD00,(int32_t)0x74b2c800,(int32_t)0xCB697E00,(int32_t)0x54ca0a00,(int32_t)0xA01C4D00,(int32_t)0x25e84500,(int32_t)0x85BDF000,
(int32_t)0x745f9d00,(int32_t)0xCAB27000,(int32_t)0x539b2a00,(int32_t)0x9F13C800,(int32_t)0x23a68800,(int32_t)0x85109D00,(int32_t)0x740b5300,(int32_t)0xC9FBE600,(int32_t)0x52691200,(int32_t)0x9E0F0000,(int32_t)0x2161b300,(int32_t)0x846DF500,
(int32_t)0x73b5eb00,(int32_t)0xC945E000,(int32_t)0x5133cc00,(int32_t)0x9D0DFF00,(int32_t)0x1f19f900,(int32_t)0x83D60500,(int32_t)0x735f6600,(int32_t)0xC8906200,(int32_t)0x4ffb6500,(int32_t)0x9C10CE00,(int32_t)0x1ccf8c00,(int32_t)0x8348D900,
(int32_t)0x7307c300,(int32_t)0xC7DB6D00,(int32_t)0x4ebfe800,(int32_t)0x9B177700,(int32_t)0x1a82a000,(int32_t)0x82C68000,(int32_t)0x72af0500,(int32_t)0xC7270200,(int32_t)0x4d816200,(int32_t)0x9A220500,(int32_t)0x18336600,(int32_t)0x824F0300,
(int32_t)0x72552c00,(int32_t)0xC6732300,(int32_t)0x4c3fdf00,(int32_t)0x99307F00,(int32_t)0x15e21400,(int32_t)0x81E26D00,(int32_t)0x71fa3900,(int32_t)0xC5BFD300,(int32_t)0x4afb6c00,(int32_t)0x9842F100,(int32_t)0x138edb00,(int32_t)0x8180C700,
(int32_t)0x719e2c00,(int32_t)0xC50D1200,(int32_t)0x49b41500,(int32_t)0x97596200,(int32_t)0x1139f000,(int32_t)0x812A1B00,(int32_t)0x71410800,(int32_t)0xC45AE200,(int32_t)0x4869e600,(int32_t)0x9673DC00,(int32_t)0x0ee38700,(int32_t)0x80DE6F00,
(int32_t)0x70e2cb00,(int32_t)0xC3A94600,(int32_t)0x471cec00,(int32_t)0x95926800,(int32_t)0x0c8bd300,(int32_t)0x809DCA00,(int32_t)0x70837800,(int32_t)0xC2F83F00,(int32_t)0x45cd3500,(int32_t)0x94B50E00,(int32_t)0x0a330800,(int32_t)0x80683200,
(int32_t)0x70231000,(int32_t)0xC247CE00,(int32_t)0x447acd00,(int32_t)0x93DBD700,(int32_t)0x07d95b00,(int32_t)0x803DAB00,(int32_t)0x6fc19300,(int32_t)0xC197F500,(int32_t)0x4325c100,(int32_t)0x9306CC00,(int32_t)0x057f0000,(int32_t)0x801E3900,
(int32_t)0x6f5f0200,(int32_t)0xC0E8B700,(int32_t)0x41ce1e00,(int32_t)0x9235F300,(int32_t)0x03242a00,(int32_t)0x8009DF00,(int32_t)0x6efb5f00,(int32_t)0xC03A1400,(int32_t)0x4073f200,(int32_t)0x91695700,(int32_t)0x00c90f00,(int32_t)0x80009E00,
(int32_t)0x6e96a900,(int32_t)0xBF8C0E00,(int32_t)0x3f174900,(int32_t)0x90A0FE00,(int32_t)0xfe6de200,(int32_t)0x80027800,(int32_t)0x6e30e300,(int32_t)0xBEDEA800,(int32_t)0x3db83200,(int32_t)0x8FDCF000,(int32_t)0xfc12d900,(int32_t)0x800F6C00,
(int32_t)0x6dca0d00,(int32_t)0xBE31E200,(int32_t)0x3c56ba00,(int32_t)0x8F1D3500,(int32_t)0xf9b82600,(int32_t)0x80277900,(int32_t)0x6d622700,(int32_t)0xBD85BF00,(int32_t)0x3af2ee00,(int32_t)0x8E61D400,(int32_t)0xf75dff00,(int32_t)0x804A9D00,
(int32_t)0x6cf93400,(int32_t)0xBCDA3F00,(int32_t)0x398cdd00,(int32_t)0x8DAAD400,(int32_t)0xf5049700,(int32_t)0x8078D500,(int32_t)0x6c8f3500,(int32_t)0xBC2F6600,(int32_t)0x38249300,(int32_t)0x8CF83D00,(int32_t)0xf2ac2400,(int32_t)0x80B21C00,
(int32_t)0x6c242900,(int32_t)0xBB853300,(int32_t)0x36ba2000,(int32_t)0x8C4A1500,(int32_t)0xf054d800,(int32_t)0x80F66F00,(int32_t)0x6bb81200,(int32_t)0xBADBAA00,(int32_t)0x354d9000,(int32_t)0x8BA06300,(int32_t)0xedfee900,(int32_t)0x8145C600,
(int32_t)0x6b4af200,(int32_t)0xBA32CB00,(int32_t)0x33def200,(int32_t)0x8AFB2D00,(int32_t)0xebaa8900,(int32_t)0x81A01C00,(int32_t)0x6adcc900,(int32_t)0xB98A9800,(int32_t)0x326e5400,(int32_t)0x8A5A7B00,(int32_t)0xe957ec00,(int32_t)0x82056800,
(int32_t)0x6a6d9800,(int32_t)0xB8E31400,(int32_t)0x30fbc500,(int32_t)0x89BE5100,(int32_t)0xe7074700,(int32_t)0x8275A100,(int32_t)0x69fd6100,(int32_t)0xB83C3E00,(int32_t)0x2f875200,(int32_t)0x8926B700,(int32_t)0xe4b8cd00,(int32_t)0x82F0BE00,
(int32_t)0x698c2400,(int32_t)0xB7961A00,(int32_t)0x2e110a00,(int32_t)0x8893B200,(int32_t)0xe26cb000,(int32_t)0x8376B500,(int32_t)0x6919e300,(int32_t)0xB6F0A900,(int32_t)0x2c98fb00,(int32_t)0x88054700,(int32_t)0xe0232300,(int32_t)0x84077800,
(int32_t)0x68a69e00,(int32_t)0xB64BEB00,(int32_t)0x2b1f3400,(int32_t)0x877B7C00,(int32_t)0xdddc5b00,(int32_t)0x84A2FD00,(int32_t)0x68325700,(int32_t)0xB5A7E400,(int32_t)0x29a3c400,(int32_t)0x86F65700,(int32_t)0xdb988800,(int32_t)0x85493500,
(int32_t)0x67bd0f00,(int32_t)0xB5049400,(int32_t)0x2826b900,(int32_t)0x8675DD00,(int32_t)0xd957de00,(int32_t)0x85FA1200,(int32_t)0x6746c700,(int32_t)0xB461FD00,(int32_t)0x26a82100,(int32_t)0x85FA1200,(int32_t)0xd71a8e00,(int32_t)0x86B58400,
(int32_t)0x66cf8100,(int32_t)0xB3C02100,(int32_t)0x25280c00,(int32_t)0x8582FB00,(int32_t)0xd4e0cb00,(int32_t)0x877B7C00,(int32_t)0x66573c00,(int32_t)0xB31F0000,(int32_t)0x23a68800,(int32_t)0x85109D00,(int32_t)0xd2aac500,(int32_t)0x884BE900,
(int32_t)0x65ddfb00,(int32_t)0xB27E9E00,(int32_t)0x2223a400,(int32_t)0x84A2FD00,(int32_t)0xd078ad00,(int32_t)0x8926B700,(int32_t)0x6563bf00,(int32_t)0xB1DEFA00,(int32_t)0x209f7000,(int32_t)0x843A1E00,(int32_t)0xce4ab500,(int32_t)0x8A0BD400,
(int32_t)0x64e88900,(int32_t)0xB1401800,(int32_t)0x1f19f900,(int32_t)0x83D60500,(int32_t)0xcc210d00,(int32_t)0x8AFB2D00,(int32_t)0x646c5900,(int32_t)0xB0A1F800,(int32_t)0x1d934f00,(int32_t)0x8376B500,(int32_t)0xc9fbe500,(int32_t)0x8BF4AD00,
(int32_t)0x63ef3200,(int32_t)0xB0049B00,(int32_t)0x1c0b8200,(int32_t)0x831C3200,(int32_t)0xc7db6c00,(int32_t)0x8CF83D00,(int32_t)0x63711400,(int32_t)0xAF680400,(int32_t)0x1a82a000,(int32_t)0x82C68000,(int32_t)0xc5bfd200,(int32_t)0x8E05C700,
(int32_t)0x62f20100,(int32_t)0xAECC3400,(int32_t)0x18f8b800,(int32_t)0x8275A100,(int32_t)0xc3a94500,(int32_t)0x8F1D3500,(int32_t)0x6271fa00,(int32_t)0xAE312C00,(int32_t)0x176dd900,(int32_t)0x82299A00,(int32_t)0xc197f400,(int32_t)0x903E6D00,
(int32_t)0x61f10000,(int32_t)0xAD96EE00,(int32_t)0x15e21400,(int32_t)0x81E26D00,(int32_t)0xbf8c0d00,(int32_t)0x91695700,(int32_t)0x616f1400,(int32_t)0xACFD7B00,(int32_t)0x14557600,(int32_t)0x81A01C00,(int32_t)0xbd85be00,(int32_t)0x929DD900,
(int32_t)0x60ec3800,(int32_t)0xAC64D600,(int32_t)0x12c81000,(int32_t)0x8162AB00,(int32_t)0xbb853200,(int32_t)0x93DBD700,(int32_t)0x60686c00,(int32_t)0xABCCFE00,(int32_t)0x1139f000,(int32_t)0x812A1B00,(int32_t)0xb98a9700,(int32_t)0x95233700,
(int32_t)0x5fe3b300,(int32_t)0xAB35F600,(int32_t)0x0fab2700,(int32_t)0x80F66F00,(int32_t)0xb7961900,(int32_t)0x9673DC00,(int32_t)0x5f5e0d00,(int32_t)0xAA9FC000,(int32_t)0x0e1bc200,(int32_t)0x80C7A900,(int32_t)0xb5a7e300,(int32_t)0x97CDA900,
(int32_t)0x5ed77c00,(int32_t)0xAA0A5C00,(int32_t)0x0c8bd300,(int32_t)0x809DCA00,(int32_t)0xb3c02000,(int32_t)0x99307F00,(int32_t)0x5e500100,(int32_t)0xA975CC00,(int32_t)0x0afb6800,(int32_t)0x8078D500,(int32_t)0xb1def900,(int32_t)0x9A9C4100,
(int32_t)0x5dc79d00,(int32_t)0xA8E21200,(int32_t)0x096a9000,(int32_t)0x8058CA00,(int32_t)0xb0049a00,(int32_t)0x9C10CE00,(int32_t)0x5d3e5200,(int32_t)0xA84F2E00,(int32_t)0x07d95b00,(int32_t)0x803DAB00,(int32_t)0xae312b00,(int32_t)0x9D8E0600,
(int32_t)0x5cb42000,(int32_t)0xA7BD2300,(int32_t)0x0647d900,(int32_t)0x80277900,(int32_t)0xac64d500,(int32_t)0x9F13C800,(int32_t)0x5c290a00,(int32_t)0xA72BF200,(int32_t)0x04b61900,(int32_t)0x80163500,(int32_t)0xaa9fbf00,(int32_t)0xA0A1F300,
(int32_t)0x5b9d1100,(int32_t)0xA69B9C00,(int32_t)0x03242a00,(int32_t)0x8009DF00,(int32_t)0xa8e21100,(int32_t)0xA2386300,(int32_t)0x5b103500,(int32_t)0xA60C2200,(int32_t)0x01921d00,(int32_t)0x80027800,(int32_t)0xa72bf100,(int32_t)0xA3D6F600,
(int32_t)0x5a827900,(int32_t)0xA57D8700,(int32_t)0x00000000,(int32_t)0x80000000,(int32_t)0xa57d8600,(int32_t)0xA57D8700,(int32_t)0x59f3de00,(int32_t)0xA4EFCB00,(int32_t)0xfe6de200,(int32_t)0x80027800,(int32_t)0xa3d6f500,(int32_t)0xA72BF200,
(int32_t)0x59646400,(int32_t)0xA462EF00,(int32_t)0xfcdbd500,(int32_t)0x8009DF00,(int32_t)0xa2386200,(int32_t)0xA8E21200,(int32_t)0x58d40e00,(int32_t)0xA3D6F600,(int32_t)0xfb49e600,(int32_t)0x80163500,(int32_t)0xa0a1f200,(int32_t)0xAA9FC000,
(int32_t)0x5842dd00,(int32_t)0xA34BE000,(int32_t)0xf9b82600,(int32_t)0x80277900,(int32_t)0x9f13c700,(int32_t)0xAC64D600,(int32_t)0x57b0d200,(int32_t)0xA2C1AE00,(int32_t)0xf826a400,(int32_t)0x803DAB00,(int32_t)0x9d8e0500,(int32_t)0xAE312C00,
(int32_t)0x571dee00,(int32_t)0xA2386300,(int32_t)0xf6956f00,(int32_t)0x8058CA00,(int32_t)0x9c10cd00,(int32_t)0xB0049B00,(int32_t)0x568a3400,(int32_t)0xA1AFFF00,(int32_t)0xf5049700,(int32_t)0x8078D500,(int32_t)0x9a9c4000,(int32_t)0xB1DEFA00,
(int32_t)0x55f5a400,(int32_t)0xA1288400,(int32_t)0xf3742c00,(int32_t)0x809DCA00,(int32_t)0x99307e00,(int32_t)0xB3C02100,(int32_t)0x55604000,(int32_t)0xA0A1F300,(int32_t)0xf1e43d00,(int32_t)0x80C7A900,(int32_t)0x97cda800,(int32_t)0xB5A7E400,
(int32_t)0x54ca0a00,(int32_t)0xA01C4D00,(int32_t)0xf054d800,(int32_t)0x80F66F00,(int32_t)0x9673db00,(int32_t)0xB7961A00,(int32_t)0x54330200,(int32_t)0x9F979400,(int32_t)0xeec60f00,(int32_t)0x812A1B00,(int32_t)0x95233600,(int32_t)0xB98A9800,
(int32_t)0x539b2a00,(int32_t)0x9F13C800,(int32_t)0xed37ef00,(int32_t)0x8162AB00,(int32_t)0x93dbd600,(int32_t)0xBB853300,(int32_t)0x53028500,(int32_t)0x9E90EC00,(int32_t)0xebaa8900,(int32_t)0x81A01C00,(int32_t)0x929dd800,(int32_t)0xBD85BF00,
(int32_t)0x52691200,(int32_t)0x9E0F0000,(int32_t)0xea1deb00,(int32_t)0x81E26D00,(int32_t)0x91695600,(int32_t)0xBF8C0E00,(int32_t)0x51ced400,(int32_t)0x9D8E0600,(int32_t)0xe8922600,(int32_t)0x82299A00,(int32_t)0x903e6c00,(int32_t)0xC197F500,
(int32_t)0x5133cc00,(int32_t)0x9D0DFF00,(int32_t)0xe7074700,(int32_t)0x8275A100,(int32_t)0x8f1d3400,(int32_t)0xC3A94600,(int32_t)0x5097fc00,(int32_t)0x9C8EEC00,(int32_t)0xe57d5f00,(int32_t)0x82C68000,(int32_t)0x8e05c600,(int32_t)0xC5BFD300,
(int32_t)0x4ffb6500,(int32_t)0x9C10CE00,(int32_t)0xe3f47d00,(int32_t)0x831C3200,(int32_t)0x8cf83c00,(int32_t)0xC7DB6D00,(int32_t)0x4f5e0800,(int32_t)0x9B93A700,(int32_t)0xe26cb000,(int32_t)0x8376B500,(int32_t)0x8bf4ac00,(int32_t)0xC9FBE600,
(int32_t)0x4ebfe800,(int32_t)0x9B177700,(int32_t)0xe0e60600,(int32_t)0x83D60500,(int32_t)0x8afb2c00,(int32_t)0xCC210E00,(int32_t)0x4e210600,(int32_t)0x9A9C4100,(int32_t)0xdf608f00,(int32_t)0x843A1E00,(int32_t)0x8a0bd300,(int32_t)0xCE4AB600,
(int32_t)0x4d816200,(int32_t)0x9A220500,(int32_t)0xdddc5b00,(int32_t)0x84A2FD00,(int32_t)0x8926b600,(int32_t)0xD078AE00,(int32_t)0x4ce10000,(int32_t)0x99A8C400,(int32_t)0xdc597700,(int32_t)0x85109D00,(int32_t)0x884be800,(int32_t)0xD2AAC600,
(int32_t)0x4c3fdf00,(int32_t)0x99307F00,(int32_t)0xdad7f300,(int32_t)0x8582FB00,(int32_t)0x877b7b00,(int32_t)0xD4E0CC00,(int32_t)0x4b9e0300,(int32_t)0x98B93900,(int32_t)0xd957de00,(int32_t)0x85FA1200,(int32_t)0x86b58300,(int32_t)0xD71A8F00,
(int32_t)0x4afb6c00,(int32_t)0x9842F100,(int32_t)0xd7d94600,(int32_t)0x8675DD00,(int32_t)0x85fa1100,(int32_t)0xD957DF00,(int32_t)0x4a581c00,(int32_t)0x97CDA900,(int32_t)0xd65c3b00,(int32_t)0x86F65700,(int32_t)0x85493400,(int32_t)0xDB988900,
(int32_t)0x49b41500,(int32_t)0x97596200,(int32_t)0xd4e0cb00,(int32_t)0x877B7C00,(int32_t)0x84a2fc00,(int32_t)0xDDDC5C00,(int32_t)0x490f5700,(int32_t)0x96E61D00,(int32_t)0xd3670400,(int32_t)0x88054700,(int32_t)0x84077700,(int32_t)0xE0232400,
(int32_t)0x4869e600,(int32_t)0x9673DC00,(int32_t)0xd1eef500,(int32_t)0x8893B200,(int32_t)0x8376b400,(int32_t)0xE26CB100,(int32_t)0x47c3c200,(int32_t)0x96029F00,(int32_t)0xd078ad00,(int32_t)0x8926B700,(int32_t)0x82f0bd00,(int32_t)0xE4B8CE00,
(int32_t)0x471cec00,(int32_t)0x95926800,(int32_t)0xcf043a00,(int32_t)0x89BE5100,(int32_t)0x8275a000,(int32_t)0xE7074800,(int32_t)0x46756800,(int32_t)0x95233700,(int32_t)0xcd91ab00,(int32_t)0x8A5A7B00,(int32_t)0x82056700,(int32_t)0xE957ED00,
(int32_t)0x45cd3500,(int32_t)0x94B50E00,(int32_t)0xcc210d00,(int32_t)0x8AFB2D00,(int32_t)0x81a01b00,(int32_t)0xEBAA8A00,(int32_t)0x45245600,(int32_t)0x9447EE00,(int32_t)0xcab26f00,(int32_t)0x8BA06300,(int32_t)0x8145c500,(int32_t)0xEDFEEA00,
(int32_t)0x447acd00,(int32_t)0x93DBD700,(int32_t)0xc945df00,(int32_t)0x8C4A1500,(int32_t)0x80f66e00,(int32_t)0xF054D900,(int32_t)0x43d09a00,(int32_t)0x9370CB00,(int32_t)0xc7db6c00,(int32_t)0x8CF83D00,(int32_t)0x80b21b00,(int32_t)0xF2AC2500,
(int32_t)0x4325c100,(int32_t)0x9306CC00,(int32_t)0xc6732200,(int32_t)0x8DAAD400,(int32_t)0x8078d400,(int32_t)0xF5049800,(int32_t)0x427a4100,(int32_t)0x929DD900,(int32_t)0xc50d1100,(int32_t)0x8E61D400,(int32_t)0x804a9c00,(int32_t)0xF75E0000,
(int32_t)0x41ce1e00,(int32_t)0x9235F300,(int32_t)0xc3a94500,(int32_t)0x8F1D3500,(int32_t)0x80277800,(int32_t)0xF9B82700,(int32_t)0x41215800,(int32_t)0x91CF1D00,(int32_t)0xc247cd00,(int32_t)0x8FDCF000,(int32_t)0x800f6b00,(int32_t)0xFC12DA00,
(int32_t)0x4073f200,(int32_t)0x91695700,(int32_t)0xc0e8b600,(int32_t)0x90A0FE00,(int32_t)0x80027700,(int32_t)0xFE6DE300,(int32_t)0x3fc5ec00,(int32_t)0x9104A100,(int32_t)0xbf8c0d00,(int32_t)0x91695700,(int32_t)0x80009d00,(int32_t)0x00C91000,
(int32_t)0x3f174900,(int32_t)0x90A0FE00,(int32_t)0xbe31e100,(int32_t)0x9235F300,(int32_t)0x8009de00,(int32_t)0x03242B00,(int32_t)0x3e680b00,(int32_t)0x903E6D00,(int32_t)0xbcda3e00,(int32_t)0x9306CC00,(int32_t)0x801e3800,(int32_t)0x057F0100,
(int32_t)0x3db83200,(int32_t)0x8FDCF000,(int32_t)0xbb853200,(int32_t)0x93DBD700,(int32_t)0x803daa00,(int32_t)0x07D95C00,(int32_t)0x3d07c100,(int32_t)0x8F7C8800,(int32_t)0xba32ca00,(int32_t)0x94B50E00,(int32_t)0x80683100,(int32_t)0x0A330900,
(int32_t)0x3c56ba00,(int32_t)0x8F1D3500,(int32_t)0xb8e31300,(int32_t)0x95926800,(int32_t)0x809dc900,(int32_t)0x0C8BD400,(int32_t)0x3ba51e00,(int32_t)0x8EBEF800,(int32_t)0xb7961900,(int32_t)0x9673DC00,(int32_t)0x80de6e00,(int32_t)0x0EE38800,
(int32_t)0x3af2ee00,(int32_t)0x8E61D400,(int32_t)0xb64bea00,(int32_t)0x97596200,(int32_t)0x812a1a00,(int32_t)0x1139F100,(int32_t)0x3a402d00,(int32_t)0x8E05C700,(int32_t)0xb5049300,(int32_t)0x9842F100,(int32_t)0x8180c600,(int32_t)0x138EDC00,
(int32_t)0x398cdd00,(int32_t)0x8DAAD400,(int32_t)0xb3c02000,(int32_t)0x99307F00,(int32_t)0x81e26c00,(int32_t)0x15E21500,(int32_t)0x38d8fe00,(int32_t)0x8D50FB00,(int32_t)0xb27e9d00,(int32_t)0x9A220500,(int32_t)0x824f0200,(int32_t)0x18336700,
(int32_t)0x38249300,(int32_t)0x8CF83D00,(int32_t)0xb1401700,(int32_t)0x9B177700,(int32_t)0x82c67f00,(int32_t)0x1A82A100,(int32_t)0x376f9e00,(int32_t)0x8CA09A00,(int32_t)0xb0049a00,(int32_t)0x9C10CE00,(int32_t)0x8348d800,(int32_t)0x1CCF8D00,
(int32_t)0x36ba2000,(int32_t)0x8C4A1500,(int32_t)0xaecc3300,(int32_t)0x9D0DFF00,(int32_t)0x83d60400,(int32_t)0x1F19FA00,(int32_t)0x36041a00,(int32_t)0x8BF4AD00,(int32_t)0xad96ed00,(int32_t)0x9E0F0000,(int32_t)0x846df400,(int32_t)0x2161B400,
(int32_t)0x354d9000,(int32_t)0x8BA06300,(int32_t)0xac64d500,(int32_t)0x9F13C800,(int32_t)0x85109c00,(int32_t)0x23A68900,(int32_t)0x34968200,(int32_t)0x8B4D3800,(int32_t)0xab35f500,(int32_t)0xA01C4D00,(int32_t)0x85bdef00,(int32_t)0x25E84600,
(int32_t)0x33def200,(int32_t)0x8AFB2D00,(int32_t)0xaa0a5b00,(int32_t)0xA1288400,(int32_t)0x8675dc00,(int32_t)0x2826BA00,(int32_t)0x3326e200,(int32_t)0x8AAA4300,(int32_t)0xa8e21100,(int32_t)0xA2386300,(int32_t)0x87385400,(int32_t)0x2A61B200,
(int32_t)0x326e5400,(int32_t)0x8A5A7B00,(int32_t)0xa7bd2200,(int32_t)0xA34BE000,(int32_t)0x88054600,(int32_t)0x2C98FC00,(int32_t)0x31b54a00,(int32_t)0x8A0BD400,(int32_t)0xa69b9b00,(int32_t)0xA462EF00,(int32_t)0x88dca000,(int32_t)0x2ECC6900,
(int32_t)0x30fbc500,(int32_t)0x89BE5100,(int32_t)0xa57d8600,(int32_t)0xA57D8700,(int32_t)0x89be5000,(int32_t)0x30FBC600,(int32_t)0x3041c700,(int32_t)0x8971F200,(int32_t)0xa462ee00,(int32_t)0xA69B9C00,(int32_t)0x8aaa4200,(int32_t)0x3326E300,
(int32_t)0x2f875200,(int32_t)0x8926B700,(int32_t)0xa34bdf00,(int32_t)0xA7BD2300,(int32_t)0x8ba06200,(int32_t)0x354D9100,(int32_t)0x2ecc6800,(int32_t)0x88DCA100,(int32_t)0xa2386200,(int32_t)0xA8E21200,(int32_t)0x8ca09900,(int32_t)0x376F9F00,
(int32_t)0x2e110a00,(int32_t)0x8893B200,(int32_t)0xa1288300,(int32_t)0xAA0A5C00,(int32_t)0x8daad300,(int32_t)0x398CDE00,(int32_t)0x2d553a00,(int32_t)0x884BE900,(int32_t)0xa01c4c00,(int32_t)0xAB35F600,(int32_t)0x8ebef700,(int32_t)0x3BA51F00,
(int32_t)0x2c98fb00,(int32_t)0x88054700,(int32_t)0x9f13c700,(int32_t)0xAC64D600,(int32_t)0x8fdcef00,(int32_t)0x3DB83300,(int32_t)0x2bdc4e00,(int32_t)0x87BFCD00,(int32_t)0x9e0eff00,(int32_t)0xAD96EE00,(int32_t)0x9104a000,(int32_t)0x3FC5ED00,
(int32_t)0x2b1f3400,(int32_t)0x877B7C00,(int32_t)0x9d0dfe00,(int32_t)0xAECC3400,(int32_t)0x9235f200,(int32_t)0x41CE1F00,(int32_t)0x2a61b100,(int32_t)0x87385500,(int32_t)0x9c10cd00,(int32_t)0xB0049B00,(int32_t)0x9370ca00,(int32_t)0x43D09B00,
(int32_t)0x29a3c400,(int32_t)0x86F65700,(int32_t)0x9b177600,(int32_t)0xB1401800,(int32_t)0x94b50d00,(int32_t)0x45CD3600,(int32_t)0x28e57100,(int32_t)0x86B58400,(int32_t)0x9a220400,(int32_t)0xB27E9E00,(int32_t)0x96029e00,(int32_t)0x47C3C300,
(int32_t)0x2826b900,(int32_t)0x8675DD00,(int32_t)0x99307e00,(int32_t)0xB3C02100,(int32_t)0x97596100,(int32_t)0x49B41600,(int32_t)0x27679d00,(int32_t)0x86376100,(int32_t)0x9842f000,(int32_t)0xB5049400,(int32_t)0x98b93800,(int32_t)0x4B9E0400,
(int32_t)0x26a82100,(int32_t)0x85FA1200,(int32_t)0x97596100,(int32_t)0xB64BEB00,(int32_t)0x9a220400,(int32_t)0x4D816300,(int32_t)0x25e84500,(int32_t)0x85BDF000,(int32_t)0x9673db00,(int32_t)0xB7961A00,(int32_t)0x9b93a600,(int32_t)0x4F5E0900,
(int32_t)0x25280c00,(int32_t)0x8582FB00,(int32_t)0x95926700,(int32_t)0xB8E31400,(int32_t)0x9d0dfe00,(int32_t)0x5133CD00,(int32_t)0x24677700,(int32_t)0x85493500,(int32_t)0x94b50d00,(int32_t)0xBA32CB00,(int32_t)0x9e90eb00,(int32_t)0x53028600,
(int32_t)0x23a68800,(int32_t)0x85109D00,(int32_t)0x93dbd600,(int32_t)0xBB853300,(int32_t)0xa01c4c00,(int32_t)0x54CA0B00,(int32_t)0x22e54100,(int32_t)0x84D93500,(int32_t)0x9306cb00,(int32_t)0xBCDA3F00,(int32_t)0xa1affe00,(int32_t)0x568A3500,
(int32_t)0x2223a400,(int32_t)0x84A2FD00,(int32_t)0x9235f200,(int32_t)0xBE31E200,(int32_t)0xa34bdf00,(int32_t)0x5842DE00,(int32_t)0x2161b300,(int32_t)0x846DF500,(int32_t)0x91695600,(int32_t)0xBF8C0E00,(int32_t)0xa4efca00,(int32_t)0x59F3DF00,
(int32_t)0x209f7000,(int32_t)0x843A1E00,(int32_t)0x90a0fd00,(int32_t)0xC0E8B700,(int32_t)0xa69b9b00,(int32_t)0x5B9D1200,(int32_t)0x1fdcdc00,(int32_t)0x84077800,(int32_t)0x8fdcef00,(int32_t)0xC247CE00,(int32_t)0xa84f2d00,(int32_t)0x5D3E5300,
(int32_t)0x1f19f900,(int32_t)0x83D60500,(int32_t)0x8f1d3400,(int32_t)0xC3A94600,(int32_t)0xaa0a5b00,(int32_t)0x5ED77D00,(int32_t)0x1e56ca00,(int32_t)0x83A5C300,(int32_t)0x8e61d300,(int32_t)0xC50D1200,(int32_t)0xabccfd00,(int32_t)0x60686D00,
(int32_t)0x1d934f00,(int32_t)0x8376B500,(int32_t)0x8daad300,(int32_t)0xC6732300,(int32_t)0xad96ed00,(int32_t)0x61F10100,(int32_t)0x1ccf8c00,(int32_t)0x8348D900,(int32_t)0x8cf83c00,(int32_t)0xC7DB6D00,(int32_t)0xaf680300,(int32_t)0x63711500,
(int32_t)0x1c0b8200,(int32_t)0x831C3200,(int32_t)0x8c4a1400,(int32_t)0xC945E000,(int32_t)0xb1401700,(int32_t)0x64E88A00,(int32_t)0x1b473200,(int32_t)0x82F0BE00,(int32_t)0x8ba06200,(int32_t)0xCAB27000,(int32_t)0xb31eff00,(int32_t)0x66573D00,
(int32_t)0x1a82a000,(int32_t)0x82C68000,(int32_t)0x8afb2c00,(int32_t)0xCC210E00,(int32_t)0xb5049300,(int32_t)0x67BD1000,(int32_t)0x19bdcb00,(int32_t)0x829D7600,(int32_t)0x8a5a7a00,(int32_t)0xCD91AC00,(int32_t)0xb6f0a800,(int32_t)0x6919E400,
(int32_t)0x18f8b800,(int32_t)0x8275A100,(int32_t)0x89be5000,(int32_t)0xCF043B00,(int32_t)0xb8e31300,(int32_t)0x6A6D9900,(int32_t)0x18336600,(int32_t)0x824F0300,(int32_t)0x8926b600,(int32_t)0xD078AE00,(int32_t)0xbadba900,(int32_t)0x6BB81300,
(int32_t)0x176dd900,(int32_t)0x82299A00,(int32_t)0x8893b100,(int32_t)0xD1EEF600,(int32_t)0xbcda3e00,(int32_t)0x6CF93500,(int32_t)0x16a81300,(int32_t)0x82056800,(int32_t)0x88054600,(int32_t)0xD3670500,(int32_t)0xbedea700,(int32_t)0x6E30E400,
(int32_t)0x15e21400,(int32_t)0x81E26D00,(int32_t)0x877b7b00,(int32_t)0xD4E0CC00,(int32_t)0xc0e8b600,(int32_t)0x6F5F0300,(int32_t)0x151bdf00,(int32_t)0x81C0A900,(int32_t)0x86f65600,(int32_t)0xD65C3C00,(int32_t)0xc2f83e00,(int32_t)0x70837900,
(int32_t)0x14557600,(int32_t)0x81A01C00,(int32_t)0x8675dc00,(int32_t)0xD7D94700,(int32_t)0xc50d1100,(int32_t)0x719E2D00,(int32_t)0x138edb00,(int32_t)0x8180C700,(int32_t)0x85fa1100,(int32_t)0xD957DF00,(int32_t)0xc7270100,(int32_t)0x72AF0600,
(int32_t)0x12c81000,(int32_t)0x8162AB00,(int32_t)0x8582fa00,(int32_t)0xDAD7F400,(int32_t)0xc945df00,(int32_t)0x73B5EC00,(int32_t)0x12011600,(int32_t)0x8145C600,(int32_t)0x85109c00,(int32_t)0xDC597800,(int32_t)0xcb697d00,(int32_t)0x74B2C900,
(int32_t)0x1139f000,(int32_t)0x812A1B00,(int32_t)0x84a2fc00,(int32_t)0xDDDC5C00,(int32_t)0xcd91ab00,(int32_t)0x75A58600,(int32_t)0x1072a000,(int32_t)0x810FA800,(int32_t)0x843a1d00,(int32_t)0xDF609000,(int32_t)0xcfbe3800,(int32_t)0x768E0F00,
(int32_t)0x0fab2700,(int32_t)0x80F66F00,(int32_t)0x83d60400,(int32_t)0xE0E60700,(int32_t)0xd1eef500,(int32_t)0x776C4F00,(int32_t)0x0ee38700,(int32_t)0x80DE6F00,(int32_t)0x8376b400,(int32_t)0xE26CB100,(int32_t)0xd423b100,(int32_t)0x78403400,
(int32_t)0x0e1bc200,(int32_t)0x80C7A900,(int32_t)0x831c3100,(int32_t)0xE3F47E00,(int32_t)0xd65c3b00,(int32_t)0x7909AA00,(int32_t)0x0d53db00,(int32_t)0x80B21C00,(int32_t)0x82c67f00,(int32_t)0xE57D6000,(int32_t)0xd8986200,(int32_t)0x79C8A000,
(int32_t)0x0c8bd300,(int32_t)0x809DCA00,(int32_t)0x8275a000,(int32_t)0xE7074800,(int32_t)0xdad7f300,(int32_t)0x7A7D0600,(int32_t)0x0bc3ac00,(int32_t)0x808AB200,(int32_t)0x82299900,(int32_t)0xE8922700,(int32_t)0xdd1abe00,(int32_t)0x7B26CC00,
(int32_t)0x0afb6800,(int32_t)0x8078D500,(int32_t)0x81e26c00,(int32_t)0xEA1DEC00,(int32_t)0xdf608f00,(int32_t)0x7BC5E300,(int32_t)0x0a330800,(int32_t)0x80683200,(int32_t)0x81a01b00,(int32_t)0xEBAA8A00,(int32_t)0xe1a93500,(int32_t)0x7C5A3E00,
(int32_t)0x096a9000,(int32_t)0x8058CA00,(int32_t)0x8162aa00,(int32_t)0xED37F000,(int32_t)0xe3f47d00,(int32_t)0x7CE3CF00,(int32_t)0x08a20000,(int32_t)0x804A9D00,(int32_t)0x812a1a00,(int32_t)0xEEC61000,(int32_t)0xe6423400,(int32_t)0x7D628B00,
(int32_t)0x07d95b00,(int32_t)0x803DAB00,(int32_t)0x80f66e00,(int32_t)0xF054D900,(int32_t)0xe8922600,(int32_t)0x7DD66700,(int32_t)0x0710a300,(int32_t)0x8031F400,(int32_t)0x80c7a800,(int32_t)0xF1E43E00,(int32_t)0xeae42000,(int32_t)0x7E3F5800,
(int32_t)0x0647d900,(int32_t)0x80277900,(int32_t)0x809dc900,(int32_t)0xF3742D00,(int32_t)0xed37ef00,(int32_t)0x7E9D5600,(int32_t)0x057f0000,(int32_t)0x801E3900,(int32_t)0x8078d400,(int32_t)0xF5049800,(int32_t)0xef8d5f00,(int32_t)0x7EF05900,
(int32_t)0x04b61900,(int32_t)0x80163500,(int32_t)0x8058c900,(int32_t)0xF6957000,(int32_t)0xf1e43d00,(int32_t)0x7F385800,(int32_t)0x03ed2600,(int32_t)0x800F6C00,(int32_t)0x803daa00,(int32_t)0xF826A500,(int32_t)0xf43c5300,(int32_t)0x7F754F00,
(int32_t)0x03242a00,(int32_t)0x8009DF00,(int32_t)0x80277800,(int32_t)0xF9B82700,(int32_t)0xf6956f00,(int32_t)0x7FA73700,(int32_t)0x025b2600,(int32_t)0x80058E00,(int32_t)0x80163400,(int32_t)0xFB49E700,(int32_t)0xf8ef5c00,(int32_t)0x7FCE0D00,
(int32_t)0x01921d00,(int32_t)0x80027800,(int32_t)0x8009de00,(int32_t)0xFCDBD600,(int32_t)0xfb49e600,(int32_t)0x7FE9CC00,(int32_t)0x00c90f00,(int32_t)0x80009E00,(int32_t)0x80027700,(int32_t)0xFE6DE300,(int32_t)0xfda4d900,(int32_t)0x7FFA7300
};
static const tFftDescr descr={  1024,fft_inc1024,(cint32_ptr)fft24x24_twd1024};
const fft_handle_t cfft24_1024 = (fft_handle_t)&descr;
const fft_handle_t rfft24_2048 = (fft_handle_t)&descr;
