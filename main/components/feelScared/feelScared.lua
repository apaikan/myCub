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
sender:open("...")

speech = yarp.Port()
speech:open("/feelScared/speech:o")


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



steps_defence = {
        "30 30 0 30",
        }

function makeDefendPose(steps, speed)
    local cmd = yarp.Bottle()
    local rep = yarp.Bottle()
    for i=1,#steps do
        cmd:clear()
        --rep:clear()
        cmd:addString("gotoPoseAll")
        pos = cmd:addList()
        pos:clear()
        pos:fromString(steps[i])
        spd = cmd:addList()
        spd:clear()
        for s=1,4 do spd:addInt(speed) end 
        sender:write(cmd, rep)
        --print(i, cmd:toString(), rep:toString())
        --yarp.Time_delay(1.2*speed/1000)
    end
end

messages = {"You scared the hell out of me!", 
            "You scared the devil out of me!", 
            "What the hell!", 
            "Hey! be careful!", 
            "It made my blood run clod!",
            "You scared me to death!"}
function sayTheWords()
    bt = yarp.Bottle()
    bt:clear()
    msg = messages[math.random(1,#messages)]
    bt:addString(msg)
    speech:write(bt)
end

prev_dist_time = 0
prev_dist_value = 0
while true do
    front_obstacle = getDistance()
    if front_obstacle < 400 then
        now = yarp.Time_now()
        --print("prev_dist:", prev_dist_value, 
        --      "current dist:", front_obstacle,
        --     "time span:", math.abs(now - prev_dist_time))

        drv = (prev_dist_value - front_obstacle) / (math.abs(now - prev_dist_time)*10);
        if drv > 40 and drv <100 then
            print("scared!")
            sayTheWords()
            makeDefendPose(steps_defence, 500)
        end  
        prev_dist_time = now;
        prev_dist_value = front_obstacle                 
    end
    yarp.Time_delay(0.1)
end

-- disconnect sender from receiver
yarp.NetworkBase_disconnect(sender:getName(), "/MyCubInterface/cmd:i")

-- close the ports
sender:close()

-- Deinitialize yarp network
yarp.Network_fini()

