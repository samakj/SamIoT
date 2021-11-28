source ./env.sh
source ./init_roles.sh

services="$(dirname "$(pwd)")"
root="$(dirname $services)"

export PGPASSWORD=$IOT_PASS
if [ -f "$root/data/iot/devices.csv" ]; then
    echo "\nImporting devices data...\n"
    psql --host=$DB_HOST --port=$DB_PORT --username=$POSTGRES_USER --command="COPY devices FROM '$root/data/iot/devices.csv' DELIMITER ',' CSV HEADER"
fi
if [ -f "$root/data/iot/locations.csv" ]; then
    echo "\nImporting locations data...\n"
    psql --host=$DB_HOST --port=$DB_PORT --username=$POSTGRES_USER --command="COPY locations FROM '$root/data/iot/locations.csv' DELIMITER ',' CSV HEADER"
fi
if [ -f "$root/data/iot/metrics.csv" ]; then
    echo "\nImporting metrics data...\n"
    psql --host=$DB_HOST --port=$DB_PORT --username=$POSTGRES_USER --command="COPY metrics FROM '$root/data/iot/metrics.csv' DELIMITER ',' CSV HEADER"
fi
if [ -f "$root/data/iot/measurements.csv" ]; then
    echo "\nImporting measurements data...\n"
    psql --host=$DB_HOST --port=$DB_PORT --username=$POSTGRES_USER --command="COPY measurements FROM '$root/data/iot/measurements.csv' DELIMITER ',' CSV HEADER"
fi
export PGPASSWORD=