cd src
if [ "$1" == "-c" ]
    then make clean
elif [ "$1" == "-h" ]
    then
    echo "formats to run:"
    echo "-h for help"
    echo "-c for clean"
    echo "your.txt (int)iterations (out.txt or stdout to print in console) to run. Out file will be made under src dir."
elif [ $# -eq 3 ]
    then
    make
    ./a.out $1 $2 $3
else
  echo "try -h for help"
fi