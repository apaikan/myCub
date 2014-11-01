pragma Task_Dispatching_Policy(FIFO_Within_Priorities);

with Ada.Real_Time;     use Ada.Real_Time;
with MyCub.BoardInterface;    use MyCub.BoardInterface;

procedure Main is
--        pragma Priority (120);
begin    
     loop
        null;
    end loop;
end Main;


