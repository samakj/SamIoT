#!/bin/bash

init_api () {
    root=$(pwd)
    
    . "$root/generate_env_functions.sh"
    generate_postgres_env_files
    generate_iot_api_env_files
    generate_utilities_api_env_files
    generate_weather_api_env_files
    generate_api_docker_compose_env_files

    root=$(pwd)
    shared="$root/shared"
    services="$root/services"
    python="$services/python"
    nginx="$services/nginx"
    redis="$services/redis"
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

    mkdir "$docker_compose/api/portainer-data"
    mkdir "$docker_compose/api/postgres-data"
    mkdir "$docker_compose/api/redis-data"
    
    export IOT_API_CONTAINER_NAME=$IOT_API_CONTAINER_NAME
    export IOT_API_PORT=$IOT_API_PORT
    export UTILITIES_API_CONTAINER_NAME=$UTILITIES_API_CONTAINER_NAME
    export UTILITIES_API_PORT=$UTILITIES_API_PORT
    export WEATHER_API_CONTAINER_NAME=$WEATHER_API_CONTAINER_NAME
    export WEATHER_API_PORT=$WEATHER_API_PORT
    export NGINX_PORT=$NGINX_PORT

    envsubst '${NGINX_PORT}:${IOT_API_CONTAINER_NAME}:${IOT_API_PORT}:${UTILITIES_API_CONTAINER_NAME}:${UTILITIES_API_PORT}:${WEATHER_API_CONTAINER_NAME}:${WEATHER_API_PORT}' \
        < "$nginx/api.template" \
        > "$nginx/api.conf"

    export CACHE_PORT=$CACHE_PORT

    envsubst '${CACHE_PORT}' \
        < "$redis/api.template" \
        > "$redis/api.conf"
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
    . "$docker_compose/scrapers/env.sh"

    mkdir "$docker_compose/scrapers/portainer-data"

    export IOT_FORAGER_CONTAINER_NAME=$IOT_FORAGER_CONTAINER_NAME
    export IOT_FORAGER_PORT=$IOT_FORAGER_PORT
    export UTILITIES_SCRAPER_CONTAINER_NAME=$UTILITIES_SCRAPER_CONTAINER_NAME
    export UTILITIES_SCRAPER_PORT=$UTILITIES_SCRAPER_PORT
    export WEATHER_SCRAPER_CONTAINER_NAME=$WEATHER_SCRAPER_CONTAINER_NAME
    export WEATHER_SCRAPER_PORT=$WEATHER_SCRAPER_PORT
    export NGINX_PORT=$NGINX_PORT

    envsubst '${NGINX_PORT}:${IOT_FORAGER_CONTAINER_NAME}:${IOT_FORAGER_PORT}:${UTILITIES_SCRAPER_CONTAINER_NAME}:${UTILITIES_SCRAPER_PORT}:${WEATHER_SCRAPER_CONTAINER_NAME}:${WEATHER_SCRAPER_PORT}' \
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

init_managers () {
    root=$(pwd)
    
    . "$root/generate_env_functions.sh"
    generate_solar_manager_env_files
    generate_managers_docker_compose_env_files

    root=$(pwd)
    shared="$root/shared"
    services="$root/services"
    python="$services/python"
    nginx="$services/nginx"
    managers="$python/managers"

    mkdir "$managers/solar/shared"
    cp -r "$root/shared/python" "$managers/solar/shared"

    docker_compose="$root/services/docker-compose"
    . "$docker_compose/managers/env.sh"

    mkdir "$docker_compose/managers/portainer-data"
    
    export SOLAR_MANAGER_CONTAINER_NAME=$SOLAR_MANAGER_CONTAINER_NAME
    export SOLAR_MANAGER_PORT=$SOLAR_MANAGER_PORT
    export NGINX_PORT=$NGINX_PORT

    envsubst '${NGINX_PORT}:${SOLAR_MANAGER_CONTAINER_NAME}:${SOLAR_MANAGER_PORT}' \
        < "$nginx/managers.template" \
        > "$nginx/managers.conf"
}

clean_managers () {
    . "$root/clean_env.sh"
    unset root
    unset shared
    unset services
    unset python
    unset managers
    unset docker_compose
}

run_managers () {
    init_managers

    echo "Starting managers..."

    # Useful for debugging 
    # docker-compose \
    #     --file "$docker_compose/managers/docker-compose.yml" \
    #     --env-file "$docker_compose/managers/.env" \
    #     --project-name "managers"\
    #     down
    # docker rm ${SOLAR_MANAGER_CONTAINER_NAME}
    # docker rmi ${SOLAR_MANAGER_CONTAINER_NAME}

    docker-compose \
        --file "$docker_compose/managers/docker-compose.yml" \
        --env-file "$docker_compose/managers/.env" \
        --project-name "managers"\
        build

    docker-compose \
        --file "$docker_compose/managers/docker-compose.yml" \
        --env-file "$docker_compose/managers/.env" \
        --project-name "managers"\
        up

    clean_managers
}

restart_managers () {
    init_managers

    echo "Restarting managers..."

    docker-compose \
        --file "$docker_compose/managers/docker-compose.yml" \
        --env-file "$docker_compose/managers/.env" \
        --project-name "managers"\
        restart

    clean_managers
}

init_frontend () {
    root=$(pwd)
    
    . "$root/generate_env_functions.sh"
    generate_frontend_env_files
    generate_frontend_docker_compose_env_files

    root=$(pwd)
    shared="$root/shared"
    services="$root/services"
    javascript="$services/javascript"
    nginx="$services/nginx"
    frontend="$javascript/frontend"

    mkdir "$frontend/shared"
    cp -r "$root/shared/javascript" "$frontend/shared"

    docker_compose="$root/services/docker-compose"
    . "$docker_compose/frontend/env.sh"

    mkdir "$docker_compose/managers/portainer-data"
    
    export FRONTEND_CONTAINER_NAME=$FRONTEND_CONTAINER_NAME
    export FRONTEND_PORT=$FRONTEND_PORT
    export NGINX_PORT=$NGINX_PORT

    envsubst '${NGINX_PORT}:${FRONTEND_CONTAINER_NAME}:${FRONTEND_PORT}' \
        < "$nginx/frontend.template" \
        > "$nginx/frontend.conf"
}

clean_frontend () {
    . "$root/clean_env.sh"
    unset root
    unset shared
    unset services
    unset javascript
    unset frontend
    unset docker_compose
}

run_frontend () {
    init_frontend

    echo "Starting frontend..."

    # Useful for debugging 
    # docker-compose \
    #     --file "$docker_compose/frontend/docker-compose.yml" \
    #     --env-file "$docker_compose/frontend/.env" \
    #     --project-name "frontend"\
    #     down
    # docker rm ${FRONTEND_CONTAINER_NAME}
    # docker rmi ${FRONTEND_CONTAINER_NAME}

    docker-compose \
        --file "$docker_compose/frontend/docker-compose.yml" \
        --env-file "$docker_compose/frontend/.env" \
        --project-name "frontend"\
        build

    docker-compose \
        --file "$docker_compose/frontend/docker-compose.yml" \
        --env-file "$docker_compose/frontend/.env" \
        --project-name "frontend"\
        up

    clean_frontend
}

restart_frontend () {
    init_frontend

    echo "Restarting frontend..."

    docker-compose \
        --file "$docker_compose/frontend/docker-compose.yml" \
        --env-file "$docker_compose/frontend/.env" \
        --project-name "frontend"\
        restart

    clean_frontend
}