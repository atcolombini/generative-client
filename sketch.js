'use strict';

const SerialPort = require('serialport');
const Readline = require('@serialport/parser-readline');

let port;
let data = null;

function setup()
{
	// Create the canvas
	createCanvas(windowWidth, windowHeight);

    // Setup serial port to write lines to data
    port = new SerialPort("COM8", { baudRate: 9600 });
    const lineStream = port.pipe(new Readline({delimiter:"\r\n"}));
	lineStream.on("data", function(d) { data = d; });
}

function draw()
{
	var isOn = true;
	
	if(data != null)
	{
		if(data === "off")
		{
			isOn = false;
		}
	}

	//let fillColor = getFillColor(isOn);

	let fillColor = getFillColor(mouseIsPressed);

	fill(fillColor);
	ellipse(mouseX, mouseY, 80, 80);
}

function windowResized()
{
	resizeCanvas(windowWidth, windowHeight);
}

function getFillColor(mouseIsPressed)
{
	return mouseIsPressed ? 0 : 255;
}