--  This package has been generated automatically by GNATtest.
--  Do not edit any part of it, see GNATtest documentation for more details.

--  begin read only
with AUnit.Test_Caller;
with Gnattest_Generated;

package body LM4F.Uart.Test_Data.Tests.Suite is

   package Runner_1 is new AUnit.Test_Caller
     (GNATtest_Generated.GNATtest_Standard.LM4F.Uart.Test_Data.Tests.Test);

   Result : aliased AUnit.Test_Suites.Test_Suite;

   Case_1_1_Test_IntEnable_45899e : aliased Runner_1.Test_Case;
   Case_2_1_Test_IntDisable_857b29 : aliased Runner_1.Test_Case;
   Case_3_1_Test_IntStatus_251f08 : aliased Runner_1.Test_Case;
   Case_4_1_Test_IntClear_ca438a : aliased Runner_1.Test_Case;
   Case_5_1_Test_CharsAvail_620643 : aliased Runner_1.Test_Case;
   Case_6_1_Test_SpaceAvail_f972e9 : aliased Runner_1.Test_Case;
   Case_7_1_Test_CharGet_ff0cc0 : aliased Runner_1.Test_Case;
   Case_8_1_Test_CharGetNonBlocking_519497 : aliased Runner_1.Test_Case;
   Case_9_1_Test_CharPut_948d0f : aliased Runner_1.Test_Case;
   Case_10_1_Test_CharPutNonBlocking_7e7cfe : aliased Runner_1.Test_Case;
   Case_11_1_Test_Busy_dd0a1a : aliased Runner_1.Test_Case;

   function Suite return AUnit.Test_Suites.Access_Test_Suite is
   begin

      Runner_1.Create
        (Case_1_1_Test_IntEnable_45899e,
         "lm4f-uart.ads:65:4:",
         Test_IntEnable_45899e'Access);
      Runner_1.Create
        (Case_2_1_Test_IntDisable_857b29,
         "lm4f-uart.ads:68:4:",
         Test_IntDisable_857b29'Access);
      Runner_1.Create
        (Case_3_1_Test_IntStatus_251f08,
         "lm4f-uart.ads:71:4:",
         Test_IntStatus_251f08'Access);
      Runner_1.Create
        (Case_4_1_Test_IntClear_ca438a,
         "lm4f-uart.ads:74:4:",
         Test_IntClear_ca438a'Access);
      Runner_1.Create
        (Case_5_1_Test_CharsAvail_620643,
         "lm4f-uart.ads:77:4:",
         Test_CharsAvail_620643'Access);
      Runner_1.Create
        (Case_6_1_Test_SpaceAvail_f972e9,
         "lm4f-uart.ads:80:4:",
         Test_SpaceAvail_f972e9'Access);
      Runner_1.Create
        (Case_7_1_Test_CharGet_ff0cc0,
         "lm4f-uart.ads:83:4:",
         Test_CharGet_ff0cc0'Access);
      Runner_1.Create
        (Case_8_1_Test_CharGetNonBlocking_519497,
         "lm4f-uart.ads:86:4:",
         Test_CharGetNonBlocking_519497'Access);
      Runner_1.Create
        (Case_9_1_Test_CharPut_948d0f,
         "lm4f-uart.ads:89:4:",
         Test_CharPut_948d0f'Access);
      Runner_1.Create
        (Case_10_1_Test_CharPutNonBlocking_7e7cfe,
         "lm4f-uart.ads:92:4:",
         Test_CharPutNonBlocking_7e7cfe'Access);
      Runner_1.Create
        (Case_11_1_Test_Busy_dd0a1a,
         "lm4f-uart.ads:95:4:",
         Test_Busy_dd0a1a'Access);

      Result.Add_Test (Case_1_1_Test_IntEnable_45899e'Access);
      Result.Add_Test (Case_2_1_Test_IntDisable_857b29'Access);
      Result.Add_Test (Case_3_1_Test_IntStatus_251f08'Access);
      Result.Add_Test (Case_4_1_Test_IntClear_ca438a'Access);
      Result.Add_Test (Case_5_1_Test_CharsAvail_620643'Access);
      Result.Add_Test (Case_6_1_Test_SpaceAvail_f972e9'Access);
      Result.Add_Test (Case_7_1_Test_CharGet_ff0cc0'Access);
      Result.Add_Test (Case_8_1_Test_CharGetNonBlocking_519497'Access);
      Result.Add_Test (Case_9_1_Test_CharPut_948d0f'Access);
      Result.Add_Test (Case_10_1_Test_CharPutNonBlocking_7e7cfe'Access);
      Result.Add_Test (Case_11_1_Test_Busy_dd0a1a'Access);

      return Result'Access;

   end Suite;

end LM4F.Uart.Test_Data.Tests.Suite;
--  end read only
