. ./env.sh

# echo "\nSetting postgres user password...\n"
# psql --host=$DB_HOST --port=$DB_PORT --username=$POSTGRES_USER --command="ALTER USER postgres PASSWORD '$POSTGRES_PASS';"

export PGPASSWORD=$POSTGRES_PASS
echo "\nCreating IoT database...\n"
psql --host=$DB_HOST --port=$DB_PORT --username=$POSTGRES_USER --command="CREATE DATABASE $IOT_DB_NAME;"
echo "\nCreating IoT API user...\n"
psql --host=$DB_HOST --port=$DB_PORT --username=$POSTGRES_USER --command="CREATE USER $IOT_DB_USER WITH PASSWORD '$IOT_DB_PASS';"

echo "\nGranting IoT API user access...\n"
psql --host=$DB_HOST --port=$DB_PORT --username=$POSTGRES_USER --dbname=$IOT_DB_NAME --command="GRANT SELECT, UPDATE, INSERT, DELETE ON ALL TABLES IN SCHEMA public TO $IOT_DB_USER;"
psql --host=$DB_HOST --port=$DB_PORT --username=$POSTGRES_USER --dbname=$IOT_DB_NAME --command="GRANT USAGE, SELECT ON ALL SEQUENCES IN SCHEMA public TO $IOT_DB_USER;"

echo "\nCreating Utilities database...\n"
psql --host=$DB_HOST --port=$DB_PORT --username=$POSTGRES_USER --command="CREATE DATABASE $UTILITIES_DB_NAME;"
echo "\nCreating Utilities API user...\n"
psql --host=$DB_HOST --port=$DB_PORT --username=$POSTGRES_USER --command="CREATE USER $UTILITIES_DB_USER WITH PASSWORD '$UTILITIES_PASS';"

echo "\nGranting Utilities API user access...\n"
psql --host=$DB_HOST --port=$DB_PORT --username=$POSTGRES_USER --dbname=$UTILITIES_DB_NAME --command="GRANT SELECT, UPDATE, INSERT, DELETE ON ALL TABLES IN SCHEMA public TO $UTILITIES_DB_USER;"
psql --host=$DB_HOST --port=$DB_PORT --username=$POSTGRES_USER --dbname=$UTILITIES_DB_NAME --command="GRANT USAGE, SELECT ON ALL SEQUENCES IN SCHEMA public TO $UTILITIES_DB_USER;"

echo "\nCreating Weather database...\n"
psql --host=$DB_HOST --port=$DB_PORT --username=$POSTGRES_USER --command="CREATE DATABASE $WEATHER_DB_NAME;"
echo "\nCreating Weather API user...\n"
psql --host=$DB_HOST --port=$DB_PORT --username=$POSTGRES_USER --command="CREATE USER $WEATHER_DB_USER WITH PASSWORD '$WEATHER_PASS';"

echo "\nGranting Weather API user access...\n"
psql --host=$DB_HOST --port=$DB_PORT --username=$POSTGRES_USER --dbname=$WEATHER_DB_NAME --command="GRANT SELECT, UPDATE, INSERT, DELETE ON ALL TABLES IN SCHEMA public TO $WEATHER_DB_USER;"
psql --host=$DB_HOST --port=$DB_PORT --username=$POSTGRES_USER --dbname=$WEATHER_DB_NAME --command="GRANT USAGE, SELECT ON ALL SEQUENCES IN SCHEMA public TO $WEATHER_DB_USER;"
export PGPASSWORD=
