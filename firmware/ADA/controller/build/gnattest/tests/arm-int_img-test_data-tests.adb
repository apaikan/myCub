--  This package has been generated automatically by GNATtest.
--  You are allowed to add your code to the bodies of test routines.
--  Such changes will be kept during further regeneration of this file.
--  All code placed outside of test routine bodies will be lost. The
--  code intended to set up and tear down the test environment should be
--  placed into ARM.Int_Img.Test_Data.

with AUnit.Assertions; use AUnit.Assertions;

package body ARM.Int_Img.Test_Data.Tests is


--  begin read only
   procedure Test_U32_Img (Gnattest_T : in out Test);
   procedure Test_U32_Img_2f7bb6 (Gnattest_T : in out Test) renames Test_U32_Img;
--  id:2.2/2f7bb62185ff0da3/U32_Img/1/0/
   procedure Test_U32_Img (Gnattest_T : in out Test) is
   --  arm-int_img.ads:46:4:U32_Img
--  end read only

      pragma Unreferenced (Gnattest_T);

   begin

      AUnit.Assertions.Assert
        (Gnattest_Generated.Default_Assert_Value,
         "Test not implemented.");

--  begin read only
   end Test_U32_Img;
--  end read only

end ARM.Int_Img.Test_Data.Tests;
