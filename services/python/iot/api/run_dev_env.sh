#!/bin/bash

iot="$(dirname "$(pwd)")"
python="$(dirname $iot)"
services="$(dirname $python)"
root="$(dirname $services)"

python3 -m venv .
source bin/activate
ln -s "$root/shared" ./

pip3 install -r requirements.txt
