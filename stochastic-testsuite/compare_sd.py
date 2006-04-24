#!/usr/bin/python

import sys
import math
import string

SD_FILE=sys.argv[1]
REFERENCE_SD_FILE=sys.argv[2]
REPEATS=float(sys.argv[3])
OUTFILE=sys.argv[4]

SD=file(SD_FILE,"r").readlines()
REFERENCE_SD=file(REFERENCE_SD_FILE,"r").readlines()

EXIT_STATUS=0

if(len(SD) != len(REFERENCE_SD)):
    print "ERROR: The input files don't have the same number of lines."
    sys.exit(1)

NUMCOLUMNS=len(string.split(SD[1],","))
OUT=file(OUTFILE,"w")
OUT.write(SD[0])
for X in range(2,len(SD)):
    REF_SD_COLS=string.split(REFERENCE_SD[X],",")[1:]
    SD_COLS=string.split(SD[X],",")[1:]
    if(len(SD_COLS) != len(REF_SD_COLS)):
        print "ERROR: Number of columns differs between files at line %d"%(X)
        OUT.close()
        sys.exit(1)
    RESULT=str(X-1)+","
    for Y in range(0,NUMCOLUMNS-1):
       v=0.0
       if(REF_SD_COLS[Y]!=0.0):
         v=(math.pow(float(SD_COLS[Y]),2)/math.pow(float(REF_SD_COLS[Y]),2)-1.0)*math.sqrt(REPEATS/2.0)
       if(math.fabs(v)>=5.0):
          print "ERROR: Value of %f to hight at %s line %d."%(v,SD_FILE,X) 
          EXIT_STATUS=1
       RESULT=string.join([RESULT,str(v)],",")
    RESULT=RESULT+"\n"
    OUT.write(RESULT)   
OUT.close()    
sys.exit(EXIT_STATUS)
