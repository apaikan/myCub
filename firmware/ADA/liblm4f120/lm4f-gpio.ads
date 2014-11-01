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

with Interfaces.C; use Interfaces.C;

package LM4F.Gpio is

   -- LM4F gpio ports
   PORTA : constant := 16#40004000#;
   PORTB : constant := 16#40005000#;
   PORTC : constant := 16#40006000#;
   PORTD : constant := 16#40007000#;
   PORTE : constant := 16#40024000#;
   PORTF : constant := 16#40025000#;
   PORTG : constant := 16#40026000#;
   PORTH : constant := 16#40027000#;

   --LM4F gpio pins
   PIN0 : constant := 16#00000001#; 
   PIN1 : constant := 16#00000002#; 
   PIN2 : constant := 16#00000004#; 
   PIN3 : constant := 16#00000008#; 
   PIN4 : constant := 16#00000010#; 
   PIN5 : constant := 16#00000020#; 
   PIN6 : constant := 16#00000040#; 
   PIN7 : constant := 16#00000080#; 


   procedure PinTypeInput (Port : Integer; Pins : Integer);
   pragma Import (C, PinTypeInput, "GPIOPinTypeGPIOInput");

   procedure PinTypeOutput (Port : Integer; Pins : Integer);
   pragma Import (C, PinTypeOutput, "GPIOPinTypeGPIOOutput");

   procedure PinTypeOutputOD (Port : Integer; Pins : Integer);
   pragma Import (C, PinTypeOutputOD, "GPIOPinTypeGPIOOutputOD");

   procedure PinWrite (Port : Integer; Pins : Integer; Val : Integer);
   pragma Import (C, PinWrite, "GPIOPinWrite");

   function PinRead (Port : Integer; Pins : Integer) return Integer; 
   pragma Import (C, PinRead, "GPIOPinRead");

    
end LM4F.Gpio;







