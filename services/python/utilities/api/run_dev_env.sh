#!/bin/bash

. ./env.sh

scrapers="$(dirname "$(pwd)")"
python="$(dirname $scrapers)"
services="$(dirname $python)"
root="$(dirname $services)"

python3 -m venv .
. bin/activate
cp -r "$root/shared" ./

pip3 install -r requirements.txt

gunicorn app:create_app \
    --bind "$UTILITIES_API_HOST:$UTILITIES_API_PORT" \
    --worker-class aiohttp.GunicornWebWorker \
    --log-level 'info' \
    --reload

deactivate