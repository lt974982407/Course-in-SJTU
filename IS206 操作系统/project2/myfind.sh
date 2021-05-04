if [ $# -lt 1 ] #输入参数个数小于1的情况
then
   echo "The number of parameter cannot be less than 1."
   exit
fi

if [ $# -gt 2 ] #输入参数个数大于2的情况
then
   echo "The number of parameter cannot be more than 2."
   exit
fi

if [ $# -eq 1 ] #仅输入一个pattern参数
then
   path=./ #在当前目录下寻找
   pattern=$1
else #输入两个参数
   path=$1
   pattern=$2
fi

find $path -name "*.[ch]" | xargs grep -Hn $pattern #进行搜索和打印

