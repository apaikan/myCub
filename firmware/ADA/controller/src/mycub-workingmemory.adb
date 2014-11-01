
with Ada.Real_Time;     use Ada.Real_Time;
with Interfaces.C;      use Interfaces.C;
with LM4F.ADC;          use LM4F.ADC;
with LM4F.Gpio;         use LM4F.Gpio;
with LM4F.HTimer;
with ARM.I2C;
with SimpleMath;        use SimpleMath;

package body MyCub.WorkingMemory is

    package HTime is new LM4F.HTimer;
    package I2C is new ARM.I2C(Base => 16#40021000#); --I2C 1

    --
    -- GetBatteryVolt
    --
    function GetBatteryVolt return Integer
    is
        RawData : BatteryStatus;
    begin
        BatteryRaw.Get(RawData);
        return Integer(Float(RawData.Voltage) * 0.805860806 * 11.0);
    end GetBatteryVolt;

    --
    -- getBatteryCurrent
    --
    function GetBatteryCurrent return Integer
    is
        RawData : BatteryStatus;
    begin
        BatteryRaw.Get(RawData);
        return Integer(Float(RawData.Current));
    end GetBatteryCurrent;

    --
    --  GetDistance 
    --
    function GetDistance return Integer
    is
        RawData : Integer;
    begin
        DistanceRaw.Get(RawData);
        return RawData;
    end getDistance;

    --
    -- GetOrientation
    --
    function GetOrientation return Orientation 
    is
        RawData : Orientation;
    begin
        OrientationRaw.Get(RawData);
        return RawData;
    end GetOrientation;

    --
    -- protected BatteryRaw
    -- 
    protected body BatteryRaw is
        procedure Put(Data : BatteryStatus) is
        begin
            Container := Data;
        end Put;

        procedure Get(Data : out BatteryStatus) is
        begin
            Data := Container;
        end Get;
    end BatteryRaw;

    --
    -- protected DistanceRaw
    -- 
    protected body DistanceRaw is
        procedure Put(Data : Integer) is
        begin
            Container := Data;
        end Put;

        procedure Get(Data : out Integer) is
        begin
            Data := Container;
        end Get;
    end DistanceRaw;

    --
    -- protected OrientationRaw
    -- 
    protected body OrientationRaw is
        procedure Put(Data : Orientation) is
        begin
            Container := Data;
        end Put;

        procedure Get(Data : out Orientation) is
        begin
            Data := Container;
        end Get;
    end OrientationRaw;

    --
    -- BatteryStatusUpdater task
    --
    task body BatteryStatusUpdater is
        Activation : Time := Clock;
        Value : aliased Integer := 0;
        Status : BatteryStatus;
        Ret : Integer := 0;
    begin
        loop
            delay until Activation;
            Activation := Activation + BatTaskParam.Period;

            -- reading adc value channel 6 for battery voltage
            SequenceDisable(ADC0_BASE, 3);
            SequenceConfigure(ADC0_BASE, 3, ADC_TRIGGER_PROCESSOR, 0);
            SequenceStepConfigure(ADC0_BASE, 3, 0, ADC_CTL_CH6 + ADC_CTL_IE + ADC_CTL_END);
            SequenceEnable(ADC0_BASE, 3);
            IntClear(ADC0_BASE, 3);           
            ProcessorTrigger(ADC0_BASE, 3);
            -- while IntStatus(ADC0_BASE, 3, 0) /= 0 loop null; end loop;
            IntClear(ADC0_BASE, 3);
            Ret := SequenceDataGet(ADC0_BASE, 3, Value'Access);
            Status.Voltage := Value;

            -- reading adc value channel 5 for battery current
            SequenceDisable(ADC0_BASE, 3);
            SequenceConfigure(ADC0_BASE, 3, ADC_TRIGGER_PROCESSOR, 0);
            SequenceStepConfigure(ADC0_BASE, 3, 0, ADC_CTL_CH5 + ADC_CTL_IE + ADC_CTL_END);
            SequenceEnable(ADC0_BASE, 3);
            IntClear(ADC0_BASE, 3);           
            ProcessorTrigger(ADC0_BASE, 3);
            -- while IntStatus(ADC0_BASE, 3, 0) /= 0 loop null; end loop;
            IntClear(ADC0_BASE, 3);
            Ret := SequenceDataGet(ADC0_BASE, 3, Value'Access);
            Status.Current := Value;

            -- update Status
            BatteryRaw.Put(Status);
        end loop;
    end BatteryStatusUpdater;

    --
    -- SonarStatusUpdater
    --
    task body SonarStatusUpdater is
        Next_Time : Time := Clock;
        Value : Integer := 0;
        -- reading pulse width 
        function PulseIn(Port : Integer; 
                         Pin : Integer; TimeoutMs : Long_Integer) return Long_Integer is
        Now : Long_Integer;       
        begin
            -- wait for any previous pulse to finish
            Now := HTime.GetTick; 
            while PinRead(Port, Pin) /= 0 loop 
                if HTime.GetSpanMsec(Now, HTime.GetTick) > TimeoutMs then
                    return -1;
                end if;
            end loop;
            
            -- wait for pulse to begin
            Now := HTime.GetTick; 
            while PinRead(Port, Pin) = 0 loop 
                if HTime.GetSpanMsec(Now, HTime.GetTick) > TimeoutMs then
                    return -1;
                end if;
            end loop;
            
            -- wait for pulse to finish
            Now := HTime.GetTick; 
            while PinRead(Port, Pin) /= 0 loop 
                if HTime.GetSpanMsec(Now, HTime.GetTick) > TimeoutMs then
                    return -1;
                end if;
            end loop;
            return HTime.GetSpanUsec(Now, HTime.GetTick); 
        end;

      begin
        HTime.Init;
        PinTypeOutput(PORTA, PIN2);     -- Trig pin
        PinTypeInput(PORTA, PIN3);      -- Echo pin
        loop
            Next_Time := Next_Time + SonarTaskParam.Period;
            delay until Next_Time; 

            -- trriging the sensor
            PinWrite(PORTA, PIN2, 0);
            HTime.WaitUsec(10);
            PinWrite(PORTA, PIN2, PIN2);
            HTime.WaitUsec(10);
            PinWrite(PORTA, PIN2, 0);
            
            -- reading the pulse width
            Value := Integer(Float(PulseIn(PORTA, PIN3, 500)) / 5.246);
            DistanceRaw.Put(Value);
        end loop;
    end SonarStatusUpdater;

    --
    -- OrientationStatusUpdater task
    --
    task body OrientationStatusUpdater is
        Activation : Time := Clock;
        Value : Orientation := (X => 0.0, Y => 0.0, Z => 0.0);
        Ret : Boolean;

        HMC5883_ADDRESS_MAG            : constant := 2#0001_1110#;
        HMC5883_REGISTER_MAG_CRB_REG_M : constant := 16#01#;
        HMC5883_REGISTER_MAG_MR_REG_M  : constant := 16#02#;
        HMC5883_REGISTER_MAG_OUT_X_H_M : constant := 16#03#;            
        HMC5883_MAGGAIN_1_3            : constant := 16#20#;
        -- for Gain 1.3 the LSB is defined as below:
        GAUSS_LSB_XY                   : constant := 1100.0; 
        GAUSS_LSB_Z                    : constant := 980.0; 
        GAUSS_TO_MICROTESLA            : constant := 100.0;

        --
        -- InitCompass
        --
        function InitCompass return Boolean is
        begin
            I2C.SetFrequency(400);

            -- Enable the magnetometer
            I2C.SetAddress(HMC5883_ADDRESS_MAG, HMC5883_REGISTER_MAG_MR_REG_M);
            if I2C.Write(0) = False then
                return False;
            end if;
            
            -- Set the gain to a known level
            I2C.SetAddress(HMC5883_ADDRESS_MAG, HMC5883_REGISTER_MAG_CRB_REG_M);
            if I2C.Write(HMC5883_MAGGAIN_1_3) = False then
                return False;
            end if;
            return True;
        end;

        --
        -- ReadCompass
        --
        procedure ReadCompass(Orit : out Orientation; Succeed : out Boolean) is
            Data : I2C.ByteVector (1 .. 8); 
            Len : Integer;            
        begin
            I2C.SetAddress(HMC5883_ADDRESS_MAG, HMC5883_REGISTER_MAG_OUT_X_H_M);
            Len := 6;
            I2C.Read(Data, Len);
            if Len /= 6 then 
                Succeed := False;
                return;
            end if;
            
            Orit.X := Float( Combine(Data(2), Data(1)) );
            Orit.Y := Float( Combine(Data(6), Data(5)) );
            Orit.Z := Float( Combine(Data(4), Data(3)) );

            Orit.X := Orit.X / GAUSS_LSB_XY * GAUSS_TO_MICROTESLA;
            Orit.Y := Orit.Y / GAUSS_LSB_XY * GAUSS_TO_MICROTESLA;
            Orit.Z := Orit.Z / GAUSS_LSB_Z * GAUSS_TO_MICROTESLA;

           -- Heading := Atan2(Y, X);
           -- -- Once you have your heading, you must then add your 'Declination Angle', which is the 'Error' of the magnetic field in your location.
           -- -- Find yours here: http://www.magnetic-declination.com/
           -- -- Mine is: -13* 2' W, which is ~13 Degrees, or (which we need) 0.22 radians
           -- -- If you cannot find your Declination, comment out these two lines, your compass will be slightly off.
           -- Heading := Heading + 0.22;

           -- -- Correct for when signs are reversed.
           -- if Heading < 0.0 then
           --     Heading := Heading + 2.0 * M_PI;
           -- end if;    

           -- -- Check for wrap due to addition of declination.
           -- if Heading > (2.0 * M_PI) then 
           --     Heading := Heading - 2.0 * M_PI;
           -- end if;

           -- -- Convert radians to degrees for readability.
           -- Heading := Heading * 180.0 / M_PI; 
            Succeed := True;
        end;

    begin
        Ret := InitCompass; 
        loop
            delay until Activation;
            Activation := Activation + CmpsTaskParam.Period;
            -- reading the compass value
            if Ret = True then 
                ReadCompass(Value, Ret);
                OrientationRaw.Put(Value);
            else
                Value.X := 0.0; Value.Y := 0.0; Value.Z := 0.0;
                OrientationRaw.Put(Value);
            end if;    
        end loop;
    end OrientationStatusUpdater;

end MyCub.WorkingMemory;

