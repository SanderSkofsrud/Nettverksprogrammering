// types.ts

import { Socket } from 'net';

export interface WebSocketConnection {
  socket: Socket;
  sendData: (data: string) => void;
  close: () => void;
}
