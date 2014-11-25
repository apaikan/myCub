#!/bin/bash

if [[ "$(uname -m)" == "x86_64" ]]; then ARCH="x64"; else ARCH="x86"; fi
(cd "./"; bin/linux/$ARCH/lua src/main.lua myCubGui "$@") &
