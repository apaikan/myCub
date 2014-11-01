
with Ada.Real_Time;     use Ada.Real_Time;
with Interfaces.C;      use Interfaces.C;
with LM4F.Gpio;         use LM4F.Gpio;

package body MyCub.Controller is
    
    function SetPose(joint: Joint_ID; pos: Joint_POS) return Boolean
    is
        Ret : Boolean;
    begin
        Ret := SetPoseRaw(joint, pos);
        Current_Poses.Set(joint, pos);
        return Ret;
    end;
   
    function GetPose(joint: Joint_ID) return Joint_POS
    is
        pos : Joint_POS;
    begin
        Current_Poses.Get(joint, pos);
        return pos;
    end;

    function GoToPose(joint: Joint_ID; pos: Joint_POS; tms: Integer) return Boolean
    is
        Position : PositionSpeed;
    begin
        Position.Pos := pos;
        Position.Duration := tms;
        case joint is 
        when 0 =>
            Position_0.Put(Position);
        when 1 =>
            Position_1.Put(Position);
        when 2 =>
            Position_2.Put(Position);
        when 3 =>
            Position_3.Put(Position);
        when others =>
            return false;
        end case;
        return true;
    end; 

    --
    -- protected 
    -- 
    protected body Current_Poses is
        procedure Set(joint: Joint_ID; pos: Joint_POS) is
        begin
            Container(joint) := pos;
        end Set;

        procedure Get(joint: Joint_ID; pos : out Joint_POS) is
        begin
            pos := Container(joint);
        end Get;
    end Current_Poses;

    protected body Position_0 is
        procedure Put(Data : PositionSpeed) is
        begin
            Container := Data;
            Received := True;
        end Put;

        entry Get(Data : out PositionSpeed)
            when Received is
        begin
            Data := Container;
            Received := False;
        end Get;
    end Position_0;

    protected body Position_1 is
        procedure Put(Data : PositionSpeed) is
        begin
            Container := Data;
            Received := True;
        end Put;

        entry Get(Data : out PositionSpeed)
            when Received is
        begin
            Data := Container;
            Received := False;
        end Get;
    end Position_1;

    protected body Position_2 is
        procedure Put(Data : PositionSpeed) is
        begin
            Container := Data;
            Received := True;
        end Put;

        entry Get(Data : out PositionSpeed)
            when Received is
        begin
            Data := Container;
            Received := False;
        end Get;
    end Position_2;

    protected body Position_3 is
        procedure Put(Data : PositionSpeed) is
        begin
            Container := Data;
            Received := True;
        end Put;

        entry Get(Data : out PositionSpeed)
            when Received is
        begin
            Data := Container;
            Received := False;
        end Get;
    end Position_3;

    procedure PositionController(joint: Joint_ID; Current: Joint_POS; Target: PositionSpeed)
    is
        Next_Time : Time;
        Resolution : Integer;
        Ret : Boolean;
    begin
        Ret := True;
        Next_Time := Clock;
        if Current < Target.Pos then
            Resolution := Target.Duration / (Target.Pos - Current);
            for I in Current .. Target.Pos loop
                Ret := Ret and SetPose(joint, I);
                Next_Time := Next_Time + Milliseconds(Resolution);
                delay until Next_Time;            
            end loop;
        elsif Current > Target.Pos then
            Resolution := Target.Duration / (Current - Target.Pos);
            for I in reverse Target.Pos .. Current loop
                Ret := Ret and SetPose(joint, I);
                Next_Time := Next_Time + Milliseconds(Resolution);
                delay until Next_Time;
            end loop;
        end if;
    end;

    --
    -- PositionController_0 task
    --
    task body PositionController_Task0 is
        Target : PositionSpeed;
        Current : Joint_POS;
    begin
         loop
            -- wait until new pos is requested
            Position_0.Get(Target);
            Current := GetPose(0); 
            PositionController(0, Current, Target);            
        end loop;
    end PositionController_Task0;

    --
    -- PositionController_1 task
    --
    task body PositionController_Task1 is
        Target : PositionSpeed;
        Current : Joint_POS;
    begin
         loop
            -- wait until new pos is requested
            Position_1.Get(Target);
            Current := GetPose(1); 
            PositionController(1, Current, Target);            
        end loop;
    end PositionController_Task1;

    --
    -- PositionController_2 task
    --
    task body PositionController_Task2 is
        Target : PositionSpeed;
        Current : Joint_POS;
    begin
         loop
            -- wait until new pos is requested
            Position_2.Get(Target);
            Current := GetPose(2); 
            PositionController(2, Current, Target);            
        end loop;
    end PositionController_Task2;

    --
    -- PositionController_3 task
    --
    task body PositionController_Task3 is
        Target : PositionSpeed;
        Current : Joint_POS;
    begin
         loop
            -- wait until new pos is requested
            Position_3.Get(Target);
            Current := GetPose(3); 
            PositionController(3, Current, Target);            
        end loop;
    end PositionController_Task3;

end MyCub.Controller;

