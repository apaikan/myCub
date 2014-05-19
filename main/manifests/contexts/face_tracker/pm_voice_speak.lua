--
-- Copyright (C) 2012 IITRBCS
-- Authors: Ali Paikan
-- CopyPolicy: Released under the terms of the LGPLv2.1 or later, see LGPL.TXT
--
 
-- loading lua-yarp binding library
require("yarp")

--
-- create is called when the port monitor is created 
-- @return Boolean
--
PortMonitor.create = function()
    -- set the constraint here 
    PortMonitor.setConstraint("not e_face_detected")
    print('in pm_voice_speak.lua')
    prev_spk = 0
    return true
end

PortMonitor.accept = function(thing)    
    local bt = thing:asBottle()
    local msg = bt:toString()
    if string.find(msg, "HELLO") == nil then
        return false
    end
    PortMonitor.setEvent('e_voice_hello', 5.0)
    return true
    --[[
    if (yarp.Time_now() - prev_spk) < 5.0 then
        return false
    end
    prev_spk = yarp.Time_now()    
    ]]--
end

PortMonitor.update = function(thing)
    bt = thing:asBottle()
    bt:clear()
    bt:addString("Yes?")
    return thing

   --th = yarp.Things()
    --msg = yarp.Bottle()
    --msg:addString("Hello!")
    --th:setPortWriter(msg)

end

