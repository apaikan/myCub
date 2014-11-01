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

package LM4F.Uart is

   UART0 : constant := 16#4000C000#;  --  hw_memmap.h:65
   UART1 : constant := 16#4000D000#;  --  hw_memmap.h:66
   UART2 : constant := 16#4000E000#;  --  hw_memmap.h:67
   UART3 : constant := 16#4000F000#;  --  hw_memmap.h:68
   UART4 : constant := 16#40010000#;  --  hw_memmap.h:69
   UART5 : constant := 16#40011000#;  --  hw_memmap.h:70
   UART6 : constant := 16#40012000#;  --  hw_memmap.h:71
   UART7 : constant := 16#40013000#;  --  hw_memmap.h:72

   INT_DMATX : constant := 16#20000#;  --  uart.h:64
   INT_DMARX : constant := 16#10000#;  --  uart.h:65
   INT_9BIT : constant := 16#1000#;  --  uart.h:66
   INT_OE : constant := 16#400#;  --  uart.h:67
   INT_BE : constant := 16#200#;  --  uart.h:68
   INT_PE : constant := 16#100#;  --  uart.h:69
   INT_FE : constant := 16#080#;  --  uart.h:70
   INT_RT : constant := 16#040#;  --  uart.h:71
   INT_TX : constant := 16#020#;  --  uart.h:72
   INT_RX : constant := 16#010#;  --  uart.h:73
   INT_DSR : constant := 16#008#;  --  uart.h:74
   INT_DCD : constant := 16#004#;  --  uart.h:75
   INT_CTS : constant := 16#002#;  --  uart.h:76
   INT_RI : constant := 16#001#;  --  uart.h:77

   procedure IntEnable (Base : Integer; Flags : Integer);
   pragma Import (C, IntEnable, "UARTIntEnable");

   procedure IntDisable (Base : Integer; Flags : Integer);
   pragma Import (C, IntDisable, "UARTIntDisable");

   function IntStatus (Base : Integer; Masked : Boolean) return Integer;
   pragma Import (C, IntStatus, "UARTIntStatus");

   procedure IntClear (Base : Integer; Flags : Integer);
   pragma Import (C, IntClear, "UARTIntClear");

   function CharsAvail (Base : Integer) return Boolean;
   pragma Import (C, CharsAvail, "UARTCharsAvail");

   function SpaceAvail (Base : Integer) return Boolean;
   pragma Import (C, SpaceAvail, "UARTSpaceAvail");

   function CharGet (Base : Integer) return Unsigned_8;
   pragma Import (C, CharGet, "UARTCharGet");
   
   function CharGetNonBlocking (Base : Integer) return Unsigned_8;
   pragma Import (C, CharGetNonBlocking, "UARTCharGetNonBlocking");

   procedure CharPut (Base : Integer; Data : Unsigned_8);
   pragma Import (C, CharPut, "UARTCharPut");
   
   function CharPutNonBlocking (Base : Integer; Data : Unsigned_8) return Boolean;
   pragma Import (C, CharPutNonBlocking, "UARTCharPutNonBlocking");

   function Busy (Base : Integer) return Boolean;
   pragma Import (C, Busy, "UARTBusy");

end LM4F.Uart;





