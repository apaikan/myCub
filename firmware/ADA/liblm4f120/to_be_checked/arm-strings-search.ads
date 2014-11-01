

package ARM.Strings.Search is
   pragma Pure;

   function Index
     (Source   : in ARM_String;
      Pattern  : in ARM_String;
      Going    : in Direction := Forward)
      return     Unsigned_8;

   function Index
     (Source   : in ARM_String;
      Pattern  : in Character;
      Going    : in Direction := Forward)
      return     Unsigned_8;

   function Index_Non_Blank
     (Source : in ARM_String;
      Going  : in Direction := Forward)
     return   Unsigned_8;

   function Count
     (Source   : in ARM_String;
      Pattern  : in ARM_String)
      return     Unsigned_8;

   function Count
     (Source   : in ARM_String;
      Pattern  : in Character)
     return     Unsigned_8;

end ARM.Strings.Search;
