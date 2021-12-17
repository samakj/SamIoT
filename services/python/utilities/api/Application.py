from typing import Optional

from stores.gas import GasStore
from stores.electricity import ElectricityStore
from shared.python.extensions.aiohttp import ApplicationWithDatabaseAndCache


class UtilitiesAPIApplication(ApplicationWithDatabaseAndCache):
    gas_store: Optional[GasStore] = None
    electricity_store: Optional[ElectricityStore] = None

    def connect_gas_store(self) -> None:
        if self.db is None:
            raise ValueError(
                "Database not initialised before connecting store."
            )

        self.gas_store = GasStore(self.db)

    def connect_electricity_store(self) -> None:
        if self.db is None:
            raise ValueError(
                "Database not initialised before connecting store."
            )

        self.electricity_store = ElectricityStore(self.db)
