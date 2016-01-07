[ -f ./mirunc ] && [ -f ./mirunvm ] || echo "use mirun_build.sh to build compiler and vm first"
[ -f ./mirunc ] && [ -f ./mirunvm ] || exit 1

./mirunc $1 mirun_bc.out
./mirunvm mirun_bc.out ${@:2}
rm mirun_bc.out