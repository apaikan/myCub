--  This package has been generated automatically by GNATtest.
--  Do not edit any part of it, see GNATtest documentation for more details.

--  begin read only
with AUnit.Test_Caller;
with Gnattest_Generated;

package body SimpleMath.Test_Data.Tests.Suite is

   package Runner_1 is new AUnit.Test_Caller
     (GNATtest_Generated.GNATtest_Standard.SimpleMath.Test_Data.Tests.Test);

   Result : aliased AUnit.Test_Suites.Test_Suite;

   Case_1_1_Test_Atan_6d67f5 : aliased Runner_1.Test_Case;
   Case_2_1_Test_Atan2_baee2c : aliased Runner_1.Test_Case;

   function Suite return AUnit.Test_Suites.Access_Test_Suite is
   begin

      Runner_1.Create
        (Case_1_1_Test_Atan_6d67f5,
         "simplemath.ads:40:5:",
         Test_Atan_6d67f5'Access);
      Runner_1.Create
        (Case_2_1_Test_Atan2_baee2c,
         "simplemath.ads:41:5:",
         Test_Atan2_baee2c'Access);

      Result.Add_Test (Case_1_1_Test_Atan_6d67f5'Access);
      Result.Add_Test (Case_2_1_Test_Atan2_baee2c'Access);

      return Result'Access;

   end Suite;

end SimpleMath.Test_Data.Tests.Suite;
--  end read only
