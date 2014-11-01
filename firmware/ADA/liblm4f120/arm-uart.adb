with Interfaces;    use Interfaces;
with Interfaces.C;  use Interfaces.C;
with ARM.Strings;   use ARM.Strings;
with LM4F.Uart;      use LM4F.Uart;
with Ada.Unchecked_Conversion;

package body ARM.Uart is

    procedure Put (Ch : Character) is
    begin
        CharPut (Port, To_U8 (Ch));
    end Put;

    procedure New_Line is
      EOL : constant := 16#0A#;
    begin
      CharPut(Port, EOL);
    end New_Line;


    procedure CRLF is
      LF : constant := 16#0A#;
      CR : constant := 16#0D#;
    begin
      CharPut (Port, CR);
      CharPut (Port, LF);
    end CRLF;


    procedure Put (S : ARM_String) is
    begin
      for I in S'Range loop
         Put (S (I));
      end loop;
    end Put;

    procedure Put_Line (S : ARM_String) is
    begin
      Put (S);
      --New_Line;
      CRLF;
    end Put_Line;


    procedure Get (Ch : out Character) is
    begin
        Monitor.Get(Ch);
    end Get;


    procedure Get_Line (S : out ARM_String; 
                        Last : out Unsigned_8;
                        Echo : in Boolean := False)
    is
        C : Character;
    begin
        for I in S'First .. S'Last loop
            Monitor.Get(C);
            if Echo = True then
                CharPut (Port, To_U8(C));     
            end if;
            if C = ASCII.CR or C = ASCII.LF then
                Last :=  I - 1;
                return;
            else
                S (I) := C;
            end if;
        end loop;
        Last := S'Last;
        return;
    end Get_Line;

    procedure IntToStr(Value : Integer;
                       Str : out ARM_String;
                       Last : out Unsigned_8)
    is
        Val : Integer := Value;
        Res : AStr20;
        Pos : Unsigned_8 := Res'Last;        
    begin
        Last := Str'First;
        if Val > 0 then
            Val := -Val;
         else
            Str(Last) := '-';
            Last := Last + 1;
        end if;
        loop
             Res (Pos) := Character'Val (Character'Pos ('0') - (Val mod (-10)));
             Val := Val / 10;
             exit when Val = 0;
             Pos := Pos - 1;
        end loop;
       
        for I in Pos .. Res'Last loop
            Str(Last) := Res(I);
            Last := Last + 1;
        end loop;
        Last := Last - 1;
    end;

    procedure LongToStr(Value : Long_Integer;
                       Str : out ARM_String;
                       Last : out Unsigned_8)
    is
        Val : Long_Integer := Value;
        Res : AStr20;
        Pos : Unsigned_8 := Res'Last;        
    begin
        Last := Str'First;
        if Val > 0 then
            Val := -Val;
         else
            Str(Last) := '-';
            Last := Last + 1;
        end if;
        loop
             Res (Pos) := Character'Val (Character'Pos ('0') - (Val mod (-10)));
             Val := Val / 10;
             exit when Val = 0;
             Pos := Pos - 1;
        end loop;
       
        for I in Pos .. Res'Last loop
            Str(Last) := Res(I);
            Last := Last + 1;
        end loop;
        Last := Last - 1;
    end;

    function StrToInt(Str: ARM_String; Last: Unsigned_8) return Integer
    is
        Val : Integer := 0;
        Pwr : Integer;
    begin
        for I in reverse 1..Last loop
            Pwr := 1;
            if I > 1 then
                for J in 2..I loop Pwr := Pwr * 10; end loop;
            end if;    
            Val := Val + (Character'Pos(Str(Last-I+1)) - Character'Pos('0')) * Pwr;
        end loop;
        return Val;
    end;

    --
    -- Monitor object
    -- 
    protected body Monitor is
        --pragma Volatile(Rx_Inx);
        procedure UartIntHandler is
            Flag   : Integer;
        begin
            Flag := IntStatus(Port, TRUE);
            IntClear(Port, Flag);
            --if Flag = (INT_RX + INT_RT) then
            if Flag = INT_RX or Flag = INT_RT then
                while CharsAvail(Port) = TRUE loop
                    Container(Rx_Inx) := To_Char(CharGetNonBlocking(Port));
                    if Rx_Inx = Container'Last then
                        Rx_Inx := Container'First;
                    else
                        Rx_Inx := Rx_Inx + 1;
                    end if; 
                    Available := True;
                end loop;
            end if;
        end UartIntHandler;

        entry Get(Data: out Character) when Available is
        begin
            Data := Container(Rx_Outx);

            if Rx_Outx = Container'Last then
               Rx_Outx := Container'First;
            else
               Rx_Outx := Rx_Outx + 1;
            end if;            

            if Rx_Inx = Rx_Outx then
                Available := False;
            end if;
        end Get;       
    end Monitor;

end ARM.Uart;

