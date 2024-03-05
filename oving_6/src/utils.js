"use strict";
// utils.ts
Object.defineProperty(exports, "__esModule", { value: true });
exports.encodeMessage = exports.decodeMessage = exports.parseHeaders = exports.generateAcceptValue = void 0;
const crypto_1 = require("crypto");
function generateAcceptValue(acceptKey) {
    return (0, crypto_1.createHash)('sha1').update(acceptKey + '258EAFA5-E914-47DA-95CA-C5AB0DC85B11', 'binary').digest('base64');
}
exports.generateAcceptValue = generateAcceptValue;
function parseHeaders(request) {
    const headers = new Map();
    const lines = request.split('\r\n');
    for (let i = 1; i < lines.length - 2; i++) {
        const [key, value] = lines[i].split(': ');
        headers.set(key.toLowerCase(), value);
    }
    return headers;
}
exports.parseHeaders = parseHeaders;
function decodeMessage(buffer) {
    let secondByte = buffer[1];
    let length = secondByte & 127;
    let maskStart = 2;
    let dataStart = maskStart + 4;
    let data = buffer.slice(dataStart);
    for (let i = 0; i < data.length; i++) {
        data[i] ^= buffer[maskStart + (i % 4)];
    }
    return data.toString();
}
exports.decodeMessage = decodeMessage;
function encodeMessage(msg) {
    let msgBuffer = Buffer.from(msg);
    let length = msgBuffer.length;
    let buffer = Buffer.alloc(2 + length);
    buffer[0] = 0x81; // FIN bit set, text frame
    buffer[1] = length;
    for (let i = 0; i < length; i++) {
        buffer[i + 2] = msgBuffer[i];
    }
    return buffer;
}
exports.encodeMessage = encodeMessage;
