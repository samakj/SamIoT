let state;
let websocketHandler;
let uiHandler;
let loopInterval;

const SECOND_IN_MS = 1000;
const MINUTE_IN_MS = 60 * SECOND_IN_MS;
const HOUR_IN_MS = 60 * MINUTE_IN_MS;
const DAY_IN_MS = 24 * HOUR_IN_MS;

const initialiseState = () => {
  state = {
    connection: {
      attemptingReconnect: false,
      pageLoad: new Date(),
      lastConnect: null,
      reconnectCount: 0,
      messageCount: 0,
      lastMessage: null,
    },
    measurements: {},
    wifi: {},
  };
};

const initialiseElements = () => {
  elements = {
    state: document.getElementById("state"),
    messages: document.getElementById("messages"),
    reconnects: document.getElementById("reconnects"),
    terminal: document.getElementById("terminal"),
    wifi: document.getElementById("wifi"),
    toggles: document.getElementById("toggles"),
  };
};

const toTimeDifferenceString = (date, startDate = null) => {
  startDate = startDate == null ? new Date() : new Date(startDate);
  date = new Date(date);

  const msDifference = Math.abs(+startDate - +date);

  const days = Math.floor(msDifference / DAY_IN_MS);
  const hours = Math.floor((msDifference % DAY_IN_MS) / HOUR_IN_MS);
  const minutes = Math.floor((msDifference % HOUR_IN_MS) / MINUTE_IN_MS);
  const seconds = Math.floor((msDifference % MINUTE_IN_MS) / SECOND_IN_MS);
  const milliseconds = msDifference % SECOND_IN_MS;

  if (msDifference < SECOND_IN_MS) return `${milliseconds}ms`;
  if (msDifference < 10 * SECOND_IN_MS)
    return `${(seconds + milliseconds / 1000).toFixed(2)}s`;
  if (msDifference < MINUTE_IN_MS)
    return `${(seconds + milliseconds / 1000).toFixed(1)}s`;
  if (msDifference < HOUR_IN_MS) return `${minutes}m ${seconds}s`;
  if (msDifference < DAY_IN_MS) return `${hours}h ${minutes}m ${seconds}s`;
  return `${days}d ${hours}h ${minutes}m`;
};

const toDateString = (date = null) => {
  return (date == null ? new Date() : new Date(date))
    .toISOString()
    .slice(0, 19)
    .replace("T", " ");
};

class UIHandler {
  elements;

  constructor() {
    this.elements = {
      measurements: document.getElementById("measurements"),
      page: {
        load: document.querySelector("#page .load"),
      },
      connection: {
        start: document.querySelector("#connection .start"),
        duration: document.querySelector("#connection .duration"),
      },
      reconnects: {
        count: document.querySelector("#reconnects .count"),
        rate: document.querySelector("#reconnects .rate"),
      },
      messages: {
        count: document.querySelector("#messages .count"),
        last: document.querySelector("#messages .last"),
        sinceLast: document.querySelector("#messages .since-last"),
      },
      terminal: document.getElementById("terminal"),
      wifi: document.getElementById("wifi"),
      toggles: document.getElementById("toggles"),
    };

    this.toggleParent = this.toggleParent.bind(this);
    this.applySectionToggleEvents();
  }

  addTerminalLine = (message, date = null, level = null, classes = null) => {
    const hasScrolled =
      this.elements.terminal.scrollTop <
      this.elements.scrollHeight - this.elements.terminal.clientHeight;

    this.elements.terminal.insertAdjacentHTML(
      "beforeend",
      `<pre class="line${level != null ? ` ${level}` : ""}${
        classes != null ? ` ${classes}` : ""
      }">` +
        `${toDateString(date)}| ${message}` +
        `</pre>`
    );

    if (!hasScrolled) {
      this.elements.terminal.scrollTop = this.elements.terminal.scrollHeight;
    }
  };

  addPageTerminalInfoLine = (message) => {
    this.addTerminalLine(`---- ${message} ----`, null, "info");
  };

  trimTerminal = () => {
    while (this.elements.terminal.childElementCount > 1000)
      this.elements.terminal.removeChild(terminal.childNodes[0]);
  };

  toggleParent = (event) => {
    const parentClassList = event.target.parentElement.classList;
    if (parentClassList.contains("closed")) {
      parentClassList.remove("closed");
      parentClassList.add("open");
    } else {
      parentClassList.remove("open");
      parentClassList.add("closed");
    }
  };

