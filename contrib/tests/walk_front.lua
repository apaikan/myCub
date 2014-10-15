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

function getDistance() 
    local cmd = yarp.Bottle()
    local rep = yarp.Bottle()
    cmd:clear()
    cmd:addString("getDistance")
    cmd:addInt(0)
    sender:write(cmd, rep)
    if rep:size() then
        return rep:get(0):asInt()
    end
    return -1
end



speed = 500

steps_front = {
        "0 30 0 10",
        "30 30 30 10",
        "30 0 30 10",
        "0 0 0 10",
        "0 0 0 0",
        }

steps_turn = {
        "0 10 0 30",
        "30 10 30 30",
        "30 10 30 0",
        "0 10 0 0",
        "0 0 0 0",
        }

function move(steps)
    for i=1,#steps do
        cmd:clear()
        rep:clear()
        cmd:addString("gotoPoseAll")
        pos = cmd:addList()
        pos:clear()
        pos:fromString(steps[i])
        spd = cmd:addList()
        spd:clear()
        for s=1,4 do spd:addInt(speed) end 
        sender:write(cmd, rep)
        print(i, cmd:toString(), rep:toString())
        yarp.Time_delay(1.2*speed/1000)
    end
end

cmd = yarp.Bottle()
rep = yarp.Bottle()

while true do
    front_obstacle = getDistance()
    print("Front Obstacle:", front_obstacle, "\n")
    if front_obstacle > 150 then
        print("Moving forward...\n")
        move(steps_front)
    else
        print("Obstacle on the way! try to turn\n")
        while getDistance() < 1500 do
            print("Turning... distance:", getDistance())
            move(steps_turn)
        end
        --yarp.Time_delay(1.0)
    end    
end


-- disconnect sender from receiver
yarp.NetworkBase_disconnect(sender:getName(), "/MyCubInterface/cmd:i")

-- close the ports
sender:close()

-- Deinitialize yarp network
yarp.Network_fini()

