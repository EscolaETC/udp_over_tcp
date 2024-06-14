const datagram = require("dgram");
const socketioclient = require("socket.io-client");

const server = datagram.createSocket('udp4');
const socket = socketioclient.io("https://beca-131-72-222-154.ngrok-free.app"); 

server.on("message", (data, info) => {
    //console.log("Received from game client", data);
    socket.emit("game_client", data);
})

socket.on("game_server", (data) => {
    //console.log("Received from game server", data);
    server.send(data, 27005, "localhost", (error, bytes) => {});
})

server.bind(27000);
