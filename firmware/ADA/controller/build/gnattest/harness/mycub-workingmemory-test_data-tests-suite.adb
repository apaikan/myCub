--  This package has been generated automatically by GNATtest.
--  Do not edit any part of it, see GNATtest documentation for more details.

--  begin read only
with AUnit.Test_Caller;
with Gnattest_Generated;

package body MyCub.WorkingMemory.Test_Data.Tests.Suite is

   package Runner_1 is new AUnit.Test_Caller
     (GNATtest_Generated.GNATtest_Standard.MyCub.WorkingMemory.Test_Data.Tests.Test);

   Result : aliased AUnit.Test_Suites.Test_Suite;

   Case_1_1_Test_GetBatteryVolt_d5461c : aliased Runner_1.Test_Case;
   Case_2_1_Test_GetBatteryCurrent_caabb0 : aliased Runner_1.Test_Case;
   Case_3_1_Test_GetDistance_9dd451 : aliased Runner_1.Test_Case;
   Case_4_1_Test_GetOrientation_f74f9d : aliased Runner_1.Test_Case;
   Case_5_1_Test_Combine_ebf75c : aliased Runner_1.Test_Case;

   function Suite return AUnit.Test_Suites.Access_Test_Suite is
   begin

      Runner_1.Create
        (Case_1_1_Test_GetBatteryVolt_d5461c,
         "mycub-workingmemory.ads:17:5:",
         Test_GetBatteryVolt_d5461c'Access);
      Runner_1.Create
        (Case_2_1_Test_GetBatteryCurrent_caabb0,
         "mycub-workingmemory.ads:18:5:",
         Test_GetBatteryCurrent_caabb0'Access);
      Runner_1.Create
        (Case_3_1_Test_GetDistance_9dd451,
         "mycub-workingmemory.ads:19:5:",
         Test_GetDistance_9dd451'Access);
      Runner_1.Create
        (Case_4_1_Test_GetOrientation_f74f9d,
         "mycub-workingmemory.ads:20:5:",
         Test_GetOrientation_f74f9d'Access);
      Runner_1.Create
        (Case_5_1_Test_Combine_ebf75c,
         "mycub-workingmemory.ads:60:5:",
         Test_Combine_ebf75c'Access);

      Result.Add_Test (Case_1_1_Test_GetBatteryVolt_d5461c'Access);
      Result.Add_Test (Case_2_1_Test_GetBatteryCurrent_caabb0'Access);
      Result.Add_Test (Case_3_1_Test_GetDistance_9dd451'Access);
      Result.Add_Test (Case_4_1_Test_GetOrientation_f74f9d'Access);
      Result.Add_Test (Case_5_1_Test_Combine_ebf75c'Access);

      return Result'Access;

   end Suite;

end MyCub.WorkingMemory.Test_Data.Tests.Suite;
--  end read only
