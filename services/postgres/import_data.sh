source ./env.sh
source ./init_roles.sh

export PGPASSWORD=$IOT_PASS
if [ -f "../../data/iot/devices.csv" ]; then
    echo "\nImporting devices data...\n"
    psql --host=$DB_HOST --port=$DB_PORT --username=$POSTGRES_USER --command="COPY devices FROM '../../data/iot/devices.csv' DELIMITER ',' CSV HEADER"
fi
if [ -f "../../data/iot/locations.csv" ]; then
    echo "\nImporting locations data...\n"
    psql --host=$DB_HOST --port=$DB_PORT --username=$POSTGRES_USER --command="COPY locations FROM '../../data/iot/locations.csv' DELIMITER ',' CSV HEADER"
fi
if [ -f "../../data/iot/metrics.csv" ]; then
    echo "\nImporting metrics data...\n"
    psql --host=$DB_HOST --port=$DB_PORT --username=$POSTGRES_USER --command="COPY metrics FROM '../../data/iot/metrics.csv' DELIMITER ',' CSV HEADER"
fi
if [ -f "../../data/iot/measurements.csv" ]; then
    echo "\nImporting measurements data...\n"
    psql --host=$DB_HOST --port=$DB_PORT --username=$POSTGRES_USER --command="COPY measurements FROM '../../data/iot/measurements.csv' DELIMITER ',' CSV HEADER"
fi
export PGPASSWORD=