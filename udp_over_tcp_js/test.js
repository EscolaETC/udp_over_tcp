const data = Buffer.from([10, 150, 20, 33, 43, 80, 231, 157, 76, 3, 5, -3, 2, 231, 157, 76]);

console.log(data);

var start = 0;
var end = 0;

function send(bytes){
    console.log(bytes);
}

for (var i = 0; i < data.length; i++){
    if (data[i] == 231 && data[i + 1] == 157 && data[i + 2] == 76){
        send(data.subarray(start, end));
        start = end + 3;
    }

    end++;
}