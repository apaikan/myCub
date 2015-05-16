pragma Ada_95;
pragma Restrictions (No_Exception_Propagation);
with System;
package ada_main is
   pragma Warnings (Off);


   GNAT_Version : constant String :=
                    "GNAT Version: 4.6.2" & ASCII.NUL;
   pragma Export (C, GNAT_Version, "__gnat_version");

   Ada_Main_Program_Name : constant String := "_ada_main" & ASCII.NUL;
   pragma Export (C, Ada_Main_Program_Name, "__gnat_ada_main_program_name");

   procedure adainit;
   pragma Export (C, adainit, "adainit");

   procedure Break_Start;
   pragma Export (C, Break_Start, "__gnat_break_start");

   procedure main;
   pragma Export (C, main, "main");

   type Version_32 is mod 2 ** 32;
   u00001 : constant Version_32 := 16#78230696#;
   pragma Export (C, u00001, "mainB");
   u00002 : constant Version_32 := 16#3ffc8e18#;
   pragma Export (C, u00002, "adaS");
   u00003 : constant Version_32 := 16#e86c1dfa#;
   pragma Export (C, u00003, "ada__real_timeB");
   u00004 : constant Version_32 := 16#91d158a6#;
   pragma Export (C, u00004, "ada__real_timeS");
   u00005 : constant Version_32 := 16#72fb9d13#;
   pragma Export (C, u00005, "systemS");
   u00006 : constant Version_32 := 16#1aa31618#;
   pragma Export (C, u00006, "system__assertionsB");
   u00007 : constant Version_32 := 16#d15f7bb8#;
   pragma Export (C, u00007, "system__assertionsS");
   u00008 : constant Version_32 := 16#2969216a#;
   pragma Export (C, u00008, "ada__exceptionsB");
   u00009 : constant Version_32 := 16#48f79630#;
   pragma Export (C, u00009, "ada__exceptionsS");
   u00010 : constant Version_32 := 16#f379f76d#;
   pragma Export (C, u00010, "system__task_primitivesS");
   u00011 : constant Version_32 := 16#5d104bfc#;
   pragma Export (C, u00011, "system__os_interfaceS");
   u00012 : constant Version_32 := 16#faccf5c5#;
   pragma Export (C, u00012, "system__bbS");
   u00013 : constant Version_32 := 16#cca10615#;
   pragma Export (C, u00013, "system__bb__cpu_primitivesB");
   u00014 : constant Version_32 := 16#e6c043e0#;
   pragma Export (C, u00014, "system__bb__cpu_primitivesS");
   u00015 : constant Version_32 := 16#f77d8799#;
   pragma Export (C, u00015, "interfacesS");
   u00016 : constant Version_32 := 16#02a6b2bd#;
   pragma Export (C, u00016, "system__bb__interruptsB");
   u00017 : constant Version_32 := 16#827aea06#;
   pragma Export (C, u00017, "system__bb__interruptsS");
   u00018 : constant Version_32 := 16#1a6835fd#;
   pragma Export (C, u00018, "interfaces__cS");
   u00019 : constant Version_32 := 16#391a479a#;
   pragma Export (C, u00019, "system__bb__peripheralsB");
   u00020 : constant Version_32 := 16#13f5369b#;
   pragma Export (C, u00020, "system__bb__peripheralsS");
   u00021 : constant Version_32 := 16#6855b65a#;
   pragma Export (C, u00021, "system__bb__peripherals__registersS");
   u00022 : constant Version_32 := 16#6c2e201a#;
   pragma Export (C, u00022, "system__bb__timeB");
   u00023 : constant Version_32 := 16#b6bad725#;
   pragma Export (C, u00023, "system__bb__timeS");
   u00024 : constant Version_32 := 16#7276ed7d#;
   pragma Export (C, u00024, "system__bb__protectionB");
   u00025 : constant Version_32 := 16#7cbd1653#;
   pragma Export (C, u00025, "system__bb__protectionS");
   u00026 : constant Version_32 := 16#4c0197ee#;
   pragma Export (C, u00026, "system__bb__parametersS");
   u00027 : constant Version_32 := 16#fa422e1c#;
   pragma Export (C, u00027, "system__parametersB");
   u00028 : constant Version_32 := 16#81ecd91a#;
   pragma Export (C, u00028, "system__parametersS");
   u00029 : constant Version_32 := 16#ace32e1e#;
   pragma Export (C, u00029, "system__storage_elementsB");
   u00030 : constant Version_32 := 16#8836e08b#;
   pragma Export (C, u00030, "system__storage_elementsS");
   u00031 : constant Version_32 := 16#c64ad6a7#;
   pragma Export (C, u00031, "system__bb__threadsB");
   u00032 : constant Version_32 := 16#3b792fe4#;
   pragma Export (C, u00032, "system__bb__threadsS");
   u00033 : constant Version_32 := 16#f22a3c08#;
   pragma Export (C, u00033, "system__bb__cpu_primitives__multiprocessorsB");
   u00034 : constant Version_32 := 16#6425a019#;
   pragma Export (C, u00034, "system__bb__cpu_primitives__multiprocessorsS");
   u00035 : constant Version_32 := 16#5a83c3a4#;
   pragma Export (C, u00035, "system__multiprocessorsB");
   u00036 : constant Version_32 := 16#4c29558f#;
   pragma Export (C, u00036, "system__multiprocessorsS");
   u00037 : constant Version_32 := 16#489a05b4#;
   pragma Export (C, u00037, "system__bb__threads__queuesB");
   u00038 : constant Version_32 := 16#12e64875#;
   pragma Export (C, u00038, "system__bb__threads__queuesS");
   u00039 : constant Version_32 := 16#48e44b8c#;
   pragma Export (C, u00039, "system__machine_codeS");
   u00040 : constant Version_32 := 16#6d4c1c8f#;
   pragma Export (C, u00040, "system__task_primitives__operationsB");
   u00041 : constant Version_32 := 16#cfc7d8c8#;
   pragma Export (C, u00041, "system__task_primitives__operationsS");
   u00042 : constant Version_32 := 16#7c7898e1#;
   pragma Export (C, u00042, "system__taskingB");
   u00043 : constant Version_32 := 16#a0849d6f#;
   pragma Export (C, u00043, "system__taskingS");
   u00044 : constant Version_32 := 16#1f8fe6cf#;
   pragma Export (C, u00044, "system__secondary_stackB");
   u00045 : constant Version_32 := 16#6d01adf3#;
   pragma Export (C, u00045, "system__secondary_stackS");
   u00046 : constant Version_32 := 16#81c6b8c3#;
   pragma Export (C, u00046, "system__task_infoB");
   u00047 : constant Version_32 := 16#93db7f29#;
   pragma Export (C, u00047, "system__task_infoS");
   u00048 : constant Version_32 := 16#0f8eba36#;
   pragma Export (C, u00048, "system__tasking__debugB");
   u00049 : constant Version_32 := 16#a54f0737#;
   pragma Export (C, u00049, "system__tasking__debugS");
   u00050 : constant Version_32 := 16#a479f75e#;
   pragma Export (C, u00050, "mycubS");
   u00051 : constant Version_32 := 16#32bb332e#;
   pragma Export (C, u00051, "mycub__boardinterfaceB");
   u00052 : constant Version_32 := 16#f599a9b5#;
   pragma Export (C, u00052, "mycub__boardinterfaceS");
   u00053 : constant Version_32 := 16#a34e0368#;
   pragma Export (C, u00053, "ada__interruptsB");
   u00054 : constant Version_32 := 16#4d3b525e#;
   pragma Export (C, u00054, "ada__interruptsS");
   u00055 : constant Version_32 := 16#4eb204d9#;
   pragma Export (C, u00055, "system__interruptsB");
   u00056 : constant Version_32 := 16#f77a8a85#;
   pragma Export (C, u00056, "system__interruptsS");
   u00057 : constant Version_32 := 16#26fcb789#;
   pragma Export (C, u00057, "system__tasking__protected_objectsB");
   u00058 : constant Version_32 := 16#01119edd#;
   pragma Export (C, u00058, "system__tasking__protected_objectsS");
   u00059 : constant Version_32 := 16#c756dd86#;
   pragma Export (C, u00059, "system__multiprocessors__fair_locksB");
   u00060 : constant Version_32 := 16#a70e2885#;
   pragma Export (C, u00060, "system__multiprocessors__fair_locksS");
   u00061 : constant Version_32 := 16#d677746a#;
   pragma Export (C, u00061, "system__multiprocessors__spin_locksB");
   u00062 : constant Version_32 := 16#9ac42bf1#;
   pragma Export (C, u00062, "system__multiprocessors__spin_locksS");
   u00063 : constant Version_32 := 16#b6ebc047#;
   pragma Export (C, u00063, "system__tasking__protected_objects__single_entryB");
   u00064 : constant Version_32 := 16#7097dccb#;
   pragma Export (C, u00064, "system__tasking__protected_objects__single_entryS");
   u00065 : constant Version_32 := 16#3cbe35c1#;
   pragma Export (C, u00065, "system__tasking__protected_objects__multiprocessorsB");
   u00066 : constant Version_32 := 16#c1a39b31#;
   pragma Export (C, u00066, "system__tasking__protected_objects__multiprocessorsS");
   u00067 : constant Version_32 := 16#4a32d090#;
   pragma Export (C, u00067, "ada__real_time__delaysB");
   u00068 : constant Version_32 := 16#1a269a20#;
   pragma Export (C, u00068, "ada__real_time__delaysS");
   u00069 : constant Version_32 := 16#27795e3f#;
   pragma Export (C, u00069, "armB");
   u00070 : constant Version_32 := 16#e3a61ee6#;
   pragma Export (C, u00070, "armS");
   u00071 : constant Version_32 := 16#181ac165#;
   pragma Export (C, u00071, "system__unsigned_typesS");
   u00072 : constant Version_32 := 16#5c9550ee#;
   pragma Export (C, u00072, "arm__stringsS");
   u00073 : constant Version_32 := 16#5d8a4840#;
   pragma Export (C, u00073, "arm__uartB");
   u00074 : constant Version_32 := 16#1f9a4f37#;
   pragma Export (C, u00074, "arm__uartS");
   u00075 : constant Version_32 := 16#3f5dedc7#;
   pragma Export (C, u00075, "lm4fS");
   u00076 : constant Version_32 := 16#5c57a13c#;
   pragma Export (C, u00076, "lm4f__uartS");
   u00077 : constant Version_32 := 16#eb885f8e#;
   pragma Export (C, u00077, "lm4f__gpioS");
   u00078 : constant Version_32 := 16#252d1767#;
   pragma Export (C, u00078, "mycub__controllerB");
   u00079 : constant Version_32 := 16#677ceac9#;
   pragma Export (C, u00079, "mycub__controllerS");
   u00080 : constant Version_32 := 16#84062552#;
   pragma Export (C, u00080, "system__tasking__restricted__stagesB");
   u00081 : constant Version_32 := 16#a30a7965#;
   pragma Export (C, u00081, "system__tasking__restricted__stagesS");
   u00082 : constant Version_32 := 16#328f4145#;
   pragma Export (C, u00082, "system__tasking__restrictedS");
   u00083 : constant Version_32 := 16#c6cdcd0c#;
   pragma Export (C, u00083, "mycub__workingmemoryB");
   u00084 : constant Version_32 := 16#a4c1be6e#;
   pragma Export (C, u00084, "mycub__workingmemoryS");
   u00085 : constant Version_32 := 16#e61c45f2#;
   pragma Export (C, u00085, "arm__i2cB");
   u00086 : constant Version_32 := 16#86e5294d#;
   pragma Export (C, u00086, "arm__i2cS");
   u00087 : constant Version_32 := 16#61928457#;
   pragma Export (C, u00087, "lm4f__i2cS");
   u00088 : constant Version_32 := 16#0226c42a#;
   pragma Export (C, u00088, "lm4f__adcS");
   u00089 : constant Version_32 := 16#d5149817#;
   pragma Export (C, u00089, "lm4f__htimerB");
   u00090 : constant Version_32 := 16#48bbd76e#;
   pragma Export (C, u00090, "lm4f__htimerS");
   u00091 : constant Version_32 := 16#4b60802f#;
   pragma Export (C, u00091, "simplemathB");
   u00092 : constant Version_32 := 16#acb0b5e8#;
   pragma Export (C, u00092, "simplemathS");

   --  BEGIN ELABORATION ORDER
   --  ada%s
   --  interfaces%s
   --  interfaces.c%s
   --  system%s
   --  ada.exceptions%s
   --  ada.exceptions%b
   --  system.bb%s
   --  system.bb.protection%s
   --  system.machine_code%s
   --  system.parameters%s
   --  system.parameters%b
   --  system.storage_elements%s
   --  system.storage_elements%b
   --  system.bb.parameters%s
   --  system.bb.cpu_primitives%s
   --  system.bb.peripherals%s
   --  system.bb.peripherals.registers%s
   --  system.bb.interrupts%s
   --  system.multiprocessors%s
   --  system.multiprocessors%b
   --  system.bb.cpu_primitives.multiprocessors%s
   --  system.bb.cpu_primitives.multiprocessors%b
   --  system.bb.time%s
   --  system.bb.peripherals%b
   --  system.bb.threads%s
   --  system.bb.threads.queues%s
   --  system.bb.cpu_primitives%b
   --  system.multiprocessors.spin_locks%s
   --  system.multiprocessors.spin_locks%b
   --  system.multiprocessors.fair_locks%s
   --  system.os_interface%s
   --  system.multiprocessors.fair_locks%b
   --  system.task_info%s
   --  system.task_info%b
   --  system.task_primitives%s
   --  system.tasking%s
   --  system.task_primitives.operations%s
   --  system.tasking.debug%s
   --  system.unsigned_types%s
   --  system.assertions%s
   --  system.tasking.debug%b
   --  system.task_primitives.operations%b
   --  system.bb.threads.queues%b
   --  system.bb.threads%b
   --  system.bb.interrupts%b
   --  system.bb.time%b
   --  system.bb.protection%b
   --  system.assertions%b
   --  ada.real_time%s
   --  ada.real_time%b
   --  ada.real_time.delays%s
   --  ada.real_time.delays%b
   --  system.secondary_stack%s
   --  system.tasking%b
   --  system.secondary_stack%b
   --  system.tasking.protected_objects%s
   --  system.tasking.protected_objects%b
   --  system.tasking.protected_objects.multiprocessors%s
   --  system.tasking.protected_objects.multiprocessors%b
   --  system.tasking.protected_objects.single_entry%s
   --  system.tasking.protected_objects.single_entry%b
   --  system.interrupts%s
   --  system.interrupts%b
   --  ada.interrupts%s
   --  ada.interrupts%b
   --  system.tasking.restricted%s
   --  system.tasking.restricted.stages%s
   --  system.tasking.restricted.stages%b
   --  arm%s
   --  arm%b
   --  arm.strings%s
   --  arm.i2c%s
   --  arm.uart%s
   --  lm4f%s
   --  lm4f.adc%s
   --  lm4f.gpio%s
   --  lm4f.htimer%s
   --  lm4f.htimer%b
   --  lm4f.i2c%s
   --  arm.i2c%b
   --  lm4f.uart%s
   --  arm.uart%b
   --  mycub%s
   --  mycub.boardinterface%s
   --  main%b
   --  mycub.controller%s
   --  mycub.controller%b
   --  mycub.workingmemory%s
   --  simplemath%s
   --  simplemath%b
   --  mycub.workingmemory%b
   --  mycub.boardinterface%b
   --  END ELABORATION ORDER

end ada_main;
