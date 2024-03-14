"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
exports.encodeMessage = exports.decodeMessage = exports.parseHeaders = exports.generateAcceptValue = void 0;
const crypto_1 = require("crypto");
/**
 * Generates a Sec-WebSocket-Accept value for a given Sec-WebSocket-Key.
 * The function follows the WebSocket Protocol by concatenating the client's
 * Sec-WebSocket-Key with a specific UUID, hashing the result using SHA1, and
 * then encoding it in base64. This is part of the WebSocket handshake process.
 *
 * @param acceptKey The client's Sec-WebSocket-Key header value.
 * @returns The computed Sec-WebSocket-Accept header value.
 */
function generateAcceptValue(acceptKey) {
    return (0, crypto_1.createHash)('sha1').update(acceptKey + '258EAFA5-E914-47DA-95CA-C5AB0DC85B11', 'binary').digest('base64');
}
exports.generateAcceptValue = generateAcceptValue;
/**
 * Parses the headers from an HTTP request string.
 * The function splits the request string into lines, then splits each header
 * line by ": " to separate the header name from its value, and stores them in a Map.
 *
 * @param request The full HTTP request string.
 * @returns A Map object containing header names and values.
 */
function parseHeaders(request) {
    const headers = new Map();
    const lines = request.split('\r\n'); // Split the request into lines
    for (let i = 1; i < lines.length - 2; i++) {
        const [key, value] = lines[i].split(': '); // Split each line into key and value.
        headers.set(key.toLowerCase(), value); // Add them to the map, converting key to lowercase.
    }
    return headers;
}
exports.parseHeaders = parseHeaders;
/**
 * Decodes a masked WebSocket frame into a string.
 * According to the WebSocket protocol, data from the client is masked.
 * This function uses the masking key to decode the payload data.
 *
 * @param buffer The received raw buffer from the WebSocket frame.
 * @returns The decoded message as a string.
 */
function decodeMessage(buffer) {
    let secondByte = buffer[1];
    let length = secondByte & 127; // Extract the payload length using bitwise AND with 127.
    let maskStart = 2; // Start of the masking key.
    let dataStart = maskStart + 4; // Start of the data payload.
    let data = buffer.slice(dataStart); // Extract the data payload.
    for (let i = 0; i < data.length; i++) {
        // Decode each byte of the data by XORing it with the masking key.
        data[i] ^= buffer[maskStart + (i % 4)];
    }
    return data.toString(); // Convert the decoded bytes to a string.
}
exports.decodeMessage = decodeMessage;
/**
 * Encodes a message into a WebSocket frame format to be sent to the client.
 * This method prepares a simple unmasked text frame. Only supports payloads
 * less than 126 bytes (for simplicity).
 *
 * @param msg The message string to be encoded.
 * @returns The message encoded as a Buffer in WebSocket frame format.
 */
function encodeMessage(msg) {
    let msgBuffer = Buffer.from(msg); // Convert the message to a Buffer.
    let length = msgBuffer.length; // Calculate the length of the message.
    let buffer = Buffer.alloc(2 + length); // Allocate a buffer for the frame.
    buffer[0] = 0x81; // Sets the FIN flag and text frame opcode
    buffer[1] = length; // Assumes payload is less than 126 bytes
    for (let i = 0; i < length; i++) {
        buffer[i + 2] = msgBuffer[i]; // Copies the message into the buffer
    }
    return buffer;
}
exports.encodeMessage = encodeMessage;
