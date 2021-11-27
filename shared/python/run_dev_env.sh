#!/bin/bash

python3 -m venv .
source bin/activate
ln -s /Users/$(whoami)/repos/SamIoT/shared ./

pip3 install -r requirements.txt
