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
    PortMonitor.setConstraint("true")
    print('in pm_speak.lua')
    prev_spk = 0
    return true
end

PortMonitor.accept = function(thing)    
    if (yarp.Time_now() - prev_spk) < 10.0 then
        return false
    end
    prev_spk = yarp.Time_now()    
    return true
end

PortMonitor.update = function(thing)
    bt = thing:asBottle()
    bt:clear()
    bt:addString("Hello")
    --th = yarp.Things()
    --msg = yarp.Bottle()
    --msg:addString("Hello!")
    --th:setPortWriter(msg)
    return thing
end