  applySectionToggleEvents = () => {
    document.querySelectorAll("section .section-title").forEach((element) => {
      element.removeEventListener("click", this.toggleParent);
      element.addEventListener("click", this.toggleParent);
    });
  };

  drawPage = () => {
    const load = websocketHandler?.meta?.pageLoad
      ? toDateString(websocketHandler?.meta?.pageLoad)
      : "-";
    if (this.elements.page.load.innerText !== load)
      this.elements.page.load.innerText = load;
  };

  drawConnection = () => {
    const start = websocketHandler?.meta?.lastConnect
      ? toDateString(websocketHandler?.meta?.lastConnect)
      : "-";
    const duration = websocketHandler?.meta?.lastConnect
      ? toTimeDifferenceString(websocketHandler?.meta?.lastConnect)
      : "-";

    if (this.elements.connection.start.innerText !== start)
      this.elements.connection.start.innerText = start;
    if (this.elements.connection.duration.innerText !== duration)
      this.elements.connection.duration.innerText = duration;
  };

  drawReconnects = () => {
    const count = (websocketHandler?.meta?.reconnectCount).toLocaleString();
    const rate = `${(
      (websocketHandler?.meta?.reconnectCount * HOUR_IN_MS) /
      (+new Date() - +websocketHandler?.meta?.pageLoad)
    ).toFixed(1)}/h`;

    if (this.elements.reconnects.count.innerText !== count)
      this.elements.reconnects.count.innerText = count;
    if (this.elements.reconnects.rate.innerText !== rate)
      this.elements.reconnects.rate.innerText = rate;
  };

  drawMessages = () => {
    const count = websocketHandler?.meta?.messageCount.toLocaleString();
    const last = websocketHandler?.meta?.lastMessage
      ? toDateString(websocketHandler?.meta?.lastMessage)
      : "-";
    const sinceLast = websocketHandler?.meta?.lastMessage
      ? toTimeDifferenceString(websocketHandler?.meta?.lastMessage)
      : "-";

    if (this.elements.messages.count.innerText !== count)
      this.elements.messages.count.innerText = count;
    if (this.elements.messages.last.innerText !== last)
      this.elements.messages.last.innerText = last;
    if (this.elements.messages.sinceLast.innerText !== sinceLast)
      this.elements.messages.sinceLast.innerText = sinceLast;
  };

  drawToggles = () => {
    let html = "";
    Object.keys(state.measurements).forEach((group) => {
      html += `<label><input type="checkbox" onChange="console.log"> Hide ${group}</label>`;
    });

    if (this.elements.toggles.innerHTML !== html) {
      this.elements.toggles.innerHTML = html;
    }
  };

  drawState = (forceRedraw = false) => {
    if (
      this.elements.measurements.childElementCount - 1 !=
        Object.keys(state?.measurements || {}).length ||
      forceRedraw
    ) {
      let html = '<h2 class="section-title">Measurement data</h2>';

      Object.keys(state?.measurements || {})
        .sort()
        .filter((key) => key != "wifi")
        .forEach((group) => {
          html += `<div id="${group}" class="measurement-item">`;
          html += `<h3>${group}</h3>`;
          html += `<table>`;
          html += `<tbody>`;

          Object.keys(state.measurements[group]).forEach((metric) => {
            html += `<tr>`;
            html += `<th>${metric}</th>`;
            html += `<td class="${metric}">`;

            if (state.measurements[group][metric] == null) {
              html += "null";
            } else if (metric === "temperature") {
              html += `${state.measurements[group][metric].toFixed(1)}\u00B0c`;
            } else if (metric === "humidity") {
              html += `${state.measurements[group][metric].toFixed(1)}%`;
            } else if (metric === "co2") {
              html += `${state.measurements[group][metric]}ppm`;
            } else if (metric === "eco2") {
              html += `${state.measurements[group][metric]}ppm`;
            } else if (metric === "tvoc") {
              html += `${state.measurements[group][metric]}ppb`;
            } else if (metric === "distance") {
              html +=
                Math.abs(state.measurements[group][metric]) >= 1
                  ? `${state.measurements[group][metric].toFixed(2)}m`
                  : `${(state.measurements[group][metric] * 1000).toFixed(
                      0
                    )}mm`;
            } else {
              html += state.measurements[group][metric];
            }

            html += `</td>`;
          });

          html += `</tbody>`;
          html += `</table>`;
          html += `</div>`;
        });

      if (this.elements.measurements.innerHTML !== html) {
        this.elements.measurements.innerHTML = html;
      }
      this.applySectionToggleEvents();
    } else {
      for (const group in state?.measurements || {}) {
        for (const metric in state.measurements[group]) {
          let content = state.measurements[group][metric];
          if (state.measurements[group][metric] == null) {
            content = "null";
          } else if (metric === "temperature") {
            content = `${state.measurements[group][metric].toFixed(1)}\u00B0c`;
          } else if (metric === "humidity") {
            content = `${state.measurements[group][metric].toFixed(1)}%`;
          } else if (metric === "co2") {
            content = `${state.measurements[group][metric]}ppm`;
          } else if (metric === "eco2") {
            content = `${state.measurements[group][metric]}ppm`;
          } else if (metric === "tvoc") {
            content = `${state.measurements[group][metric]}ppb`;
          } else if (metric === "distance") {
            content =
              Math.abs(state.measurements[group][metric]) >= 1
                ? `${state.measurements[group][metric].toFixed(2)}m`
                : `${(state.measurements[group][metric] * 1000).toFixed(0)}mm`;
          }

          const element = this.elements.measurements.querySelector(
            `#${group} .${metric}`
          );

          if (element) {
            if (element.innerText != content) element.innerText = content;
          } else {
            return this.drawState(true);
          }
        }
      }
    }
  };

