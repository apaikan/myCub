--  This package has been generated automatically by GNATtest.
--  Do not edit any part of it, see GNATtest documentation for more details.

--  begin read only
with MyCub.BoardInterface.Test_Data.Tests.Suite;
with ARM.Test_Data.Tests.Suite;
with MyCub.Controller.Test_Data.Tests.Suite;
with MyCub.WorkingMemory.Test_Data.Tests.Suite;
with SimpleMath.Test_Data.Tests.Suite;
with ARM.Int_Img.Test_Data.Tests.Suite;
with ARM.Strings.Edit.Test_Data.Tests.Suite;
with LM4F.ADC.Test_Data.Tests.Suite;
with LM4F.Gpio.Test_Data.Tests.Suite;
with LM4F.I2C.Test_Data.Tests.Suite;
with LM4F.Uart.Test_Data.Tests.Suite;

package body Gnattest_Main_Suite is

   Result : aliased AUnit.Test_Suites.Test_Suite;

   function Suite return AUnit.Test_Suites.Access_Test_Suite is
   begin

      Result.Add_Test (MyCub.BoardInterface.Test_Data.Tests.Suite.Suite);
      Result.Add_Test (ARM.Test_Data.Tests.Suite.Suite);
      Result.Add_Test (MyCub.Controller.Test_Data.Tests.Suite.Suite);
      Result.Add_Test (MyCub.WorkingMemory.Test_Data.Tests.Suite.Suite);
      Result.Add_Test (SimpleMath.Test_Data.Tests.Suite.Suite);
      Result.Add_Test (ARM.Int_Img.Test_Data.Tests.Suite.Suite);
      Result.Add_Test (ARM.Strings.Edit.Test_Data.Tests.Suite.Suite);
      Result.Add_Test (LM4F.ADC.Test_Data.Tests.Suite.Suite);
      Result.Add_Test (LM4F.Gpio.Test_Data.Tests.Suite.Suite);
      Result.Add_Test (LM4F.I2C.Test_Data.Tests.Suite.Suite);
      Result.Add_Test (LM4F.Uart.Test_Data.Tests.Suite.Suite);

      return Result'Access;

   end Suite;

end Gnattest_Main_Suite;
--  end read only
