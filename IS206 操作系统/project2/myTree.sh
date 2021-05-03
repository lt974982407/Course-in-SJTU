function myTree(){
   for file in `ls $1`
   do
      if [ -d "$1/$file" ]
      then
         echo "$2$file"
         myTree "$1/$file" "    $2"
      else
         echo "$2$file"
      fi
   done
}
thispath='./'
myTree $thispath ""
