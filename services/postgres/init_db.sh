export PGPASSWORD=$IOT_PASS
echo "\nInitialising iot devices table...\n"
psql --port=$DB_PORT --username=$POSTGRES_USER --db=$IOT_DB_NAME --file=/migrations/iot/devices/1.init.sql
echo "\nInitialising iot locations table...\n"
psql --port=$DB_PORT --username=$POSTGRES_USER --db=$IOT_DB_NAME --file=/migrations/iot/locations/1.init.sql
echo "\nInitialising iot metrics table...\n"
psql --port=$DB_PORT --username=$POSTGRES_USER --db=$IOT_DB_NAME --file=/migrations/iot/metrics/1.init.sql
echo "\nInitialising iot measurements table...\n"
psql --port=$DB_PORT --username=$POSTGRES_USER --db=$IOT_DB_NAME --file=/migrations/iot/measurements/1.init.sql

echo "\nInitialising gas utilities table...\n"
psql --port=$DB_PORT --username=$POSTGRES_USER --db=$UTILITIES_DB_NAME --file=/migrations/utilities/gas/1.init.sql
echo "\nInitialising electric utilities table...\n"
psql --port=$DB_PORT --username=$POSTGRES_USER --db=$UTILITIES_DB_NAME --file=/migrations/utilities/electric/1.init.sql

echo "\nInitialising current weather table...\n"
psql --port=$DB_PORT --username=$POSTGRES_USER --db=$WEATHER_DB_NAME --file=/migrations/weather/current/1.init.sql
echo "\nInitialising hourly weather table...\n"
psql --port=$DB_PORT --username=$POSTGRES_USER --db=$WEATHER_DB_NAME --file=/migrations/weather/hourly/1.init.sql
echo "\nInitialising daily weather table...\n"
psql --port=$DB_PORT --username=$POSTGRES_USER --db=$WEATHER_DB_NAME --file=/migrations/weather/daily/1.init.sql
export PGPASSWORD=
