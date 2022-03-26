#!/bin/bash

. ./env.sh

get_env () {
    root=$(pwd)
    devices="$root/devices"
    services="$root/services"
    shared="$root/shared"
    javascript="$services/javascript"
    postgres="$services/postgres"
    redis="$services/redis"
    python="$services/python"
    docker_compose="$services/docker-compose"
    nginx="$services/nginx"
    frontend="$javascript/frontend"
    iot="$python/iot"
    managers="$python/managers"
    utilities="$python/utilities"
    weather="$python/weather"
}

clean_env () {
    unset root
    unset devices
    unset services
    unset shared
    unset javascript
    unset postgres
    unset redis
    unset python
    unset docker_compose
    unset nginx
    unset frontend
    unset iot
    unset managers
    unset utilities
    unset weather
}


generate_postgres_env_files () {
    get_env
    echo "Writing postgres service env files."
    cat > "$postgres/.env" <<- EOF
# Database

DB_HOST="$DB_CONTAINER_NAME"
DB_PORT=$DB_PORT

POSTGRES_USER="$POSTGRES_USER"
POSTGRES_PASS="$POSTGRES_PASS"

# IoT Database

IOT_DB_NAME="$IOT_DB_NAME"
IOT_DB_USER="$IOT_DB_USER"
IOT_DB_PASS="$IOT_DB_PASS"

# Utilities Database

UTILITIES_DB_NAME="$UTILITIES_DB_NAME"
UTILITIES_DB_USER="$UTILITIES_DB_USER"
UTILITIES_DB_PASS="$UTILITIES_DB_PASS"

# Weather Database

WEATHER_DB_NAME="$WEATHER_DB_NAME"
WEATHER_DB_USER="$WEATHER_DB_USER"
WEATHER_DB_PASS="$WEATHER_DB_PASS"
EOF
    cp "$postgres/.env" "$postgres/env.sh"
    clean_env
}

generate_frontend_env_files () {
    get_env
    echo "Writing javascript/frontend service env files."
    cat > "$javascript/frontend/.env" <<- EOF
# Frontend

FRONTEND_HOST="$FRONTEND_HOST"
FRONTEND_PORT=$FRONTEND_PORT

# IoT API

IOT_API_HOST="$IOT_API_HOST"
IOT_API_PORT=$IOT_API_PORT

# Utilities API

UTILITIES_API_HOST="$UTILITIES_API_HOST"
UTILITIES_API_PORT=$UTILITIES_API_PORT

# Weather API

WEATHER_API_HOST="$WEATHER_API_HOST"
WEATHER_API_PORT=$WEATHER_API_PORT
EOF
    cp "$javascript/frontend/.env" "$javascript/frontend/env.sh"
    clean_env
}

generate_iot_api_env_files() {
    get_env
    echo "Writing iot/api service env files."
    cat > "$iot/api/.env" <<- EOF
# Database

IOT_DB_HOST="$IOT_DB_HOST"
IOT_DB_PORT=$IOT_DB_PORT
IOT_DB_NAME="$IOT_DB_NAME"
IOT_DB_USER="$IOT_DB_USER"
IOT_DB_PASS="$IOT_DB_PASS"

# Cache

IOT_CACHE_HOST="$IOT_CACHE_HOST"
IOT_CACHE_PORT=$IOT_CACHE_PORT
IOT_CACHE_NAME="$IOT_CACHE_NAME"
IOT_CACHE_USER="$IOT_CACHE_USER"
IOT_CACHE_PASS="$IOT_CACHE_PASS"

# API

IOT_API_HOST="$IOT_API_HOST"
IOT_API_PORT=$IOT_API_PORT
EOF
    cp "$iot/api/.env" "$iot/api/env.sh"
    clean_env
}

