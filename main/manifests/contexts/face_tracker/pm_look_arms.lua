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
    print('in pm_look_arms.lua')
    return true
end

PortMonitor.accept = function(thing)   
    return true
end

PortMonitor.update = function(thing)

    val = math.random(3, 5) * 10
    bt = thing:asBottle()
    bt:clear()
    bt:addString("goto")
    bt:addString("posall")
    poses = bt:addList()
    poses:addInt(5)
    poses:addInt(0)
    poses:addInt(5)
    poses:addInt(0)
    speeds = bt:addList()
    speeds:addInt(3000)
    speeds:addInt(3000)
    speeds:addInt(3000)
    speeds:addInt(3000)
    --th = yarp.Things()
    --msg = yarp.Bottle()
    --msg:addString("Hello!")
    --th:setPortWriter(msg)
    return thing
end

