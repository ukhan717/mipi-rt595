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
    tables for log2f(x) approximation
*/

#include "NatureDSP_types.h"
#include "log2f_tbl.h"
#include "common.h"

/*
        Generated by Matlab:
        x=(sqrt(0.5):pow2(1,-16):sqrt(2));
        z=1-x;
        y=log(x)./z;
        p=polyfit(z,y,9);
*/
const union ufloat32uint32 ALIGN(8) log2f_tbl[]=
{
    {0xbddec9c5},
    {0xbe3c57b3},
    {0xbe42fd78},
    {0xbe51d13f},
    {0xbe757aae},
    {0xbe93bd93},
    {0xbeb8aca2},
    {0xbef63850},
    {0xbf38aa39},
    {0xbfb8aa3b}
};
