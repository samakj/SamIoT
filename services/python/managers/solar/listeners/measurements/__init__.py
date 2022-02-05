import logging
from shared.python.extensions.aiohttp.websocket import Websocket

from shared.python.clients.iot import IoTClient
from listeners.measurements.EventHandlers import EventHandlers 

LOG = logging.getLogger()


class MeasurementsListener:
    iot_client: IoTClient
    websocket: Websocket
    
    event_handlers: EventHandlers

    def __init__(self, iot_client: IoTClient) -> None:
        self.iot_client = iot_client
        self.event_handlers = EventHandlers(iot_client)
    
    async def start(self) -> None:
        await self.event_handlers.update_metrics()
        await self.event_handlers.update_devices()
        await self.event_handlers.get_relay_states()
        
        self.websocket = await self.iot_client.measurements.listen_measurements(
            socket_id="solar_manager",
            name="Solar Manager",
            on_connect=self.event_handlers.connect(),
            on_message=self.event_handlers.message(),
            on_text=self.event_handlers.text(),
            on_json=self.event_handlers.json(),
            on_error=self.event_handlers.error(),
            on_close=self.event_handlers.close(),
            on_disconnect=self.event_handlers.disconnect(),
        )
        
        await self.websocket.start()