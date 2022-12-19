# import the os library: useful to get the absolute path to the input file
import os
from os.path import exists
import sys
import csv
from pathlib import Path
# import this library
import blifparser.blifparser as blifparser

def model_(blif):
    model = blif.model.name
    return model

def inputs_(blif):
    inputs = blif.inputs.inputs
    return inputs

def outputs_(blif):
    outputs = blif.outputs.outputs
    return outputs

def subckts_(blif):
    subckts = blif.subcircuits
    return subckts

def names_(blif):    
    names = blif.booleanfunctions
    return names
 
def namesSet(names, listFalse, listTrue):
    setNames = set()
    for i in names:
        if isinstance(i.inputs, str):
            setInputs = tuple(i.inputs)
        elif i.inputs:
            k = 0
            while k < len(i.inputs):
                if i.inputs[k] in listFalse:
                    i.inputs[k] = "$false"
                if i.inputs[k] in listTrue:
                    i.inputs[k] = "$true"
                k += 1 
            setInputs = tuple(i.inputs)
        setOutputs = i.output
        if i.truthtable:
            truthTable = []
            for j in i.truthtable:
                row = tuple(j)
                truthTable.append(row)
            truthTable = tuple(truthTable)
            
        if i.inputs and i.truthtable:
            setName = tuple([setInputs, setOutputs, truthTable]) 
            setNames.add(setName)
        elif i.truthtable:
            setName = tuple([setOutputs, truthTable])
            setNames.add(setName)
        else:
            setName = setOutputs
            setNames.add(setName)
    return setNames

def compareNames(names, names1, listFalse, listTrue):
    setNames = namesSet(names, listFalse, listTrue)
    setNames1 = namesSet(names1, listFalse, listTrue)
    return True if(setNames == setNames1) else False

def subcktsSet(subckts, listFalse, listTrue):
    subcircuits = set()
    for i in subckts:
        modelName = i.modelname
        j = 0
        while j < len(i.params):
            splitConnection = i.params[j].split("=")
            wireName = splitConnection[-1]
            if splitConnection[-1] in listFalse:
                splitConnection[-1] = "$false"
                i.params[j] = "=".join(splitConnection)
            elif splitConnection[-1] in listTrue:
                splitConnection[-1] = "$true"
                i.params[j] = "=".join(splitConnection)
            j += 1
        connections = frozenset(i.params)
        subcircuit = tuple([modelName, connections])
        subcircuits.add(subcircuit)
    return subcircuits

def compareSubckts(subckts, subckts1, listFalse, listTrue):
    setSubckts = subcktsSet(subckts, listFalse, listTrue)
    setSubckts1 = subcktsSet(subckts1, listFalse, listTrue)
    return True if(setSubckts == setSubckts1) else False

def connectedToFalse(names):
    listFalse = []
    for i in names:
        if i.inputs == ["$false"]:
            listFalse.append(i.output)
    return listFalse

def connectedToTrue(names):
    listTrue = []
    for i in names:
        if i.inputs == ["$true"]:
            listTrue.append(i.output)
    return listTrue

def compare(blif, blif1):
    model = model_(blif)
    model1 = model_(blif1)
    inputs = inputs_(blif)
    inputs1 = inputs_(blif1)
    outputs = outputs_(blif)
    outputs1 = outputs_(blif1)
    names = names_(blif)
    names1 = names_(blif1)
    subckts = subckts_(blif)
    subckts1 = subckts_(blif1)

    listFalse = connectedToFalse(names)
    listFalse1 = connectedToFalse(names1)
    sameFalse = True if set(listFalse) == set(listFalse1) else False

    listTrue = connectedToTrue(names)
    listTrue1 = connectedToTrue(names1)
    sameTrue = True if set(listTrue) == set(listTrue1) else False

    sameModel = True if model == model1 else False
    sameInputs = True if set(inputs) == set(inputs1) else False 
    sameOutputs = True if set(outputs) == set(outputs1) else False  
    sameNames = True if (sameFalse and sameTrue and compareNames(names, names1, listFalse, listTrue)) else False
    sameSubckts = True if (sameFalse and sameTrue and compareSubckts(subckts, subckts1, listFalse, listTrue)) else False 
    return sameModel, sameInputs, sameOutputs, sameNames, sameSubckts

def appendCSV(csvFile, data):
    with open(csvFile, 'a', newline='') as f:
        writer = csv.writer(f)
        writer.writerow(data)
    f.close()

def genReport(outDir, result, fileName, fileName1):
    reportFile = "{}/report.csv".format(outDir)
    keys = ["file1", "file2", "Model", "Inputs", "Outputs", "Names", "Sub circuits", "File"]
    isSame = result[0] and result[1] and result[2] and result[3] and result[4]
    data = fileName, fileName1, result[0], result[1], result[2], result[3], result[4], isSame
    if not exists(reportFile):
        appendCSV(reportFile, keys)
    appendCSV(reportFile, data)
    if not isSame:
        print("Error : " + fileName + " and " + fileName1 + " are different")
        sys.exit(1)

def main():
    if len(sys.argv) == 4:
        filePath = os.path.abspath(sys.argv[1])
        blif = blifparser.BlifParser(filePath).blif
        filePath1 = os.path.abspath(sys.argv[2])
        blif1 = blifparser.BlifParser(filePath1).blif
        outDir = os.path.abspath(sys.argv[3])

        result = compare(blif, blif1)
        fileName = filePath.split("/")
        fileName = fileName[-1]
        fileName1 = filePath1.split("/")
        fileName1 = fileName1[-1]
        genReport(outDir, result, fileName, fileName1)
    else:
        print("blifparser expects exactly three parameters: two input BLIF files  and output directory path")

if __name__ == "__main__":

    main()