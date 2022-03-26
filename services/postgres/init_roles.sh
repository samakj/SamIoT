# echo "Setting postgres user password..."
# psql --host=$DB_HOST --port=$DB_PORT --username=$POSTGRES_USER --command="ALTER USER postgres PASSWORD '$POSTGRES_PASS';"

export PGPASSWORD=$POSTGRES_PASS
echo "Creating IoT API user..."
psql --port=$DB_PORT --username=$POSTGRES_USER --command="CREATE USER $IOT_DB_USER WITH PASSWORD '$IOT_DB_PASS';"

echo "Granting IoT API user access..."
psql --port=$DB_PORT --username=$POSTGRES_USER --dbname=$IOT_DB_NAME --command="GRANT SELECT, UPDATE, INSERT, DELETE ON ALL TABLES IN SCHEMA public TO $IOT_DB_USER;"
psql --port=$DB_PORT --username=$POSTGRES_USER --dbname=$IOT_DB_NAME --command="GRANT USAGE, SELECT ON ALL SEQUENCES IN SCHEMA public TO $IOT_DB_USER;"

echo "Creating Utilities API user..."
psql --port=$DB_PORT --username=$POSTGRES_USER --command="CREATE USER $UTILITIES_DB_USER WITH PASSWORD '$UTILITIES_DB_PASS';"

echo "Granting Utilities API user access..."
psql --port=$DB_PORT --username=$POSTGRES_USER --dbname=$UTILITIES_DB_NAME --command="GRANT SELECT, UPDATE, INSERT, DELETE ON ALL TABLES IN SCHEMA public TO $UTILITIES_DB_USER;"
psql --port=$DB_PORT --username=$POSTGRES_USER --dbname=$UTILITIES_DB_NAME --command="GRANT USAGE, SELECT ON ALL SEQUENCES IN SCHEMA public TO $UTILITIES_DB_USER;"

echo "Creating Weather API user..."
psql --port=$DB_PORT --username=$POSTGRES_USER --command="CREATE USER $WEATHER_DB_USER WITH PASSWORD '$WEATHER_DB_PASS';"

echo "Granting Weather API user access..."
psql --port=$DB_PORT --username=$POSTGRES_USER --dbname=$WEATHER_DB_NAME --command="GRANT SELECT, UPDATE, INSERT, DELETE ON ALL TABLES IN SCHEMA public TO $WEATHER_DB_USER;"
psql --port=$DB_PORT --username=$POSTGRES_USER --dbname=$WEATHER_DB_NAME --command="GRANT USAGE, SELECT ON ALL SEQUENCES IN SCHEMA public TO $WEATHER_DB_USER;"
export PGPASSWORD=
