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
with Ada.Real_Time;

package MyCub is

    package RT renames Ada.Real_Time;    
    type TaskParameters is 
    record 
        Priority : Integer := 120;
        Period   : RT.Time_Span := RT.Milliseconds(0); 
    end record;

    -- board interface task priorities
    BlinkyTaskParam : constant TaskParameters := (Priority => 120, 
                                                  Period =>  RT.Seconds(3));
    UartTaskParam   : constant TaskParameters := (Priority => 130, 
                                                  Period => RT.Milliseconds(0));

    -- controller task priorities
    Ctrl0TaskParam  : constant TaskParameters := (Priority => 125,
                                                  Period => RT.Milliseconds(0));
    Ctrl1TaskParam  : constant TaskParameters := (Priority => 125,
                                                  Period => RT.Milliseconds(0));
    Ctrl2TaskParam  : constant TaskParameters := (Priority => 125,
                                                  Period => RT.Milliseconds(0));
    Ctrl3TaskParam  : constant TaskParameters := (Priority => 125,
                                                  Period => RT.Milliseconds(0));

    -- working memory task priorities
    BatTaskParam    : constant TaskParameters := (Priority => 121, 
                                                  Period =>  RT.Seconds(10));
    CmpsTaskParam   : constant TaskParameters := (Priority => 122, 
                                                  Period =>  RT.Milliseconds(500));
    SonarTaskParam  : constant TaskParameters := (Priority => 123, 
                                                  Period =>  RT.Milliseconds(500));
end MyCub;

