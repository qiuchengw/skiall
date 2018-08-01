pull_m(){

  local th=`pwd`;
  echo $th
  echo "---->"
  for d in `ls .`
  do
    cd "$th/$d"
    echo "----->"
    git checkout master
    pwd
  done
}

pull_m $1