generate_iot_forager_env_files () {
    get_env
    echo "Writing iot/forager service env files."
    cat > "$iot/forager/.env" <<- EOF
# API

IOT_API_HOST="$IOT_API_HOST"
IOT_API_PORT=$IOT_API_PORT

# Forager

IOT_FORAGER_HOST="$IOT_FORAGER_HOST"
IOT_FORAGER_PORT=$IOT_FORAGER_PORT

# Cache

IOT_CACHE_HOST="$IOT_CACHE_HOST"
IOT_CACHE_PORT=$IOT_CACHE_PORT
IOT_CACHE_NAME="$IOT_CACHE_NAME"
IOT_CACHE_USER="$IOT_CACHE_USER"
IOT_CACHE_PASS="$IOT_CACHE_PASS"
EOF
    cp "$iot/forager/.env" "$iot/forager/env.sh"
    clean_env
}

generate_solar_manager_env_files () {
    get_env
    echo "Writing manager/solar service env files."
    cat > "$managers/solar/.env" <<- EOF
# API

IOT_API_HOST="$IOT_API_HOST"
IOT_API_PORT=$IOT_API_PORT

# Solar Manager

SOLAR_MANAGER_HOST="$SOLAR_MANAGER_HOST"
SOLAR_MANAGER_PORT=$SOLAR_MANAGER_PORT

# Cache

SOLAR_MANAGER_CACHE_HOST="$SOLAR_MANAGER_CACHE_HOST"
SOLAR_MANAGER_CACHE_PORT=$SOLAR_MANAGER_CACHE_PORT
SOLAR_MANAGER_CACHE_NAME="$SOLAR_MANAGER_CACHE_NAME"
SOLAR_MANAGER_CACHE_USER="$SOLAR_MANAGER_CACHE_USER"
SOLAR_MANAGER_CACHE_PASS="$SOLAR_MANAGER_CACHE_PASS"
EOF
    cp "$managers/solar/.env" "$managers/solar/env.sh"
    clean_env
}

generate_utilities_api_env_files () {
    get_env
    echo "Writing utilities/api service env files."
    cat > "$utilities/api/.env" <<- EOF
# Database

UTILITIES_DB_HOST="$UTILITIES_DB_HOST"
UTILITIES_DB_PORT=$UTILITIES_DB_PORT
UTILITIES_DB_NAME="$UTILITIES_DB_NAME"
UTILITIES_DB_USER="$UTILITIES_DB_USER"
UTILITIES_DB_PASS="$UTILITIES_DB_PASS"

# Cache

UTILITIES_CACHE_HOST="$UTILITIES_CACHE_HOST"
UTILITIES_CACHE_PORT=$UTILITIES_CACHE_PORT
UTILITIES_CACHE_NAME="$UTILITIES_CACHE_NAME"
UTILITIES_CACHE_USER="$UTILITIES_CACHE_USER"
UTILITIES_CACHE_PASS="$UTILITIES_CACHE_PASS"

# API

UTILITIES_API_HOST="$UTILITIES_API_HOST"
UTILITIES_API_PORT=$UTILITIES_API_PORT
EOF
    cp "$utilities/api/.env" "$utilities/api/env.sh"
    clean_env
}

generate_utilities_scraper_env_files () {
    get_env
    echo "Writing utilities/scraper service env files."
    cat > "$utilities/scraper/.env" <<- EOF
# API

UTILITIES_API_HOST="$UTILITIES_API_HOST"
UTILITIES_API_PORT=$UTILITIES_API_PORT

# Scraper

UTILITIES_SCRAPER_HOST="$UTILITIES_SCRAPER_HOST"
UTILITIES_SCRAPER_PORT=$UTILITIES_SCRAPER_PORT

# Cache

UTILITIES_CACHE_HOST="$UTILITIES_CACHE_HOST"
UTILITIES_CACHE_PORT=$UTILITIES_CACHE_PORT
UTILITIES_CACHE_NAME="$UTILITIES_CACHE_NAME"
UTILITIES_CACHE_USER="$UTILITIES_CACHE_USER"
UTILITIES_CACHE_PASS="$UTILITIES_CACHE_PASS"

# Octopus Energy

OCTOPUS_ENERGY_API_KEY="$OCTOPUS_ENERGY_API_KEY"
ELECTRIC_METER_MPAN=$ELECTRIC_METER_MPAN
ELECTRIC_METER_SERIAL_NUMBER="$ELECTRIC_METER_SERIAL_NUMBER"
GAS_METER_MPRN=$GAS_METER_MPRN
GAS_METER_SERIAL_NUMBER="$GAS_METER_SERIAL_NUMBER"
EOF
    cp "$utilities/scraper/.env" "$utilities/scraper/env.sh"
    clean_env
}

