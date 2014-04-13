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
sender:open("/rotate/cmd:o")

-- connect sender to receiver
yarp.NetworkBase_connect(sender:getName(), "/myCubInterface/cmd:i")

while do
    -- write to the sender port
    local Bottle cmd;
    sender:write(cmd)

    -- wait for a second
    yarp.Time_delay(1.0)
end

-- disconnect sender from receiver
yarp.NetworkBase_disconnect(sender:getName(), "/myCubInterface/cmd:i")

-- close the ports
sender:close()

-- Deinitialize yarp network
yarp.Network_fini()

