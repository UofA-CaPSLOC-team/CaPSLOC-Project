var http = require('http');
var express = require('express');
var net = require('net');
var fs = require('fs');

var CONFIG_FILE_LOCATION = '/opt/CaPSLOC/config.txt';
var SCRIPT_FOLDER_LOCATION = '/opt/CaPSLOC/scripts';
var SOCKET_LOCATION = '/opt/CaPSLOC/socket/socket';

var conn = net.createConnection(SOCKET_LOCATION);
conn.on('connect', function(){
	console.log('connected to unix socket server');
});
conn.on('data', function(data){
	console.log('data returned from socket:');
	console.log(data.toString());
});

var app = express();
app.use(express.bodyParser());


app.configure(function() {
	app.use(express.favicon());
	app.use(express['static'](__dirname )); 
});

// Route for sending a script to the ALT
app.post('/CaPSLOC/Script', function(req, res){
	// Debugging
	console.log('Script Received:');
	console.log(req.body.name);
	console.log(req.body.script);
	
	// Create the file (will be overwritten if exists)
	var filename = SCRIPT_FOLDER_LOCATION + '/' + req.body.name + '.xml';
	var fd = fs.openSync(filename, 'w');
	var scriptBuffer = new Buffer(req.body.script);
	fs.writeSync(fd, scriptBuffer, 0, scriptBuffer.length, 0);
	
	// Send the script location through the socket
	conn.write(filename);
	
	// Send back a success message
	var respData = {success: true};
	res.send(JSON.stringify(respData));
});

// Route for sending a command to the ALT
app.post('/CaPSLOC/Command', function(req, res){
	// Debugging
	console.log('Command Received:');
	console.log(req.body.command);
		
	// Send the script location through the socket
	var cmdBuffer = new Buffer(req.body.command);
	conn.write(cmdBuffer);
	
	// Send back a success message
	var respData = {success: true};
	res.send(JSON.stringify(respData));
});

// Express route for getting name of ALT
app.get('/CaPSLOC/Status', function(req, res){
	var contents = fs.readFileSync(CONFIG_FILE_LOCATION, {'encoding': 'utf8'});
	var configItems = contents.split('\n');
	var name = "";
	for(var i = 0; i < configItems.length; i++){
		if(configItems[i].indexOf('Name=') == 0){
			name = configItems[i].substr(5);
			break;
		}
	}
	res.send(name);
}); 

// Express route for any other unrecognised incoming requests
app.get('*', function(req, res){
 	res.send('Unrecognised API call', 404);
});

// Express route to handle errors
app.use(function(err, req, res, next){
	if (req.xhr) {
 		res.send(500, 'Oops, Something went wrong!');
	} else {
 		next(err);
	}
});


app.listen(3000);
console.log('App Server running at port 3000');
