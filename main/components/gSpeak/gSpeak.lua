#!/usr/bin/lua 

-- Copyright: (C) 2011 Robotics, Brain and Cognitive Sciences - Italian Institute of Technology (IIT)
-- Author: Ali Paikan <ali.paikan@iit.it>
-- Copy Policy: Released under the terms of the LGPLv2.1 or later, see LGPL.TXT

--
-- Global Settings 
------------------------------------------------

external_player  = "mpg123"
db_name          = "cache.db"
cache_folder     = "./cache"
default_language = "en"

------------------------------------------------


require("yarp")
luasql = require "luasql.sqlite3"

-- initialize yarp network
yarp.Network()



function open_cache(dbname)
    env = assert(luasql.sqlite3())
    con = env:connect(dbname)
    res = con:execute "CREATE TABLE Speech( message varchar(255) PRIMARY KEY, langauge varchar(32), filename varchar(64) )"    
    return true
end

function close_cache(dbname)
    cur:close()
    con:close()
    env:close()
end

function vocfile_exists(fname)
    local handle = io.popen('ls '..cache_folder.."/"..fname)
    local result = handle:read("*a")
    handle:close() 
    if string.find(result, "cannot access") ~= nil then
        return false
    end 
    return true
end

function cache_speech(msg, lang)
    -- create new filename
    if lang == nil then lang = default_language end
    cur:close()
    cur = con:execute "SELECT COUNT(filename) as last_id FROM Speech"
    row = cur:fetch ({}, "a")
    fname = row.last_id+1 .. ".voc"
    os.execute("wget -q -U Mozilla -O \"" .. cache_folder .. "/".. fname .. "\" \"http://translate.google.com/translate_tts?ie=UTF-8\&tl="..lang.."\&q=\\'" .. msg .."\\'\"")
    if vocfile_exists(fname) == false then
        return nil
    end 
    con:execute(string.format( "INSERT INTO Speech VALUES ('%s', '%s', '%s')" , msg, lang, fname))
    return fname
end

function uncache(msg, lang)
    if lang == nil then lang = default_language end
    cur = con:execute("SELECT * FROM Speech WHERE message='" .. msg .. "' and langauge='"..lang.."'")
    row = cur:fetch ({}, "a")
    if row ~= nil then
        if vocfile_exists(row.filename) ~= false then
            return row.filename
        end  
    end
    return nil
end

-- call static member in this way
-- t = yarp.Time_now()

-- create and open msg_port ports
msg_port = yarp.BufferedPortBottle()
msg_port:open("/gSpeak")

-- initializing caching 
open_cache(db_name)
os.execute("mkdir -p " .. cache_folder)

while true do
    bt = msg_port:read()
    if bt ~= nil then 
        msg = bt:toString()
        msg:match "^%s*(.-)%s*$"    -- triming
        print("Received:", msg) 

        -- check if it is not cached 
        filename = uncache(msg)
        if filename == nil then
            print("Caching new speech...")
            filename = cache_speech(msg)
        end

        if filename ~= nil then
            print('Playing...')
            os.execute(external_player .." ".. cache_folder .."/".. filename) 
        end
    end
    -- wait for a second
    --yarp.Time_delay(1.0)
end

-- close the ports and cache
msg_port:close()
close_cache()

-- Deinitialize yarp network
yarp.Network_fini()

