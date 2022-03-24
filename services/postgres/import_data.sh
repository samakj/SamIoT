. ./env.sh
. ./init_roles.sh

services="$(dirname "$(pwd)")"
root="$(dirname $services)"

export PGPASSWORD=$POSTGRES_PASS
if [ -f "$root/data/iot/devices.csv" ]; 
then
    echo "\nImporting iot devices data...\n"
    psql --host=$DB_HOST --port=$DB_PORT --username=$POSTGRES_USER --db=$IOT_DB_NAME --command="\copy devices FROM '$root/data/iot/devices.csv' DELIMITER ',' CSV HEADER"
else
    echo "\nFailed to import iot devices data, $root/data/iot/devices.csv not found...\n"
fi
if [ -f "$root/data/iot/locations.csv" ]; 
then
    echo "\nImporting iot locations data...\n"
    psql --host=$DB_HOST --port=$DB_PORT --username=$POSTGRES_USER --db=$IOT_DB_NAME --command="\copy locations FROM '$root/data/iot/locations.csv' DELIMITER ',' CSV HEADER"
else
    echo "\nFailed to import iot locations data, $root/data/iot/locations.csv not found...\n"
fi
if [ -f "$root/data/iot/metrics.csv" ]; 
then
    echo "\nImporting iot metrics data...\n"
    psql --host=$DB_HOST --port=$DB_PORT --username=$POSTGRES_USER --db=$IOT_DB_NAME --command="\copy metrics FROM '$root/data/iot/metrics.csv' DELIMITER ',' CSV HEADER"
else
    echo "\nFailed to import iot metrics data, $root/data/iot/metrics.csv not found...\n"
fi
if [ -f "$root/data/iot/measurements.csv" ]; 
then
    echo "\nImporting iot measurements data...\n"
    psql --host=$DB_HOST --port=$DB_PORT --username=$POSTGRES_USER --db=$IOT_DB_NAME --command="\copy measurements FROM '$root/data/iot/measurements.csv' DELIMITER ',' CSV HEADER"
else
    echo "\nFailed to import iot measurements data, $root/data/iot/measurements.csv not found...\n"
fi
if [ -f "$root/data/iot/float_measurements.csv" ]; 
then
    echo "\nImporting iot float_measurements data...\n"
    psql --host=$DB_HOST --port=$DB_PORT --username=$POSTGRES_USER --db=$IOT_DB_NAME --command="\copy float_measurements FROM '$root/data/iot/float_measurements.csv' DELIMITER ',' CSV HEADER"
else
    echo "\nFailed to import iot float_measurements data, $root/data/iot/float_measurements.csv not found...\n"
fi
if [ -f "$root/data/iot/integer_measurements.csv" ]; 
then
    echo "\nImporting iot integer_measurements data...\n"
    psql --host=$DB_HOST --port=$DB_PORT --username=$POSTGRES_USER --db=$IOT_DB_NAME --command="\copy integer_measurements FROM '$root/data/iot/integer_measurements.csv' DELIMITER ',' CSV HEADER"
else
    echo "\nFailed to import iot integer_measurements data, $root/data/iot/integer_measurements.csv not found...\n"
fi
if [ -f "$root/data/iot/string_measurements.csv" ]; 
then
    echo "\nImporting iot string_measurements data...\n"
    psql --host=$DB_HOST --port=$DB_PORT --username=$POSTGRES_USER --db=$IOT_DB_NAME --command="\copy string_measurements FROM '$root/data/iot/string_measurements.csv' DELIMITER ',' CSV HEADER"
else
    echo "\nFailed to import iot string_measurements data, $root/data/iot/string_measurements.csv not found...\n"
fi
if [ -f "$root/data/iot/boolean_measurements.csv" ]; 
then
    echo "\nImporting iot boolean_measurements data...\n"
    psql --host=$DB_HOST --port=$DB_PORT --username=$POSTGRES_USER --db=$IOT_DB_NAME --command="\copy boolean_measurements FROM '$root/data/iot/boolean_measurements.csv' DELIMITER ',' CSV HEADER"
else
    echo "\nFailed to import iot boolean_measurements data, $root/data/iot/boolean_measurements.csv not found...\n"
fi



if [ -f "$root/data/utilities/gas.csv" ]; 
then
    echo "\nImporting gas utilities data...\n"
    psql --host=$DB_HOST --port=$DB_PORT --username=$POSTGRES_USER --db=$UTILITIES_DB_NAME --command="\copy gas FROM '$root/data/utilities/gas.csv' DELIMITER ',' CSV HEADER"
else
    echo "\nFailed to import gas utilities data, $root/data/utilities/gas.csv not found...\n"
fi
if [ -f "$root/data/utilities/electric.csv" ]; 
then
    echo "\nImporting electric utilities data...\n"
    psql --host=$DB_HOST --port=$DB_PORT --username=$POSTGRES_USER --db=$UTILITIES_DB_NAME --command="\copy electric FROM '$root/data/utilities/electric.csv' DELIMITER ',' CSV HEADER"
else
    echo "\nFailed to import electric utilities data, $root/data/utilities/electric.csv not found...\n"
fi



if [ -f "$root/data/weather/current.csv" ]; 
then
    echo "\nImporting current weather data...\n"
    psql --host=$DB_HOST --port=$DB_PORT --username=$POSTGRES_USER --db=$WEATHER_DB_NAME --command="\copy current FROM '$root/data/weather/current.csv' DELIMITER ',' CSV HEADER"
else
    echo "\nFailed to import current weather data, $root/data/weather/current.csv not found...\n"
fi
if [ -f "$root/data/weather/hourly.csv" ]; 
then
    echo "\nImporting hourly weather data...\n"
    psql --host=$DB_HOST --port=$DB_PORT --username=$POSTGRES_USER --db=$WEATHER_DB_NAME --command="\copy hourly FROM '$root/data/weather/hourly.csv' DELIMITER ',' CSV HEADER"
else
    echo "\nFailed to import hourly weather data, $root/data/weather/hourly.csv not found...\n"
fi
if [ -f "$root/data/weather/daily.csv" ]; 
then
    echo "\nImporting daily weather data...\n"
    psql --host=$DB_HOST --port=$DB_PORT --username=$POSTGRES_USER --db=$WEATHER_DB_NAME --command="\copy daily FROM '$root/data/weather/daily.csv' DELIMITER ',' CSV HEADER"
else
    echo "\nFailed to import daily weather data, $root/data/weather/daily.csv not found...\n"
fi
export PGPASSWORD=