const express = require('express');
const app = express();
const http = require('http');
const { Server } = require("socket.io");
const server = http.createServer(app);
const io = new Server(server);
const fs = require('fs');
// excuse my javascript i only wrote this because socket.io was the best option imo

io.on('connection', (socket) => {
    console.log('a user connected');

    socket.on('message', (msg) => {
        console.log('message:' + msg);
        io.emit('message', msg);
    });

    // Only broadcast because we don't need to know our own location
    socket.on('move', (msg) => socket.broadcast.emit('move', msg));

    socket.on('disconnect', () => console.log('a user disconnected'));
});

ipstring = 'localhost';
if (!fs.existsSync('ip.txt')) fs.writeFileSync("ip.txt", ipstring);
else
    try {
        ipstring = fs.readFileSync('ip.txt', 'utf8');
    }
    catch (err) {
        console.error(err);
    }

port = 4000;
if (!fs.existsSync('port.txt')) fs.writeFileSync("port.txt", '4000');
else
    try {
        port = Number(fs.readFileSync('port.txt', 'utf8'));
    }
    catch (err) {
        console.error(err);
    }

server.listen(port, ipstring);
console.log("listening on " + ipstring + ':' + port);