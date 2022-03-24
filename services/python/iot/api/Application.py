import logging
from typing import Optional
from shared.python.extensions.aiohttp import ApplicationWithDatabaseAndCache

from stores.devices import DevicesStore
from stores.locations import LocationsStore
from stores.measurements import MeasurementsStore
from stores.metrics import MetricsStore

LOG = logging.getLogger(__name__)


class IoTAPIApplication(ApplicationWithDatabaseAndCache):
    devices_store: Optional[DevicesStore] = None
    locations_store: Optional[LocationsStore] = None
    measurements_store: Optional[MeasurementsStore] = None
    metrics_store: Optional[MetricsStore] = None

    def connect_devices_store(self) -> None:
        if self.db is None:
            LOG.error(
                "Database not initialised before connecting store."
            )

        self.devices_store = DevicesStore(self.db)

    def connect_locations_store(self) -> None:
        if self.db is None:
            LOG.error(
                "Database not initialised before connecting store."
            )

        self.locations_store = LocationsStore(self.db)

    def connect_measurements_store(self) -> None:
        if self.db is None:
            LOG.error(
                "Database not initialised before connecting store."
            )

        self.measurements_store = MeasurementsStore(self.db)

    def connect_metrics_store(self) -> None:
        if self.db is None:
            LOG.error(
                "Database not initialised before connecting store."
            )

        self.metrics_store = MetricsStore(self.db)