  drawWifi = () => {
    let html =
      `${state?.wifi?.ssid} ` + `${state?.wifi?.strength?.toFixed(0)}%`;

    if (this.elements.wifi.innerText !== html) {
      this.elements.wifi.innerText = html;
    }
  };

  draw = () => {
    this.drawPage();
    this.drawConnection();
    this.drawReconnects();
    this.drawMessages();
    this.drawWifi();
    this.drawToggles();
    this.drawState();

    this.trimTerminal();
  };
}

class WebSocketHandler {
  websocket;
  meta;

  constructor() {
    this.meta = {
      attemptingReconnect: false,
      pageLoad: new Date(),
      lastConnect: null,
      reconnectCount: 0,
      messageCount: 0,
      lastMessage: null,
    };

    this.onWebsocketOpen = this.onWebsocketOpen.bind(this);
    this.onWebsocketMessage = this.onWebsocketMessage.bind(this);
    this.onWebsocketClose = this.onWebsocketClose.bind(this);
    this.onWebsocketError = this.onWebsocketError.bind(this);
  }

  handlePingMessage = () => {};

  handleWifiMessage = (data) => {
    if (data.metric === "ssid") state.wifi.ssid = data.value;
    if (data.metric === "strength")
      state.wifi.strength = data.value != null ? parseFloat(data.value) : null;
  };

  addTemperatureLine = (data) => {
    return uiHandler?.addTerminalLine(
      `${data.tags?.length ? `${data.tags.join("-")}: ` : ""}` +
        `${data.metric} => ` +
        `${data?.value != null ? `${data.value.toFixed(1)}\u00B0c` : "null"}`,
      data.timestamp,
      "info",
      `${data.tags?.length ? data.tags.join("-") : ""} ${data.metric}`
    );
  };

  addHumidityLine = (data) => {
    return uiHandler?.addTerminalLine(
      `${data.tags?.length ? `${data.tags.join("-")}: ` : ""}` +
        `${data.metric} => ` +
        `${data?.value != null ? `${data.value.toFixed(1)}%` : "null"}`,
      data.timestamp,
      "info",
      `${data.tags?.length ? data.tags.join("-") : ""} ${data.metric}`
    );
  };

  addCO2Line = (data) => {
    return uiHandler?.addTerminalLine(
      `${data.tags?.length ? `${data.tags.join("-")}: ` : ""}` +
        `${data.metric} => ` +
        `${data?.value != null ? `${data.value}ppm` : "null"}`,
      data.timestamp,
      "info",
      `${data.tags?.length ? data.tags.join("-") : ""} ${data.metric}`
    );
  };