generate_weather_api_env_files () {
    get_env
    echo "Writing weather/api service env files."
    cat > "$weather/api/.env" <<- EOF
# Database

WEATHER_DB_HOST="$WEATHER_DB_HOST"
WEATHER_DB_PORT=$WEATHER_DB_PORT
WEATHER_DB_NAME="$WEATHER_DB_NAME"
WEATHER_DB_USER="$WEATHER_DB_USER"
WEATHER_DB_PASS="$WEATHER_DB_PASS"

# Cache

WEATHER_CACHE_HOST="$WEATHER_CACHE_HOST"
WEATHER_CACHE_PORT=$WEATHER_CACHE_PORT
WEATHER_CACHE_NAME="$WEATHER_CACHE_NAME"
WEATHER_CACHE_USER="$WEATHER_CACHE_USER"
WEATHER_CACHE_PASS="$WEATHER_CACHE_PASS"

# API

WEATHER_API_HOST="$WEATHER_API_HOST"
WEATHER_API_PORT=$WEATHER_API_PORT
EOF
    cp "$weather/api/.env" "$weather/api/env.sh"
    clean_env
}

generate_weather_scraper_env_files () {
    get_env
    echo "Writing weather/scraper service env files."
    cat > "$weather/scraper/.env" <<- EOF
# API

WEATHER_API_HOST="$WEATHER_API_HOST"
WEATHER_API_PORT=$WEATHER_API_PORT

# Scraper

WEATHER_SCRAPER_HOST="$WEATHER_SCRAPER_HOST"
WEATHER_SCRAPER_PORT=$WEATHER_SCRAPER_PORT

# Cache

WEATHER_CACHE_HOST="$WEATHER_CACHE_HOST"
WEATHER_CACHE_PORT=$WEATHER_CACHE_PORT
WEATHER_CACHE_NAME="$WEATHER_CACHE_NAME"
WEATHER_CACHE_USER="$WEATHER_CACHE_USER"
WEATHER_CACHE_PASS="$WEATHER_CACHE_PASS"

# Weather

LOCATION_LATITUDE="$LOCATION_LATITUDE"
LOCATION_LONGITUDE="$LOCATION_LONGITUDE"

# Open Weather Map

OWM_API_KEY="$OWM_API_KEY"
EOF
    cp "$weather/scraper/.env" "$weather/scraper/env.sh"
    clean_env
}

