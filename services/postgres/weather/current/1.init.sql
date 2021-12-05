CREATE TABLE IF NOT EXISTS current (
    id             SERIAL PRIMARY KEY,
    dt             TIMESTAMP UNIQUE,
    sunrise        TIMESTAMP NOT NULL,
    sunset         TIMESTAMP NOT NULL,
    temp           NUMERIC(8,4) NOT NULL,
    feels_like     NUMERIC(8,4) NOT NULL,
    pressure       INTEGER NOT NULL,
    humidity       INTEGER NOT NULL,
    dew_point      NUMERIC(8,4) NOT NULL,
    clouds         INTEGER NOT NULL,
    visibility     INTEGER NOT NULL,
    wind_speed     NUMERIC(8,4) NOT NULL,
    wind_deg       INTEGER NOT NULL,
    weather        JSON[] NOT NULL,
    wind_gust      NUMERIC(8,4),
    uvi            NUMERIC(8,4)
);