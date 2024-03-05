import * as net from 'net';
import * as fs from 'fs';
import * as path from 'path';
import { createWebSocketServer } from './websocket';

// Create a WebSocket server
const wsServer = createWebSocketServer(3001, (msg, conn) => {
  // Broadcast received message to all connected clients
  wsServer.broadcast(msg);
});

console.log('WebSocket server is running on port 3001');

// Create a simple HTTP server on port 3000
const httpServer = net.createServer(connection => {
  connection.on('data', () => {
    // Serve the HTML file
    const filePath = path.join(__dirname, 'index.html');
    fs.readFile(filePath, { encoding: 'utf-8' }, (err, data) => {
      if (err) {
        connection.write('HTTP/1.1 500 Internal Server Error\r\n\r\n');
        connection.end();
      } else {
        connection.write('HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n');
        connection.write(data);
        connection.end();
      }
    });
  });
});

httpServer.listen(3000, () => {
  console.log('HTTP server listening on port 3000');
});
