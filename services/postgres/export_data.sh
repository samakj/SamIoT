source ./env.sh
source ./init_roles.sh

services="$(dirname "$(pwd)")"
root="$(dirname $services)"

export PGPASSWORD=$IOT_PASS
if [ -f "$root/data/iot/devices.csv" ]; then
    echo "\nRenaming old devices CSV...\n"
    mv $root/data/iot/devices.csv $root/data/iot/devices.old.csv
fi
echo "\nExporting devices...\n"
psql --host=$DB_HOST --port=$DB_PORT --username=$POSTGRES_USER --command="COPY devices TO '$root/data/iot/devices.csv' DELIMITER ',' CSV HEADER;"

if [ -f "$root/data/iot/locations.csv" ]; then
    echo "\nRenaming old locations CSV...\n"
    mv $root/data/iot/locations.csv $root/data/iot/locations.old.csv
fi
echo "\nExporting locations...\n"
psql --host=$DB_HOST --port=$DB_PORT --username=$POSTGRES_USER --command="COPY locations TO '$root/data/iot/locations.csv' DELIMITER ',' CSV HEADER;"

if [ -f "$root/data/iot/metrics.csv" ]; then
    echo "\nRenaming old metrics CSV...\n"
    mv $root/data/iot/metrics.csv $root/data/iot/metrics.old.csv
fi
echo "\nExporting metrics...\n"
psql --host=$DB_HOST --port=$DB_PORT --username=$POSTGRES_USER --command="COPY metrics TO '$root/data/iot/metrics.csv' DELIMITER ',' CSV HEADER;"

if [ -f "$root/data/iot/measurements.csv" ]; then
    echo "\nRenaming old measurements CSV...\n"
    mv $root/data/iot/measurements.csv $root/data/iot/measurements.old.csv
fi
echo "\nExporting measurements...\n"
psql --host=$DB_HOST --port=$DB_PORT --username=$POSTGRES_USER --command="COPY measurements TO '$root/data/iot/measurements.csv' DELIMITER ',' CSV HEADER;"

export PGPASSWORD=