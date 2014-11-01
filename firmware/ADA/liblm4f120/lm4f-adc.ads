-----------------------------------------------------------------------
--
-- Copyright (C) 2014 Ali Paikan (ali.paikan@gmail.com).  
--
--   Redistribution and use in source and binary forms, with or without
--   modification, are permitted provided that the following conditions
--   are met:
-- 
--   Redistributions of source code must retain the above copyright
--   notice, this list of conditions and the following disclaimer.
-- 
--   Redistributions in binary form must reproduce the above copyright
--   notice, this list of conditions and the following disclaimer in the
--   documentation and/or other materials provided with the  
--   distribution.
-- 
--   Neither the name of Texas Instruments Incorporated nor the names of
--   its contributors may be used to endorse or promote products derived
--   from this software without specific prior written permission.
-- 
-- THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
-- "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
-- LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
-- A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
-- OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
-- SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
-- LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
-- DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
-- THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
-- (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
-- OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
-- 
-------------------------------------------------------------------------

with Interfaces;    use Interfaces;
with Interfaces.C;  use Interfaces.C;
with Ada.Unchecked_Conversion;

package LM4F.ADC is

   ADC0_BASE : constant := 16#40038000#;  --  hw_memmap.h:97
   ADC1_BASE : constant := 16#40039000#;  --  hw_memmap.h:98
   ADC_TRIGGER_PROCESSOR : constant := 16#00000000#;  --  adc.h:65
   ADC_TRIGGER_NEVER : constant  := 16#0000000E#;  --  adc.h:75
   ADC_TRIGGER_ALWAYS : constant := 16#0000000F#;  --  adc.h:76
   ADC_CTL_TS : constant  := 16#00000080#;  --  adc.h:84
   ADC_CTL_IE : constant  := 16#00000040#;  --  adc.h:85
   ADC_CTL_END : constant := 16#00000020#;  --  adc.h:86
   ADC_CTL_D : constant := 16#00000010#;  --  adc.h:87
   ADC_CTL_CH0 : constant := 16#00000000#;  --  adc.h:88
   ADC_CTL_CH1 : constant := 16#00000001#;  --  adc.h:89
   ADC_CTL_CH2 : constant := 16#00000002#;  --  adc.h:90
   ADC_CTL_CH3 : constant := 16#00000003#;  --  adc.h:91
   ADC_CTL_CH4 : constant := 16#00000004#;  --  adc.h:92
   ADC_CTL_CH5 : constant := 16#00000005#;  --  adc.h:93
   ADC_CTL_CH6 : constant := 16#00000006#;  --  adc.h:94
   ADC_CTL_CH7 : constant := 16#00000007#;  --  adc.h:95
   ADC_CTL_CH8 : constant := 16#00000008#;  --  adc.h:96
   ADC_CTL_CH9 : constant := 16#00000009#;  --  adc.h:97
   ADC_CTL_CH10 : constant := 16#0000000A#;  --  adc.h:98
   ADC_CTL_CH11 : constant := 16#0000000B#;  --  adc.h:99
   ADC_CTL_CH12 : constant := 16#0000000C#;  --  adc.h:100
--   ADC_CTL_CH13 : constant := 16#0000000D#;  --  adc.h:101
--   ADC_CTL_CH14 : constant := 16#0000000E#;  --  adc.h:102
--   ADC_CTL_CH15 : constant := 16#0000000F#;  --  adc.h:103
--   ADC_CTL_CH16 : constant := 16#00000100#;  --  adc.h:104
--   ADC_CTL_CH17 : constant := 16#00000101#;  --  adc.h:105
--   ADC_CTL_CH18 : constant := 16#00000102#;  --  adc.h:106
--   ADC_CTL_CH19 : constant := 16#00000103#;  --  adc.h:107
--   ADC_CTL_CH20 : constant := 16#00000104#;  --  adc.h:108
--   ADC_CTL_CH21 : constant := 16#00000105#;  --  adc.h:109
--   ADC_CTL_CH22 : constant := 16#00000106#;  --  adc.h:110
--   ADC_CTL_CH23 : constant := 16#00000107#;  --  adc.h:111
--   ADC_CTL_CMP0 : constant := 16#00080000#;  --  adc.h:112
--   ADC_CTL_CMP1 : constant := 16#00090000#;  --  adc.h:113
--   ADC_CTL_CMP2 : constant := 16#000A0000#;  --  adc.h:114
--   ADC_CTL_CMP3 : constant := 16#000B0000#;  --  adc.h:115
--   ADC_CTL_CMP4 : constant := 16#000C0000#;  --  adc.h:116
--   ADC_CTL_CMP5 : constant := 16#000D0000#;  --  adc.h:117
--   ADC_CTL_CMP6 : constant := 16#000E0000#;  --  adc.h:118
--   ADC_CTL_CMP7 : constant := 16#000F0000#;  --  adc.h:119

   procedure SequenceConfigure
     (Base : Integer;
      SequenceNum : Integer;
      Trigger : Integer;
      Priority : Integer);  -- adc.h:241
   pragma Import (C, SequenceConfigure, "ADCSequenceConfigure");

   procedure SequenceStepConfigure
     (Base : Integer;
      SequenceNum : Integer;
      Step : Integer;
      Config : Integer);  -- adc.h:243
   pragma Import (C, SequenceStepConfigure, "ADCSequenceStepConfigure");

   procedure SequenceEnable (Base : Integer; SequenceNum : Integer);  -- adc.h:239
   pragma Import (C, SequenceEnable, "ADCSequenceEnable");

   procedure SequenceDisable (Base : Integer; SequenceNum : Integer);  -- adc.h:240
   pragma Import (C, SequenceDisable, "ADCSequenceDisable");

   procedure IntClear (Base : Integer; SequenceNum : Integer);  -- adc.h:238
   pragma Import (C, IntClear, "ADCIntClear");

   procedure ProcessorTrigger (Base : Integer; SequenceNum : Integer);  -- adc.h:256
   pragma Import (C, ProcessorTrigger, "ADCProcessorTrigger");

   function IntStatus
     (Base : Integer;
      SequenceNum : Integer;
      bMasked : Integer) return Integer;  -- adc.h:236
   pragma Import (C, IntStatus, "ADCIntStatus");

   function SequenceDataGet
     (Base : Integer;
      SequenceNum : Integer;
      pBuffer : access Integer) return Integer;  -- adc.h:254
   pragma Import (C, SequenceDataGet, "ADCSequenceDataGet");


end LM4F.ADC;





