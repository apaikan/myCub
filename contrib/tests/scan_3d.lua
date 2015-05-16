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

-- create and open interface/receiver ports
interface = yarp.RpcClient()
interface:open("...")
gaze = yarp.RpcClient()
gaze:open("...")

-- connect interface to receiver
ret = yarp.NetworkBase_connect(interface:getName(), "/MyCubInterface/cmd:i")
if ret == false then
    print("Cannot connect to /myCubInterface/cmd:i")
    return
end

ret = yarp.NetworkBase_connect(gaze:getName(), "/GazeControl/cmd:i")
if ret == false then
    print("Cannot connect to /GazeControl/cmd:i")
    return
end


t_start = yarp.Time_now()
function setGaze(j1, j2) 
    local cmd = yarp.Bottle()
    local rep = yarp.Bottle()
    cmd:clear()
    cmd:addString("joint")
    local pos = cmd:addList()
    pos:addInt(j1)
    pos:addInt(j2)
    gaze:write(cmd, rep)
    return rep:toString()
end 

function getDistance() 
    local cmd = yarp.Bottle()
    local rep = yarp.Bottle()
    cmd:clear()
    cmd:addString("getDistance")
    cmd:addInt(0)
    interface:write(cmd, rep)
    if rep:size() then
        return rep:get(0):asInt()
    end
    return -1
end

--j1: 30 , 180
--j2 70, 130 
index = 1
a = {}
for j1=30,180,3 do
    for j2=70,130,3 do
        setGaze(j1,j2)
        if j2 == 70 then yarp.Time_delay(0.5) end
        dist = getDistance()
        a[index] = {j1, j2, dist}
        index = index + 1
        --print(j1, j2, dist)        
        yarp.Time_delay(0.2)
    end
end

local file = io.open("scan.log", "w")
for i=1, #a do
    file:write(a[i][1], "\t", a[i][2], "\t", a[i][3], "\n")
end


--
-- disconnect interface from receiver
yarp.NetworkBase_disconnect(interface:getName(), "/MyCubInterface/cmd:i")
yarp.NetworkBase_disconnect(gaze:getName(), "/GazeControl/cmd:i")

-- close the ports
interface:close()
gaze:close()

-- Deinitialize yarp network
yarp.Network_fini()

