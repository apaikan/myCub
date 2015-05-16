--  This package has been generated automatically by GNATtest.
--  Do not edit any part of it, see GNATtest documentation for more details.

--  begin read only
with AUnit.Test_Caller;
with Gnattest_Generated;

package body LM4F.ADC.Test_Data.Tests.Suite is

   package Runner_1 is new AUnit.Test_Caller
     (GNATtest_Generated.GNATtest_Standard.LM4F.ADC.Test_Data.Tests.Test);

   Result : aliased AUnit.Test_Suites.Test_Suite;

   Case_1_1_Test_SequenceConfigure_ecf7fb : aliased Runner_1.Test_Case;
   Case_2_1_Test_SequenceStepConfigure_95f172 : aliased Runner_1.Test_Case;
   Case_3_1_Test_SequenceEnable_770c65 : aliased Runner_1.Test_Case;
   Case_4_1_Test_SequenceDisable_f3e0be : aliased Runner_1.Test_Case;
   Case_5_1_Test_IntClear_ca438a : aliased Runner_1.Test_Case;
   Case_6_1_Test_ProcessorTrigger_831849 : aliased Runner_1.Test_Case;
   Case_7_1_Test_IntStatus_dcb6ab : aliased Runner_1.Test_Case;
   Case_8_1_Test_SequenceDataGet_5e8382 : aliased Runner_1.Test_Case;

   function Suite return AUnit.Test_Suites.Access_Test_Suite is
   begin

      Runner_1.Create
        (Case_1_1_Test_SequenceConfigure_ecf7fb,
         "lm4f-adc.ads:83:4:",
         Test_SequenceConfigure_ecf7fb'Access);
      Runner_1.Create
        (Case_2_1_Test_SequenceStepConfigure_95f172,
         "lm4f-adc.ads:90:4:",
         Test_SequenceStepConfigure_95f172'Access);
      Runner_1.Create
        (Case_3_1_Test_SequenceEnable_770c65,
         "lm4f-adc.ads:97:4:",
         Test_SequenceEnable_770c65'Access);
      Runner_1.Create
        (Case_4_1_Test_SequenceDisable_f3e0be,
         "lm4f-adc.ads:100:4:",
         Test_SequenceDisable_f3e0be'Access);
      Runner_1.Create
        (Case_5_1_Test_IntClear_ca438a,
         "lm4f-adc.ads:103:4:",
         Test_IntClear_ca438a'Access);
      Runner_1.Create
        (Case_6_1_Test_ProcessorTrigger_831849,
         "lm4f-adc.ads:106:4:",
         Test_ProcessorTrigger_831849'Access);
      Runner_1.Create
        (Case_7_1_Test_IntStatus_dcb6ab,
         "lm4f-adc.ads:109:4:",
         Test_IntStatus_dcb6ab'Access);
      Runner_1.Create
        (Case_8_1_Test_SequenceDataGet_5e8382,
         "lm4f-adc.ads:115:4:",
         Test_SequenceDataGet_5e8382'Access);

      Result.Add_Test (Case_1_1_Test_SequenceConfigure_ecf7fb'Access);
      Result.Add_Test (Case_2_1_Test_SequenceStepConfigure_95f172'Access);
      Result.Add_Test (Case_3_1_Test_SequenceEnable_770c65'Access);
      Result.Add_Test (Case_4_1_Test_SequenceDisable_f3e0be'Access);
      Result.Add_Test (Case_5_1_Test_IntClear_ca438a'Access);
      Result.Add_Test (Case_6_1_Test_ProcessorTrigger_831849'Access);
      Result.Add_Test (Case_7_1_Test_IntStatus_dcb6ab'Access);
      Result.Add_Test (Case_8_1_Test_SequenceDataGet_5e8382'Access);

      return Result'Access;

   end Suite;

end LM4F.ADC.Test_Data.Tests.Suite;
--  end read only
