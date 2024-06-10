
const dgram = require("dgram");
const net = require('net');

const udp_client = dgram.createSocket('udp4');

var g_socket;

const port = 8000;
const host = '127.0.0.1';

udp_client.on("message", (data, info) => {
    console.log(`Server: Sent ${data.toString().length}`);
    g_socket.write(Buffer.concat([data, Buffer.from([231, 157, 76])]));
})

const tcp_server = net.createServer((socket) => {
    g_socket = socket;

    socket.setNoDelay();

    socket.on('data', (data) => {
        var start = 0;
        var end = 0;
        for (var i = 0; i < data.length; i++){
            if (data[i] == 231 && data[i + 1] == 157 && data[i + 2] == 76){
                //console.log(`Server: Received ${data.subarray(start, end).toString().length}`);
                //131.196.198.196:27069
                udp_client.send(data.subarray(start, end), 27069, '131.196.198.196', function(error){});
                start = end + 3;
            }
            end++;
        }
    });

    socket.on('end', () => {
        console.log('Server: Client Disconnected');
    });
});

tcp_server.on('connection', (socket) => {
    console.log(`Server: ${socket.remoteAddress}:${socket.remotePort} has connected`);
});

tcp_server.on('error', (err) => {
    throw err;
});

tcp_server.on('listening', () => {
    const address = tcp_server.address();
    console.log(`opened server on ${address.address}:${address.port}`);
});

tcp_server.listen(port,host);