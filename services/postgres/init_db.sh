source ./env.sh
source ./init_roles.sh

export PGPASSWORD=$IOT_PASS
echo "\nInitialising devices table...\n"
psql --host=$DB_HOST --port=$DB_PORT --username=$POSTGRES_USER --db=$IOT_DB_NAME --file=./iot/devices/1.init.sql
echo "\nInitialising locations table...\n"
psql --host=$DB_HOST --port=$DB_PORT --username=$POSTGRES_USER --db=$IOT_DB_NAME --file=./iot/locations/1.init.sql
echo "\nInitialising  metrics table...\n"
psql --host=$DB_HOST --port=$DB_PORT --username=$POSTGRES_USER --db=$IOT_DB_NAME --file=./iot/metrics/1.init.sql
echo "\nInitialising measurements table...\n"
psql --host=$DB_HOST --port=$DB_PORT --username=$POSTGRES_USER --db=$IOT_DB_NAME --file=./iot/measurements/1.init.sql
export PGPASSWORD=
