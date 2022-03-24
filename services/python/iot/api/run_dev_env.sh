#!/bin/bash

. ./env.sh

iot="$(dirname "$(pwd)")"
python="$(dirname $iot)"
services="$(dirname $python)"
root="$(dirname $services)"

python3 -m venv .
. bin/activate
cp -r "$root/shared" ./

pip3 install -r requirements.txt

gunicorn app:create_app \
    --bind "$IOT_API_HOST:$IOT_API_PORT" \
    --worker-class aiohttp.GunicornWebWorker \
    --log-level 'info' \
    --reload

deactivate