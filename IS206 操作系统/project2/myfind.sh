if [ $# -lt 1 ]
then
   echo "The number of parameter cannot be less than 1."
   exit
fi

if [ $# -gt 2 ]
then
   echo "The number of parameter cannot be more than 2."
   exit
fi

if [ $# -eq 1 ]
then
   path=./
   target=$1
else
   path=$1
   target=$2
fi

find $path -name "*.[ch]" | xargs grep -Hn $target

