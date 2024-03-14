// websocket.ts

import { createServer, Socket } from 'net';
import { WebSocketConnection } from './types';
import { decodeMessage, encodeMessage, generateAcceptValue, parseHeaders } from './utils';

export function createWebSocketServer(port: number, onMessage: (msg: string, conn: WebSocketConnection) => void) {
  const clients = new Set<WebSocketConnection>();

  const server = createServer(socket => {
    const connection: WebSocketConnection = {
      socket,
      sendData: (data: string) => {
        const encoded = encodeMessage(data);
        socket.write(encoded);
      },
      close: () => {
        clients.delete(connection);
        socket.end();
      }
    };

    socket.on('data', buffer => {
      // Check for a connection close frame
      if (buffer[0] === 0x88) { // 0x88 is the opcode for connection close frame
        socket.end();
        return;
      }

      if (buffer.toString().match(/^GET /)) {
        const headers = parseHeaders(buffer.toString());
        const acceptKey = headers.get('sec-websocket-key');
        if (acceptKey) {
          const acceptValue = generateAcceptValue(acceptKey);
          socket.write('HTTP/1.1 101 Web Socket Protocol Handshake\r\n' +
                  'Upgrade: WebSocket\r\n' +
                  'Connection: Upgrade\r\n' +
                  'Sec-WebSocket-Accept: ' + acceptValue + '\r\n\r\n');
          clients.add(connection);
        }
      } else {
        const message = decodeMessage(buffer);
        onMessage(message, connection);
      }
    });
    

    socket.on('end', () => {
      clients.delete(connection);
    });
  });

  server.listen(port);

  return {
    broadcast: (msg: string) => {
      const encoded = encodeMessage(msg);
      clients.forEach(client => client.socket.write(encoded));
    }
  };
}
