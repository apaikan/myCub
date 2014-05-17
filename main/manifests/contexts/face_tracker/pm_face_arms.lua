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
    print('in pm_face_arms.lua')
    prev_spk = 0
    return true
end

PortMonitor.accept = function(thing)    
    PortMonitor.setEvent('e_face_detected', 5.0)
    if (yarp.Time_now() - prev_spk) < 5.0 then
        return false
    end
    prev_spk = yarp.Time_now()    
    return true
end

PortMonitor.update = function(thing)

    val = math.random(3, 5) * 10
    bt = thing:asBottle()
    bt:clear()
    bt:addString("goto")
    bt:addString("posall")
    poses = bt:addList()
    poses:addInt(val)
    poses:addInt(0)
    poses:addInt(val)
    poses:addInt(0)
    speeds = bt:addList()
    speeds:addInt(1)
    speeds:addInt(1)
    speeds:addInt(1)
    speeds:addInt(1)
    --th = yarp.Things()
    --msg = yarp.Bottle()
    --msg:addString("Hello!")
    --th:setPortWriter(msg)
    return thing
end

