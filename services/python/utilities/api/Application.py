import logging
from typing import Optional

from stores.gas import GasStore
from stores.electric import ElectricStore
from shared.python.extensions.aiohttp import ApplicationWithDatabaseAndCache

LOG = logging.getLogger(__name__)


class UtilitiesAPIApplication(ApplicationWithDatabaseAndCache):
    gas_store: Optional[GasStore] = None
    electric_store: Optional[ElectricStore] = None

    def connect_gas_store(self) -> None:
        if self.db is None:
            LOG.error(
                "Database not initialised before connecting store."
            )

        self.gas_store = GasStore(self.db)

    def connect_electric_store(self) -> None:
        if self.db is None:
            LOG.error(
                "Database not initialised before connecting store."
            )

        self.electric_store = ElectricStore(self.db)
