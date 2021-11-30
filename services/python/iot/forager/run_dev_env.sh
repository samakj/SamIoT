#!/bin/bash

iot="$(dirname "$(pwd)")"
python="$(dirname $iot)"
services="$(dirname $python)"
root="$(dirname $services)"

python3 -m venv .
source bin/activate
ln -s "$root/shared" ./

pip3 install -r requirements.txt

# gunicorn app:create_app \
#     --bind 0.0.0.0:8080 \
#     --worker-class aiohttp.GunicornWebWorker \
#     --log-level 'info' \
#     --reload

deactivate