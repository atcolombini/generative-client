
// Wrapper for serialport module

const Serial = new class
{
    constructor()
    {
        // List of known devices
        this.KnownDevices = require('./devices');
        
        // Lines read from the serial port will be placed here constantly
        this.lastLine = null;

        this.SerialPort = require('serialport');
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
        const port = new SerialPort(path, { baudRate: baudRate });
        const lineStream = port.pipe(new Readline({ delimiter: "\r\n" }));
        lineStream.on("data", function(d) { instance.lastLine = d; });
        
        document.title += " - Connected to " + path;
    }

    read()
    {
        

        return this.lastLine;
    }

    write(message)
    {
        const SerialPort = this.SerialPort;
        
    }
}
