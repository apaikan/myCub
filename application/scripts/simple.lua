#!/usr/bin/lua 

-- Copyright: 2011 (C) Robotics, Brain and Cognitive Sciences - Italian Institute of Technology (IIT)
-- Author: Ali Paikan <ali.paikan@iit.it>
-- Copy Policy: Released under the terms of the LGPLv2.1 or later, see LGPL.TXT


-- LUA_CPATH should have the path to yarp-lua binding library (i.e. yarp.so, yarp.dll) 
--require("yarp")

fd=require("rs232")

--B9600=13        --  9600 bauds
B115200=17
V24_8BIT=3        -- 8 bits
NO_PARITY=0        -- no parity

-- Open the serial port
-- /dev/ttyS0,V24_STANDARD
-------------------------------------
print("open port /dev/rfcomm0")
result=fd.V24OpenPort("/dev/rfcomm0",0,0)
print(result)

-- Define serial parameters
---------------------------------------------
print("115200 bauds, 8 bits , no parity")
result=fd.V24SetParameters(B115200, V24_8BIT, NO_PARITY)
print(result)

-- Timeout 5 seconds
------------------------
print("Receive Timeout is 5 seconds")
result=fd.V24SetTimeouts(50)
print(result)

-- Wait a character on the serial port
----------------------------------------------
--print("Waiting.......")
--result=fd.V24Getc()
--print(result)

-- Send 'A'
-------------------------
--print("sending 'A'")
--result=fd.V24Putc('\n')
--print(result)

-- Send the string "hello"
-----------------------------
print("sending string 'myCub'")
result=fd.V24Write("myCub\n", 7)
print(result)

os.execute("sleep 2")

result=fd.V24Write("moveFront\n", 10)
print(result)

os.execute("sleep 10")

result=fd.V24Write("stop\n", 5)
print(result)


result=fd.V24Write("exit\n", 5)
print(result)


-- Close the serial port
--------------------
print("Close serial port")
result=fd.V24ClosePort()
print(result)

