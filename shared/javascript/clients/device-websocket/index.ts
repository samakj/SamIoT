/** @format */

import { SECOND_IN_MS } from '../../static/times';

interface WebsocketCallbacks {
  open?: (event: WebSocketEventMap['open']) => void;
  message?: (event: WebSocketEventMap['message']) => void;
  error?: (event: WebSocketEventMap['error']) => void;
  close?: (event: WebSocketEventMap['close']) => void;
}

interface WebsocketMeta {
  attemptingReconnect?: Date;
  lastConnect?: Date;
  reconnectCount: number;
  lastMessage?: Date;
  messageCount: { [type: string]: number };
}

interface Message {
  date: Date;
  text: string;
  data?: {
    location: string;
    mac: string;
    metric: string;
    tags: string[];
    timestamp: string;
    type: string;
    value: string;
  };
}

export class DeviceWebsocket {
  websocket?: WebSocket;
  endpoint: string;
  callbacks: WebsocketCallbacks;
  meta: WebsocketMeta;

  keepPingMessages: boolean;
  maxStackSize: number;
  messageStack: Message[];

  constructor(
    endpoint: string = '/ws',
    callbacks: WebsocketCallbacks = {},
    maxStackSize: number = 1000,
    keepPingMessages: boolean = false
  ) {
    this.endpoint = endpoint;
    this.callbacks = callbacks;
    this.meta = {
      attemptingReconnect: null,
      lastConnect: null,
      reconnectCount: -1,
      lastMessage: null,
      messageCount: {},
    };
    this.messageStack = [];
    this.maxStackSize = maxStackSize;
    this.keepPingMessages = keepPingMessages;
  }

  start() {
    if (this.websocket) this.stop();

    this.messageStack.push({
      date: new Date(),
      text: `Connecting to websocket at ${this.endpoint}.`,
    });
    this.websocket = new WebSocket(`ws://${location.pathname}${this.endpoint}`);
    this.websocket.onopen = this.open;
    this.websocket.onmessage = this.message;
    this.websocket.onerror = this.error;
    this.websocket.onclose = this.close;
  }

  stop() {
    if (this.websocket) {
      this.messageStack.push({
        date: new Date(),
        text: `Closing connection to websocket at ${this.endpoint}.`,
      });
      this.websocket.close();
      this.websocket = null;
    }
  }

  open(event: Event) {
    this.callbacks.open(event);
    this.meta.lastConnect = new Date();
    this.meta.reconnectCount += 1;
    this.messageStack.push({
      date: new Date(),
      text: `Connected to websocket at ${this.endpoint}.`,
    });
  }

  message(event: MessageEvent) {
    this.callbacks.message(event);

    const now = new Date();
    this.meta.lastMessage = now;

    const message: Message = {
      date: now,
      text: event.data,
    };

    try {
      message.data = JSON.parse(event.data);
    } catch (_error) {
      console.warn(`Failed to pass message data as json:\n${event.data}`);
    }

    const type = message.data?.type || 'text';
    this.meta.messageCount[type] = this.meta.messageCount[type] || 0;
    this.meta.messageCount[type] += 1;

    if (type != 'ping' || this.keepPingMessages) this.messageStack.push(message);
  }

  error(event: Event) {
    this.callbacks.error(event);
    this.messageStack.push({
      date: new Date(),
      text: `Connection to websocket at ${this.endpoint} errored.`,
    });
  }

  close(event: CloseEvent) {
    this.callbacks.close(event);
    this.messageStack.push({
      date: new Date(),
      text: `Connection to websocket at ${this.endpoint} closed.`,
    });
  }

  checkLastMessageTime = () => {
    if (
      this.meta.attemptingReconnect &&
      +new Date() - +this.meta.attemptingReconnect < 10 * SECOND_IN_MS
    )
      return;
    if (this?.meta?.lastMessage && +new Date() - +this.meta.lastMessage > 15 * SECOND_IN_MS) {
      this.messageStack.push({
        date: new Date(),
        text: `No message from ${this.endpoint} in last 15s, assuming dead.`,
      });
      this.reconnect();
    }
  };

  reconnect() {
    if (
      this.meta.attemptingReconnect != null &&
      +new Date() - +this.meta.attemptingReconnect < 10 * SECOND_IN_MS
    )
      return;

    this.meta.attemptingReconnect = new Date();
    this.meta.reconnectCount++;
    this.messageStack.push({
      date: new Date(),
      text: 'Attempting to reconnect in 1s.',
    });
    setTimeout(this.start, 1000);
  }
}
