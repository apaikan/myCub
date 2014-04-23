#!/usr/bin/lua 

-- Copyright: (C) 2011 Robotics, Brain and Cognitive Sciences - Italian Institute of Technology (IIT)
-- Author: Ali Paikan <ali.paikan@iit.it>
-- Copy Policy: Released under the terms of the LGPLv2.1 or later, see LGPL.TXT


-- LUA_CPATH should have the path to yarp-lua binding library (i.e. yarp.so, yarp.dll) 
require("yarp")

-- initialize yarp network
yarp.Network()


-- call static member in this way
-- t = yarp.Time_now()

-- create and open sender/receiver ports
sender = yarp.Port()
sender:open("/lookAround/target")

-- connect sender to receiver
--yarp.NetworkBase_connect(sender:getName(), "/myCubInterface/cmd:i")

search_range   = {xmin=10, xmax=150, ymin=50, ymax=130}
search_delay   = {min=3, max=5}
search_speed   = {min=1, max=2} 

while true do
    -- write to the sender port
    local cmd = yarp.Bottle()
    cmd:clear()
    cmd:addString("joint")
    pos = cmd:addList()
    pos:addDouble(math.random(search_range.xmin*10, search_range.xmax*10)/10)
    pos:addDouble(math.random(search_range.ymin*10, search_range.ymax*10)/10)
    pos:addDouble(math.random(search_speed.min, search_speed.max))
    sender:write(cmd)
    yarp.Time_delay(math.random(search_delay.min, search_delay.max))
end

-- disconnect sender from receiver
--yarp.NetworkBase_disconnect(sender:getName(), "/myCubInterface/cmd:i")

-- close the ports
sender:close()

-- Deinitialize yarp network
yarp.Network_fini()

