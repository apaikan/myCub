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
    greetings = {"Hello", "Hi there", "Hey you", "TIME", "How are you?", "How are you doing?", "How is going?"}
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
    msg = greetings[math.random(1,#greetings)]
    if msg == "TIME" then
        h = os.date("%H") -- hour
        if tonumber(h) < 12 then msg = "Good morning"
        elseif h < 18 then msg = "Good afternoon" 
        else msg = "Good evening"
        end
    end
    bt:addString(msg)
    --th = yarp.Things()
    --msg = yarp.Bottle()
    --msg:addString("Hello!")
    --th:setPortWriter(msg)
    return thing
end

