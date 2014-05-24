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
sender = yarp.RpcClient()
sender:open("...")

-- connect sender to receiver
ret = yarp.NetworkBase_connect(sender:getName(), "/MyCubInterface/cmd:i")
if ret == false then
    print("Cannot connect to /myCubInterface/cmd:i")
    return
end

speed = 1000
--[[
steps = {
        "0 30 0 0",
        "30 30 30 0",
        "30 0 30 0",
        "0 0 0 0",
        }
]]--

steps = {
        "0 30 0 0",
        "30 30 30 0",
        "30 0 30 0",
        "0 0 0 0",
        }


cmd = yarp.Bottle()
rep = yarp.Bottle()

while true do
    for i=1,#steps do
        cmd:clear()
        rep:clear()
        cmd:addString("goto")
        cmd:addString("posall")
        pos = cmd:addList()
        pos:clear()
        pos:fromString(steps[i])
        spd = cmd:addList()
        spd:clear()
        for s=1,4 do spd:addInt(speed) end 
        sender:write(cmd, rep)
        print(i, cmd:toString(), rep:toString())
        yarp.Time_delay(2*speed/1000)
    end    
end


-- disconnect sender from receiver
yarp.NetworkBase_disconnect(sender:getName(), "/MyCubInterface/cmd:i")

-- close the ports
sender:close()

-- Deinitialize yarp network
yarp.Network_fini()

