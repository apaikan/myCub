--  This package has been generated automatically by GNATtest.
--  Do not edit any part of it, see GNATtest documentation for more details.

--  begin read only
with AUnit.Test_Caller;
with Gnattest_Generated;

package body MyCub.BoardInterface.Test_Data.Tests.Suite is

   package Runner_1 is new AUnit.Test_Caller
     (GNATtest_Generated.GNATtest_Standard.MyCub.BoardInterface.Test_Data.Tests.Test);

   Result : aliased AUnit.Test_Suites.Test_Suite;

   Case_1_1_Test_UartSetup_a2af26 : aliased Runner_1.Test_Case;
   Case_2_1_Test_ParseCommand_469d10 : aliased Runner_1.Test_Case;

   function Suite return AUnit.Test_Suites.Access_Test_Suite is
   begin

      Runner_1.Create
        (Case_1_1_Test_UartSetup_a2af26,
         "mycub-boardinterface.ads:17:5:",
         Test_UartSetup_a2af26'Access);
      Runner_1.Create
        (Case_2_1_Test_ParseCommand_469d10,
         "mycub-boardinterface.ads:20:5:",
         Test_ParseCommand_469d10'Access);

      Result.Add_Test (Case_1_1_Test_UartSetup_a2af26'Access);
      Result.Add_Test (Case_2_1_Test_ParseCommand_469d10'Access);

      return Result'Access;

   end Suite;

end MyCub.BoardInterface.Test_Data.Tests.Suite;
--  end read only
