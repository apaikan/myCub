-- Copyright 2011-14 Paul Kulchenko, ZeroBrane LLC
-- authors: Luxinia Dev (Eike Decker & Christoph Kubisch)
---------------------------------------------------------

-- put bin/ and lualibs/ first to avoid conflicts with included modules
-- that may have other versions present somewhere else in path/cpath.
local iswindows = os.getenv('WINDIR') or (os.getenv('OS') or ''):match('[Ww]indows')
local islinux = not iswindows and not os.getenv('DYLD_LIBRARY_PATH') and io.open("/proc")
local arch = "x86" -- use 32bit by default
local unpack = table.unpack or unpack

if islinux then
  local file = io.popen("uname -m")
  if file then
    arch = file:read("*a"):find("x86_64") and "x64" or "x86"
    file:close()
  end
end

package.cpath = (
  iswindows and 'bin/?.dll;bin/clibs/?.dll;' or
  islinux and ('bin/linux/%s/lib?.so;bin/linux/%s/clibs/?.so;'):format(arch,arch) or
  --[[isosx]] 'bin/lib?.dylib;bin/clibs/?.dylib;')
    .. package.cpath
package.path  = 'lualibs/?.lua;lualibs/?/?.lua;lualibs/?/init.lua;lualibs/?/?/?.lua;lualibs/?/?/init.lua;'
              .. package.path

require("wx")
require("bit")
require("yarp")


frame = nil
control = {}
joints_value = {}

-- initialize yarp network
yarp.Network()
cmd = yarp.Bottle()
rep = yarp.Bottle()


ID_SLIDER1           = 1001
ID_SLIDER2           = 1001
ID_SLIDER3           = 1001
ID_SLIDER4           = 1001

function gotoPose(joint, pos, speed)
    cmd:clear()
    rep:clear()
    cmd:addString("gotoPose")
    cmd:addInt(joint)
    cmd:addInt(pos)
    cmd:addInt(speed)
    sender:write(cmd, rep)
    print(cmd:toString(), rep:toString())
    joints_value[joint]:SetLabel(string.format("%d", pos))
end

-- Make a function to reduce the amount of duplicate code.
function AddSliderControl(name_string, sliderID, id)
    local staticText = wx.wxStaticText( panel, wx.wxID_ANY, name_string)
    joints_value[id] = wx.wxStaticText( panel, wx.wxID_ANY, "0")
    control[id] = wx.wxSlider(panel, sliderID, 0, 0, 70, wx.wxDefaultPosition, wx.wxSize(200, -1))
    flexGridSizer:Add( staticText, 0, wx.wxALIGN_CENTER_VERTICAL+wx.wxALL, 10 )
    flexGridSizer:Add( control[id], 0, wx.wxGROW+wx.wxALIGN_CENTER+wx.wxALL, 10 )
    flexGridSizer:Add( joints_value[id], 0, wx.wxALIGN_CENTER_VERTICAL+wx.wxALL, 10 )
end


