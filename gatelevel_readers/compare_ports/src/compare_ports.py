import os
import sys
import json

def set_ports(rtlPortsFile):
    f = open(rtlPortsFile)
    data = json.load(f)

    ports = data[0]["ports"]
    setPorts = set()
    for port in ports:
        portName = port["name"]
        portDirection = port["direction"]
        #portType = port["type"]
        if "range" in port:
            lsb = port["range"]["lsb"]
            msb = port["range"]["msb"]
            if lsb > msb:
                temp = lsb
                lsb = msb
                msb = temp

            if lsb != msb:
                for idx in range(lsb, msb+1):
                    indexedPort = portName + "[" + str(idx) + "]"
                    portInfo = tuple([indexedPort, portDirection])
                    setPorts.add(portInfo)
            else:
                portInfo = tuple([portName, portDirection])
                setPorts.add(portInfo)
        else:
            if portDirection == "input":
                portDirection = "Input"
            elif portDirection == "output":
                portDirection = "Output"
            elif portDirection == "inout":
                portDirection = "Inout"
            portInfo = tuple([portName, portDirection])
            setPorts.add(portInfo)
    return setPorts

def main():
    if len(sys.argv) == 3:
        rtlPortsFile = os.path.abspath(sys.argv[1])
        postSynthPortsFile = os.path.abspath(sys.argv[2])

        setRTLPorts = set_ports(rtlPortsFile)
        setPostSynthPorts = set_ports(postSynthPortsFile)
        print(setRTLPorts)
        print(setPostSynthPorts)
        if setRTLPorts == setPostSynthPorts:
            print("files are same")
        else:
            print("files are different")

if __name__ == "__main__":

    main()
