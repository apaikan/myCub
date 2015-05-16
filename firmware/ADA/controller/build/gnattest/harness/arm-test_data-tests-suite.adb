--  This package has been generated automatically by GNATtest.
--  Do not edit any part of it, see GNATtest documentation for more details.

--  begin read only
with AUnit.Test_Caller;
with Gnattest_Generated;

package body ARM.Test_Data.Tests.Suite is

   package Runner_1 is new AUnit.Test_Caller
     (GNATtest_Generated.GNATtest_Standard.ARM.Test_Data.Tests.Test);

   Result : aliased AUnit.Test_Suites.Test_Suite;

   Case_1_1_Test_BN_8ea9d4 : aliased Runner_1.Test_Case;
   Case_2_1_Test_High_2b39ec : aliased Runner_1.Test_Case;
   Case_3_1_Test_Low_1a50f4 : aliased Runner_1.Test_Case;
   Case_4_1_Test_Low_Byte_232e9d : aliased Runner_1.Test_Case;
   Case_5_1_Test_High_Byte_337f85 : aliased Runner_1.Test_Case;

   function Suite return AUnit.Test_Suites.Access_Test_Suite is
   begin

      Runner_1.Create
        (Case_1_1_Test_BN_8ea9d4,
         "arm.ads:83:4:",
         Test_BN_8ea9d4'Access);
      Runner_1.Create
        (Case_2_1_Test_High_2b39ec,
         "arm.ads:89:4:",
         Test_High_2b39ec'Access);
      Runner_1.Create
        (Case_3_1_Test_Low_1a50f4,
         "arm.ads:90:4:",
         Test_Low_1a50f4'Access);
      Runner_1.Create
        (Case_4_1_Test_Low_Byte_232e9d,
         "arm.ads:93:4:",
         Test_Low_Byte_232e9d'Access);
      Runner_1.Create
        (Case_5_1_Test_High_Byte_337f85,
         "arm.ads:99:4:",
         Test_High_Byte_337f85'Access);

      Result.Add_Test (Case_1_1_Test_BN_8ea9d4'Access);
      Result.Add_Test (Case_2_1_Test_High_2b39ec'Access);
      Result.Add_Test (Case_3_1_Test_Low_1a50f4'Access);
      Result.Add_Test (Case_4_1_Test_Low_Byte_232e9d'Access);
      Result.Add_Test (Case_5_1_Test_High_Byte_337f85'Access);

      return Result'Access;

   end Suite;

end ARM.Test_Data.Tests.Suite;
--  end read only
