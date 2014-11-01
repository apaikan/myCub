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
with ARM.Strings;   use ARM.Strings;
with Ada.Unchecked_Conversion;
with Ada.Interrupts;

generic 
    Port : Integer;
    IRQ  : Ada.Interrupts.Interrupt_ID := 75; -- 21 for UART0

package ARM.Uart is

    function To_U8 is new Ada.Unchecked_Conversion (Source => Character,
                                                    Target => Unsigned_8);

    function To_Char is new Ada.Unchecked_Conversion (Source => Unsigned_8,
                                                      Target => Character);
    procedure Put (Ch : Character);
    procedure Put (S : ARM_String);
    procedure Put_Line (S : ARM_String);
    
    procedure Get (Ch : out Character);
    procedure Get_Line (S : out ARM_String; 
                        Last : out Unsigned_8;
                        Echo : in Boolean := False);

    procedure IntToStr(Value : Integer;
                       Str : out ARM_String;
                       Last : out Unsigned_8);

    procedure LongToStr(Value : Long_Integer;
                       Str : out ARM_String;
                       Last : out Unsigned_8);

    function StrToInt(Str: ARM_String; Last: Unsigned_8) return Integer;

    procedure New_Line;  --  only line-feed (LF)
    procedure CRLF;      --  DOS like CR & LF

    protected Monitor is
        -- pragma Priority (System.Interrupt_Priority'Last - 10);
        procedure UartIntHandler;
        pragma Attach_Handler (UartIntHandler, IRQ);
        entry Get(Data : out Character);
    private
        Container  : AStr128;
        Available  : Boolean := False;
        Rx_Inx     : Unsigned_8 := 1;
        Rx_Outx    : Unsigned_8 := 1;
    end Monitor;

private
   pragma Inline (New_Line);
   pragma Inline (CRLF);

end ARM.Uart;





