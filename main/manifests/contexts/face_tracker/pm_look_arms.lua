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
    PortMonitor.setConstraint("not e_face_detected and not e_scared")
    print('in pm_look_arms.lua')
    prev_streching = yarp.Time_now()
    return true
end

PortMonitor.accept = function(thing)   
    return true
end

PortMonitor.update = function(thing)
    val = math.random(5, 10) * 60
    if (yarp.Time_now() - prev_streching) > val then
        prev_streching = yarp.Time_now()
        return stretch(thing)
    end
    return relax(thing)
end

function relax(thing)
    bt = thing:asBottle()
    bt:clear()
    bt:addString("gotoPoseAll")
    poses = bt:addList()
    poses:addInt(0)
    poses:addInt(5)
    poses:addInt(0)
    poses:addInt(5)
    speeds = bt:addList()
    speeds:addInt(3000)
    speeds:addInt(3000)
    speeds:addInt(3000)
    speeds:addInt(3000)
    return thing
end

function stretch(thing)
    bt = thing:asBottle()
    bt:clear()
    bt:addString("gotoPoseAll")
    poses = bt:addList()
    poses:addInt(20)
    poses:addInt(20)
    poses:addInt(20)
    poses:addInt(20)
    speeds = bt:addList()
    speeds:addInt(3000)
    speeds:addInt(3000)
    speeds:addInt(3000)
    speeds:addInt(3000)
    return thing
end
