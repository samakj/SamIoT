#!/bin/bash

. ./env.sh

managers="$(dirname "$(pwd)")"
python="$(dirname $managers)"
services="$(dirname $python)"
root="$(dirname $services)"

python3 -m venv .
. bin/activate
ln -s "$root/shared" ./

echo $controllers

pip3 install -r requirements.txt

gunicorn app:create_app \
    --bind "$SOLAR_MANAGER_HOST:$SOLAR_MANAGER_PORT" \
    --worker-class aiohttp.GunicornWebWorker \
    --log-level 'info' \
    --reload

deactivate