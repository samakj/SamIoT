#!/bin/bash

source ./env.sh
source ./init_roles.sh

export PGPASSWORD=$IOT_PASS
psql --host=$DB_HOST --port=$DB_PORT --username=$POSTGRES_USER --file=./iot/devices/1.init.sql
psql --host=$DB_HOST --port=$DB_PORT --username=$POSTGRES_USER --file=./iot/locations/1.init.sql
psql --host=$DB_HOST --port=$DB_PORT --username=$POSTGRES_USER --file=./iot/metrics/1.init.sql
psql --host=$DB_HOST --port=$DB_PORT --username=$POSTGRES_USER --file=./iot/measurements/1.init.sql
export PGPASSWORD=
