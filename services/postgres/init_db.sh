export PGPASSWORD=$IOT_PASS
echo "Creating IoT database..."
psql --port=$DB_PORT --username=$POSTGRES_USER --command="CREATE DATABASE $IOT_DB_NAME;"
echo "Initialising iot devices table..."
psql --port=$DB_PORT --username=$POSTGRES_USER --db=$IOT_DB_NAME --file=/migrations/iot/devices/1.init.sql
echo "Initialising iot locations table..."
psql --port=$DB_PORT --username=$POSTGRES_USER --db=$IOT_DB_NAME --file=/migrations/iot/locations/1.init.sql
echo "Initialising iot metrics table..."
psql --port=$DB_PORT --username=$POSTGRES_USER --db=$IOT_DB_NAME --file=/migrations/iot/metrics/1.init.sql
echo "Initialising iot measurements table..."
psql --port=$DB_PORT --username=$POSTGRES_USER --db=$IOT_DB_NAME --file=/migrations/iot/measurements/1.init.sql

echo "Creating Utilities database..."
psql --port=$DB_PORT --username=$POSTGRES_USER --command="CREATE DATABASE $UTILITIES_DB_NAME;"
echo "Initialising gas utilities table..."
psql --port=$DB_PORT --username=$POSTGRES_USER --db=$UTILITIES_DB_NAME --file=/migrations/utilities/gas/1.init.sql
echo "Initialising electric utilities table..."
psql --port=$DB_PORT --username=$POSTGRES_USER --db=$UTILITIES_DB_NAME --file=/migrations/utilities/electric/1.init.sql

echo "Creating Weather database..."
psql --port=$DB_PORT --username=$POSTGRES_USER --command="CREATE DATABASE $WEATHER_DB_NAME;"
echo "Initialising current weather table..."
psql --port=$DB_PORT --username=$POSTGRES_USER --db=$WEATHER_DB_NAME --file=/migrations/weather/current/1.init.sql
echo "Initialising hourly weather table..."
psql --port=$DB_PORT --username=$POSTGRES_USER --db=$WEATHER_DB_NAME --file=/migrations/weather/hourly/1.init.sql
echo "Initialising daily weather table..."
psql --port=$DB_PORT --username=$POSTGRES_USER --db=$WEATHER_DB_NAME --file=/migrations/weather/daily/1.init.sql
export PGPASSWORD=
