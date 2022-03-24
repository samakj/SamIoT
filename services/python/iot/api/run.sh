#!/bin/bash

gunicorn app:create_app \
    --bind "0.0.0.0:$IOT_API_PORT" \
    --worker-class aiohttp.GunicornWebWorker \
    --log-level 'info' \
    --reload