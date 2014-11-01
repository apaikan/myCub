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

generic 
    Timer : Integer := 16#40034000#;
    TickPerSec : Long_Integer := 80_000_000;

package LM4F.HTimer is

   TIMER0 : constant := 16#40030000#;  --  hw_memmap.h:89
   TIMER1 : constant := 16#40031000#;  --  hw_memmap.h:90
   TIMER2 : constant := 16#40032000#;  --  hw_memmap.h:91
   TIMER3 : constant := 16#40033000#;  --  hw_memmap.h:92
   TIMER4 : constant := 16#40034000#;  --  hw_memmap.h:93
   TIMER5 : constant := 16#40035000#;  --  hw_memmap.h:94

    --
    -- Initialize timer the Timer
    --
    procedure Init;

    --
    -- Return the current timer tick
    --
    function GetTick return Long_Integer;

    --
    -- Get duration in ticks
    --
    function GetSpan( StartT : Long_Integer; EndT : Long_Integer) return Long_Integer; 

    --
    -- Get duration in micro seconds
    --
    function GetSpanUsec( StartT : Long_Integer; EndT : Long_Integer) return Long_Integer; 

    --
    -- Get duration in milli seconds
    --
    function GetSpanMsec( StartT : Long_Integer; EndT : Long_Integer) return Long_Integer; 

    --
    -- Get duration in seconds
    --
    function GetSpanSec( StartT : Long_Integer; EndT : Long_Integer) return Long_Integer; 

    -- busy waiting in microseconds 
    procedure WaitUsec(Duration : Long_Integer);

    -- busy waiting in miliseconds 
    procedure WaitMsec(Duration : Long_Integer);


end LM4F.HTimer;





