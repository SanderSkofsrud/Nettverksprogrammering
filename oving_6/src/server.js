"use strict";
var __createBinding = (this && this.__createBinding) || (Object.create ? (function(o, m, k, k2) {
    if (k2 === undefined) k2 = k;
    var desc = Object.getOwnPropertyDescriptor(m, k);
    if (!desc || ("get" in desc ? !m.__esModule : desc.writable || desc.configurable)) {
      desc = { enumerable: true, get: function() { return m[k]; } };
    }
    Object.defineProperty(o, k2, desc);
}) : (function(o, m, k, k2) {
    if (k2 === undefined) k2 = k;
    o[k2] = m[k];
}));
var __setModuleDefault = (this && this.__setModuleDefault) || (Object.create ? (function(o, v) {
    Object.defineProperty(o, "default", { enumerable: true, value: v });
}) : function(o, v) {
    o["default"] = v;
});
var __importStar = (this && this.__importStar) || function (mod) {
    if (mod && mod.__esModule) return mod;
    var result = {};
    if (mod != null) for (var k in mod) if (k !== "default" && Object.prototype.hasOwnProperty.call(mod, k)) __createBinding(result, mod, k);
    __setModuleDefault(result, mod);
    return result;
};
Object.defineProperty(exports, "__esModule", { value: true });
const net = __importStar(require("net"));
const fs = __importStar(require("fs"));
const path = __importStar(require("path"));
const websocket_1 = require("./websocket");
// Create and start a WebSocket server listening on port 3001.
// The callback function handles messages received from clients.
const wsServer = (0, websocket_1.createWebSocketServer)(3001, (msg, conn) => {
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
            }
            else {
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
