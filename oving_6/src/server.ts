import * as net from 'net';
import * as fs from 'fs';
import * as path from 'path';
import { createWebSocketServer } from './websocket';

// Create and start a WebSocket server listening on port 3001.
// The callback function handles messages received from clients.
const wsServer = createWebSocketServer(3001, (msg, conn) => {
  // Broadcasts received message to all connected clients
  wsServer.broadcast(msg);
});

console.log('WebSocket server is running on port 3001');

/**
 * Sets up a simple HTTP server that serves an HTML file.
 * This server listens on port 3000 and serves the 'index.html' file to any GET request.
 * This is primarily used to serve the client-side application that connects to the WebSocket server.
 */

// Create a TCP server using the net module to handle incoming HTTP requests.
const httpServer = net.createServer(connection => {
  // Event listener for data received over the TCP connection
  connection.on('data', () => {
    // Construct the file path to 'index.html'
    const filePath = path.join(__dirname, 'index.html');
    // Asynchronously read the contents of 'index.html'
    fs.readFile(filePath, { encoding: 'utf-8' }, (err, data) => {
      if (err) {
        // If an error occurs, send an HTTP 500 Internal Server Error response
        connection.write('HTTP/1.1 500 Internal Server Error\r\n\r\n');
        connection.end();
      } else {
        // If the file is successfully read, send an HTTP 200 OK response followed by the file content
        connection.write('HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n');
        connection.write(data);
        // Close the TCP connection
        connection.end();
      }
    });
  });
});

// Make the HTTP server listen on port 3000.
httpServer.listen(3000, () => {
  console.log('HTTP server listening on port 3000');
});
