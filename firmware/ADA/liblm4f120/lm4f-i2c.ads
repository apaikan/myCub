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

package LM4F.I2C is

   I2C_INT_MASTER : constant := 16#00000001#;  --  i2c.h:67
   I2C_INT_SLAVE : constant := 16#00000002#;  --  i2c.h:68

   I2C_MASTER_CMD_SINGLE_SEND : constant := 16#00000007#;  --  i2c.h:75
   I2C_MASTER_CMD_SINGLE_RECEIVE : constant := 16#00000007#;  --  i2c.h:77
   I2C_MASTER_CMD_BURST_SEND_START : constant := 16#00000003#;  --  i2c.h:79
   I2C_MASTER_CMD_BURST_SEND_CONT : constant := 16#00000001#;  --  i2c.h:81
   I2C_MASTER_CMD_BURST_SEND_FINISH : constant := 16#00000005#;  --  i2c.h:83
   I2C_MASTER_CMD_BURST_SEND_STOP : constant := 16#00000004#;  --  i2c.h:85
   I2C_MASTER_CMD_BURST_SEND_ERROR_STOP : constant := 16#00000004#;  --  i2c.h:87
   I2C_MASTER_CMD_BURST_RECEIVE_START : constant := 16#0000000b#;  --  i2c.h:89
   I2C_MASTER_CMD_BURST_RECEIVE_CONT : constant := 16#00000009#;  --  i2c.h:91
   I2C_MASTER_CMD_BURST_RECEIVE_FINISH : constant := 16#00000005#;  --  i2c.h:93
   I2C_MASTER_CMD_BURST_RECEIVE_ERROR_STOP : constant := 16#00000004#;  --  i2c.h:95
   I2C_MASTER_CMD_QUICK_COMMAND : constant := 16#00000027#;  --  i2c.h:97
   I2C_MASTER_CMD_HS_MASTER_CODE_SEND : constant := 16#00000011#;  --  i2c.h:99

   I2C_MASTER_ERR_NONE : constant := 0;  --  i2c.h:107
   I2C_MASTER_ERR_ADDR_ACK : constant := 16#00000004#;  --  i2c.h:108
   I2C_MASTER_ERR_DATA_ACK : constant := 16#00000008#;  --  i2c.h:109
   I2C_MASTER_ERR_ARB_LOST : constant := 16#00000010#;  --  i2c.h:110
   I2C_MASTER_ERR_CLK_TOUT : constant := 16#00000080#;  --  i2c.h:111

   I2C_SLAVE_ACT_NONE : constant := 0;  --  i2c.h:118
   I2C_SLAVE_ACT_RREQ : constant := 16#00000001#;  --  i2c.h:119
   I2C_SLAVE_ACT_TREQ : constant := 16#00000002#;  --  i2c.h:120
   I2C_SLAVE_ACT_RREQ_FBR : constant := 16#00000005#;  --  i2c.h:121
   I2C_SLAVE_ACT_OWN2SEL : constant := 16#00000008#;  --  i2c.h:122
   I2C_SLAVE_ACT_QCMD : constant := 16#00000010#;  --  i2c.h:123
   I2C_SLAVE_ACT_QCMD_DATA : constant := 16#00000020#;  --  i2c.h:124

   I2C_MASTER_MAX_RETRIES : constant := 1000;  --  i2c.h:131

   I2C_MASTER_INT_TIMEOUT : constant := 16#00000002#;  --  i2c.h:138
   I2C_MASTER_INT_DATA : constant := 16#00000001#;  --  i2c.h:139
   I2C_SLAVE_INT_STOP : constant := 16#00000004#;  --  i2c.h:146
   I2C_SLAVE_INT_START : constant := 16#00000002#;  --  i2c.h:147
   I2C_SLAVE_INT_DATA : constant := 16#00000001#;  --  i2c.h:148


   procedure IntRegister (ulBase : Long_Integer; fnHandler : access procedure);  -- .h:155
   pragma Import (C, IntRegister, "I2CIntRegister");

   procedure IntUnregister (ulBase : Long_Integer);  -- .h:156
   pragma Import (C, IntUnregister, "I2CIntUnregister");

   function MasterBusBusy (ulBase : Long_Integer) return Integer;  -- .h:157
   pragma Import (C, MasterBusBusy, "I2CMasterBusBusy");

   function MasterBusy (ulBase : Long_Integer) return Integer;  -- .h:158
   pragma Import (C, MasterBusy, "I2CMasterBusy");

   procedure MasterControl (ulBase : Long_Integer; ulCmd : Long_Integer);  -- .h:159
   pragma Import (C, MasterControl, "I2CMasterControl");

   function MasterDataGet (ulBase : Long_Integer) return Long_Integer;  -- .h:160
   pragma Import (C, MasterDataGet, "I2CMasterDataGet");

   procedure MasterDataPut (ulBase : Long_Integer; ucData : Integer);  -- .h:161 ucData: unsigned_char
   pragma Import (C, MasterDataPut, "I2CMasterDataPut");

   procedure MasterDisable (ulBase : Long_Integer);  -- .h:162
   pragma Import (C, MasterDisable, "I2CMasterDisable");

   procedure MasterEnable (ulBase : Long_Integer);  -- .h:163
   pragma Import (C, MasterEnable, "I2CMasterEnable");

   function MasterErr (ulBase : Long_Integer) return Long_Integer;  -- .h:164
   pragma Import (C, MasterErr, "I2CMasterErr");

   procedure MasterInitExpClk
     (ulBase : Long_Integer;
      ulI2CClk : Long_Integer;
      bFast : Integer);  -- .h:165
   pragma Import (C, MasterInitExpClk, "I2CMasterInitExpClk");

   procedure MasterIntClear (ulBase : Long_Integer);  -- .h:167
   pragma Import (C, MasterIntClear, "I2CMasterIntClear");

   procedure MasterIntDisable (ulBase : Long_Integer);  -- .h:168
   pragma Import (C, MasterIntDisable, "I2CMasterIntDisable");

   procedure MasterIntEnable (ulBase : Long_Integer);  -- .h:169
   pragma Import (C, MasterIntEnable, "I2CMasterIntEnable");

   function MasterIntStatus (ulBase : Long_Integer; bMasked : Integer) return Integer;  -- .h:170
   pragma Import (C, MasterIntStatus, "I2CMasterIntStatus");

   procedure MasterIntEnableEx (ulBase : Long_Integer; ulIntFlags : Long_Integer);  -- .h:171
   pragma Import (C, MasterIntEnableEx, "I2CMasterIntEnableEx");

   procedure MasterIntDisableEx (ulBase : Long_Integer; ulIntFlags : Long_Integer);  -- .h:173
   pragma Import (C, MasterIntDisableEx, "I2CMasterIntDisableEx");

   function MasterIntStatusEx (ulBase : Long_Integer; bMasked : Integer) return Long_Integer;  -- .h:175
   pragma Import (C, MasterIntStatusEx, "I2CMasterIntStatusEx");

   procedure MasterIntClearEx (ulBase : Long_Integer; ulIntFlags : Long_Integer);  -- .h:177
   pragma Import (C, MasterIntClearEx, "I2CMasterIntClearEx");

   procedure MasterTimeoutSet (ulBase : Long_Integer; ulValue : Long_Integer);  -- .h:179
   pragma Import (C, MasterTimeoutSet, "I2CMasterTimeoutSet");

   procedure SlaveACKOverride (ulBase : Long_Integer; bEnable : Integer);  -- .h:180
   pragma Import (C, SlaveACKOverride, "I2CSlaveACKOverride");

   procedure SlaveACKValueSet (ulBase : Long_Integer; bACK : Integer);  -- .h:181
   pragma Import (C, SlaveACKValueSet, "I2CSlaveACKValueSet");

   function MasterLineStateGet (ulBase : Long_Integer) return Long_Integer;  -- .h:182
   pragma Import (C, MasterLineStateGet, "I2CMasterLineStateGet");

   procedure MasterSlaveAddrSet
     (ulBase : Long_Integer;
      ucSlaveAddr : Integer; --unsigned_char;
      bReceive : Integer);  -- .h:183
   pragma Import (C, MasterSlaveAddrSet, "I2CMasterSlaveAddrSet");

   function SlaveDataGet (ulBase : Long_Integer) return Long_Integer;  -- .h:186
   pragma Import (C, SlaveDataGet, "I2CSlaveDataGet");

   procedure SlaveDataPut (ulBase : Long_Integer; ucData : unsigned_char);  -- .h:187
   pragma Import (C, SlaveDataPut, "I2CSlaveDataPut");

   procedure SlaveDisable (ulBase : Long_Integer);  -- .h:188
   pragma Import (C, SlaveDisable, "I2CSlaveDisable");

   procedure SlaveEnable (ulBase : Long_Integer);  -- .h:189
   pragma Import (C, SlaveEnable, "I2CSlaveEnable");

   procedure SlaveInit (ulBase : Long_Integer; ucSlaveAddr : unsigned_char);  -- .h:190
   pragma Import (C, SlaveInit, "I2CSlaveInit");

   procedure SlaveAddressSet
     (ulBase : Long_Integer;
      ucAddrNum : unsigned_char;
      ucSlaveAddr : unsigned_char);  -- .h:191
   pragma Import (C, SlaveAddressSet, "I2CSlaveAddressSet");

   procedure SlaveIntClear (ulBase : Long_Integer);  -- .h:193
   pragma Import (C, SlaveIntClear, "I2CSlaveIntClear");

   procedure SlaveIntDisable (ulBase : Long_Integer);  -- .h:194
   pragma Import (C, SlaveIntDisable, "I2CSlaveIntDisable");

   procedure SlaveIntEnable (ulBase : Long_Integer);  -- .h:195
   pragma Import (C, SlaveIntEnable, "I2CSlaveIntEnable");

   procedure SlaveIntClearEx (ulBase : Long_Integer; ulIntFlags : Long_Integer);  -- .h:196
   pragma Import (C, SlaveIntClearEx, "I2CSlaveIntClearEx");

   procedure SlaveIntDisableEx (ulBase : Long_Integer; ulIntFlags : Long_Integer);  -- .h:197
   pragma Import (C, SlaveIntDisableEx, "I2CSlaveIntDisableEx");

   procedure SlaveIntEnableEx (ulBase : Long_Integer; ulIntFlags : Long_Integer);  -- .h:199
   pragma Import (C, SlaveIntEnableEx, "I2CSlaveIntEnableEx");

   function SlaveIntStatus (ulBase : Long_Integer; bMasked : Integer) return Integer;  -- .h:200
   pragma Import (C, SlaveIntStatus, "I2CSlaveIntStatus");

   function SlaveIntStatusEx (ulBase : Long_Integer; bMasked : Integer) return Long_Integer;  -- .h:201
   pragma Import (C, SlaveIntStatusEx, "I2CSlaveIntStatusEx");

   function SlaveStatus (ulBase : Long_Integer) return Long_Integer;  -- .h:203
   pragma Import (C, SlaveStatus, "I2CSlaveStatus");

end LM4F.I2C;





