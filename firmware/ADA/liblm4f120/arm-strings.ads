------------------------------------------------------------------------------
--                                                                          --
--                         ARM-Ada RUNTIME COMPONENTS                       --
--                                                                          --
--                            A V R . S T R I N G S                         --
--                                                                          --
--                                 S p e c                                  --
--                                                                          --
--  This specification is adapted  from the Ada Reference Manual for use    --
--  with ARM-Ada.  In  accordance with  the copyright of that document,     --
--  you  can freely  copy and modify  this specification, provided  that    --
--  if you redistribute a modified version, any changes that you have made  --
--  are clearly indicated.                                                  --
--                                                                          --
------------------------------------------------------------------------------

--  In accordance to the above copyright notice all modifications in
--  this files are marked with one of the following comments:
--
--     <Id> is not part of the ARM.
--  or
--     <Id> is part of the ARM, but removed here.
--
--  where ARM stands for Ada Reference Manual.


package ARM.Strings is
   pragma Pure;

   Space      : constant Character      := ' ';

   --  ARM_String is not part of the ARM.
   type ARM_String is array (Unsigned_8 range <>) of aliased Character;
   --  The equivalent to Standard.String except that it is indexed not
   --  by Positive (2 bytes) but by Unsigned_8 (1 byte).

   --  some string subtypes with predefined length.  They are not part
   --  of the ARM.

   subtype AStr2  is ARM_String (1 ..  2);
   subtype AStr3  is ARM_String (1 ..  3);
   subtype AStr4  is ARM_String (1 ..  4);
   subtype AStr5  is ARM_String (1 ..  5);
   subtype AStr6  is ARM_String (1 ..  6);
   subtype AStr7  is ARM_String (1 ..  7);
   subtype AStr8  is ARM_String (1 ..  8);
   subtype AStr9  is ARM_String (1 ..  9);
   subtype AStr10 is ARM_String (1 .. 10);
   subtype AStr11 is ARM_String (1 .. 11);
   subtype AStr12 is ARM_String (1 .. 12);
   subtype AStr13 is ARM_String (1 .. 13);
   subtype AStr14 is ARM_String (1 .. 14);
   subtype AStr15 is ARM_String (1 .. 15);
   subtype AStr16 is ARM_String (1 .. 16);
   subtype AStr17 is ARM_String (1 .. 17);
   subtype AStr18 is ARM_String (1 .. 18);
   subtype AStr19 is ARM_String (1 .. 19);
   subtype AStr20 is ARM_String (1 .. 20);
   subtype AStr128 is ARM_String (1 .. 128);

   --  subtype AStr_Too_Long is ARM_String (Unsigned_8);
   --  type AStr_Ref is access all AStr_Too_Long;
   --  a general reference to a constrained ARM_String

   --  Progmem_String and PStr are not part of the ARM.
   type Progmem_String is new ARM_String;
   --  pragma Linker_Section (Progmem_String, ".progmem.data");
   --  gcc-4.5.1: "pragma "LINKER_SECTION" applies only to objects"
   subtype PStr is Progmem_String;


   --  Length_Error, Pattern_Error, Index_Error, Translation_Error are
   --  part of the ARM, but removed here.

   type Alignment  is (Left, Right, Center);

   --  type Truncation is (Left, Right, Error);
   --  Truncation.Error is part of the ARM, but removed here.
   type Truncation is (Left, Right);
   type Membership is (Inside, Outside);
   type Direction  is (Forward, Backward);
   type Trim_End   is (Left, Right, Both);

end ARM.Strings;