  handleMeasurementMessage = (data) => {
    let logFn;
    if (data.timestamp < "2021-01-01T00:00:00Z") {
      data = {
        ...data,
        timestamp: new Date().toISOString(),
      };
    }
    if (data.metric === "temperature") {
      data = {
        ...data,
        value: data.value != null ? parseFloat(data.value) : null,
      };
      logFn = this.addTemperatureLine;
    }
    if (data.metric === "humidity") {
      data = {
        ...data,
        value: data.value != null ? parseFloat(data.value) : null,
      };
      logFn = this.addHumidityLine;
    }
    if (data.metric === "co2") {
      data = {
        ...data,
        value: data.value != null ? parseInt(data.value) : null,
      };
      logFn = this.addCO2Line;
    }
    if (data.metric === "eco2") {
      data = {
        ...data,
        value: data.value != null ? parseInt(data.value) : null,
      };
      logFn = this.addCO2Line;
    }
    if (data.metric === "tvoc") {
      data = {
        ...data,
        value: data.value != null ? parseInt(data.value) : null,
      };
      logFn = this.addCO2Line;
    }
    if (data.metric === "depth") {
      data = {
        ...data,
        value: data.value != null ? parseFloat(data.value) : null,
      };
      logFn = this.addDistanceLine;
    }

    state.measurements[data.tags?.join("-")] =
      state?.measurements[data.tags?.join("-")] || {};
    state.measurements[data.tags?.join("-")][data.metric] = data.value;

    return logFn
      ? logFn(data)
      : uiHandler.addTerminalLine(
          `${data.tags?.length ? `${data.tags.join("-")}: ` : ""}` +
            `${data.metric} => ${data?.value != null ? data.value : "null"}`,
          data.timestamp,
          "info",
          `${data.tags?.length ? data.tags.join("-") : ""} ${data.metric}`
        );
  };

  handleErrorMessage = () => {};

  onWebsocketOpen = () => {
    uiHandler?.addPageTerminalInfoLine(
      `Connected to websocket at ${this.websocket?.url}`
    );
    this.meta.lastConnect = new Date();
    this.meta.lastMessage = new Date();
    this.meta.attemptingReconnect = false;
  };

  onWebsocketMessage = (event) => {
    this.meta.lastMessage = new Date();
    this.meta.messageCount++;

    const data = JSON.parse(event.data);
    if (data.tags?.includes("wifi")) return this.handleWifiMessage(data);
    if (data.type === "error") return this.handleErrorMessage(data);
    if (data.type === "ping") return this.handlePingMessage(data);

    return this.handleMeasurementMessage(data);
  };

  onWebsocketClose = () => {
    if (this.meta.attemptingReconnect) return;

    uiHandler?.addPageTerminalInfoLine(
      `Connection to websocket at ${this.websocket.url} closed`
    );
    this.reconnect();
  };

  onWebsocketError = () => {
    if (this.meta.attemptingReconnect) return;

    uiHandler?.addPageTerminalInfoLine(
      `Connection to websocket at ${this.websocket.url} errored`
    );
    this.reconnect();
  };

  checkLastMessageTime = () => {
    if (
      this.meta.attemptingReconnect &&
      +new Date() - this.meta.attemptingReconnect < 10 * SECOND_IN_MS
    )
      return;
    if (
      this?.meta?.lastMessage &&
      +new Date() - +this.meta.lastMessage > 15 * SECOND_IN_MS
    ) {
      uiHandler?.addPageTerminalInfoLine(
        "Last message recieved more than 15 seconds ago, assuming dead"
      );
      this.reconnect();
    }
  };

  reconnect = () => {
    if (
      this.meta.attemptingReconnect &&
      +new Date() - this.meta.attemptingReconnect < 10 * SECOND_IN_MS
    )
      return;

    this.meta.attemptingReconnect = new Date();
    this.meta.reconnectCount++;
    uiHandler?.addPageTerminalInfoLine("Attempting to reconnect in 1s...");
    setTimeout(this.connect, 1000);
  };

  connect = () => {
    this.websocket?.close();
    this.websocket = new WebSocket(`ws://${location.host}/ws`);
    this.websocket.onopen = this.onWebsocketOpen;
    this.websocket.onmessage = this.onWebsocketMessage;
    this.websocket.onclose = this.onWebsocketClose;
    this.websocket.onerror = this.onWebsocketError;
  };
}

const runLoop = () => {
  clearInterval(loopInterval);
  loopInterval = setInterval(() => {
    websocketHandler?.checkLastMessageTime();
    uiHandler?.draw();
  }, 100);
};

const init = () => {
  initialiseState();
  uiHandler = new UIHandler();
  websocketHandler = new WebSocketHandler();
  websocketHandler.connect();
  runLoop();
};

document.readyState === "complete"
  ? init()
  : (document.onreadystatechange = () => {
      if (document.readyState === "complete") init();
    });
