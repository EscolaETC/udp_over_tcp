const datagram = require("dgram");

const server = datagram.createSocket("udp4");

server.on("message", (data, info) => {
    console.log(info.port, info.address);
    server.send("Success", info.port, "127.0.0.1", (error, bytes) => {
        console.log(error, bytes);
    });
});

server.bind(28000);