function CreateGui()
    frame = wx.wxFrame(wx.NULL, wx.wxID_ANY, "myCubGui",
                       wx.wxDefaultPosition, wx.wxSize(450, 450),
                       wx.wxDEFAULT_FRAME_STYLE)

    -- create a simple file menu
    local fileMenu = wx.wxMenu()
    fileMenu:Append(wx.wxID_EXIT, "E&xit", "Quit the myCubGui")
    -- create a simple help menu
    local helpMenu = wx.wxMenu()
    helpMenu:Append(wx.wxID_ABOUT, "&About",
                    "Very simple Gui to control myCub")

    -- create a menu bar and append the file and help menus
    local menuBar = wx.wxMenuBar()
    menuBar:Append(fileMenu, "&File")
    menuBar:Append(helpMenu, "&Help")
    -- attach the menu bar into the frame
    frame:SetMenuBar(menuBar)

    -- create a simple status bar
    frame:CreateStatusBar(1)
    frame:SetStatusText("Welcome to myCubGui.")

    -- connect the selection event of the exit menu item to an
    -- event handler that closes the window
    frame:Connect(wx.wxID_EXIT, wx.wxEVT_COMMAND_MENU_SELECTED,
                  function (event) frame:Close(true) end )
    -- connect the selection event of the about menu item
    frame:Connect(wx.wxID_ABOUT, wx.wxEVT_COMMAND_MENU_SELECTED,
            function (event)
                wx.wxMessageBox('This is a simple Gui to control myCub.',
                                "About myCubGui",
                                wx.wxOK + wx.wxICON_INFORMATION,
                                frame)
            end )

    -- Create a wxPanel to contain all the buttons. It's a good idea to always
    -- create a single child window for top level windows (frames, dialogs) since
    -- by default the top level window will want to expand the child to fill the
    -- whole client area. The wxPanel also gives us keyboard navigation with TAB key.
    panel = wx.wxPanel(frame, wx.wxID_ANY)
    -- Layout all the buttons using wxSizers
    mainSizer      = wx.wxBoxSizer(wx.wxVERTICAL)
    staticBox      = wx.wxStaticBox(panel, wx.wxID_ANY, "Joint Position Control")
    staticBoxSizer = wx.wxStaticBoxSizer(staticBox, wx.wxVERTICAL)
    flexGridSizer  = wx.wxFlexGridSizer( 0, 3, 0, 0 )
    flexGridSizer:AddGrowableCol(1, 0)

    AddSliderControl("0) Front", ID_SLIDER1, 0)
    AddSliderControl("1) Right", ID_SLIDER2, 1)
    AddSliderControl("2) Back", ID_SLIDER3, 2)
    AddSliderControl("3) Left", ID_SLIDER4, 3)

    staticBoxSizer:Add( flexGridSizer,  0, wx.wxGROW+wx.wxALIGN_CENTER+wx.wxALL, 5 )
    mainSizer:Add(      staticBoxSizer, 1, wx.wxGROW+wx.wxALIGN_CENTER+wx.wxALL, 5 )
    panel:SetSizer( mainSizer )
    mainSizer:SetSizeHints( panel )

    control[0]:Connect(wx.wxID_ANY, wx.wxEVT_SCROLL_CHANGED,
            function (event) gotoPose(0, control[0]:GetValue(), 1000); end )
    control[1]:Connect(wx.wxID_ANY, wx.wxEVT_SCROLL_CHANGED,
            function (event) gotoPose(1, control[1]:GetValue(), 1000); end )
    control[2]:Connect(wx.wxID_ANY, wx.wxEVT_SCROLL_CHANGED,
            function (event) gotoPose(2, control[2]:GetValue(), 1000); end )
    control[3]:Connect(wx.wxID_ANY, wx.wxEVT_SCROLL_CHANGED,
            function (event) gotoPose(3, control[3]:GetValue(), 1000); end )

    -- finally, show the frame window
    frame:Show(true)


end

-- create and open sender/receiver ports
sender = yarp.RpcClient()
sender:open("...")

-- connect sender to receiver
ret = yarp.NetworkBase_connect(sender:getName(), "/MyCubInterface/cmd:i")
if ret == false then
    print("Cannot connect to /myCubInterface/cmd:i")
    return
end


CreateGui()

-- Call wx.wxGetApp():MainLoop() last to start the wxWidgets event loop,
-- otherwise the wxLua program will exit immediately.
-- Does nothing if running from wxLua, wxLuaFreeze, or wxLuaEdit since the
-- MainLoop is already running or will be started by the C++ program.
wx.wxGetApp():MainLoop()


-- disconnect sender from receiver
yarp.NetworkBase_disconnect(sender:getName(), "/MyCubInterface/cmd:i")

-- close the ports
sender:close()


-- Deinitialize yarp network
yarp.Network_fini()

-- There are several reasons for this call:
-- (1) to fix a crash on OSX when closing with debugging in progress.
-- (2) to fix a crash on Linux 32/64bit during GC cleanup in wxlua
-- after an external process has been started from the IDE.
-- (3) to fix exit on Windows when started as "bin\lua src\main.lua".
os.exit()

