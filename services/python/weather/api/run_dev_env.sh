#!/bin/bash

source ./env.sh

scrapers="$(dirname "$(pwd)")"
python="$(dirname $scrapers)"
services="$(dirname $python)"
root="$(dirname $services)"

python3 -m venv .
source bin/activate
ln -s "$root/shared" ./

pip3 install -r requirements.txt

gunicorn app:create_app \
    --bind "$WEATHER_API_HOST:$WEATHER_API_PORT" \
    --worker-class aiohttp.GunicornWebWorker \
    --log-level 'info' \
    --reload

deactivate