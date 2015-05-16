--  This package has been generated automatically by GNATtest.
--  Do not edit any part of it, see GNATtest documentation for more details.

--  begin read only
with AUnit.Test_Caller;
with Gnattest_Generated;

package body LM4F.Gpio.Test_Data.Tests.Suite is

   package Runner_1 is new AUnit.Test_Caller
     (GNATtest_Generated.GNATtest_Standard.LM4F.Gpio.Test_Data.Tests.Test);

   Result : aliased AUnit.Test_Suites.Test_Suite;

   Case_1_1_Test_PinTypeInput_48f8c1 : aliased Runner_1.Test_Case;
   Case_2_1_Test_PinTypeOutput_aa2133 : aliased Runner_1.Test_Case;
   Case_3_1_Test_PinTypeOutputOD_709f18 : aliased Runner_1.Test_Case;
   Case_4_1_Test_PinWrite_a2ff94 : aliased Runner_1.Test_Case;
   Case_5_1_Test_PinRead_890e18 : aliased Runner_1.Test_Case;

   function Suite return AUnit.Test_Suites.Access_Test_Suite is
   begin

      Runner_1.Create
        (Case_1_1_Test_PinTypeInput_48f8c1,
         "lm4f-gpio.ads:60:4:",
         Test_PinTypeInput_48f8c1'Access);
      Runner_1.Create
        (Case_2_1_Test_PinTypeOutput_aa2133,
         "lm4f-gpio.ads:63:4:",
         Test_PinTypeOutput_aa2133'Access);
      Runner_1.Create
        (Case_3_1_Test_PinTypeOutputOD_709f18,
         "lm4f-gpio.ads:66:4:",
         Test_PinTypeOutputOD_709f18'Access);
      Runner_1.Create
        (Case_4_1_Test_PinWrite_a2ff94,
         "lm4f-gpio.ads:69:4:",
         Test_PinWrite_a2ff94'Access);
      Runner_1.Create
        (Case_5_1_Test_PinRead_890e18,
         "lm4f-gpio.ads:72:4:",
         Test_PinRead_890e18'Access);

      Result.Add_Test (Case_1_1_Test_PinTypeInput_48f8c1'Access);
      Result.Add_Test (Case_2_1_Test_PinTypeOutput_aa2133'Access);
      Result.Add_Test (Case_3_1_Test_PinTypeOutputOD_709f18'Access);
      Result.Add_Test (Case_4_1_Test_PinWrite_a2ff94'Access);
      Result.Add_Test (Case_5_1_Test_PinRead_890e18'Access);

      return Result'Access;

   end Suite;

end LM4F.Gpio.Test_Data.Tests.Suite;
--  end read only
