source ./env.sh
source ./init_roles.sh

services="$(dirname "$(pwd)")"
root="$(dirname $services)"

export PGPASSWORD=$IOT_PASS
if [ -f "$root/data/iot/devices.csv" ]; then
    echo "\nImporting iot devices data...\n"
    psql --host=$DB_HOST --port=$DB_PORT --username=$POSTGRES_USER --db=$IOT_DB_NAME --command="COPY devices FROM '$root/data/iot/devices.csv' DELIMITER ',' CSV HEADER"
fi
if [ -f "$root/data/iot/locations.csv" ]; then
    echo "\nImporting iot locations data...\n"
    psql --host=$DB_HOST --port=$DB_PORT --username=$POSTGRES_USER --db=$IOT_DB_NAME --command="COPY locations FROM '$root/data/iot/locations.csv' DELIMITER ',' CSV HEADER"
fi
if [ -f "$root/data/iot/metrics.csv" ]; then
    echo "\nImporting iot metrics data...\n"
    psql --host=$DB_HOST --port=$DB_PORT --username=$POSTGRES_USER --db=$IOT_DB_NAME --command="COPY metrics FROM '$root/data/iot/metrics.csv' DELIMITER ',' CSV HEADER"
fi
if [ -f "$root/data/iot/measurements.csv" ]; then
    echo "\nImporting iot measurements data...\n"
    psql --host=$DB_HOST --port=$DB_PORT --username=$POSTGRES_USER --db=$IOT_DB_NAME --command="COPY measurements FROM '$root/data/iot/measurements.csv' DELIMITER ',' CSV HEADER"
fi
if [ -f "$root/data/iot/float_measurements.csv" ]; then
    echo "\nImporting iot float_measurements data...\n"
    psql --host=$DB_HOST --port=$DB_PORT --username=$POSTGRES_USER --db=$IOT_DB_NAME --command="COPY float_measurements FROM '$root/data/iot/float_measurements.csv' DELIMITER ',' CSV HEADER"
fi
if [ -f "$root/data/iot/integer_measurements.csv" ]; then
    echo "\nImporting iot integer_measurements data...\n"
    psql --host=$DB_HOST --port=$DB_PORT --username=$POSTGRES_USER --db=$IOT_DB_NAME --command="COPY integer_measurements FROM '$root/data/iot/integer_measurements.csv' DELIMITER ',' CSV HEADER"
fi
if [ -f "$root/data/iot/string_measurements.csv" ]; then
    echo "\nImporting iot string_measurements data...\n"
    psql --host=$DB_HOST --port=$DB_PORT --username=$POSTGRES_USER --db=$IOT_DB_NAME --command="COPY string_measurements FROM '$root/data/iot/string_measurements.csv' DELIMITER ',' CSV HEADER"
fi
if [ -f "$root/data/iot/boolean_measurements.csv" ]; then
    echo "\nImporting iot boolean_measurements data...\n"
    psql --host=$DB_HOST --port=$DB_PORT --username=$POSTGRES_USER --db=$IOT_DB_NAME --command="COPY boolean_measurements FROM '$root/data/iot/boolean_measurements.csv' DELIMITER ',' CSV HEADER"
fi



if [ -f "$root/data/utilities/gas.csv" ]; then
    echo "\nImporting gas utilities data...\n"
    psql --host=$DB_HOST --port=$DB_PORT --username=$POSTGRES_USER --db=$UTILITIES_DB_NAME --command="COPY gas FROM '$root/data/utilities/gas.csv' DELIMITER ',' CSV HEADER"
fi
if [ -f "$root/data/utilities/electricity.csv" ]; then
    echo "\nImporting electricity utilities data...\n"
    psql --host=$DB_HOST --port=$DB_PORT --username=$POSTGRES_USER --db=$UTILITIES_DB_NAME --command="COPY electricity FROM '$root/data/utilities/electricity.csv' DELIMITER ',' CSV HEADER"
fi



if [ -f "$root/data/weather/current.csv" ]; then
    echo "\nImporting current weather data...\n"
    psql --host=$DB_HOST --port=$DB_PORT --username=$POSTGRES_USER --db=$WEATHER_DB_NAME --command="COPY current FROM '$root/data/weather/current.csv' DELIMITER ',' CSV HEADER"
fi
if [ -f "$root/data/weather/hourly.csv" ]; then
    echo "\nImporting hourly weather data...\n"
    psql --host=$DB_HOST --port=$DB_PORT --username=$POSTGRES_USER --db=$WEATHER_DB_NAME --command="COPY hourly FROM '$root/data/weather/hourly.csv' DELIMITER ',' CSV HEADER"
fi
if [ -f "$root/data/weather/daily.csv" ]; then
    echo "\nImporting daily weather data...\n"
    psql --host=$DB_HOST --port=$DB_PORT --username=$POSTGRES_USER --db=$WEATHER_DB_NAME --command="COPY daily FROM '$root/data/weather/daily.csv' DELIMITER ',' CSV HEADER"
fi
export PGPASSWORD=