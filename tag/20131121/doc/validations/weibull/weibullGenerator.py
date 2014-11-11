import numpy
#import scipy
import matplotlib.pyplot
import math

def weibullGenerator(scale, shape, start, end, step):
    '''
    Generates Weibull sample lists per the parameters.
    Returns two lists of X and Y values distributed per Weibull.
    '''
    
    weibullSamplesY = []
    weibullSamplesX = []
    for i in numpy.arange(start, end, step):
        weibullSamplesY.append((shape/scale)*((i/scale)**(shape-1.0))*(math.exp(-(i/scale)**shape)))
        weibullSamplesX.append(i)
    return weibullSamplesX, weibullSamplesY
    
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
# list1 is weibullGenerator parameters to generate a Weibull graph from matplotlib,
# list2 is set of parameters to plot the samples from the filename.
weibullParameterMap = {
    'weibull_Scale1.0_Shape5.0.csv': [[1.0,5.0,0.0,3.0,.01], [0.0,3.0,0.0,2.0]],
    'weibull_Scale1.0_Shape1.0.csv': [[1.0,1.0,0.0,3.0,.01], [0.0,3.0,0.0,1.0]],
    'weibull_Scale1.0_Shape2.0.csv': [[1.0,2.0,0.0,3.0,.01], [0.0,3.0,0.0,0.9]],
    'weibull_Scale1.0_Shape0.5.csv': [[1.0,0.5,0.0,3.0,.01], [0.0,3.0,0.0,5.0]]
}

# Iterate through dictionary and generate graphs.
for filename, parameters in weibullParameterMap.items():
    weibullSamplesX, weibullSamplesY = weibullGenerator(parameters[0][0], parameters[0][1],
                                                        parameters[0][2], parameters[0][3],
                                                        parameters[0][4])
    print("Parameters: ", parameters)
    matplotlib.pyplot.figure()
    matplotlib.pyplot.plot(weibullSamplesX, weibullSamplesY)
    matplotlib.pyplot.grid(True)
    matplotlib.pyplot.xlabel("x values")
    matplotlib.pyplot.ylabel("Probability")
    matplotlib.pyplot.title('Weibull pdf: ' + str(parameters[0]))
    matplotlib.pyplot.savefig(filename + '_pdf.png')
    matplotlib.pyplot.show()
    #matplotlib.pyplot.close()
    ySamples = readValuesFromFile(filename)
    matplotlib.pyplot.figure()
    matplotlib.pyplot.hist(ySamples, bins=300, range=(parameters[1][0], parameters[1][1]), 
                                                       normed=True, color='r')
    matplotlib.pyplot.axis(parameters[1])
    matplotlib.pyplot.grid(True)
    matplotlib.pyplot.xlabel("x values")
    matplotlib.pyplot.ylabel("Probability")
    matplotlib.pyplot.title('Samples from ' + filename)
    matplotlib.pyplot.savefig(filename + '_sample.png')
    matplotlib.pyplot.show()
    #matplotlib.pyplot.close()
print("*** Done! ***")
    
    

##print (weibullGenerator(1.0,5.0,0.0,3.0,.001))
#weibullSamplesX, weibullSamplesY = weibullGenerator(1.0,5.0,0.0,3.0,.001)
#matplotlib.pyplot.figure()
##matplotlib.pyplot.hist(weibullSamplesY, 100)
#matplotlib.pyplot.plot(weibullSamplesX, weibullSamplesY)
#matplotlib.pyplot.grid(True)
#matplotlib.pyplot.show()
##matplotlib.pyplot.close()
#print("Done Figure 1.")
#ySamples = readValuesFromFile('weibull_Scale1.0_Shape5.0.csv')
#print("Done reading.")
##print (ySamples)
#matplotlib.pyplot.figure()
#matplotlib.pyplot.hist(ySamples, 500)
#matplotlib.pyplot.axis([0.0,3.0,0.0,650])
#matplotlib.pyplot.grid(True)
#matplotlib.pyplot.show()
##matplotlib.pyplot.close()
#print("Done Figure 2.")
