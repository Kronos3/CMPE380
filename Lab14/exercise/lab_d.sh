#!/bin/bash
#----------------------------------------------------------------------------
# Complex function exercise
#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
#A function to get the list of files in the current directory
#----------------------------------------------------------------------------
getFiles(){
  myFiles="`ls *.*`"
}

#----------------------------------------------------------------------------
#A function to count # of lines.
#It expects file name as argument
#----------------------------------------------------------------------------
getLineCount() {
  local fileParam=$1  # set file name from argument
  currentLineCount=`wc -l $fileParam | sed 's/^\([0-9]*\).*$/\1/'` # number of lines
}

#----------------------------------------------------------------------------
# The main code
#----------------------------------------------------------------------------
#initialize variables!
currentLineCount=0
fileCount=0
totalLines=0


printf  "\nThis $0 script counts the # of lines in all files in:\n   $(pwd)!\n" 


getFiles # call function to get a list of files

#for each file aFile myFiles
for aFile in $myFiles
do	
   getLineCount $aFile #call function to get line count for aFile
   printf  "$aFile\t--->\t$currentLineCount\n"	
   fileCount=$[ $fileCount + 1 ]
   totalLines=$[ $totalLines + $currentLineCount ]
done

printf  "Total files: $fileCount, Total lines:$totalLines\n"