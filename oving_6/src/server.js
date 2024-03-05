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
// Create a WebSocket server
const wsServer = (0, websocket_1.createWebSocketServer)(3001, (msg, conn) => {
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
            }
            else {
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
