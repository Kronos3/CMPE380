#!/bin/bash

# Lab14 Exercise B

function parmFun() {
  echo "parmFun got: $1 $2"
}


function secondPart() {
  retnum=999
  echo $retnum
}

function globVar() {
  local var1
  local var2

  printf "(before) var1=$var1 var2=$var2\n"

  var1="dog"
  var2="cat"

  printf "(after) var1=$var1 var2=$var2\n"
}


parmFun 5 7
printf "Second part: $(secondPart)\n"
globVar

