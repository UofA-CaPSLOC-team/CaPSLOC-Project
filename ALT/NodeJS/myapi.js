var http = require('http');
var express = require('express');
var net = require('net');
var fs = require('fs');

var CONFIG_FILE_LOCATION = '/opt/CaPSLOC/config.txt';
var SCRIPT_FOLDER_LOCATION = '/opt/CaPSLOC/scripts';
var SOCKET_LOCATION = '/opt/CaPSLOC/socket/socket';


var app = express();
app.use(express.bodyParser());


app.configure(function() {
	app.use(express.favicon());
	app.use(express['static'](__dirname )); 
});

// Route for deleting a script from the ALT
app.delete('/CaPSLOC/Script', function(req, res){
	// Debugging
	console.log('Delete request');
	console.log(req.query.name);
	
	// Delete (unlink) file
	var filename = SCRIPT_FOLDER_LOCATION + '/' + req.query.name + '.xml';
	fs.unlinkSync(filename);
	
	var respBody = getScriptNamesAndTimes();
	res.send(JSON.stringify({success: true, data:respBody}));
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

	var conn = net.createConnection(SOCKET_LOCATION);
	conn.on('connect', function(){
		console.log('connected to unix socket server');
	});

	
	// Send the script location through the socket
	conn.end(filename);
	
	// Send back a success message
	var respBody = getScriptNamesAndTimes();
	var respData = {success: true, data: respBody};
	res.send(JSON.stringify(respData));
});

// Route to get all the scripts without changing them
app.get('/CaPSLOC/Scripts', function(req, res){
	console.log('Received request for scripts');
	// Send back a success message
	var respBody = getScriptNamesAndTimes();
	var respData = {success: true, data: respBody};
	res.send(JSON.stringify(respData));
});

function getScriptNamesAndTimes(){

	var files = fs.readdirSync(SCRIPT_FOLDER_LOCATION);
	var respBody = [];
	
	// More debugging
	console.log(files.length + ' scripts remaining');
	for(var i = 0; i < files.length; i++){
		console.log(files[i]);
		var fileToStat = SCRIPT_FOLDER_LOCATION + '/' + files[i];
		var stats = fs.statSync(fileToStat);
		console.log(stats.mtime);
		respBody.push({Name: files[i], Modified: stats.mtime});
	}
	return respBody;
}

// Route for sending a command to the ALT
app.post('/CaPSLOC/Command', function(req, res){
	// Debugging
	console.log('Command Received:');
	console.log(req.body.command);
	
	var conn = net.createConnection(SOCKET_LOCATION);
	conn.on('connect', function(){
		console.log('connected to unix socket server');
	});
	
	// Send the script location through the socket
	var cmdBuffer = new Buffer(req.body.command);
	conn.end(cmdBuffer);
	
	// Send back a success message
	var respData = {success: true};
	res.send(JSON.stringify(respData));
});

// Express route for getting name of ALT
app.get('/CaPSLOC/Status', function(req, res){
	var contents = fs.readFileSync(CONFIG_FILE_LOCATION, {'encoding': 'utf8'});
	var configItems = contents.split('\n');
	var name = "";
	var ctrlLine = "CTRL=" + req.ip;
	var ctrlFound = false;
	var ctrlChanged = false;
	for(var i = 0; i < configItems.length; i++){
		if(configItems[i].indexOf('Name=') == 0){
			name = configItems[i].substr(5);
		}
		if(configItems[i].indexOf('CTRL=') == 0){
			ctrlFound = true;
			if(configItems[i] != ctrlLine){
				console.log('Updating entry...');
				console.log(ctrlLine);
				configItems[i] = ctrlLine;
				ctrlChanged = true;
			} else {
				console.log('No changes required.');
			}
		}
	}
	if(!ctrlFound){
		console.log('No CTRL address found. Adding a new entry.');
		configItems.push(ctrlLine);
		ctrlChanged = true;
	}

	// Write the changes
	if(ctrlChanged){
		console.log('Modifying the config file...');
		var fd = fs.openSync(CONFIG_FILE_LOCATION, 'w');
		var allConfig = new Buffer(configItems.join('\n'));
		var bytesWritten = fs.writeSync(fd, allConfig, 0, allConfig.length, 0);
		console.log(bytesWritten + ' bytes written');
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


app.listen(80);
console.log('App Server running at port 80');
