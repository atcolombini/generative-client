/*
 * Generative Artifact
 * Copyright (C) 2021 Vicent Ramirez
 * Copyright (C) 2021 Alex T. Colombini
 *
 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of  MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 */

// Wrapper for serialport module and Input 

const { read } = require('original-fs');

let Artifact = {};

const Serial = new class
{
    constructor()
    {
        // List of known devices
        this.KnownDevices = require('./devices');
        
        // Lines read from the serial port will be placed here constantly
        this.lastLine = null;

        this.SerialPort = require('serialport');

        // Stores the open port to read or write from it
        this.port = null;
    }

    async begin(baudRate)
    {
        const SerialPort = this.SerialPort;
        const Readline = require('@serialport/parser-readline');

        let instance = this;
        let path = null;
        
        // Look for a known device in the serial ports
        await SerialPort.list().then
        (
            ports => ports.forEach
            (
                function(p,index)
                {
                    if (p.serialNumber != null)
                    {
                        let msg = " device with serial " + p.serialNumber + " connected in port " + p.path;
                        
                        if (instance.KnownDevices.includes(p.serialNumber))
                        {
                            path = p.path;
                            console.log("Knwon" + msg);
                        }
                        else
                        {
                            console.log("Unknwon" + msg);
                        }
                    }
                }
            ),
            err => console.error(err)
        )
    
        // If no known devices are connected, notify the user and leave
        if (path == null)
        {
            let str = "Couldn't find any known device connected."
            document.title += " - " + str;
            console.error(str);
            return;
        }
    
        // Setup serial port to read from the device and write lines to serialData
        this.port = new SerialPort(path, { baudRate: baudRate });
        const lineStream = this.port.pipe(new Readline({ delimiter: "\r\n" }));
        lineStream.on("data", function(d)
            {
                instance.lastLine = d;
            });
        
        document.title += " - Connected to " + path;
    }

    /*
     * Reads the data from the serial input to the Artifact
     * The read order matters because the string received is formatted in
     * this order:
     *  - photocell
     *  - sound sensor
     *  - red switch
     *  - green switch
     *  - blue switch
     *  - encoder value
     *  - distance value
     *  - distance switch
     *  - keypad pressed button flags
     */
    read()
    {
        if(this.lastLine == null)
        {
            return;
        }

        let values = this.lastLine.split(",");
        let i;

        Artifact.photocell = parseFloat(values[0]);
        Artifact.sound = parseInt(values[1]);
        
        Artifact.red = values[2] == "1" ? true : false;
        Artifact.green = values[3] == "1" ? true : false;
        Artifact.blue = values[4] == "1" ? true : false;

        Artifact.encoder = parseInt(values[5]);

        Artifact.distance = parseFloat(values[6]);
        Artifact.distanceActive = values[7] == "1" ? true : false;

        Artifact.keypad = parseInt(values[8]);
    }

    /* Sends data to the serial port for the Artifact to update
     * the state of the feedback outputs thet need it.
     */
    write()
    {
        if(this.port == null)
        {
            return;
        }

        let payload = "";

        payload += ArtifactOutput.color + ",";
        payload += (ArtifactOutput.isShapeSelected ? 1 : 0);

        //console.log(payload);
        this.port.write(payload);
    }
}

const Key = 
{
    K1: 1 << 0,
    K2: 1 << 1,
    K3: 1 << 2,
    K4: 1 << 3,
    K5: 1 << 4,
    K6: 1 << 5,
    K7: 1 << 6,
    K8: 1 << 7,
    K9: 1 << 8,
    K10: 1 << 9,
    K11: 1 << 10,
    K12: 1 << 11,
    K13: 1 << 12,
    K14: 1 << 13,
    K15: 1 << 14,
    K16: 1 << 15
}

const Color = 
{
    RED: 0,
    GREEN: 1,
    BLUE: 2,
    MAGENTA: 3,
    CYAN: 4,
    YELLOW: 5,
    WHITE: 6,
    NONE: 7
};

let ArtifactOutput =
{
    color: Color.NONE,
    isShapeSelected: false,
}
