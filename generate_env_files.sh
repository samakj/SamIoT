#!/bin/bash

. ./generate_env_functions.sh

generate_postgres_env_files
generate_frontend_env_files
generate_iot_api_env_files
generate_iot_forager_env_files
generate_solar_manager_env_files
generate_utilities_api_env_files
generate_utilities_scraper_env_files
generate_weather_api_env_files
generate_weather_scraper_env_files
generate_devices_env_files
generate_solar_controller_env_files
generate_api_docker_compose_env_files