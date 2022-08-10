import subprocess
import os

def main():
    # use subprocess.Popen for this
    CFilePath = input("C source file path: ")
    # use OS.listdir for this
    directoryPath = input("Test file directory path: ")
    command = "gcc -Wall -Werror -std=c11 " + CFilePath + " -lm"
    toFindCommand = "find -name *txt"
    runCommand = "./a.out"
    try:
        # compiles code
        subprocess.call(command.split())
    except:
        print("failed to compile your code: ")
        exit()
    
    # these arrays are the relative paths of the test cases
    inputFiles = []
    outputFiles = []
    fileLocations = subprocess.Popen(toFindCommand.strip().split(), stdout=subprocess.PIPE) 
    for line in fileLocations.stdout:
        print(line)
        line = line.decode()
        line = line.strip("\n")
        if ("output.txt" in line):
            outputFiles.append(line)
        elif ("input.txt" in line):
            inputFiles.append(line)
    # goes through all the test cases   
    for i in range(len(inputFiles)):
        out = ""
        file = open(inputFiles[i], 'r')
        # puts the file data into tests.stdout
        tests = subprocess.Popen(runCommand.strip().split(), stdin=file, stdout=subprocess.PIPE)
        # out is the solution that we get from the program
        for line in tests.stdout.readlines():
            out += line.decode()
        isTheSame(out, outputFiles[i])
            
# returns true if they match, false otherwise
def isTheSame(out, outputFile):
    file = open(outputFile, 'r')
    real = ""
    for line in file.readlines():
        real += line
    if (out == real):
        print("#### Test: " + outputFile[8:16] + " passed! ####")
    else:
        print("#### Test: " + outputFile[8:16] + " failed! ####")
        print("#### EXPECTED TO SEE:")
        print(real.strip("\n"))
        print("#### INSTEAD GOT:")
        print(out.strip("\n"))
        
    
if __name__ == "__main__":
    main()
    

