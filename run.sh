./geo --compile
read -r -p "[run.sh] Do you want to start the game? [Y/n] " input
 
case $input in
      [nN])
            exit 0
            ;;
      [yY] | *)
            cd ./Geometria/
            ./badapple
            cd ..
            ;;
esac