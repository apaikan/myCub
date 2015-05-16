--  This package has been generated automatically by GNATtest.
--  Do not edit any part of it, see GNATtest documentation for more details.

--  begin read only
with AUnit.Test_Caller;
with Gnattest_Generated;

package body ARM.Strings.Edit.Test_Data.Tests.Suite is

   package Runner_1 is new AUnit.Test_Caller
     (GNATtest_Generated.GNATtest_Standard.ARM.Strings.Edit.Test_Data.Tests.Test);

   Result : aliased AUnit.Test_Suites.Test_Suite;

   Case_1_1_Test_Length_474eb4 : aliased Runner_1.Test_Case;
   Case_2_1_Test_First_01dcae : aliased Runner_1.Test_Case;
   Case_3_1_Test_Last_8a379c : aliased Runner_1.Test_Case;
   Case_4_1_Test_Skip_e58a7c : aliased Runner_1.Test_Case;
   Case_5_1_Test_Get_Str_0df929 : aliased Runner_1.Test_Case;
   Case_6_1_Test_Get_Str_9410ed : aliased Runner_1.Test_Case;
   Case_7_1_Test_Put_56cc4b : aliased Runner_1.Test_Case;
   Case_8_1_Test_Put_f39da1 : aliased Runner_1.Test_Case;
   Case_9_1_Test_Put_Line_43fcf8 : aliased Runner_1.Test_Case;
   Case_10_1_Test_New_Line_a03cfd : aliased Runner_1.Test_Case;
   Case_11_1_Test_CRLF_bcbd7e : aliased Runner_1.Test_Case;

   function Suite return AUnit.Test_Suites.Access_Test_Suite is
   begin

      Runner_1.Create
        (Case_1_1_Test_Length_474eb4,
         "arm-strings-edit.ads:29:4:",
         Test_Length_474eb4'Access);
      Runner_1.Create
        (Case_2_1_Test_First_01dcae,
         "arm-strings-edit.ads:30:4:",
         Test_First_01dcae'Access);
      Runner_1.Create
        (Case_3_1_Test_Last_8a379c,
         "arm-strings-edit.ads:31:4:",
         Test_Last_8a379c'Access);
      Runner_1.Create
        (Case_4_1_Test_Skip_e58a7c,
         "arm-strings-edit.ads:42:4:",
         Test_Skip_e58a7c'Access);
      Runner_1.Create
        (Case_5_1_Test_Get_Str_0df929,
         "arm-strings-edit.ads:46:4:",
         Test_Get_Str_0df929'Access);
      Runner_1.Create
        (Case_6_1_Test_Get_Str_9410ed,
         "arm-strings-edit.ads:47:4:",
         Test_Get_Str_9410ed'Access);
      Runner_1.Create
        (Case_7_1_Test_Put_56cc4b,
         "arm-strings-edit.ads:64:4:",
         Test_Put_56cc4b'Access);
      Runner_1.Create
        (Case_8_1_Test_Put_f39da1,
         "arm-strings-edit.ads:68:4:",
         Test_Put_f39da1'Access);
      Runner_1.Create
        (Case_9_1_Test_Put_Line_43fcf8,
         "arm-strings-edit.ads:72:4:",
         Test_Put_Line_43fcf8'Access);
      Runner_1.Create
        (Case_10_1_Test_New_Line_a03cfd,
         "arm-strings-edit.ads:77:4:",
         Test_New_Line_a03cfd'Access);
      Runner_1.Create
        (Case_11_1_Test_CRLF_bcbd7e,
         "arm-strings-edit.ads:78:4:",
         Test_CRLF_bcbd7e'Access);

      Result.Add_Test (Case_1_1_Test_Length_474eb4'Access);
      Result.Add_Test (Case_2_1_Test_First_01dcae'Access);
      Result.Add_Test (Case_3_1_Test_Last_8a379c'Access);
      Result.Add_Test (Case_4_1_Test_Skip_e58a7c'Access);
      Result.Add_Test (Case_5_1_Test_Get_Str_0df929'Access);
      Result.Add_Test (Case_6_1_Test_Get_Str_9410ed'Access);
      Result.Add_Test (Case_7_1_Test_Put_56cc4b'Access);
      Result.Add_Test (Case_8_1_Test_Put_f39da1'Access);
      Result.Add_Test (Case_9_1_Test_Put_Line_43fcf8'Access);
      Result.Add_Test (Case_10_1_Test_New_Line_a03cfd'Access);
      Result.Add_Test (Case_11_1_Test_CRLF_bcbd7e'Access);

      return Result'Access;

   end Suite;

end ARM.Strings.Edit.Test_Data.Tests.Suite;
--  end read only
