#!/bin/sh
realdir(){
	echo $(dirname `realpath $0`)
}

makedir="$(realdir $0)"
makedir(){
	echo "$(realdir)"
}
make -C "$makedir" -f "$makedir/makefile.mk" $@
