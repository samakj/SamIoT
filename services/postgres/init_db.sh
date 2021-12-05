source ./env.sh
source ./init_roles.sh

export PGPASSWORD=$IOT_PASS
echo "\nInitialising iot devices table...\n"
psql --host=$DB_HOST --port=$DB_PORT --username=$POSTGRES_USER --db=$IOT_DB_NAME --file=./iot/devices/1.init.sql
echo "\nInitialising iot locations table...\n"
psql --host=$DB_HOST --port=$DB_PORT --username=$POSTGRES_USER --db=$IOT_DB_NAME --file=./iot/locations/1.init.sql
echo "\nInitialising iot metrics table...\n"
psql --host=$DB_HOST --port=$DB_PORT --username=$POSTGRES_USER --db=$IOT_DB_NAME --file=./iot/metrics/1.init.sql
echo "\nInitialising iot measurements table...\n"
psql --host=$DB_HOST --port=$DB_PORT --username=$POSTGRES_USER --db=$IOT_DB_NAME --file=./iot/measurements/1.init.sql

echo "\nInitialising current weather table...\n"
psql --host=$DB_HOST --port=$DB_PORT --username=$POSTGRES_USER --db=$WEATHER_DB_NAME --file=./weather/current/1.init.sql
echo "\nInitialising hourly weather table...\n"
psql --host=$DB_HOST --port=$DB_PORT --username=$POSTGRES_USER --db=$WEATHER_DB_NAME --file=./weather/hourly/1.init.sql
echo "\nInitialising daily weather table...\n"
psql --host=$DB_HOST --port=$DB_PORT --username=$POSTGRES_USER --db=$WEATHER_DB_NAME --file=./weather/daily/1.init.sql
export PGPASSWORD=
