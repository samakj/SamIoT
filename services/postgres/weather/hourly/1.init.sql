CREATE TABLE IF NOT EXISTS hourly (
    id             SERIAL PRIMARY KEY,
    dt             TIMESTAMP UNIQUE,
    temp           NUMERIC(8,4) NOT NULL,
    feels_like     NUMERIC(8,4) NOT NULL,
    pressure       INTEGER NOT NULL,
    humidity       INTEGER NOT NULL,
    dew_point      NUMERIC(8,4) NOT NULL,
    clouds         INTEGER NOT NULL,
    wind_speed     NUMERIC(8,4) NOT NULL,
    wind_deg       INTEGER NOT NULL,
    weather        JSON[] NOT NULL,
    pop            NUMERIC(8,4) NOT NULL,
    visibility     INTEGER,
    wind_gust      NUMERIC(8,4),
    uvi            NUMERIC(8,4)
);