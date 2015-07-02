#this program creates a random input for the trains program

#imports

import sys
import random

def process():

    fp = open('input.txt', 'w+')
    
    for i in range(0,100):
        tupple = genRandomTupple()
        fp.write(tupple + '\n')

    fp.close()

def genRandomTupple():
     random.seed()
     x = random.randint(1,4)
     result = ""
     if x == 1:
         result = result + "e"
     if x == 2:
         result = result + "w"
     if x == 3:
         result = result + "E"
     if x == 4:
         result = result + "W"
     
     result = result + ":" + str(random.randint(1,99)) + "," + str(random.randint(1,99))
 
     return result


if __name__ == "__main__":
        
    process()
