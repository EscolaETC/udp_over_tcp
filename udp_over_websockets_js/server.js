const datagram = require("dgram");
const express = require("express");
const http = require("http");
const socketio = require("socket.io");

//& "C:\Users\alunoetc\nodejs\node.exe"

const app = express();
const server = http.createServer(app);
const io = new socketio.Server(server);

var g_socket;

const clients = {}

io.on("connection", (socket) => {
    console.log("Client connected", socket.id);
    clients[socket.id] = datagram.createSocket("udp4");
    g_socket = socket;
    socket.on("game_client", (data) => {
        //console.log("Received from game client", data);
        clients[socket.id].send(data, 27015, "192.168.2.145", (error, bytes) => {});
    })
    clients[socket.id].on("message", (data, info) => {
        //console.log("Received from game server", data.toString());
        socket.emit("game_server", data);
    });
});

server.listen(8000, () => {
    console.log("Server is running on port 8000");
})
