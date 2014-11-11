import numpy
#import scipy
import matplotlib.pyplot
import math

def exponentialGenerator(lambda_, start, end, step):
    '''
    Generates Weibull sample lists per the parameters.
    Returns two lists of X and Y values distributed per Weibull.
    '''
    
    exponentialSamplesY = []
    exponentialSamplesX = []
    for i in numpy.arange(start, end, step):
        exponentialSamplesY.append(lambda_*math.exp(-lambda_*i))
        exponentialSamplesX.append(i)
    return exponentialSamplesX, exponentialSamplesY
    
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
    
# Now construct a map of parameters per Weibull samples.
# Key is filename with samples, value is list of lists:
# list1 is exponentialGenerator parameters to generate a Weibull graph from matplotlib,
# list2 is set of parameters to plot the samples from the filename.
exponentialParameterMap = {
    'exponential_Lambda0.5.csv': [[0.5,0.0,3.0,.01], [0.0,3.0,0.0,0.5]],
    'exponential_Lambda1.0.csv': [[1.0,0.0,3.0,.01], [0.0,3.0,0.0,1.0]],
    'exponential_Lambda1.5.csv': [[1.5,0.0,3.0,.01], [0.0,3.0,0.0,1.5]],
    'exponential_Lambda3.0.csv': [[3.0,0.0,3.0,.01], [0.0,3.0,0.0,3.0]]
}

# Iterate through dictionary and generate graphs.
for filename, parameters in exponentialParameterMap.items():
    exponentialSamplesX, exponentialSamplesY = exponentialGenerator(parameters[0][0], parameters[0][1],
                                                        parameters[0][2], parameters[0][3])
    print("Parameters: ", parameters)
    matplotlib.pyplot.figure()
    matplotlib.pyplot.plot(exponentialSamplesX, exponentialSamplesY)
    matplotlib.pyplot.axis(parameters[1])    
    matplotlib.pyplot.grid(True)
    matplotlib.pyplot.xlabel("x values")
    matplotlib.pyplot.ylabel("Probability")
    matplotlib.pyplot.title('Exponential pdf: ' + str(parameters[0]))
    matplotlib.pyplot.savefig(filename + '_pdf.png')
    matplotlib.pyplot.show()
    #matplotlib.pyplot.close()
    ySamples = readValuesFromFile(filename)
    matplotlib.pyplot.figure()
    # Histogram with limited range for normalization.
    #matplotlib.pyplot.hist(ySamples, bins=100, range=(parameters[1][0], parameters[1][1]), 
                                                       #normed=True, color='r')
    # Histogram with full range for normalization.
    # Found that exponential curve with lambda=0.5 is ill-formed if full range is not used.
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
    
    

##print (exponentialGenerator(1.0,5.0,0.0,3.0,.001))
#exponentialSamplesX, exponentialSamplesY = exponentialGenerator(1.0,5.0,0.0,3.0,.001)
#matplotlib.pyplot.figure()
##matplotlib.pyplot.hist(exponentialSamplesY, 100)
#matplotlib.pyplot.plot(exponentialSamplesX, exponentialSamplesY)
#matplotlib.pyplot.grid(True)
#matplotlib.pyplot.show()
##matplotlib.pyplot.close()
#print("Done Figure 1.")
#ySamples = readValuesFromFile('exponential_Scale1.0_Shape5.0.csv')
#print("Done reading.")
##print (ySamples)
#matplotlib.pyplot.figure()
#matplotlib.pyplot.hist(ySamples, 500)
#matplotlib.pyplot.axis([0.0,3.0,0.0,650])
#matplotlib.pyplot.grid(True)
#matplotlib.pyplot.show()
##matplotlib.pyplot.close()
#print("Done Figure 2.")
