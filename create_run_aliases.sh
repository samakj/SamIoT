#!/bin/bash

_run_api () {
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
    . "$docker_compose/api.env.sh"
    
    echo "Starting api..."

    export IOT_API_CONTAINER_NAME=$IOT_API_CONTAINER_NAME
    export IOT_API_PORT=$IOT_API_PORT
    export UTILITIES_API_CONTAINER_NAME=$UTILITIES_API_CONTAINER_NAME
    export UTILITIES_API_PORT=$UTILITIES_API_PORT
    export WEATHER_API_CONTAINER_NAME=$WEATHER_API_CONTAINER_NAME
    export WEATHER_API_PORT=$WEATHER_API_PORT

    envsubst '${IOT_API_CONTAINER_NAME}:${IOT_API_PORT}:${UTILITIES_API_CONTAINER_NAME}:${UTILITIES_API_PORT}:${WEATHER_API_CONTAINER_NAME}:${WEATHER_API_PORT}' \
        < "$nginx/api.template" \
        > "$nginx/api.conf"

    # Useful for debugging 
    # docker-compose \
    #     --file "$docker_compose/api-compose.yml" \
    #     --env-file "$docker_compose/api.env" \
    #     --project-name "api"\
    #     down
    # docker rm ${IOT_API_CONTAINER_NAME}
    # docker rm ${UTILITIES_API_CONTAINER_NAME}
    # docker rm ${WEATHER_API_CONTAINER_NAME}
    # docker rmi ${IOT_API_CONTAINER_NAME}
    # docker rmi ${UTILITIES_API_CONTAINER_NAME}
    # docker rmi ${WEATHER_API_CONTAINER_NAME}

    docker-compose \
        --file "$docker_compose/api-compose.yml" \
        --env-file "$docker_compose/api.env" \
        --project-name "api"\
        build

    docker-compose \
        --file "$docker_compose/api-compose.yml" \
        --env-file "$docker_compose/api.env" \
        --project-name "api"\
        up

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

alias run_api="_run_api"