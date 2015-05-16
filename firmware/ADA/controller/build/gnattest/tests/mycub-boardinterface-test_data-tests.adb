--  This package has been generated automatically by GNATtest.
--  You are allowed to add your code to the bodies of test routines.
--  Such changes will be kept during further regeneration of this file.
--  All code placed outside of test routine bodies will be lost. The
--  code intended to set up and tear down the test environment should be
--  placed into MyCub.BoardInterface.Test_Data.

with AUnit.Assertions; use AUnit.Assertions;

package body MyCub.BoardInterface.Test_Data.Tests is


--  begin read only
   procedure Test_UartSetup (Gnattest_T : in out Test);
   procedure Test_UartSetup_a2af26 (Gnattest_T : in out Test) renames Test_UartSetup;
--  id:2.2/a2af2651bc6f6d7d/UartSetup/1/0/
   procedure Test_UartSetup (Gnattest_T : in out Test) is
   --  mycub-boardinterface.ads:17:5:UartSetup
--  end read only

      pragma Unreferenced (Gnattest_T);

   begin

      AUnit.Assertions.Assert
        (Gnattest_Generated.Default_Assert_Value,
         "Test not implemented.");

--  begin read only
   end Test_UartSetup;
--  end read only


--  begin read only
   procedure Test_ParseCommand (Gnattest_T : in out Test);
   procedure Test_ParseCommand_469d10 (Gnattest_T : in out Test) renames Test_ParseCommand;
--  id:2.2/469d1069a8e2e7e5/ParseCommand/1/0/
   procedure Test_ParseCommand (Gnattest_T : in out Test) is
   --  mycub-boardinterface.ads:20:5:ParseCommand
--  end read only

      pragma Unreferenced (Gnattest_T);

   begin

      AUnit.Assertions.Assert
        (Gnattest_Generated.Default_Assert_Value,
         "Test not implemented.");

--  begin read only
   end Test_ParseCommand;
--  end read only

end MyCub.BoardInterface.Test_Data.Tests;
