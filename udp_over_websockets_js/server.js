const datagram = require("dgram");
const express = require("express");
const http = require("http");
const socketio = require("socket.io");

const client = datagram.createSocket("udp4");
const app = express();
const server = http.createServer(app);
const io = new socketio.Server(server);

var g_socket;

io.on("connection", (socket) => {
    console.log("Client connected");
    g_socket = socket;
    socket.on("game_client", (data) => {
        //console.log("Received from game client", data);
        client.send(data, 27015, "192.168.100.44", (error, bytes) => {});
    })
});

client.on("message", (data, info) => {
    //console.log("Received from game server", data.toString());
    g_socket.emit("game_server", data);
});

server.listen(8000, () => {
    console.log("Server is running on port 8000");
})