#!/bin/bash

source ./env.sh

iot="$(dirname "$(pwd)")"
python="$(dirname $iot)"
services="$(dirname $python)"
root="$(dirname $services)"

python3 -m venv .
source bin/activate
ln -s "$root/shared" ./

pip3 install -r requirements.txt

gunicorn app:create_app \
    --bind "$IOT_FORAGER_HOST:$IOT_FORAGER_PORT" \
    --worker-class aiohttp.GunicornWebWorker \
    --log-level 'info' \
    --reload

deactivate