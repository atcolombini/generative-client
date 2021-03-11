'use strict';

function setup()
{
	Serial.begin(9600);
	// Create the canvas
	createCanvas(windowWidth, windowHeight);
}

function draw()
{
	let serialData = Serial.read();
	
	let isOn = true;
	
	if(serialData != null)
	{
		if(serialData === "off")
		{
			isOn = false;
		}
	}

	let fillColor = getFillColor(isOn);
	//let fillColor = getFillColor(mouseIsPressed);

	fill(fillColor);
	ellipse(mouseX, mouseY, 80, 80);
}

function windowResized()
{
	resizeCanvas(windowWidth, windowHeight);
}

function getFillColor(mouseIsPressed)
{
	return mouseIsPressed ? 255 : 0;
}
