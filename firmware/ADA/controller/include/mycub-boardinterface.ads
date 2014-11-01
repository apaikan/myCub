with System;
with ARM.Strings;       use ARM.Strings;
with Interfaces;    use Interfaces;
with Interfaces.C;  use Interfaces.C;
with ARM.Strings;   use ARM.Strings;
with Ada.Unchecked_Conversion;

package MyCub.BoardInterface is
  
    type Specific_String is record
        Size : Unsigned_8;
        Str  : AStr20;
    end record; 

    type MyCub_Command is array (Unsigned_8 range 1..4) of Specific_String; 
    
    procedure UartSetup; 
    pragma Import (C, UartSetup, "uart_setup");

    procedure ParseCommand(Str: ARM_String;
                           Last: Unsigned_8;
                           Commands: out MyCub_Command;
                           Count: out Unsigned_8); 
    -- uart task
    task UartTask is
        pragma Priority (UartTaskParam.Priority);
        pragma Storage_Size (2*1024);
    end UartTask;

    -- blinky
    task BlinkyTask is
         pragma Priority (BlinkyTaskParam.Priority);
         pragma Storage_Size (2*1024);
    end BlinkyTask;

end MyCub.BoardInterface;

