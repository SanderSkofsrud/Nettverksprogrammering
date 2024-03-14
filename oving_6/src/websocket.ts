import { createServer, Socket } from 'net';
import { WebSocketConnection } from './types';
import { decodeMessage, encodeMessage, generateAcceptValue, parseHeaders } from './utils';

/**
 * Creates a WebSocket server on the specified port. It handles the WebSocket handshake,
 * incoming messages, and manages client connections.
 *
 * @param port The port number for the WebSocket server to listen on.
 * @param onMessage A callback function that is called whenever a message is received from a client.
 *                  The callback receives the message as a string and the WebSocketConnection object representing the client.
 *
 * @returns An object with a `broadcast` method that can be used to send a message to all connected clients.
 *
 * @example
 * createWebSocketServer(3001, (msg, conn) => {
 *   console.log(`Received message: ${msg}`);
 *   conn.sendData(`Echo: ${msg}`);
 * });
 */
export function createWebSocketServer(port: number, onMessage: (msg: string, conn: WebSocketConnection) => void) {
  // Set to keep track of all client connections
  const clients = new Set<WebSocketConnection>();

  // Create a TCP server to handle incoming connections
  const server = createServer(socket => {
    // Define a new WebSocket connection object
    const connection: WebSocketConnection = {
      socket,
      // Method for sending data through the socket
      sendData: (data: string) => {
        const encoded = encodeMessage(data);
        socket.write(encoded);
      },
      // Method for closing the socket connection
      close: () => {
        sendCloseFrame(socket);
        clients.delete(connection);
      }
    };

    // Event listener for receiving data on the socket
    socket.on('data', buffer => {
      // Check for a WebSocket closing frame
      if (buffer[0] === 0x88) {
        connection.close();
        return;
      }

      // Handle the initial WebSocket handshake
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
        // Decode and handle incoming WebSocket messages
        const message = decodeMessage(buffer);
        onMessage(message, connection);
      }
    });

    // Event listener for when the socket connection is ended
    socket.on('end', () => {
      clients.delete(connection);
    });

    // Event listener for socket errors
    socket.on('error', (error) => {
      console.error('WebSocket error:', error);
      connection.close();
    });
  });

  // Event listener for server errors
  server.on('error', (error) => {
    console.error('Server error:', error);
  });

  // Start listening for connections on the specified port
  server.listen(port);

  // Return an object with a method for broadcasting messages to all clients
  return {
    broadcast: (msg: string) => {
      const encoded = encodeMessage(msg);
      clients.forEach(client => client.socket.write(encoded));
    }
  };
}

/**
 * Sends a WebSocket closing frame to the specified client socket,
 * indicating that the server wishes to close the connection.
 * This function is called internally when a connection needs to be closed gracefully.
 *
 * @param socket The socket connection to which the close frame will be sent.
 *
 * @example
 * // Assuming `socket` is a Socket object for a connected client
 * sendCloseFrame(socket);
 */
function sendCloseFrame(socket: Socket) {
  const closeFrame = Buffer.alloc(2);
  closeFrame[0] = 0x88; // Set FIN bit and specify closing frame
  closeFrame[1] = 0x00; // No payload length
  socket.write(closeFrame, () => socket.end());
}