generate_devices_env_files () {
    get_env
    cp "$devices/.env" "$devices/env.sh"
    echo "Writing devices env files."
cat > "$devices/.env" <<- EOF
# Wifi

PATTY_SSID="$PATTY_SSID"
PATTY_PASS="$PATTY_PASS"
SELMA_SSID="$SELMA_SSID"
SELMA_PASS="$SELMA_PASS"
VALE_SSID="$VALE_SSID"
VALE_PASS="$VALE_PASS"

# Devices

MESH_SSID="$MESH_SSID"
MESH_PASSWORD="$MESH_PASSWORD"

DEVICE_IP_MASK="$DEVICE_IP_MASK"

MESH_BRIDGE_IP_LOCATION=$MESH_BRIDGE_IP_LOCATION
MESH_BRIDGE_HOSTNAME="$MESH_BRIDGE_HOSTNAME"
MESH_BRIDGE_LOCATION="$MESH_BRIDGE_LOCATION"
MESH_BRIDGE_OTA_PASS="$MESH_BRIDGE_OTA_PASS"

SOLAR_CONTROLLER_IP_LOCATION=$SOLAR_CONTROLLER_IP_LOCATION
SOLAR_CONTROLLER_HOSTNAME="$SOLAR_CONTROLLER_HOSTNAME"
SOLAR_CONTROLLER_LOCATION="$SOLAR_CONTROLLER_LOCATION"
SOLAR_CONTROLLER_OTA_PASS="$SOLAR_CONTROLLER_OTA_PASS"

WATER_BUTT_IP_LOCATION=$WATER_BUTT_IP_LOCATION
WATER_BUTT_HOSTNAME="$WATER_BUTT_HOSTNAME"
WATER_BUTT_LOCATION="$WATER_BUTT_LOCATION"
WATER_BUTT_OTA_PASS="$WATER_BUTT_OTA_PASS"

POND_IP_LOCATION=$POND_IP_LOCATION
POND_HOSTNAME="$POND_HOSTNAME"
POND_LOCATION="$POND_LOCATION"
POND_OTA_PASS="$POND_OTA_PASS"

AQUAPONICS_IP_LOCATION=$AQUAPONICS_IP_LOCATION
AQUAPONICS_HOSTNAME="$AQUAPONICS_HOSTNAME"
AQUAPONICS_LOCATION="$AQUAPONICS_LOCATION"
AQUAPONICS_OTA_PASS="$AQUAPONICS_OTA_PASS"
EOF
    cp "$devices/.env" "$devices/env.sh"
    clean_env
}

generate_solar_controller_env_files () {
    get_env
    echo "Writing devices/solar-controller device env files."
    cat > "$devices/solar-controller/.env" <<- EOF
# Frontend

FRONTEND_HOST="$FRONTEND_HOST"
FRONTEND_PORT=$FRONTEND_PORT

# IoT API

IOT_API_HOST="$IOT_API_HOST"
IOT_API_PORT=$IOT_API_PORT

# Utilities API

UTILITIES_API_HOST="$UTILITIES_API_HOST"
UTILITIES_API_PORT=$UTILITIES_API_PORT

# Weather API

WEATHER_API_HOST="$WEATHER_API_HOST"
WEATHER_API_PORT=$WEATHER_API_PORT
EOF
    cp "$devices/solar-controller/.env" "$devices/solar-controller/env.sh"
    clean_env
}

generate_api_docker_compose_env_files() {
    get_env
    echo "Writing docker-compose/api env files."
    cat > "$docker_compose/api/.env" <<- EOF
# Ports

IOT_API_PORT=$IOT_API_PORT
UTILITIES_API_PORT=$UTILITIES_API_PORT
WEATHER_API_PORT=$WEATHER_API_PORT
NGINX_PORT=$API_NGINX_PORT
PORTAINER_PORT=$API_PORTAINER_PORT
DB_PORT=$DB_PORT
CACHE_PORT=$CACHE_PORT

# Build Vars
POSTGRES_USER=$POSTGRES_USER
POSTGRES_PASS=$POSTGRES_PASS
IOT_DB_NAME="$IOT_DB_NAME"
IOT_DB_USER="$IOT_DB_USER"
IOT_DB_PASS="$IOT_DB_PASS"
UTILITIES_DB_NAME="$UTILITIES_DB_NAME"
UTILITIES_DB_USER="$UTILITIES_DB_USER"
UTILITIES_DB_PASS="$UTILITIES_DB_PASS"
WEATHER_DB_NAME="$WEATHER_DB_NAME"
WEATHER_DB_USER="$WEATHER_DB_USER"
WEATHER_DB_PASS="$WEATHER_DB_PASS"

# Folders

IOT_FOLDER="$iot"
UTILITIES_FOLDER="$utilities"
WEATHER_FOLDER="$weather"
NGINX_FOLDER="$nginx"
SHARED_FOLDER="$shared"
POSTGRES_FOLDER="$postgres"
REDIS_FOLDER="$redis"

# Container Name

IOT_API_CONTAINER_NAME="$IOT_API_CONTAINER_NAME"
UTILITIES_API_CONTAINER_NAME="$UTILITIES_API_CONTAINER_NAME"
WEATHER_API_CONTAINER_NAME="$WEATHER_API_CONTAINER_NAME"
NGINX_CONTAINER_NAME="$API_NGINX_CONTAINER_NAME"
PORTAINER_CONTAINER_NAME="$API_PORTAINER_CONTAINER_NAME"
DB_CONTAINER_NAME="$DB_CONTAINER_NAME"
CACHE_CONTAINER_NAME="$CACHE_CONTAINER_NAME"

EOF
    cp "$docker_compose/api/.env" "$docker_compose/api/env.sh"
    clean_env
}

