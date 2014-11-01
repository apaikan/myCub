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
    Base : Long_Integer := 16#40021000#; -- I2C1_MASTER_BASE
    TickPerSec : Long_Integer := 80_000_000;

package ARM.I2C is

   --type I2CDevice is limited private;
   --type Byte     is new unsigned_char;-- mod 2**8;
   subtype Byte    is Unsigned_8;
   type Byte_Ptr   is access Byte;
   type ByteVector is array (Integer range <>) of Byte;

    procedure SetAddress(Address : Integer; Register: Integer);
    procedure SetFrequency(Frequency: Integer);
    function Write(Data : ByteVector; Len : Integer) return Boolean;
    function Write(Data : Byte) return Boolean;
    procedure Read(Data : out ByteVector; Len : in out Integer);

private
    type I2CDevice is 
    record
        Address : Integer;
        Register : Integer; 
    end record;
    
    -- I2c device handler
    Device : I2CDevice;

end ARM.I2C;





