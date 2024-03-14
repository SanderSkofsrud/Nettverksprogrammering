import { Socket } from 'net';

/**
 * Defines the structure for a WebSocket connection within the server.
 * This interface includes the underlying TCP socket connection, a method to
 * send data through the WebSocket, and a method to close the WebSocket connection.
 */
export interface WebSocketConnection {
    // The underlying TCP socket for the WebSocket connection.
    socket: Socket;

    // Sends data over the WebSocket connection.
    sendData: (data: string) => void;

    // Closes the WebSocket connection gracefully.
    close: () => void;
}
