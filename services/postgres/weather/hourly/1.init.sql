CREATE TABLE IF NOT EXISTS hourly (
    id                      SERIAL PRIMARY KEY,
    timestamp               TIMESTAMP NOT NULL UNIQUE,
    temperature             NUMERIC(8,4) NOT NULL,
    apparent_temperature    NUMERIC(8,4) NOT NULL,
    pressure                INTEGER NOT NULL,
    humidity                INTEGER NOT NULL,
    dew_point               NUMERIC(8,4) NOT NULL,
    cloud_coverage          INTEGER NOT NULL,
    wind_speed              NUMERIC(8,4) NOT NULL,
    wind_direction          INTEGER NOT NULL,
    owm_weather_id          INTEGER NOT NULL,
    owm_weather_title       TEXT NOT NULL,
    owm_weather_description TEXT NOT NULL,
    chance_of_rain          NUMERIC(8,4) NOT NULL,
    visibility              INTEGER,
    wind_gust               NUMERIC(8,4),
    uv_index                NUMERIC(8,4)
);