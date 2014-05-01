
%include "std_string.i"
%include "std_string.i"

%module serialport
%{
    #include "SerialPort.h"
    #include "PosixSignalDispatcher.h"
%}

%include "SerialPort.h"

