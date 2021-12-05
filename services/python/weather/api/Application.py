from typing import Optional

from stores.current import CurrentStore
from stores.daily import DailyStore
from stores.hourly import HourlyStore
from shared.python.extensions.aiohttp import ApplicationWithDatabaseAndCache


class WeatherAPIApplication(ApplicationWithDatabaseAndCache):
    current_store: Optional[CurrentStore] = None
    hourly_store: Optional[HourlyStore] = None
    daily_store: Optional[DailyStore] = None

    def connect_current_store(self) -> None:
        if self.db is None:
            raise ValueError(
                "Database not initialised before connecting store."
            )

        self.current_store = CurrentStore(self.db)

    def connect_hourly_store(self) -> None:
        if self.db is None:
            raise ValueError(
                "Database not initialised before connecting store."
            )

        self.hourly_store = HourlyStore(self.db)

    def connect_daily_store(self) -> None:
        if self.db is None:
            raise ValueError(
                "Database not initialised before connecting store."
            )

        self.daily_store = DailyStore(self.db)
