#!/usr/bin/lua

require("serialport")

function rpc(cmd)    
    serial:Write(cmd .. "\n")
    serial:ReadLine()
    return serial:ReadLine()
end


serial = serialport.SerialPort("/dev/rfcomm0")
serial:Open()

-- check if myCub is running
print('running myCub...')
msg = rpc("\n") 
if string.find(msg, "[myCub]", 1) == nil then
    msg = rpc("myCub")
end

print(rpc("setPose 0 8"))
print(rpc("setPose 1 8"))
print(rpc("setPose 2 8"))
print(rpc("setPose 3 8"))

print(rpc("getdistance 0"))
print(rpc("gotoPose 0 120 3"))

print(rpc("setPose 0 8"))
print(rpc("setPose 1 8"))
print(rpc("setPose 2 8"))
print(rpc("setPose 3 8"))


serial:Close()



