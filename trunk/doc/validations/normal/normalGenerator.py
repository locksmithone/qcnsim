import numpy
#import scipy
import matplotlib.pyplot
import matplotlib.mlab
import math

def normalGenerator(mean, stddev, start, end, step):
    '''
    Generates Normal sample lists per the parameters.
    Returns two lists of X and Y values distributed per Normal. 
    '''
    
    normalSamplesY = []
    normalSamplesX = []
    for i in numpy.arange(start, end, step):
        normalSamplesY.append(matplotlib.mlab.normpdf(i, mean, stddev))
        normalSamplesX.append(i)
    return normalSamplesX, normalSamplesY
    
def readValuesFromFile(filename):
    '''
    Reads values from a file and returns a list of floats.
    '''
    yValues = [] # Y values to be read from file.
    fileHandle = open(filename, 'r') # Opens file for reading.
    #yValues= list(fileHandle) # Read all values into yValues.
    for line in fileHandle:
        yValues.append(float(line.rstrip()))
    fileHandle.close()
    return yValues
    
# Now construct a map of parameters per Normal samples.
# Key is filename with samples, value is list of lists:
# list1 is normalGenerator parameters to generate a Normal graph from matplotlib,
# list2 is set of parameters to plot the samples from the filename.
normalParameterMap = {
    'normal_mean0_var0.2.csv': [[0.0,0.2,-1.5,1.5,.01], [-1.5,1.5,0.0,1.0]],
    'normal_mean0_var0.5.csv': [[0.0,0.5,-3.0,3.0,.01], [-3.0,3.0,0.0,0.6]],
    'normal_mean0_var1.0.csv': [[0.0,1.0,-3.0,3.0,.01], [-3.0,3.0,0.0,0.5]],
    'normal_mean0_var5.0.csv': [[0.0,5.0,-5.0,5.0,.01], [-5.0,5.0,0.0,0.2]]
}

# Iterate through dictionary and generate graphs.
for filename, parameters in normalParameterMap.items():
    normalSamplesX, normalSamplesY = normalGenerator(parameters[0][0], math.sqrt(parameters[0][1]),
                                                        parameters[0][2], parameters[0][3],
                                                        parameters[0][4])
    print("Parameters: ", parameters)
    matplotlib.pyplot.figure()
    matplotlib.pyplot.plot(normalSamplesX, normalSamplesY)
    matplotlib.pyplot.axis(parameters[1])    
    matplotlib.pyplot.grid(True)
    matplotlib.pyplot.xlabel("x values")
    matplotlib.pyplot.ylabel("Probability")
    matplotlib.pyplot.title('Normal pdf: ' + str(parameters[0]))
    matplotlib.pyplot.savefig(filename + '_pdf.png')
    matplotlib.pyplot.show()
    #matplotlib.pyplot.close()
    ySamples = readValuesFromFile(filename)
    matplotlib.pyplot.figure()
    # Histogram with limited range for normalization.
    #matplotlib.pyplot.hist(ySamples, bins=100, range=(parameters[1][0], parameters[1][1]), 
                                                       #normed=True, color='r')
    # Histogram with full range for normalization.
    # Found that normal curve with lambda=0.5 is ill-formed if full range is not used.
    matplotlib.pyplot.hist(ySamples, bins=500, normed=True, color='r')
    matplotlib.pyplot.axis(parameters[1])
    matplotlib.pyplot.grid(True)
    matplotlib.pyplot.xlabel("x values")
    matplotlib.pyplot.ylabel("Probability")
    matplotlib.pyplot.title('Samples from ' + filename)
    matplotlib.pyplot.savefig(filename + '_sample.png')
    #matplotlib.pyplot.yticks(numpy.arange(parameters[1][2], parameters[1][3], 0.05))
    matplotlib.pyplot.show()
    #matplotlib.pyplot.close()
print("*** Done! ***")
    
    

##print (normalGenerator(1.0,5.0,0.0,3.0,.001))
#normalSamplesX, normalSamplesY = normalGenerator(1.0,5.0,0.0,3.0,.001)
#matplotlib.pyplot.figure()
##matplotlib.pyplot.hist(normalSamplesY, 100)
#matplotlib.pyplot.plot(normalSamplesX, normalSamplesY)
#matplotlib.pyplot.grid(True)
#matplotlib.pyplot.show()
##matplotlib.pyplot.close()
#print("Done Figure 1.")
#ySamples = readValuesFromFile('normal_Scale1.0_Shape5.0.csv')
#print("Done reading.")
##print (ySamples)
#matplotlib.pyplot.figure()
#matplotlib.pyplot.hist(ySamples, 500)
#matplotlib.pyplot.axis([0.0,3.0,0.0,650])
#matplotlib.pyplot.grid(True)
#matplotlib.pyplot.show()
##matplotlib.pyplot.close()
#print("Done Figure 2.")
