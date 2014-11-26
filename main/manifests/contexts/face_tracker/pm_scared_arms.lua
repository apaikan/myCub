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
    print('in pm_scared_arms.lua')
    return true
end

PortMonitor.accept = function(thing)    
    PortMonitor.setEvent('e_scared', 5.0)
end


