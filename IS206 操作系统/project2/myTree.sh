function myTree(){ #递归函数
   for file in `ls $1` #对目录下的每一个文件、文件夹进行判断
   do
      if [ -d "$1/$file" ] #文件夹
      then
         echo "$2$file" #输出名称
         myTree "$1/$file" "    $2" #递归调用函数
      else #文件
         echo "$2$file" #输出名称
      fi
   done
}
thispath='./'
myTree $thispath "" #调用myTree函数
