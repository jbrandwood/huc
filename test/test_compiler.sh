#!/bin/sh

export PCE_INCLUDE=`pwd`/../include/huc
export PCE_PCEAS=`pwd`/../bin/pceas
echo $PCE_INCLUDE $PCE_PCEAS

tests="$@"
test -z "$tests" && tests="tests/*.c dg/*.c"
test -n "$1" && tests="$@"

fails=0
nocompiles=0
exesuffix=

if [ "$OS" = "Windows_NT" ]; then
	exesuffix=.exe
fi

echo exesuffix="$exesuffix"

for d in large small norec noopt
do
	fails=0
	nocompiles=0
	passes=0
	test "$d" = "large" && opt="-DSTACK_SIZE=1024"
	test "$d" = "small" && opt="-msmall -DSTACK_SIZE=256 -DSMALL"
	test "$d" = "norec" && opt="-DSTACK_SIZE=1024 -DNORECURSE -fno-recursive"
	test "$d" = "noopt" && opt="-DSTACK_SIZE=1024 -O0 -DNOOPT"
	echo "testing $d"
	echo opt="$opt"
	for i in $tests
	do
		echo "HuC Type: $d   Test: $i"
		if ! ../bin/huc${exesuffix} -DNO_LABEL_VALUES $opt $i -lmalloc >/dev/null ; then
			echo NOCOMPILE
			../bin/huc${exesuffix} $opt $i
			exit
#			nocompiles=$((nocompiles + 1))
#			continue
		fi
		if ../tgemu/tgemu${exesuffix} "${i%.c}.pce" 2>/dev/null >/dev/null ; then
			echo PASS
			passes=$((passes + 1))
		else
			echo "FAIL (exit code $?)"
			../tgemu/tgemu${exesuffix} "${i%.c}.pce"
			exit
#			mkdir -p failtraces
#			mv "${i%.c}".{sym,s,pce} failtraces/
#			fails=$((fails + 1))
		fi
	done
	echo "$d passes: $passes; fails: $fails, nocompiles: $nocompiles"
	total_fails=$((total_fails + fails))
	total_nocompiles=$((total_nocompiles + nocompiles))
	total_passes=$((total_passes + passes))
done
echo "Total passes: $total_passes; fails: $total_fails, nocompiles: $total_nocompiles"
exit $((total_fails + total_nocompiles))
