package ARM.Strings.C is

   function "&" (L : String; R : ARM_String) return String;
   function "&" (L : ARM_String; R : String) return String;
   function "=" (L : String; R : ARM_String) return Boolean;
   function "=" (L : ARM_String; R : String) return Boolean;

end ARM.Strings.C;
