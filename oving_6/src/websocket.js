"use strict";
// websocket.ts
Object.defineProperty(exports, "__esModule", { value: true });
exports.createWebSocketServer = void 0;
const net_1 = require("net");
const utils_1 = require("./utils");
function createWebSocketServer(port, onMessage) {
    const clients = new Set();
    const server = (0, net_1.createServer)(socket => {
        const connection = {
            socket,
            sendData: (data) => {
                const encoded = (0, utils_1.encodeMessage)(data);
                socket.write(encoded);
            },
            close: () => {
                clients.delete(connection);
                socket.end();
            }
        };
        socket.on('data', buffer => {
            if (buffer.toString().match(/^GET /)) {
                const headers = (0, utils_1.parseHeaders)(buffer.toString());
                const acceptKey = headers.get('sec-websocket-key');
                if (acceptKey) {
                    const acceptValue = (0, utils_1.generateAcceptValue)(acceptKey);
                    socket.write('HTTP/1.1 101 Web Socket Protocol Handshake\r\n' +
                        'Upgrade: WebSocket\r\n' +
                        'Connection: Upgrade\r\n' +
                        'Sec-WebSocket-Accept: ' + acceptValue + '\r\n\r\n');
                    clients.add(connection);
                }
            }
            else {
                const message = (0, utils_1.decodeMessage)(buffer);
                onMessage(message, connection);
            }
        });
        socket.on('end', () => {
            clients.delete(connection);
        });
    });
    server.listen(port);
    return {
        broadcast: (msg) => {
            const encoded = (0, utils_1.encodeMessage)(msg);
            clients.forEach(client => client.socket.write(encoded));
        }
    };
}
exports.createWebSocketServer = createWebSocketServer;
