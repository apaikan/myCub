--  This package has been generated automatically by GNATtest.
--  Do not edit any part of it, see GNATtest documentation for more details.

--  begin read only
with AUnit.Test_Caller;
with Gnattest_Generated;

package body MyCub.Controller.Test_Data.Tests.Suite is

   package Runner_1 is new AUnit.Test_Caller
     (GNATtest_Generated.GNATtest_Standard.MyCub.Controller.Test_Data.Tests.Test);

   Result : aliased AUnit.Test_Suites.Test_Suite;

   Case_1_1_Test_StartController_0ec6a7 : aliased Runner_1.Test_Case;
   Case_2_1_Test_StopController_31ecc5 : aliased Runner_1.Test_Case;
   Case_3_1_Test_SetPose_c78047 : aliased Runner_1.Test_Case;
   Case_4_1_Test_GetPose_c54217 : aliased Runner_1.Test_Case;
   Case_5_1_Test_GoToPose_4b5a61 : aliased Runner_1.Test_Case;
   Case_6_1_Test_SetPoseRaw_56350f : aliased Runner_1.Test_Case;
   Case_7_1_Test_PositionController_701659 : aliased Runner_1.Test_Case;

   function Suite return AUnit.Test_Suites.Access_Test_Suite is
   begin

      Runner_1.Create
        (Case_1_1_Test_StartController_0ec6a7,
         "mycub-controller.ads:15:5:",
         Test_StartController_0ec6a7'Access);
      Runner_1.Create
        (Case_2_1_Test_StopController_31ecc5,
         "mycub-controller.ads:18:5:",
         Test_StopController_31ecc5'Access);
      Runner_1.Create
        (Case_3_1_Test_SetPose_c78047,
         "mycub-controller.ads:21:5:",
         Test_SetPose_c78047'Access);
      Runner_1.Create
        (Case_4_1_Test_GetPose_c54217,
         "mycub-controller.ads:22:5:",
         Test_GetPose_c54217'Access);
      Runner_1.Create
        (Case_5_1_Test_GoToPose_4b5a61,
         "mycub-controller.ads:24:5:",
         Test_GoToPose_4b5a61'Access);
      Runner_1.Create
        (Case_6_1_Test_SetPoseRaw_56350f,
         "mycub-controller.ads:86:5:",
         Test_SetPoseRaw_56350f'Access);
      Runner_1.Create
        (Case_7_1_Test_PositionController_701659,
         "mycub-controller.ads:89:5:",
         Test_PositionController_701659'Access);

      Result.Add_Test (Case_1_1_Test_StartController_0ec6a7'Access);
      Result.Add_Test (Case_2_1_Test_StopController_31ecc5'Access);
      Result.Add_Test (Case_3_1_Test_SetPose_c78047'Access);
      Result.Add_Test (Case_4_1_Test_GetPose_c54217'Access);
      Result.Add_Test (Case_5_1_Test_GoToPose_4b5a61'Access);
      Result.Add_Test (Case_6_1_Test_SetPoseRaw_56350f'Access);
      Result.Add_Test (Case_7_1_Test_PositionController_701659'Access);

      return Result'Access;

   end Suite;

end MyCub.Controller.Test_Data.Tests.Suite;
--  end read only
