--  This package has been generated automatically by GNATtest.
--  Do not edit any part of it, see GNATtest documentation for more details.

--  begin read only
with AUnit.Test_Caller;
with Gnattest_Generated;

package body ARM.Int_Img.Test_Data.Tests.Suite is

   package Runner_1 is new AUnit.Test_Caller
     (GNATtest_Generated.GNATtest_Standard.ARM.Int_Img.Test_Data.Tests.Test);

   Result : aliased AUnit.Test_Suites.Test_Suite;

   Case_1_1_Test_U32_Img_2f7bb6 : aliased Runner_1.Test_Case;

   function Suite return AUnit.Test_Suites.Access_Test_Suite is
   begin

      Runner_1.Create
        (Case_1_1_Test_U32_Img_2f7bb6,
         "arm-int_img.ads:46:4:",
         Test_U32_Img_2f7bb6'Access);

      Result.Add_Test (Case_1_1_Test_U32_Img_2f7bb6'Access);

      return Result'Access;

   end Suite;

end ARM.Int_Img.Test_Data.Tests.Suite;
--  end read only
