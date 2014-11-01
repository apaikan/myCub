
package body SimpleMath is

    --
    -- Atan
    --
    Function Atan(x: Float) return Float is

        Function AtanR (g: Float) return Float is
            p0 : constant := -0.136887e+2;
            p1 : constant := -0.205058e+2;
            p2 : constant := -0.849462e+1;
            p3 : constant := -0.837582e+0;
            q0 : constant :=  0.410663e+2;
            q1 : constant :=  0.861573e+2;
            q2 : constant :=  0.595784e+2;
            q3 : constant :=  0.150240e+2;
        begin
          return ((((p3 * g + p2) * g + p1) * g + p0) * g) /
            ((((g + q3) * g + q2) * g + q1) * g + q0);
        end AtanR;

        sqrt_3           : constant := 1.732050;
        sqrt_3_minus_1   : constant := 0.732050;
        two_minus_sqrt_3 : constant := 0.267949;
        pi_over_two      : constant := 1.570796;
        pi_over_three    : constant := 1.047197;
        pi_over_six      : constant := 0.523598;
        FLT_EPSILON      : constant := 1.0e-8; 
        f, g, result : Float;
        n : Integer;
    begin
        f := abs x;
        if f > 1.0 then         
            f := 1.0 / f;
            n := 2;        
        else        
            n := 0;
        end if;

        if f > two_minus_sqrt_3 then
            f := (((sqrt_3_minus_1 * f - 0.5) - 0.5) + f) / (sqrt_3 + f);
            n := n + 1;
        end if;
        
        g := f * f;

        if g < FLT_EPSILON then 
            result := f;
        else
            result := f + f * AtanR(g);
        end if;

        if n > 1 then
            result := -result;
        end if;

        case n is
            when 1 => result := pi_over_six + result;
            when 2 => result := pi_over_two + result;
            when 3 => result := pi_over_three + result;
            when others => null; 
        end case;
        if x < 0.0 then
            result := -result;
        end if;
        return result;
    end Atan;

    --
    -- Atan2
    --
    Function Atan2(V : Float; U: Float) return Float is
        Result : Float;
    begin
        if V = 0.0 and U = 0.0 then
            Result := 0.0;
        elsif (abs V) <= (abs U) then
            Result := Atan(V / U);            
            if U <= 0.0 then
                if V <= 0.0 then
                    Result := Result - M_PI;
                else
                    Result := Result + M_PI;
                end if;    
            end if;        
        else
            Result := Atan(-U / V);
            if V <= 0.0 then
                Result := Result - M_PI_OVER_TWO;
            else
                Result := Result + M_PI_OVER_TWO;
            end if;
        end if;
        return Result;
    end Atan2;

end SimpleMath;
