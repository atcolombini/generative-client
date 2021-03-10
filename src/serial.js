
//Serial Port Setup

// Lines read from the serial port will be placed here constantly
let serialData = null;

async function serialBegin()
{
    const SerialPort = require('serialport');
    const Readline = require('@serialport/parser-readline');
    const KnownDevices = require('./devices');

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
                    console.log("Device with serial " + p.serialNumber + " connected in port " + p.path);
                    
                    if (KnownDevices.includes(p.serialNumber))
                    {
                        path = p.path;
                    }
                }
            }
        ),
        err => console.error(err)
    )

    if (path == null)
    {
        console.error("Couldn't find any known device connected.");
        return;
    }

    // Setup serial port to read from the device and write lines to serialData
    const port = new SerialPort(path, { baudRate: 9600 });
    const lineStream = port.pipe(new Readline({ delimiter: "\r\n" }));
    lineStream.on("data", function(d){ serialData = d; });
    
    document.title += " - Connected to " + path;
};
