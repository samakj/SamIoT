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
psql --host=$DB_HOST --port=$DB_PORT --username=$POSTGRES_USER --db=$IOT_DB_NAME --command="COPY devices TO '$root/data/iot/devices.csv' DELIMITER ',' CSV HEADER;"

if [ -f "$root/data/iot/locations.csv" ]; then
    echo "\nRenaming old locations CSV...\n"
    mv $root/data/iot/locations.csv $root/data/iot/locations.old.csv
fi
echo "\nExporting locations...\n"
psql --host=$DB_HOST --port=$DB_PORT --username=$POSTGRES_USER --db=$IOT_DB_NAME --command="COPY locations TO '$root/data/iot/locations.csv' DELIMITER ',' CSV HEADER;"

if [ -f "$root/data/iot/metrics.csv" ]; then
    echo "\nRenaming old metrics CSV...\n"
    mv $root/data/iot/metrics.csv $root/data/iot/metrics.old.csv
fi
echo "\nExporting metrics...\n"
psql --host=$DB_HOST --port=$DB_PORT --username=$POSTGRES_USER --db=$IOT_DB_NAME --command="COPY metrics TO '$root/data/iot/metrics.csv' DELIMITER ',' CSV HEADER;"

if [ -f "$root/data/iot/measurements.csv" ]; then
    echo "\nRenaming old measurements CSV...\n"
    mv $root/data/iot/measurements.csv $root/data/iot/measurements.old.csv
fi
echo "\nExporting measurements...\n"
psql --host=$DB_HOST --port=$DB_PORT --username=$POSTGRES_USER --db=$IOT_DB_NAME --command="COPY measurements TO '$root/data/iot/measurements.csv' DELIMITER ',' CSV HEADER;"

if [ -f "$root/data/iot/float_measurements.csv" ]; then
    echo "\nRenaming old float_measurements CSV...\n"
    mv $root/data/iot/float_measurements.csv $root/data/iot/float_measurements.old.csv
fi
echo "\nExporting float_measurements...\n"
psql --host=$DB_HOST --port=$DB_PORT --username=$POSTGRES_USER --db=$IOT_DB_NAME --command="COPY float_measurements TO '$root/data/iot/float_measurements.csv' DELIMITER ',' CSV HEADER;"

if [ -f "$root/data/iot/integer_measurements.csv" ]; then
    echo "\nRenaming old integer_measurements CSV...\n"
    mv $root/data/iot/integer_measurements.csv $root/data/iot/integer_measurements.old.csv
fi
echo "\nExporting integer_measurements...\n"
psql --host=$DB_HOST --port=$DB_PORT --username=$POSTGRES_USER --db=$IOT_DB_NAME --command="COPY integer_measurements TO '$root/data/iot/integer_measurements.csv' DELIMITER ',' CSV HEADER;"

if [ -f "$root/data/iot/string_measurements.csv" ]; then
    echo "\nRenaming old string_measurements CSV...\n"
    mv $root/data/iot/string_measurements.csv $root/data/iot/string_measurements.old.csv
fi
echo "\nExporting string_measurements...\n"
psql --host=$DB_HOST --port=$DB_PORT --username=$POSTGRES_USER --db=$IOT_DB_NAME --command="COPY string_measurements TO '$root/data/iot/string_measurements.csv' DELIMITER ',' CSV HEADER;"

if [ -f "$root/data/iot/boolean_measurements.csv" ]; then
    echo "\nRenaming old boolean_measurements CSV...\n"
    mv $root/data/iot/boolean_measurements.csv $root/data/iot/boolean_measurements.old.csv
fi
echo "\nExporting boolean_measurements...\n"
psql --host=$DB_HOST --port=$DB_PORT --username=$POSTGRES_USER --db=$IOT_DB_NAME --command="COPY boolean_measurements TO '$root/data/iot/boolean_measurements.csv' DELIMITER ',' CSV HEADER;"





if [ -f "$root/data/utilities/gas.csv" ]; then
    echo "\nRenaming old gas utilities CSV...\n"
    mv $root/data/utilities/gas.csv $root/data/utilities/gas.old.csv
fi
echo "\nExporting gas utilities...\n"
psql --host=$DB_HOST --port=$DB_PORT --username=$POSTGRES_USER --db=$UTILITIES_DB_NAME --command="COPY gas TO '$root/data/utilities/gas.csv' DELIMITER ',' CSV HEADER;"

if [ -f "$root/data/utilities/electricity.csv" ]; then
    echo "\nRenaming old electricity utilities CSV...\n"
    mv $root/data/utilities/electricity.csv $root/data/utilities/electricity.old.csv
fi
echo "\nExporting electricity utilities...\n"
psql --host=$DB_HOST --port=$DB_PORT --username=$POSTGRES_USER --db=$UTILITIES_DB_NAME --command="COPY electricity TO '$root/data/utilities/electricity.csv' DELIMITER ',' CSV HEADER;"





if [ -f "$root/data/weather/current.csv" ]; then
    echo "\nRenaming old current weather CSV...\n"
    mv $root/data/weather/current.csv $root/data/weather/current.old.csv
fi
echo "\nExporting current weather...\n"
psql --host=$DB_HOST --port=$DB_PORT --username=$POSTGRES_USER --db=$WEATHER_DB_NAME --command="COPY current TO '$root/data/weather/current.csv' DELIMITER ',' CSV HEADER;"

if [ -f "$root/data/weather/hourly.csv" ]; then
    echo "\nRenaming old hourly weather CSV...\n"
    mv $root/data/weather/hourly.csv $root/data/weather/hourly.old.csv
fi
echo "\nExporting hourly weather...\n"
psql --host=$DB_HOST --port=$DB_PORT --username=$POSTGRES_USER --db=$WEATHER_DB_NAME --command="COPY hourly TO '$root/data/weather/hourly.csv' DELIMITER ',' CSV HEADER;"

if [ -f "$root/data/weather/daily.csv" ]; then
    echo "\nRenaming old daily weather CSV...\n"
    mv $root/data/weather/daily.csv $root/data/weather/daily.old.csv
fi
echo "\nExporting daily weather...\n"
psql --host=$DB_HOST --port=$DB_PORT --username=$POSTGRES_USER --db=$WEATHER_DB_NAME --command="COPY daily TO '$root/data/weather/daily.csv' DELIMITER ',' CSV HEADER;"
export PGPASSWORD=