--   GPIO_DIR_MODE_IN : constant := 16#00000000#;  --  lm4f120/gpio.h:77
--   GPIO_DIR_MODE_OUT : constant := 16#00000001#;  --  lm4f120/gpio.h:78
--   GPIO_DIR_MODE_HW : constant := 16#00000002#;  --  lm4f120/gpio.h:79
--
--   GPIO_FALLING_EDGE : constant := 16#00000000#;  --  lm4f120/gpio.h:87
--   GPIO_RISING_EDGE : constant := 16#00000004#;  --  lm4f120/gpio.h:88
--   GPIO_BOTH_EDGES : constant := 16#00000001#;  --  lm4f120/gpio.h:89
--   GPIO_LOW_LEVEL : constant := 16#00000002#;  --  lm4f120/gpio.h:90
--   GPIO_HIGH_LEVEL : constant := 16#00000007#;  --  lm4f120/gpio.h:91
--   GPIO_DISCRETE_INT : constant := 16#00010000#;  --  lm4f120/gpio.h:92
--
--   GPIO_STRENGTH_2MA : constant := 16#00000001#;  --  lm4f120/gpio.h:100
--   GPIO_STRENGTH_4MA : constant := 16#00000002#;  --  lm4f120/gpio.h:101
--   GPIO_STRENGTH_8MA : constant := 16#00000004#;  --  lm4f120/gpio.h:102
--   GPIO_STRENGTH_8MA_SC : constant := 16#0000000C#;  --  lm4f120/gpio.h:103
--
--   GPIO_PIN_TYPE_STD : constant := 16#00000008#;  --  lm4f120/gpio.h:112
--   GPIO_PIN_TYPE_STD_WPU : constant := 16#0000000A#;  --  lm4f120/gpio.h:113
--   GPIO_PIN_TYPE_STD_WPD : constant := 16#0000000C#;  --  lm4f120/gpio.h:114
--   GPIO_PIN_TYPE_OD : constant := 16#00000009#;  --  lm4f120/gpio.h:115
--   GPIO_PIN_TYPE_OD_WPU : constant := 16#0000000B#;  --  lm4f120/gpio.h:116
--   GPIO_PIN_TYPE_OD_WPD : constant := 16#0000000D#;  --  lm4f120/gpio.h:117
--   GPIO_PIN_TYPE_ANALOG : constant := 16#00000000#;  --  lm4f120/gpio.h:118
--
--   subtype tBoolean is unsigned;  -- lm4f120/gpio.h:54
--
--   procedure GPIODirModeSet
--     (ulPort : unsigned_long;
--      ucPins : unsigned_char;
--      ulPinIO : unsigned_long);  -- lm4f120/gpio.h:125
--   pragma Import (C, GPIODirModeSet, "GPIODirModeSet");
--
--   function GPIODirModeGet (ulPort : unsigned_long; ucPin : unsigned_char) return unsigned_long;  -- lm4f120/gpio.h:127
--   pragma Import (C, GPIODirModeGet, "GPIODirModeGet");
--
--   procedure GPIOIntTypeSet
--     (ulPort : unsigned_long;
--      ucPins : unsigned_char;
--      ulIntType : unsigned_long);  -- lm4f120/gpio.h:128
--   pragma Import (C, GPIOIntTypeSet, "GPIOIntTypeSet");
--
--   function GPIOIntTypeGet (ulPort : unsigned_long; ucPin : unsigned_char) return unsigned_long;  -- lm4f120/gpio.h:130
--   pragma Import (C, GPIOIntTypeGet, "GPIOIntTypeGet");
--
--   procedure GPIOPadConfigSet
--     (ulPort : unsigned_long;
--      ucPins : unsigned_char;
--      ulStrength : unsigned_long;
--      ulPadType : unsigned_long);  -- lm4f120/gpio.h:131
--   pragma Import (C, GPIOPadConfigSet, "GPIOPadConfigSet");
--
--   procedure GPIOPadConfigGet
--     (ulPort : unsigned_long;
--      ucPin : unsigned_char;
--      pulStrength : access unsigned_long;
--      pulPadType : access unsigned_long);  -- lm4f120/gpio.h:134
--   pragma Import (C, GPIOPadConfigGet, "GPIOPadConfigGet");
--
--   procedure GPIOPinIntEnable (ulPort : unsigned_long; ucPins : unsigned_char);  -- lm4f120/gpio.h:137
--   pragma Import (C, GPIOPinIntEnable, "GPIOPinIntEnable");
--
--   procedure GPIOPinIntDisable (ulPort : unsigned_long; ucPins : unsigned_char);  -- lm4f120/gpio.h:138
--   pragma Import (C, GPIOPinIntDisable, "GPIOPinIntDisable");
--
--   function GPIOPinIntStatus (ulPort : unsigned_long; bMasked : tBoolean) return long;  -- lm4f120/gpio.h:139
--   pragma Import (C, GPIOPinIntStatus, "GPIOPinIntStatus");
--
--   procedure GPIOPinIntClear (ulPort : unsigned_long; ucPins : unsigned_char);  -- lm4f120/gpio.h:140
--   pragma Import (C, GPIOPinIntClear, "GPIOPinIntClear");
--
--   procedure GPIOPortIntRegister (ulPort : unsigned_long; pfnIntHandler : access procedure);  -- lm4f120/gpio.h:141
--   pragma Import (C, GPIOPortIntRegister, "GPIOPortIntRegister");
--
--   procedure GPIOPortIntUnregister (ulPort : unsigned_long);  -- lm4f120/gpio.h:143
--   pragma Import (C, GPIOPortIntUnregister, "GPIOPortIntUnregister");
--
--   function GPIOPinRead (ulPort : unsigned_long; ucPins : unsigned_char) return long;  -- lm4f120/gpio.h:144
--   pragma Import (C, GPIOPinRead, "GPIOPinRead");
--
--   procedure GPIOPinWrite
--     (ulPort : unsigned_long;
--      ucPins : unsigned_char;
--      ucVal : unsigned_char);  -- lm4f120/gpio.h:145
--   pragma Import (C, GPIOPinWrite, "GPIOPinWrite");
--
--   procedure GPIOPinConfigure (ulPinConfig : unsigned_long);  -- lm4f120/gpio.h:147
--   pragma Import (C, GPIOPinConfigure, "GPIOPinConfigure");
--
--   procedure GPIOPinTypeADC (ulPort : unsigned_long; ucPins : unsigned_char);  -- lm4f120/gpio.h:148
--   pragma Import (C, GPIOPinTypeADC, "GPIOPinTypeADC");
--
--   procedure GPIOPinTypeCAN (ulPort : unsigned_long; ucPins : unsigned_char);  -- lm4f120/gpio.h:149
--   pragma Import (C, GPIOPinTypeCAN, "GPIOPinTypeCAN");
--
--   procedure GPIOPinTypeComparator (ulPort : unsigned_long; ucPins : unsigned_char);  -- lm4f120/gpio.h:150
--   pragma Import (C, GPIOPinTypeComparator, "GPIOPinTypeComparator");
--
--   procedure GPIOPinTypeEPI (ulPort : unsigned_long; ucPins : unsigned_char);  -- lm4f120/gpio.h:151
--   pragma Import (C, GPIOPinTypeEPI, "GPIOPinTypeEPI");
--
--   procedure GPIOPinTypeEthernetLED (ulPort : unsigned_long; ucPins : unsigned_char);  -- lm4f120/gpio.h:152
--   pragma Import (C, GPIOPinTypeEthernetLED, "GPIOPinTypeEthernetLED");
--
--   procedure GPIOPinTypeEthernetMII (ulPort : unsigned_long; ucPins : unsigned_char);  -- lm4f120/gpio.h:153
--   pragma Import (C, GPIOPinTypeEthernetMII, "GPIOPinTypeEthernetMII");
--
--   procedure GPIOPinTypeFan (ulPort : unsigned_long; ucPins : unsigned_char);  -- lm4f120/gpio.h:154
--   pragma Import (C, GPIOPinTypeFan, "GPIOPinTypeFan");
--
--   procedure GPIOPinTypeGPIOInput (ulPort : unsigned_long; ucPins : unsigned_char);  -- lm4f120/gpio.h:155
--   pragma Import (C, GPIOPinTypeGPIOInput, "GPIOPinTypeGPIOInput");
--
--   procedure GPIOPinTypeGPIOOutput (ulPort : unsigned_long; ucPins : unsigned_char);  -- lm4f120/gpio.h:156
--   pragma Import (C, GPIOPinTypeGPIOOutput, "GPIOPinTypeGPIOOutput");
--
--   procedure GPIOPinTypeGPIOOutputOD (ulPort : unsigned_long; ucPins : unsigned_char);  -- lm4f120/gpio.h:157
--   pragma Import (C, GPIOPinTypeGPIOOutputOD, "GPIOPinTypeGPIOOutputOD");
--
--   procedure GPIOPinTypeI2C (ulPort : unsigned_long; ucPins : unsigned_char);  -- lm4f120/gpio.h:159
--   pragma Import (C, GPIOPinTypeI2C, "GPIOPinTypeI2C");
--
--   procedure GPIOPinTypeI2CSCL (ulPort : unsigned_long; ucPins : unsigned_char);  -- lm4f120/gpio.h:160
--   pragma Import (C, GPIOPinTypeI2CSCL, "GPIOPinTypeI2CSCL");
--
--   procedure GPIOPinTypeI2S (ulPort : unsigned_long; ucPins : unsigned_char);  -- lm4f120/gpio.h:161
--   pragma Import (C, GPIOPinTypeI2S, "GPIOPinTypeI2S");
--
--   procedure GPIOPinTypeLPC (ulPort : unsigned_long; ucPins : unsigned_char);  -- lm4f120/gpio.h:162
--   pragma Import (C, GPIOPinTypeLPC, "GPIOPinTypeLPC");
--
--   procedure GPIOPinTypePECIRx (ulPort : unsigned_long; ucPins : unsigned_char);  -- lm4f120/gpio.h:163
--   pragma Import (C, GPIOPinTypePECIRx, "GPIOPinTypePECIRx");
--
--   procedure GPIOPinTypePECITx (ulPort : unsigned_long; ucPins : unsigned_char);  -- lm4f120/gpio.h:164
--   pragma Import (C, GPIOPinTypePECITx, "GPIOPinTypePECITx");
--
--   procedure GPIOPinTypePWM (ulPort : unsigned_long; ucPins : unsigned_char);  -- lm4f120/gpio.h:165
--   pragma Import (C, GPIOPinTypePWM, "GPIOPinTypePWM");
--
--   procedure GPIOPinTypeQEI (ulPort : unsigned_long; ucPins : unsigned_char);  -- lm4f120/gpio.h:166
--   pragma Import (C, GPIOPinTypeQEI, "GPIOPinTypeQEI");
--
--   procedure GPIOPinTypeSSI (ulPort : unsigned_long; ucPins : unsigned_char);  -- lm4f120/gpio.h:167
--   pragma Import (C, GPIOPinTypeSSI, "GPIOPinTypeSSI");
--
--   procedure GPIOPinTypeTimer (ulPort : unsigned_long; ucPins : unsigned_char);  -- lm4f120/gpio.h:168
--   pragma Import (C, GPIOPinTypeTimer, "GPIOPinTypeTimer");
--
--   procedure GPIOPinTypeUART (ulPort : unsigned_long; ucPins : unsigned_char);  -- lm4f120/gpio.h:169
--   pragma Import (C, GPIOPinTypeUART, "GPIOPinTypeUART");
--
--   procedure GPIOPinTypeUSBAnalog (ulPort : unsigned_long; ucPins : unsigned_char);  -- lm4f120/gpio.h:170
--   pragma Import (C, GPIOPinTypeUSBAnalog, "GPIOPinTypeUSBAnalog");
--
--   procedure GPIOPinTypeUSBDigital (ulPort : unsigned_long; ucPins : unsigned_char);  -- lm4f120/gpio.h:171
--   pragma Import (C, GPIOPinTypeUSBDigital, "GPIOPinTypeUSBDigital");
--
--   procedure GPIODMATriggerEnable (ulPort : unsigned_long; ucPins : unsigned_char);  -- lm4f120/gpio.h:172
--   pragma Import (C, GPIODMATriggerEnable, "GPIODMATriggerEnable");
--
--   procedure GPIODMATriggerDisable (ulPort : unsigned_long; ucPins : unsigned_char);  -- lm4f120/gpio.h:173
--   pragma Import (C, GPIODMATriggerDisable, "GPIODMATriggerDisable");
--
--   procedure GPIOADCTriggerEnable (ulPort : unsigned_long; ucPins : unsigned_char);  -- lm4f120/gpio.h:174
--   pragma Import (C, GPIOADCTriggerEnable, "GPIOADCTriggerEnable");
--
--   procedure GPIOADCTriggerDisable (ulPort : unsigned_long; ucPins : unsigned_char);  -- lm4f120/gpio.h:175
--   pragma Import (C, GPIOADCTriggerDisable, "GPIOADCTriggerDisable");

