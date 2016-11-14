from numpy import *
import re

def createVocabList(dataSet):
    vocabSet = set([])  
    for document in dataSet:
        vocabSet = vocabSet | set(document) 
    return list(vocabSet)
def textSplit(Str):
    listOfWord = re.split(r'\W*',Str)
    return [word.lower() for word in listOfWord if len(word) > 2]

def trainNB0(trainMatrix,trainCategory):
    numTrainDocs = len(trainMatrix)
    numWords = len(trainMatrix[0])
    p0Num = ones(numWords); p1Num = ones(numWords) ;p2Num=ones(numWords)     
    p0Denom = 3.0; p1Denom = 3.0; p2Denom=3.0
    p0Doc = 0.0; p1Doc =0.0; p2Doc = 0.0
    for i in range(numTrainDocs):
        if trainCategory[i] == 0:
            p0Num += trainMatrix[i]
            p0Denom += sum(trainMatrix[i])
            p0Doc += 1
        elif trainCategory[i] == 1:
            p1Num += trainMatrix[i]
            p1Denom += sum(trainMatrix[i])
            p1Doc += 1
        elif trainCategory[i] ==2:
            p2Num += trainMatrix[i]
            p2Denom += sum(trainMatrix[i])
            p2Doc += 1
    p0Vect = log(p0Num/p0Denom)
    p1Vect = log(p1Num/p1Denom)
    p2Vect = log(p2Num/p2Denom)

    pC0=p0Doc/float(numTrainDocs)
    pC1=p1Doc/float(numTrainDocs)
    pC2=p2Doc/float(numTrainDocs)

    print p0Doc,p1Doc,p2Doc
    return p0Vect,p1Vect,p2Vect,pC0,pC1,pC2

def classifyNB(vec2Classify, p0Vec, p1Vec,p2Vec,pC0,pC1,pC2):
    p0 = sum(vec2Classify * p0Vec) + log(pC0)
    p1 = sum(vec2Classify * p1Vec) + log(pC1)   
    p2 = sum(vec2Classify * p2Vec) + log(pC2)
    if p1 > p0 and p1 > p2:
        return 1
    elif p2 > p0 and p2 > p1: 
        return 2
    else:
        return 0
    
def bagOfWords2VecMN(vocabList, inputSet):
    returnVec = [0]*len(vocabList)
    for word in inputSet:
        if word in vocabList:
            returnVec[vocabList.index(word)] += 1
    return returnVec
    
def Test():
    docList=[]; classList = []; fullText =[]
    for i in range(1,31):
        wordList = textSplit(open('txt/business/%d.txt' % i).read())
        docList.append(wordList)
        fullText.extend(wordList)
        classList.append(0)
        wordList = textSplit(open('txt/science/%d.txt' % i).read())
        docList.append(wordList)
        fullText.extend(wordList)
        classList.append(1)
        wordList = textSplit(open('txt/sports/%d.txt' % i).read())
        docList.append(wordList)
        fullText.extend(wordList)
        classList.append(2)
    vocabList = createVocabList(docList)
    trainingSet = range(90); testSet=[]         
    for i in range(20):
        randIndex = int(random.uniform(0,len(trainingSet)))
        testSet.append(trainingSet[randIndex])
        del(trainingSet[randIndex])  
    trainMat=[]; trainClasses = []
    for docIndex in trainingSet:
        trainMat.append(bagOfWords2VecMN(vocabList, docList[docIndex]))
        trainClasses.append(classList[docIndex])
    p0V,p1V,p2V,pC0,pC1,pC2 = trainNB0(array(trainMat),array(trainClasses))
    errorCount = 0
    for docIndex in testSet:
        wordVector = bagOfWords2VecMN(vocabList, docList[docIndex])
        if classifyNB(array(wordVector),p0V,p1V,p2V,pC0,pC1,pC2) != classList[docIndex]:
            errorCount += 1
    print 'the error rate is: ',float(errorCount)/len(testSet)
    return float(errorCount)/len(testSet)

def Test10():
    s=0.0
    for i in range(10):
        rate=Test()
        s+=rate
    averageRate=s/10.0
    print 'the average error rate is :' ,averageRate
        
