with Interfaces;    use Interfaces;
with Interfaces.C;  use Interfaces.C;
with LM4F.I2C;      use LM4F.I2C;
with Ada.Unchecked_Conversion;

package body ARM.I2C is

    --
    -- SetAddress
    --
    procedure SetAddress(Address : Integer; Register: Integer) is
    begin
        Device.Address := Address;
        Device.Register := Register; 
    end SetAddress;

    --
    -- SetFrequency
    --
    procedure SetFrequency(Frequency: Integer) is
    begin
        if Frequency = 400 then
            MasterInitExpClk(Base, TickPerSec, 1);            
        else
            MasterInitExpClk(Base, TickPerSec, 0);
        end if;
    end SetFrequency;


    --
    -- Write (Byte)
    --
    function Write(Data : Byte) return Boolean is
    begin
        -- Wait until master module is done transferring.
        while MasterBusy(Base) /= 0 loop Null; end loop;

        -- Tell the master module what address it will place on the bus when
        -- writing to the slave.
        MasterSlaveAddrSet(Base, Device.Address, 0);

        -- Place the command to be sent in the data register.
        MasterDataPut(Base, Device.Register);

        -- Initiate send of data from the master.
        MasterControl(Base, I2C_MASTER_CMD_BURST_SEND_START);

        -- Wait until master module is done transferring.
        while MasterBusy(Base) /= 0 loop Null; end loop;

        -- Check for errors.
        if MasterErr(Base) /= I2C_MASTER_ERR_NONE then
            return False;
        end if;

        -- Place the value to be sent in the data register.
        MasterDataPut(Base, Integer(Data));

        -- Initiate send of data from the master.
        MasterControl(Base, I2C_MASTER_CMD_BURST_SEND_CONT);

        -- Wait until master module is done transferring.
        while MasterBusy(Base) /= 0 loop Null; end loop;

        -- Check for errors.
        if MasterErr(Base) /= I2C_MASTER_ERR_NONE then
            return False;
        end if;
        
        --Initiate send of data from the master.        
        MasterControl(Base, I2C_MASTER_CMD_BURST_SEND_FINISH);

        -- Wait until master module is done transferring.
        while MasterBusy(Base) /= 0 loop Null; end loop;

        -- Check for errors.
        if MasterErr(Base) /= I2C_MASTER_ERR_NONE then
            return False;
        end if;
        
        return True;
    end;


    --
    -- Write (Buffer)
    --
    function Write(Data : ByteVector; Len : Integer) return Boolean is
        MasterOptionCommand : Long_Integer;
    begin
        -- Wait until master module is done transferring.
        while MasterBusy(Base) /= 0 loop Null; end loop;

        -- Tell the master module what address it will place on the bus when
        -- writing to the slave.
        MasterSlaveAddrSet(Base, Device.Address, 0);

        -- Place the value to be sent in the data register.
        MasterDataPut(Base, Device.Register);

        -- Initiate send of data from the master.
        MasterControl(Base, I2C_MASTER_CMD_BURST_SEND_START);

        -- Wait until master module is done transferring.
        while MasterBusy(Base) /= 0 loop Null; end loop;

        -- Check for errors.
        if MasterErr(Base) /= I2C_MASTER_ERR_NONE then
            return False;
        end if;

        -- Start with CONT for more than one byte to write
        MasterOptionCommand := I2C_MASTER_CMD_BURST_SEND_CONT;

        for I in Integer range 1 .. Len  loop
            -- The second and intermittent byte has to be send with CONTINUE control word
            if I = 2 then
                MasterOptionCommand := I2C_MASTER_CMD_BURST_SEND_CONT;
            end if;    

            -- The last byte has to be send with FINISH control word
            if I = Len then 
                MasterOptionCommand := I2C_MASTER_CMD_BURST_SEND_FINISH;
            end if;

            -- Re-configure to SINGLE if there is only one byte to write
            if Len = 1 then
                MasterOptionCommand := I2C_MASTER_CMD_SINGLE_SEND;
            end if;

            -- Send data byte
            MasterDataPut(Base, Integer(Data(I)));

            -- Initiate send of data from the master.
            MasterControl(Base, MasterOptionCommand);

            -- Wait until master module is done transferring.
            while MasterBusy(Base) /= 0 loop Null; end loop;

            -- Check for errors.
            if MasterErr(Base) /= I2C_MASTER_ERR_NONE then
                return False;
            end if;    
        end loop;

        return True;
    end;

    --
    -- Read
    --
    procedure Read(Data : out ByteVector; Len : in out Integer) is
        MasterOptionCommand : Long_Integer;
    begin

        -- Wait until master module is done transferring.
        while MasterBusy(Base) /= 0 loop Null; end loop;

        -- Tell the master module what address it will place on the bus when
        -- writing to the slave.
        MasterSlaveAddrSet(Base, Device.Address, 0);

        -- Place the value to be sent in the data register.
        MasterDataPut(Base, Device.Register);

        -- Initiate send of data from the master.
        MasterControl(Base, I2C_MASTER_CMD_SINGLE_SEND);

        -- Wait until master module is done transferring.
        while MasterBusy(Base) /= 0 loop Null; end loop;

        -- Check for errors.
        if MasterErr(Base) /= I2C_MASTER_ERR_NONE then
            Len := 0;
            return;
        end if;
        
        --Tell the master module what address it will place on the bus when
        -- reading from the slave.
        MasterSlaveAddrSet(Base, Device.Address, 1);

        -- Start with BURST for more than one byte to write
        MasterOptionCommand := I2C_MASTER_CMD_BURST_RECEIVE_START;

        for I in Integer range 1 .. Len loop
            -- The second and intermittent byte has to be read with CONTINUE control word
            if I = 2 then
                MasterOptionCommand := I2C_MASTER_CMD_BURST_RECEIVE_CONT;
            end if;    

            -- The last byte has to be send with FINISH control word
            if I = Len then 
                MasterOptionCommand := I2C_MASTER_CMD_BURST_RECEIVE_FINISH;
            end if;

            -- Re-configure to SINGLE if there is only one byte to write
            if Len = 1 then
                MasterOptionCommand := I2C_MASTER_CMD_SINGLE_RECEIVE;
            end if;

            -- Initiate read of data from the slave.
            MasterControl(Base, MasterOptionCommand);

            -- Wait until master module is done reading.
            while MasterBusy(Base) /= 0 loop Null; end loop;

            -- Check for errors.
            if MasterErr(Base) /= I2C_MASTER_ERR_NONE then
                Len := 0;
                return;
            end if;    
            Data(I) := Byte(MasterDataGet(Base));
        end loop;
    end;

end ARM.I2C;

