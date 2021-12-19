#!/bin/bash

source ./env.sh

root=$(pwd)
services="$root/services"
postgres="$services/postgres"
python="$services/python"
iot="$python/iot"
utilities="$python/utilities"
weather="$python/weather"



echo "Writing postgres service env files."
cat > "$postgres/.env" <<- EOF
# Database

DB_HOST="$DB_HOST"
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

echo "Writing iot/forager service env files."
cat > "$iot/forager/.env" <<- EOF
# API

IOT_API_HOST="$IOT_API_HOST"
IOT_API_PORT=$IOT_API_PORT

# Forager

IOT_FORAGER_HOST="$IOT_FORAGER_HOST"
IOT_FORAGER_PORT=$IOT_FORAGER_PORT
EOF
cp "$iot/forager/.env" "$iot/forager/env.sh"



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

echo "Writing utilities/scraper service env files."
cat > "$utilities/scraper/.env" <<- EOF
# API

UTILITIES_API_HOST="$UTILITIES_API_HOST"
UTILITIES_API_PORT=$UTILITIES_API_PORT

# Scraper

UTILITIES_SCRAPER_HOST="$UTILITIES_SCRAPER_HOST"
UTILITIES_SCRAPER_PORT=$UTILITIES_SCRAPER_PORT

# Octopus Energy

OCTOPUS_ENERGY_API_KEY="$OCTOPUS_ENERGY_API_KEY"
EOF
cp "$utilities/scraper/.env" "$utilities/scraper/env.sh"



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

echo "Writing weather/scraper service env files."
cat > "$weather/scraper/.env" <<- EOF
# API

WEATHER_API_HOST="$WEATHER_API_HOST"
WEATHER_API_PORT=$WEATHER_API_PORT

# Scraper

WEATHER_SCRAPER_HOST="$WEATHER_SCRAPER_HOST"
WEATHER_SCRAPER_PORT=$WEATHER_SCRAPER_PORT

# Open Weather Map

OWM_API_KEY="$OWM_API_KEY"
EOF
cp "$weather/scraper/.env" "$weather/scraper/env.sh"