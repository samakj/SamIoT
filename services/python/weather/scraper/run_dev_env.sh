#!/bin/bash

. ./env.sh

scrapers="$(dirname "$(pwd)")"
python="$(dirname $scrapers)"
services="$(dirname $python)"
root="$(dirname $services)"

python3 -m venv .
. bin/activate
ln -s "$root/shared" ./

pip3 install -r requirements.txt

gunicorn app:create_app \
    --bind "$WEATHER_SCRAPER_HOST:$WEATHER_SCRAPER_PORT" \
    --worker-class aiohttp.GunicornWebWorker \
    --log-level 'info' \
    --reload

deactivate