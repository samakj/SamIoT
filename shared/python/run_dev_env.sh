#!/bin/bash

shared="$(dirname "$(pwd)")"
root="$(dirname $shared)"

python3 -m venv .
source bin/activate
ln -s "$root/shared" ./

pip3 install -r requirements.txt
