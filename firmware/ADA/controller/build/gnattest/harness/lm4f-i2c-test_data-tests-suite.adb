--  This package has been generated automatically by GNATtest.
--  Do not edit any part of it, see GNATtest documentation for more details.

--  begin read only
with AUnit.Test_Caller;
with Gnattest_Generated;

package body LM4F.I2C.Test_Data.Tests.Suite is

   package Runner_1 is new AUnit.Test_Caller
     (GNATtest_Generated.GNATtest_Standard.LM4F.I2C.Test_Data.Tests.Test);

   Result : aliased AUnit.Test_Suites.Test_Suite;

   Case_1_1_Test_IntRegister_e68951 : aliased Runner_1.Test_Case;
   Case_2_1_Test_IntUnregister_e3241a : aliased Runner_1.Test_Case;
   Case_3_1_Test_MasterBusBusy_f795ed : aliased Runner_1.Test_Case;
   Case_4_1_Test_MasterBusy_d31286 : aliased Runner_1.Test_Case;
   Case_5_1_Test_MasterControl_3bd667 : aliased Runner_1.Test_Case;
   Case_6_1_Test_MasterDataGet_8917e5 : aliased Runner_1.Test_Case;
   Case_7_1_Test_MasterDataPut_100b8a : aliased Runner_1.Test_Case;
   Case_8_1_Test_MasterDisable_70809e : aliased Runner_1.Test_Case;
   Case_9_1_Test_MasterEnable_fba409 : aliased Runner_1.Test_Case;
   Case_10_1_Test_MasterErr_6cadc9 : aliased Runner_1.Test_Case;
   Case_11_1_Test_MasterInitExpClk_ba6fc9 : aliased Runner_1.Test_Case;
   Case_12_1_Test_MasterIntClear_bf2621 : aliased Runner_1.Test_Case;
   Case_13_1_Test_MasterIntDisable_aa907c : aliased Runner_1.Test_Case;
   Case_14_1_Test_MasterIntEnable_dc2207 : aliased Runner_1.Test_Case;
   Case_15_1_Test_MasterIntStatus_7088ce : aliased Runner_1.Test_Case;
   Case_16_1_Test_MasterIntEnableEx_25902b : aliased Runner_1.Test_Case;
   Case_17_1_Test_MasterIntDisableEx_bb49d3 : aliased Runner_1.Test_Case;
   Case_18_1_Test_MasterIntStatusEx_9e7484 : aliased Runner_1.Test_Case;
   Case_19_1_Test_MasterIntClearEx_9da135 : aliased Runner_1.Test_Case;
   Case_20_1_Test_MasterTimeoutSet_39e89a : aliased Runner_1.Test_Case;
   Case_21_1_Test_SlaveACKOverride_ef6653 : aliased Runner_1.Test_Case;
   Case_22_1_Test_SlaveACKValueSet_9b95fb : aliased Runner_1.Test_Case;
   Case_23_1_Test_MasterLineStateGet_2d4916 : aliased Runner_1.Test_Case;
   Case_24_1_Test_MasterSlaveAddrSet_082da9 : aliased Runner_1.Test_Case;
   Case_25_1_Test_SlaveDataGet_036deb : aliased Runner_1.Test_Case;
   Case_26_1_Test_SlaveDataPut_0c6a0a : aliased Runner_1.Test_Case;
   Case_27_1_Test_SlaveDisable_cdd75e : aliased Runner_1.Test_Case;
   Case_28_1_Test_SlaveEnable_03d017 : aliased Runner_1.Test_Case;
   Case_29_1_Test_SlaveInit_c1e65f : aliased Runner_1.Test_Case;
   Case_30_1_Test_SlaveAddressSet_42fcd4 : aliased Runner_1.Test_Case;
   Case_31_1_Test_SlaveIntClear_3ebb4b : aliased Runner_1.Test_Case;
   Case_32_1_Test_SlaveIntDisable_b1df8b : aliased Runner_1.Test_Case;
   Case_33_1_Test_SlaveIntEnable_92290a : aliased Runner_1.Test_Case;
   Case_34_1_Test_SlaveIntClearEx_149d5d : aliased Runner_1.Test_Case;
   Case_35_1_Test_SlaveIntDisableEx_b31ee2 : aliased Runner_1.Test_Case;
   Case_36_1_Test_SlaveIntEnableEx_d82bb1 : aliased Runner_1.Test_Case;
   Case_37_1_Test_SlaveIntStatus_b16810 : aliased Runner_1.Test_Case;
   Case_38_1_Test_SlaveIntStatusEx_f65111 : aliased Runner_1.Test_Case;
   Case_39_1_Test_SlaveStatus_f7f8f2 : aliased Runner_1.Test_Case;

   function Suite return AUnit.Test_Suites.Access_Test_Suite is
   begin

      Runner_1.Create
        (Case_1_1_Test_IntRegister_e68951,
         "lm4f-i2c.ads:81:4:",
         Test_IntRegister_e68951'Access);
      Runner_1.Create
        (Case_2_1_Test_IntUnregister_e3241a,
         "lm4f-i2c.ads:84:4:",
         Test_IntUnregister_e3241a'Access);
      Runner_1.Create
        (Case_3_1_Test_MasterBusBusy_f795ed,
         "lm4f-i2c.ads:87:4:",
         Test_MasterBusBusy_f795ed'Access);
      Runner_1.Create
        (Case_4_1_Test_MasterBusy_d31286,
         "lm4f-i2c.ads:90:4:",
         Test_MasterBusy_d31286'Access);
      Runner_1.Create
        (Case_5_1_Test_MasterControl_3bd667,
         "lm4f-i2c.ads:93:4:",
         Test_MasterControl_3bd667'Access);
      Runner_1.Create
        (Case_6_1_Test_MasterDataGet_8917e5,
         "lm4f-i2c.ads:96:4:",
         Test_MasterDataGet_8917e5'Access);
      Runner_1.Create
        (Case_7_1_Test_MasterDataPut_100b8a,
         "lm4f-i2c.ads:99:4:",
         Test_MasterDataPut_100b8a'Access);
      Runner_1.Create
        (Case_8_1_Test_MasterDisable_70809e,
         "lm4f-i2c.ads:102:4:",
         Test_MasterDisable_70809e'Access);
      Runner_1.Create
        (Case_9_1_Test_MasterEnable_fba409,
         "lm4f-i2c.ads:105:4:",
         Test_MasterEnable_fba409'Access);
      Runner_1.Create
        (Case_10_1_Test_MasterErr_6cadc9,
         "lm4f-i2c.ads:108:4:",
         Test_MasterErr_6cadc9'Access);
      Runner_1.Create
        (Case_11_1_Test_MasterInitExpClk_ba6fc9,
         "lm4f-i2c.ads:111:4:",
         Test_MasterInitExpClk_ba6fc9'Access);
      Runner_1.Create
        (Case_12_1_Test_MasterIntClear_bf2621,
         "lm4f-i2c.ads:117:4:",
         Test_MasterIntClear_bf2621'Access);
      Runner_1.Create
        (Case_13_1_Test_MasterIntDisable_aa907c,
         "lm4f-i2c.ads:120:4:",
         Test_MasterIntDisable_aa907c'Access);
      Runner_1.Create
        (Case_14_1_Test_MasterIntEnable_dc2207,
         "lm4f-i2c.ads:123:4:",
         Test_MasterIntEnable_dc2207'Access);
      Runner_1.Create
        (Case_15_1_Test_MasterIntStatus_7088ce,
         "lm4f-i2c.ads:126:4:",
         Test_MasterIntStatus_7088ce'Access);
      Runner_1.Create
        (Case_16_1_Test_MasterIntEnableEx_25902b,
         "lm4f-i2c.ads:129:4:",
         Test_MasterIntEnableEx_25902b'Access);
      Runner_1.Create
        (Case_17_1_Test_MasterIntDisableEx_bb49d3,
         "lm4f-i2c.ads:132:4:",
         Test_MasterIntDisableEx_bb49d3'Access);
      Runner_1.Create
        (Case_18_1_Test_MasterIntStatusEx_9e7484,
         "lm4f-i2c.ads:135:4:",
         Test_MasterIntStatusEx_9e7484'Access);
      Runner_1.Create
        (Case_19_1_Test_MasterIntClearEx_9da135,
         "lm4f-i2c.ads:138:4:",
         Test_MasterIntClearEx_9da135'Access);
      Runner_1.Create
        (Case_20_1_Test_MasterTimeoutSet_39e89a,
         "lm4f-i2c.ads:141:4:",
         Test_MasterTimeoutSet_39e89a'Access);
      Runner_1.Create
        (Case_21_1_Test_SlaveACKOverride_ef6653,
         "lm4f-i2c.ads:144:4:",
         Test_SlaveACKOverride_ef6653'Access);
      Runner_1.Create
        (Case_22_1_Test_SlaveACKValueSet_9b95fb,
         "lm4f-i2c.ads:147:4:",
         Test_SlaveACKValueSet_9b95fb'Access);
      Runner_1.Create
        (Case_23_1_Test_MasterLineStateGet_2d4916,
         "lm4f-i2c.ads:150:4:",
         Test_MasterLineStateGet_2d4916'Access);
      Runner_1.Create
        (Case_24_1_Test_MasterSlaveAddrSet_082da9,
         "lm4f-i2c.ads:153:4:",
         Test_MasterSlaveAddrSet_082da9'Access);
      Runner_1.Create
        (Case_25_1_Test_SlaveDataGet_036deb,
         "lm4f-i2c.ads:159:4:",
         Test_SlaveDataGet_036deb'Access);
      Runner_1.Create
        (Case_26_1_Test_SlaveDataPut_0c6a0a,
         "lm4f-i2c.ads:162:4:",
         Test_SlaveDataPut_0c6a0a'Access);
      Runner_1.Create
        (Case_27_1_Test_SlaveDisable_cdd75e,
         "lm4f-i2c.ads:165:4:",
         Test_SlaveDisable_cdd75e'Access);
      Runner_1.Create
        (Case_28_1_Test_SlaveEnable_03d017,
         "lm4f-i2c.ads:168:4:",
         Test_SlaveEnable_03d017'Access);
      Runner_1.Create
        (Case_29_1_Test_SlaveInit_c1e65f,
         "lm4f-i2c.ads:171:4:",
         Test_SlaveInit_c1e65f'Access);
      Runner_1.Create
        (Case_30_1_Test_SlaveAddressSet_42fcd4,
         "lm4f-i2c.ads:174:4:",
         Test_SlaveAddressSet_42fcd4'Access);
      Runner_1.Create
        (Case_31_1_Test_SlaveIntClear_3ebb4b,
         "lm4f-i2c.ads:180:4:",
         Test_SlaveIntClear_3ebb4b'Access);
      Runner_1.Create
        (Case_32_1_Test_SlaveIntDisable_b1df8b,
         "lm4f-i2c.ads:183:4:",
         Test_SlaveIntDisable_b1df8b'Access);
      Runner_1.Create
        (Case_33_1_Test_SlaveIntEnable_92290a,
         "lm4f-i2c.ads:186:4:",
         Test_SlaveIntEnable_92290a'Access);
      Runner_1.Create
        (Case_34_1_Test_SlaveIntClearEx_149d5d,
         "lm4f-i2c.ads:189:4:",
         Test_SlaveIntClearEx_149d5d'Access);
      Runner_1.Create
        (Case_35_1_Test_SlaveIntDisableEx_b31ee2,
         "lm4f-i2c.ads:192:4:",
         Test_SlaveIntDisableEx_b31ee2'Access);
      Runner_1.Create
        (Case_36_1_Test_SlaveIntEnableEx_d82bb1,
         "lm4f-i2c.ads:195:4:",
         Test_SlaveIntEnableEx_d82bb1'Access);
      Runner_1.Create
        (Case_37_1_Test_SlaveIntStatus_b16810,
         "lm4f-i2c.ads:198:4:",
         Test_SlaveIntStatus_b16810'Access);
      Runner_1.Create
        (Case_38_1_Test_SlaveIntStatusEx_f65111,
         "lm4f-i2c.ads:201:4:",
         Test_SlaveIntStatusEx_f65111'Access);
      Runner_1.Create
        (Case_39_1_Test_SlaveStatus_f7f8f2,
         "lm4f-i2c.ads:204:4:",
         Test_SlaveStatus_f7f8f2'Access);

      Result.Add_Test (Case_1_1_Test_IntRegister_e68951'Access);
      Result.Add_Test (Case_2_1_Test_IntUnregister_e3241a'Access);
      Result.Add_Test (Case_3_1_Test_MasterBusBusy_f795ed'Access);
      Result.Add_Test (Case_4_1_Test_MasterBusy_d31286'Access);
      Result.Add_Test (Case_5_1_Test_MasterControl_3bd667'Access);
      Result.Add_Test (Case_6_1_Test_MasterDataGet_8917e5'Access);
      Result.Add_Test (Case_7_1_Test_MasterDataPut_100b8a'Access);
      Result.Add_Test (Case_8_1_Test_MasterDisable_70809e'Access);
      Result.Add_Test (Case_9_1_Test_MasterEnable_fba409'Access);
      Result.Add_Test (Case_10_1_Test_MasterErr_6cadc9'Access);
      Result.Add_Test (Case_11_1_Test_MasterInitExpClk_ba6fc9'Access);
      Result.Add_Test (Case_12_1_Test_MasterIntClear_bf2621'Access);
      Result.Add_Test (Case_13_1_Test_MasterIntDisable_aa907c'Access);
      Result.Add_Test (Case_14_1_Test_MasterIntEnable_dc2207'Access);
      Result.Add_Test (Case_15_1_Test_MasterIntStatus_7088ce'Access);
      Result.Add_Test (Case_16_1_Test_MasterIntEnableEx_25902b'Access);
      Result.Add_Test (Case_17_1_Test_MasterIntDisableEx_bb49d3'Access);
      Result.Add_Test (Case_18_1_Test_MasterIntStatusEx_9e7484'Access);
      Result.Add_Test (Case_19_1_Test_MasterIntClearEx_9da135'Access);
      Result.Add_Test (Case_20_1_Test_MasterTimeoutSet_39e89a'Access);
      Result.Add_Test (Case_21_1_Test_SlaveACKOverride_ef6653'Access);
      Result.Add_Test (Case_22_1_Test_SlaveACKValueSet_9b95fb'Access);
      Result.Add_Test (Case_23_1_Test_MasterLineStateGet_2d4916'Access);
      Result.Add_Test (Case_24_1_Test_MasterSlaveAddrSet_082da9'Access);
      Result.Add_Test (Case_25_1_Test_SlaveDataGet_036deb'Access);
      Result.Add_Test (Case_26_1_Test_SlaveDataPut_0c6a0a'Access);
      Result.Add_Test (Case_27_1_Test_SlaveDisable_cdd75e'Access);
      Result.Add_Test (Case_28_1_Test_SlaveEnable_03d017'Access);
      Result.Add_Test (Case_29_1_Test_SlaveInit_c1e65f'Access);
      Result.Add_Test (Case_30_1_Test_SlaveAddressSet_42fcd4'Access);
      Result.Add_Test (Case_31_1_Test_SlaveIntClear_3ebb4b'Access);
      Result.Add_Test (Case_32_1_Test_SlaveIntDisable_b1df8b'Access);
      Result.Add_Test (Case_33_1_Test_SlaveIntEnable_92290a'Access);
      Result.Add_Test (Case_34_1_Test_SlaveIntClearEx_149d5d'Access);
      Result.Add_Test (Case_35_1_Test_SlaveIntDisableEx_b31ee2'Access);
      Result.Add_Test (Case_36_1_Test_SlaveIntEnableEx_d82bb1'Access);
      Result.Add_Test (Case_37_1_Test_SlaveIntStatus_b16810'Access);
      Result.Add_Test (Case_38_1_Test_SlaveIntStatusEx_f65111'Access);
      Result.Add_Test (Case_39_1_Test_SlaveStatus_f7f8f2'Access);

      return Result'Access;

   end Suite;

end LM4F.I2C.Test_Data.Tests.Suite;
--  end read only
