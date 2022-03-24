#!/bin/bash

init_api () {
    root=$(pwd)
    
    . "$root/generate_env_functions.sh"
    generate_iot_api_env_files
    generate_utilities_api_env_files
    generate_weather_api_env_files
    generate_api_docker_compose_env_files

    root=$(pwd)
    shared="$root/shared"
    services="$root/services"
    python="$services/python"
    nginx="$services/nginx"
    iot="$python/iot"
    utilities="$python/utilities"
    weather="$python/weather"

    mkdir "$iot/api/shared"
    cp -r "$root/shared/python" "$iot/api/shared"
    mkdir "$utilities/api/shared"
    cp -r "$root/shared/python" "$utilities/api/shared"
    mkdir "$weather/api/shared"
    cp -r "$root/shared/python" "$weather/api/shared"

    docker_compose="$root/services/docker-compose"
    . "$docker_compose/api/env.sh"
    
    export IOT_API_CONTAINER_NAME=$IOT_API_CONTAINER_NAME
    export IOT_API_PORT=$IOT_API_PORT
    export UTILITIES_API_CONTAINER_NAME=$UTILITIES_API_CONTAINER_NAME
    export UTILITIES_API_PORT=$UTILITIES_API_PORT
    export WEATHER_API_CONTAINER_NAME=$WEATHER_API_CONTAINER_NAME
    export WEATHER_API_PORT=$WEATHER_API_PORT

    envsubst '${IOT_API_CONTAINER_NAME}:${IOT_API_PORT}:${UTILITIES_API_CONTAINER_NAME}:${UTILITIES_API_PORT}:${WEATHER_API_CONTAINER_NAME}:${WEATHER_API_PORT}' \
        < "$nginx/api.template" \
        > "$nginx/api.conf"
}

clean_api () {
    . "$root/clean_env.sh"
    unset root
    unset shared
    unset services
    unset python
    unset iot
    unset utilities
    unset weather
    unset docker_compose
}

run_api () {
    init_api

    echo "Starting api..."

    # Useful for debugging 
    # docker-compose \
    #     --file "$docker_compose/api/docker-compose.yml" \
    #     --env-file "$docker_compose/api/.env" \
    #     --project-name "api"\
    #     down
    # docker rm ${IOT_API_CONTAINER_NAME}
    # docker rm ${UTILITIES_API_CONTAINER_NAME}
    # docker rm ${WEATHER_API_CONTAINER_NAME}
    # docker rmi ${IOT_API_CONTAINER_NAME}
    # docker rmi ${UTILITIES_API_CONTAINER_NAME}
    # docker rmi ${WEATHER_API_CONTAINER_NAME}

    docker-compose \
        --file "$docker_compose/api/docker-compose.yml" \
        --env-file "$docker_compose/api/.env" \
        --project-name "api"\
        build

    docker-compose \
        --file "$docker_compose/api/docker-compose.yml" \
        --env-file "$docker_compose/api/.env" \
        --project-name "api"\
        up

    clean_api
}

restart_api () {
    init_api

    echo "Restarting api..."

    docker-compose \
        --file "$docker_compose/api/docker-compose.yml" \
        --env-file "$docker_compose/api/.env" \
        --project-name "api"\
        restart

    clean_api
}

init_scrapers () {
    root=$(pwd)
    
    . "$root/generate_env_functions.sh"
    generate_iot_forager_env_files
    generate_utilities_scraper_env_files
    generate_weather_scraper_env_files
    generate_scrapers_docker_compose_env_files

    root=$(pwd)
    shared="$root/shared"
    services="$root/services"
    python="$services/python"
    nginx="$services/nginx"
    iot="$python/iot"
    utilities="$python/utilities"
    weather="$python/weather"

    mkdir "$iot/forager/shared"
    cp -r "$root/shared/python" "$iot/forager/shared"
    mkdir "$utilities/scraper/shared"
    cp -r "$root/shared/python" "$utilities/scraper/shared"
    mkdir "$weather/scraper/shared"
    cp -r "$root/shared/python" "$weather/scraper/shared"

    docker_compose="$root/services/docker-compose"
    . "$docker_compose/scraper/env.sh"
    

    export IOT_FORAGER_CONTAINER_NAME=$IOT_FORAGER_CONTAINER_NAME
    export IOT_FORAGER_PORT=$IOT_FORAGER_PORT
    export UTILITIES_SCRAPER_CONTAINER_NAME=$UTILITIES_SCRAPER_CONTAINER_NAME
    export UTILITIES_SCRAPER_PORT=$UTILITIES_SCRAPER_PORT
    export WEATHER_SCRAPER_CONTAINER_NAME=$WEATHER_SCRAPER_CONTAINER_NAME
    export WEATHER_SCRAPER_PORT=$WEATHER_SCRAPER_PORT

    envsubst '${IOT_FORAGER_CONTAINER_NAME}:${IOT_FORAGER_PORT}:${UTILITIES_SCRAPER_CONTAINER_NAME}:${UTILITIES_SCRAPER_PORT}:${WEATHER_SCRAPER_CONTAINER_NAME}:${WEATHER_SCRAPER_PORT}' \
        < "$nginx/scrapers.template" \
        > "$nginx/scrapers.conf"
}

clean_scrapers () {
    . "$root/clean_env.sh"
    unset root
    unset shared
    unset services
    unset python
    unset iot
    unset utilities
    unset weather
    unset docker_compose
}

run_scrapers () {
    init_scrapers

    echo "Starting scrapers..."

    # Useful for debugging 
    # docker-compose \
    #     --file "$docker_compose/scrapers/docker-compose.yml" \
    #     --env-file "$docker_compose/scrapers/.env" \
    #     --project-name "scrapers"\
    #     down
    # docker rm ${IOT_FORAGER_CONTAINER_NAME}
    # docker rm ${UTILITIES_SCRAPER_CONTAINER_NAME}
    # docker rm ${WEATHER_SCRAPER_CONTAINER_NAME}
    # docker rmi ${IOT_FORAGER_CONTAINER_NAME}
    # docker rmi ${UTILITIES_SCRAPER_CONTAINER_NAME}
    # docker rmi ${WEATHER_SCRAPER_CONTAINER_NAME}

    docker-compose \
        --file "$docker_compose/scrapers/docker-compose.yml" \
        --env-file "$docker_compose/scrapers/.env" \
        --project-name "scrapers"\
        build

    docker-compose \
        --file "$docker_compose/scrapers/docker-compose.yml" \
        --env-file "$docker_compose/scrapers/.env" \
        --project-name "scrapers"\
        up

    clean_scrapers
}

restart_scrapers () {
    init_scrapers

    echo "Restarting scrapers..."

    docker-compose \
        --file "$docker_compose/scrapers/docker-compose.yml" \
        --env-file "$docker_compose/scrapers/.env" \
        --project-name "scrapers"\
        restart
    
    clean_scrapers
}