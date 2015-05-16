pragma Ada_95;
pragma Source_File_Name (ada_main, Spec_File_Name => "b~main.ads");
pragma Source_File_Name (ada_main, Body_File_Name => "b~main.adb");

package body ada_main is
   pragma Warnings (Off);
   procedure adainit is
      E17 : Boolean; pragma Import (Ada, E17, "system__bb__interrupts_E");
      E04 : Boolean; pragma Import (Ada, E04, "ada__real_time_E");
      E58 : Boolean; pragma Import (Ada, E58, "system__tasking__protected_objects_E");
      E66 : Boolean; pragma Import (Ada, E66, "system__tasking__protected_objects__multiprocessors_E");
      E81 : Boolean; pragma Import (Ada, E81, "system__tasking__restricted__stages_E");
      E86 : Boolean; pragma Import (Ada, E86, "arm__i2c_E");
      E74 : Boolean; pragma Import (Ada, E74, "arm__uart_E");
      E90 : Boolean; pragma Import (Ada, E90, "lm4f__htimer_E");
      E50 : Boolean; pragma Import (Ada, E50, "mycub_E");
      E52 : Boolean; pragma Import (Ada, E52, "mycub__boardinterface_E");
      E79 : Boolean; pragma Import (Ada, E79, "mycub__controller_E");
      E84 : Boolean; pragma Import (Ada, E84, "mycub__workingmemory_E");
      E92 : Boolean; pragma Import (Ada, E92, "simplemath_E");

   begin
      null;

      System.Bb.Interrupts'Elab_Body;
      E17 := True;
      Ada.Real_Time'Elab_Spec;
      Ada.Real_Time'Elab_Body;
      E04 := True;
      System.Tasking.Protected_Objects'Elab_Body;
      E58 := True;
      System.Tasking.Protected_Objects.Multiprocessors'Elab_Body;
      E66 := True;
      System.Tasking.Restricted.Stages'Elab_Body;
      E81 := True;
      E90 := True;
      E86 := True;
      E74 := True;
      MYCUB'ELAB_SPEC;
      E50 := True;
      Mycub.Boardinterface'Elab_Spec;
      Mycub.Controller'Elab_Spec;
      Mycub.Controller'Elab_Body;
      E79 := True;
      Mycub.Workingmemory'Elab_Spec;
      E92 := True;
      MYCUB.WORKINGMEMORY'ELAB_BODY;
      E84 := True;
      Mycub.Boardinterface'Elab_Body;
      E52 := True;
   end adainit;

   procedure Break_Start is
   begin
      null;
   end;

   procedure main is

      procedure Ada_Main_Program;
      pragma Import (Ada, Ada_Main_Program, "_ada_main");

      Ensure_Reference : aliased System.Address := Ada_Main_Program_Name'Address;
      pragma Volatile (Ensure_Reference);

   begin
      adainit;
      Break_Start;
      Ada_Main_Program;
   end;

--  BEGIN Object file/option list
   --   /home/apaikan/Install/myCub/firmware/ADA/controller/build/arm.o
   --   /home/apaikan/Install/myCub/firmware/ADA/controller/build/arm-strings.o
   --   /home/apaikan/Install/myCub/firmware/ADA/controller/build/lm4f.o
   --   /home/apaikan/Install/myCub/firmware/ADA/controller/build/lm4f-adc.o
   --   /home/apaikan/Install/myCub/firmware/ADA/controller/build/lm4f-gpio.o
   --   /home/apaikan/Install/myCub/firmware/ADA/controller/build/lm4f-htimer.o
   --   /home/apaikan/Install/myCub/firmware/ADA/controller/build/lm4f-i2c.o
   --   /home/apaikan/Install/myCub/firmware/ADA/controller/build/arm-i2c.o
   --   /home/apaikan/Install/myCub/firmware/ADA/controller/build/lm4f-uart.o
   --   /home/apaikan/Install/myCub/firmware/ADA/controller/build/arm-uart.o
   --   /home/apaikan/Install/myCub/firmware/ADA/controller/build/mycub.o
   --   /home/apaikan/Install/myCub/firmware/ADA/controller/build/main.o
   --   /home/apaikan/Install/myCub/firmware/ADA/controller/build/mycub-controller.o
   --   /home/apaikan/Install/myCub/firmware/ADA/controller/build/simplemath.o
   --   /home/apaikan/Install/myCub/firmware/ADA/controller/build/mycub-workingmemory.o
   --   /home/apaikan/Install/myCub/firmware/ADA/controller/build/mycub-boardinterface.o
   --   -L/home/apaikan/Install/myCub/firmware/ADA/controller/build/
   --   -L/home/apaikan/Install/ADA/arm-ada/toolchain/gnat-arm/lib/gcc/arm-none-eabi/4.6.2/rts-raven/adalib/
   --   -static
   --   -lgnarl
   --   -lgnat
--  END Object file/option list   

end ada_main;
