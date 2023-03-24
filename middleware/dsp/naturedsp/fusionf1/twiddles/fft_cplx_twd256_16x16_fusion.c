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

    twiddle factors for 16x16 complex fwd/rev FFT transforms
*/

#include "NatureDSP_Signal.h"
#include "fft_cplx_twiddles.h"
#include "common.h"
 // This tables generated by next MATLAB script:
/*
function print_cfft16x16_twd(N, isInv)
% print_cfft_twd(N, format)
% N - fft size, must be power of 2

  format = 'int16_t'; 
  if isInv==0
    fname = sprintf('fft_cplx_twd%d_16x16_fusion.c', N);
  else
    fname = sprintf('ifft_cplx_twd%d_16x16_fusion.c', N);
  end
  f = fopen(fname,'wt');
  %% Print current script to output file
  script_name = strcat(mfilename, '.m'); 
  fs = fopen(script_name, 'r'); 
  txt = fread(fs, inf, 'uint8=>char').';
  txt = regexprep(txt,'\r\n|\n|\r','\n');
  txt = regexprep(txt,' *| ',' ');
  fclose(fs); 
  fprintf(f, '\n   This tables generated by next MATLAB script:\n *\n');
  fprintf(f, '%s', txt); 
  fprintf(f, '\n \n');
  %% Display output file name
  if f>0
      fprintf('write %s\n', fname);
  end
  if isInv==0
    twd = exp(-2j*pi*[1;2;3]*(0:N/4-1)/N);
  else
    twd = exp(2j*pi*[1;2;3]*(0:N/4-1)/N);
  end
  
  %% Calculate twiddles
  tmp3 = reshape(twd(:), 6, numel(twd(:))/6);
  tmp4 = tmp3; 
  tmp4(1:2:end,:) = tmp3(1:end/ 2,:);
  tmp4(2:2:end,:) = tmp3(end/ 2+1:end, :); 
  
  twd = tmp4(:);
  twd_ri = reshape([imag(twd(:).');real(twd(:).')],1,2*numel(twd));
 %% Print values to output file  
   if isInv==0
        fprintf(f, '\nALIGN(8) static const %s _fft_twiddle_table_%d_[%d] = \n{\n', format, N, length(twd_ri));
   else
        fprintf(f, '\nALIGN(8) static const %s _ifft_twiddle_table_%d_[%d] = \n{\n', format, N, length(twd_ri));
   end
  tmp = round(twd_ri*2^15);

  for i=1:4: length(tmp)
    fprintf(f, '\n    ');
    fprintf(f, '(int16_t)0x%04X, ', typecast(int16(tmp(i:i+4-1)), 'uint16')); 
  end
  
  fprintf(f, '\n};\n');
  fclose(f);

*/
#if (XCHAL_HAVE_FUSION_16BIT_BASEBAND)
ALIGN(8)  const int16_t _fft_twiddle_table_256_[384] = 
{

    (int16_t)0x0000, (int16_t)0x7FFF, (int16_t)0xFCDC, (int16_t)0x7FF6, 
    (int16_t)0x0000, (int16_t)0x7FFF, (int16_t)0xF9B8, (int16_t)0x7FD9, 
    (int16_t)0x0000, (int16_t)0x7FFF, (int16_t)0xF695, (int16_t)0x7FA7, 
    (int16_t)0xF9B8, (int16_t)0x7FD9, (int16_t)0xF695, (int16_t)0x7FA7, 
    (int16_t)0xF374, (int16_t)0x7F62, (int16_t)0xED38, (int16_t)0x7E9D, 
    (int16_t)0xED38, (int16_t)0x7E9D, (int16_t)0xE3F4, (int16_t)0x7CE4, 
    (int16_t)0xF374, (int16_t)0x7F62, (int16_t)0xF055, (int16_t)0x7F0A, 
    (int16_t)0xE707, (int16_t)0x7D8A, (int16_t)0xE0E6, (int16_t)0x7C2A, 
    (int16_t)0xDAD8, (int16_t)0x7A7D, (int16_t)0xD1EF, (int16_t)0x776C, 
    (int16_t)0xED38, (int16_t)0x7E9D, (int16_t)0xEA1E, (int16_t)0x7E1E, 
    (int16_t)0xDAD8, (int16_t)0x7A7D, (int16_t)0xD4E1, (int16_t)0x7885, 
    (int16_t)0xC946, (int16_t)0x73B6, (int16_t)0xC0E9, (int16_t)0x6F5F, 
    (int16_t)0xE707, (int16_t)0x7D8A, (int16_t)0xE3F4, (int16_t)0x7CE4, 
    (int16_t)0xCF04, (int16_t)0x7642, (int16_t)0xC946, (int16_t)0x73B6, 
    (int16_t)0xB8E3, (int16_t)0x6A6E, (int16_t)0xB140, (int16_t)0x64E9, 
    (int16_t)0xE0E6, (int16_t)0x7C2A, (int16_t)0xDDDC, (int16_t)0x7B5D, 
    (int16_t)0xC3A9, (int16_t)0x70E3, (int16_t)0xBE32, (int16_t)0x6DCA, 
    (int16_t)0xAA0A, (int16_t)0x5ED7, (int16_t)0xA34C, (int16_t)0x5843, 
    (int16_t)0xDAD8, (int16_t)0x7A7D, (int16_t)0xD7D9, (int16_t)0x798A, 
    (int16_t)0xB8E3, (int16_t)0x6A6E, (int16_t)0xB3C0, (int16_t)0x66D0, 
    (int16_t)0x9D0E, (int16_t)0x5134, (int16_t)0x9759, (int16_t)0x49B4, 
    (int16_t)0xD4E1, (int16_t)0x7885, (int16_t)0xD1EF, (int16_t)0x776C, 
    (int16_t)0xAECC, (int16_t)0x62F2, (int16_t)0xAA0A, (int16_t)0x5ED7, 
    (int16_t)0x9236, (int16_t)0x41CE, (int16_t)0x8DAB, (int16_t)0x398D, 
    (int16_t)0xCF04, (int16_t)0x7642, (int16_t)0xCC21, (int16_t)0x7505, 
    (int16_t)0xA57E, (int16_t)0x5A82, (int16_t)0xA129, (int16_t)0x55F6, 
    (int16_t)0x89BE, (int16_t)0x30FC, (int16_t)0x8676, (int16_t)0x2827, 
    (int16_t)0xC946, (int16_t)0x73B6, (int16_t)0xC673, (int16_t)0x7255, 
    (int16_t)0x9D0E, (int16_t)0x5134, (int16_t)0x9930, (int16_t)0x4C40, 
    (int16_t)0x83D6, (int16_t)0x1F1A, (int16_t)0x81E2, (int16_t)0x15E2, 
    (int16_t)0xC3A9, (int16_t)0x70E3, (int16_t)0xC0E9, (int16_t)0x6F5F, 
    (int16_t)0x9592, (int16_t)0x471D, (int16_t)0x9236, (int16_t)0x41CE, 
    (int16_t)0x809E, (int16_t)0x0C8C, (int16_t)0x800A, (int16_t)0x0324, 
    (int16_t)0xBE32, (int16_t)0x6DCA, (int16_t)0xBB85, (int16_t)0x6C24, 
    (int16_t)0x8F1D, (int16_t)0x3C57, (int16_t)0x8C4A, (int16_t)0x36BA, 
    (int16_t)0x8027, (int16_t)0xF9B8, (int16_t)0x80F6, (int16_t)0xF055, 
    (int16_t)0xB8E3, (int16_t)0x6A6E, (int16_t)0xB64C, (int16_t)0x68A7, 
    (int16_t)0x89BE, (int16_t)0x30FC, (int16_t)0x877B, (int16_t)0x2B1F, 
    (int16_t)0x8276, (int16_t)0xE707, (int16_t)0x84A3, (int16_t)0xDDDC, 
    (int16_t)0xB3C0, (int16_t)0x66D0, (int16_t)0xB140, (int16_t)0x64E9, 
    (int16_t)0x8583, (int16_t)0x2528, (int16_t)0x83D6, (int16_t)0x1F1A, 
    (int16_t)0x877B, (int16_t)0xD4E1, (int16_t)0x8AFB, (int16_t)0xCC21, 
    (int16_t)0xAECC, (int16_t)0x62F2, (int16_t)0xAC65, (int16_t)0x60EC, 
    (int16_t)0x8276, (int16_t)0x18F9, (int16_t)0x8163, (int16_t)0x12C8, 
    (int16_t)0x8F1D, (int16_t)0xC3A9, (int16_t)0x93DC, (int16_t)0xBB85, 
    (int16_t)0xAA0A, (int16_t)0x5ED7, (int16_t)0xA7BD, (int16_t)0x5CB4, 
    (int16_t)0x809E, (int16_t)0x0C8C, (int16_t)0x8027, (int16_t)0x0648, 
    (int16_t)0x9930, (int16_t)0xB3C0, (int16_t)0x9F14, (int16_t)0xAC65, 
    (int16_t)0xA57E, (int16_t)0x5A82, (int16_t)0xA34C, (int16_t)0x5843, 
    (int16_t)0x8000, (int16_t)0x0000, (int16_t)0x8027, (int16_t)0xF9B8, 
    (int16_t)0xA57E, (int16_t)0xA57E, (int16_t)0xAC65, (int16_t)0x9F14, 
    (int16_t)0xA129, (int16_t)0x55F6, (int16_t)0x9F14, (int16_t)0x539B, 
    (int16_t)0x809E, (int16_t)0xF374, (int16_t)0x8163, (int16_t)0xED38, 
    (int16_t)0xB3C0, (int16_t)0x9930, (int16_t)0xBB85, (int16_t)0x93DC, 
    (int16_t)0x9D0E, (int16_t)0x5134, (int16_t)0x9B17, (int16_t)0x4EC0, 
    (int16_t)0x8276, (int16_t)0xE707, (int16_t)0x83D6, (int16_t)0xE0E6, 
    (int16_t)0xC3A9, (int16_t)0x8F1D, (int16_t)0xCC21, (int16_t)0x8AFB, 
    (int16_t)0x9930, (int16_t)0x4C40, (int16_t)0x9759, (int16_t)0x49B4, 
    (int16_t)0x8583, (int16_t)0xDAD8, (int16_t)0x877B, (int16_t)0xD4E1, 
    (int16_t)0xD4E1, (int16_t)0x877B, (int16_t)0xDDDC, (int16_t)0x84A3, 
    (int16_t)0x9592, (int16_t)0x471D, (int16_t)0x93DC, (int16_t)0x447B, 
    (int16_t)0x89BE, (int16_t)0xCF04, (int16_t)0x8C4A, (int16_t)0xC946, 
    (int16_t)0xE707, (int16_t)0x8276, (int16_t)0xF055, (int16_t)0x80F6, 
    (int16_t)0x9236, (int16_t)0x41CE, (int16_t)0x90A1, (int16_t)0x3F17, 
    (int16_t)0x8F1D, (int16_t)0xC3A9, (int16_t)0x9236, (int16_t)0xBE32, 
    (int16_t)0xF9B8, (int16_t)0x8027, (int16_t)0x0324, (int16_t)0x800A, 
    (int16_t)0x8F1D, (int16_t)0x3C57, (int16_t)0x8DAB, (int16_t)0x398D, 
    (int16_t)0x9592, (int16_t)0xB8E3, (int16_t)0x9930, (int16_t)0xB3C0, 
    (int16_t)0x0C8C, (int16_t)0x809E, (int16_t)0x15E2, (int16_t)0x81E2, 
    (int16_t)0x8C4A, (int16_t)0x36BA, (int16_t)0x8AFB, (int16_t)0x33DF, 
    (int16_t)0x9D0E, (int16_t)0xAECC, (int16_t)0xA129, (int16_t)0xAA0A, 
    (int16_t)0x1F1A, (int16_t)0x83D6, (int16_t)0x2827, (int16_t)0x8676, 
    (int16_t)0x89BE, (int16_t)0x30FC, (int16_t)0x8894, (int16_t)0x2E11, 
    (int16_t)0xA57E, (int16_t)0xA57E, (int16_t)0xAA0A, (int16_t)0xA129, 
    (int16_t)0x30FC, (int16_t)0x89BE, (int16_t)0x398D, (int16_t)0x8DAB, 
    (int16_t)0x877B, (int16_t)0x2B1F, (int16_t)0x8676, (int16_t)0x2827, 
    (int16_t)0xAECC, (int16_t)0x9D0E, (int16_t)0xB3C0, (int16_t)0x9930, 
    (int16_t)0x41CE, (int16_t)0x9236, (int16_t)0x49B4, (int16_t)0x9759, 
    (int16_t)0x8583, (int16_t)0x2528, (int16_t)0x84A3, (int16_t)0x2224, 
    (int16_t)0xB8E3, (int16_t)0x9592, (int16_t)0xBE32, (int16_t)0x9236, 
    (int16_t)0x5134, (int16_t)0x9D0E, (int16_t)0x5843, (int16_t)0xA34C, 
    (int16_t)0x83D6, (int16_t)0x1F1A, (int16_t)0x831C, (int16_t)0x1C0C, 
    (int16_t)0xC3A9, (int16_t)0x8F1D, (int16_t)0xC946, (int16_t)0x8C4A, 
    (int16_t)0x5ED7, (int16_t)0xAA0A, (int16_t)0x64E9, (int16_t)0xB140, 
    (int16_t)0x8276, (int16_t)0x18F9, (int16_t)0x81E2, (int16_t)0x15E2, 
    (int16_t)0xCF04, (int16_t)0x89BE, (int16_t)0xD4E1, (int16_t)0x877B, 
    (int16_t)0x6A6E, (int16_t)0xB8E3, (int16_t)0x6F5F, (int16_t)0xC0E9, 
    (int16_t)0x8163, (int16_t)0x12C8, (int16_t)0x80F6, (int16_t)0x0FAB, 
    (int16_t)0xDAD8, (int16_t)0x8583, (int16_t)0xE0E6, (int16_t)0x83D6, 
    (int16_t)0x73B6, (int16_t)0xC946, (int16_t)0x776C, (int16_t)0xD1EF, 
    (int16_t)0x809E, (int16_t)0x0C8C, (int16_t)0x8059, (int16_t)0x096B, 
    (int16_t)0xE707, (int16_t)0x8276, (int16_t)0xED38, (int16_t)0x8163, 
    (int16_t)0x7A7D, (int16_t)0xDAD8, (int16_t)0x7CE4, (int16_t)0xE3F4, 
    (int16_t)0x8027, (int16_t)0x0648, (int16_t)0x800A, (int16_t)0x0324, 
    (int16_t)0xF374, (int16_t)0x809E, (int16_t)0xF9B8, (int16_t)0x8027, 
    (int16_t)0x7E9D, (int16_t)0xED38, (int16_t)0x7FA7, (int16_t)0xF695, 
};
                                            
static const cint32_ptr seqfft_[] = {   (cint32_ptr)_fft_twiddle_table_256_, 
                                        (cint32_ptr)_fft_twiddle_table_64_,
                                        (cint32_ptr)_fft_twiddle_table_16_ };

static const tFftDescr descr = { 256, 0, (cint32_ptr)seqfft_ };

const fft_handle_t cfft16x16_256=(const void*)&descr;
const fft_handle_t rfft16x16_512=(const void*)&descr;
#endif
