const dgram = require("dgram");

const server = dgram.createSocket('udp4');

server.on('error',function(error){
    console.log('Error: ' + error);
    server.close();
});

server.on('message',function(msg, info){
    console.log('Data received from client : ' + msg.toString());
    console.log('Received %d bytes from %s:%d\n', msg.length, info.address, info.port);

    if (info.port == 27015){
        server.send(msg, 27005, '127.0.0.1', function(error){
            if(error){
                client.close();
            }else{
                console.log('Data sent !!!');
            }
        });
    } else {
        server.send(msg, 27015, '192.168.100.44', function(error){
            if(error){
                client.close();
            }else{
                console.log('Data sent !!!');
            }
        });
    }
});

server.on('listening',function(){
    var address = server.address();
    var port = address.port;
    var family = address.family;
    var ipaddr = address.address;
    console.log('Server is listening at port: ' + port);
    console.log('Server ip: ' + ipaddr);
    console.log('Server is IP4/IP6: ' + family);
});

server.on('close', function(){
    console.log('Socket is closed !');
});

server.bind(27000);