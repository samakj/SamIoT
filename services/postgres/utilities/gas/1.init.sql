CREATE TABLE IF NOT EXISTS gas (
    id                      SERIAL PRIMARY KEY,
    timestamp               TIMESTAMP NOT NULL UNIQUE,
    consumption             NUMERIC(8,4) NOT NULL
);