generate_scrapers_docker_compose_env_files() {
    get_env
    echo "Writing docker-compose/scrapers env files."
    cat > "$docker_compose/scrapers/.env" <<- EOF
# Ports

IOT_FORAGER_PORT=$IOT_FORAGER_PORT
UTILITIES_SCRAPER_PORT=$UTILITIES_SCRAPER_PORT
WEATHER_SCRAPER_PORT=$WEATHER_SCRAPER_PORT
NGINX_PORT=$SCRAPERS_NGINX_PORT
PORTAINER_PORT=$SCRAPERS_PORTAINER_PORT

# Folders

IOT_FOLDER="$iot"
UTILITIES_FOLDER="$utilities"
WEATHER_FOLDER="$weather"
NGINX_FOLDER="$nginx"
SHARED_FOLDER="$shared"

# Container Name

IOT_FORAGER_CONTAINER_NAME="$IOT_FORAGER_CONTAINER_NAME"
UTILITIES_SCRAPER_CONTAINER_NAME="$UTILITIES_SCRAPER_CONTAINER_NAME"
WEATHER_SCRAPER_CONTAINER_NAME="$WEATHER_SCRAPER_CONTAINER_NAME"
NGINX_CONTAINER_NAME="$SCRAPERS_NGINX_CONTAINER_NAME"
PORTAINER_CONTAINER_NAME="$SCRAPERS_PORTAINER_CONTAINER_NAME"

EOF
    cp "$docker_compose/scrapers/.env" "$docker_compose/scrapers/env.sh"
    clean_env
}

generate_managers_docker_compose_env_files() {
    get_env
    echo "Writing docker-compose/managers env files."
    cat > "$docker_compose/managers/.env" <<- EOF
# Ports

SOLAR_MANAGER_PORT=$SOLAR_MANAGER_PORT
NGINX_PORT=$MANAGERS_NGINX_PORT
PORTAINER_PORT=$MANAGERS_PORTAINER_PORT

# Folders

MANAGERS_FOLDER="$managers"
NGINX_FOLDER="$nginx"
SHARED_FOLDER="$shared"

# Container Name

SOLAR_MANAGER_CONTAINER_NAME="$SOLAR_MANAGER_CONTAINER_NAME"
NGINX_CONTAINER_NAME="$MANAGERS_NGINX_CONTAINER_NAME"
PORTAINER_CONTAINER_NAME="$MANAGERS_PORTAINER_CONTAINER_NAME"

EOF
    cp "$docker_compose/managers/.env" "$docker_compose/managers/env.sh"
    clean_env
}

generate_frontend_docker_compose_env_files() {
    get_env
    echo "Writing docker-compose/frontend env files."
    cat > "$docker_compose/frontend/.env" <<- EOF
# Ports

FRONTEND_PORT=$FRONTEND_PORT
NGINX_PORT=$FRONTEND_NGINX_PORT
PORTAINER_PORT=$FRONTEND_PORTAINER_PORT

# Folders

FRONTEND_FOLDER="$frontend"
NGINX_FOLDER="$nginx"
SHARED_FOLDER="$shared"

# Container Name

FRONTEND_CONTAINER_NAME="$FRONTEND_CONTAINER_NAME"
NGINX_CONTAINER_NAME="$FRONTEND_NGINX_CONTAINER_NAME"
PORTAINER_CONTAINER_NAME="$FRONTEND_PORTAINER_CONTAINER_NAME"

EOF
    cp "$docker_compose/frontend/.env" "$docker_compose/frontend/env.sh"
    clean_env
}