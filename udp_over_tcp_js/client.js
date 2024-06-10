const dgram = require("dgram");
const net = require('net');

const udp_server = dgram.createSocket('udp4');

////0.tcp.sa.ngrok.io:12523

const port = 12523;
const host = '0.tcp.sa.ngrok.io';

/* const port = 8000;
const host = '127.0.0.1'; */

const tcp_client = new net.Socket();

tcp_client.setNoDelay();

tcp_client.connect(port, host, () => {
    console.log('Client: Connected to Server');
});

udp_server.on("message", (data, info) => {
    //console.log(`Client: Sent ${data.toString().length}`);
    tcp_client.write(Buffer.concat([data, Buffer.from([231, 157, 76])]));
})

tcp_client.on('data', (data) =>{
    var start = 0;
    var end = 0;
    for (var i = 0; i < data.length; i++){
        if (data[i] == 231 && data[i + 1] == 157 && data[i + 2] == 76){
            console.log(`Client: Received ${data.subarray(start, end).toString().length}`);
            udp_server.send(data.subarray(start, end), 27005, '127.0.0.1', function(error){});
            start = end + 3;
        }
        end++;
    }
});

tcp_client.on('close', () => {
    console.log('Client: Disconnected from Server');
});

udp_server.bind(27000);