with System;

package MyCub.Controller is
   
    subtype Joint_ID is Integer range 0..3;
    subtype Joint_POS is Integer range 0..180;
    type Joint_Context is array (0..3) of Joint_POS;

    type PositionSpeed is record
        Pos      : Joint_POS;
        Duration : Integer;
    end record; 


    function StartController return Boolean; 
    pragma Import (C, StartController, "servo_start");

    procedure StopController; 
    pragma Import (C, StopController, "servo_shutdown");

    function SetPose(joint: Joint_ID; pos: Joint_POS) return Boolean;
    function GetPose(joint: Joint_ID) return Joint_POS;

    function GoToPose(joint: Joint_ID; pos: Joint_POS; tms: Integer) return Boolean;
    --function GoToPoseSync(joint: Joint_ID; pos: Joint_POS; tms: Long_Integer) return Boolean;
    --function CheckMotionDone(joint: Joint_ID) return Boolean;
    --function CheckMotionDone return Boolean;
    --function GetContext return Joint_Context;
    --function SetContext(ctx: Joint_Context) return Boolean;

    protected Position_0 is
        procedure Put (Data : PositionSpeed);
        entry Get (Data : out PositionSpeed);
    private
        Container : PositionSpeed;
        Received  : Boolean := False;
    end Position_0;

    protected Position_1 is
        procedure Put (Data : PositionSpeed);
        entry Get (Data : out PositionSpeed);
    private
        Container : PositionSpeed;
        Received  : Boolean := False;
    end Position_1;

    protected Position_2 is
        procedure Put (Data : PositionSpeed);
        entry Get (Data : out PositionSpeed);
    private
        Container : PositionSpeed;
        Received  : Boolean := False;
    end Position_2;

    protected Position_3 is
        procedure Put (Data : PositionSpeed);
        entry Get (Data : out PositionSpeed);
    private
        Container : PositionSpeed;
        Received  : Boolean := False;
    end Position_3;

    -- tasks
    task PositionController_Task0 is
        pragma Priority (Ctrl0TaskParam.Priority);
        pragma Storage_Size (2*1024);
    end PositionController_Task0;

    task PositionController_Task1 is
        pragma Priority (Ctrl1TaskParam.Priority);
        pragma Storage_Size (2*1024);
    end PositionController_Task1;

    task PositionController_Task2 is
        pragma Priority (Ctrl2TaskParam.Priority);
        pragma Storage_Size (2*1024);
    end PositionController_Task2;

    task PositionController_Task3 is
        pragma Priority (Ctrl3TaskParam.Priority);
        pragma Storage_Size (2*1024);
    end PositionController_Task3;

private

    function SetPoseRaw(joint: Joint_ID; pos: Joint_POS) return Boolean;
    pragma Import (C, SetPoseRaw, "servo_setpos");

    procedure PositionController(joint: Joint_ID; Current: Joint_POS; Target: PositionSpeed);

    protected Current_Poses is
       procedure Set(joint: Joint_ID; pos: Joint_POS); 
       procedure Get(joint: Joint_ID; pos : out Joint_POS);
    private
       Container : Joint_Context;
    end Current_Poses;

end MyCub.Controller;


