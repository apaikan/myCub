with System;
with Interfaces;    use Interfaces;

package MyCub.WorkingMemory is
  
    type Orientation is record
        X : Float;
        Y : Float;
        Z : Float;
    end record;
    
    type BatteryStatus is record
        Voltage: Integer;
        Current: Integer;
    end record;

    function GetBatteryVolt return Integer; 
    function GetBatteryCurrent return Integer; 
    function GetDistance return Integer; 
    function GetOrientation return Orientation;
    function GetMotion return Boolean;

    protected BatteryRaw is
        procedure Put (Data : BatteryStatus);
        procedure Get (Data : out BatteryStatus);
    private
        Container : BatteryStatus;
    end BatteryRaw;

    protected DistanceRaw is
        procedure Put (Data : Integer);
        procedure Get (Data : out Integer);
    private
        Container : Integer;
    end DistanceRaw;

    protected OrientationRaw is
        procedure Put (Data : Orientation);
        procedure Get (Data : out Orientation);
    private
        Container : Orientation;
    end OrientationRaw;

    protected MotionRaw is
        procedure Put (Data : Boolean);
        procedure Get (Data : out Boolean);
    private
        Container : Boolean;
    end MotionRaw;

    -- tasks
    task BatteryStatusUpdater is
        pragma Priority (BatTaskParam.Priority);
        pragma Storage_Size (2*1024);
    end BatteryStatusUpdater;

    task OrientationStatusUpdater is
        pragma Priority (CmpsTaskParam.Priority);
        pragma Storage_Size (2*1024);
    end OrientationStatusUpdater;

    task MotionStatusUpdater is
        pragma Priority (MotionTaskParam.Priority);
        pragma Storage_Size (2*1024);
    end MotionStatusUpdater;

    task SonarStatusUpdater is
        pragma Priority (SonarTaskParam.Priority);
        pragma Storage_Size (2*1024);
    end SonarStatusUpdater;

private
    function Combine(lo : Unsigned_8; hi : Unsigned_8) return Integer;
    pragma Import (C, Combine, "combine");

end MyCub.WorkingMemory;


