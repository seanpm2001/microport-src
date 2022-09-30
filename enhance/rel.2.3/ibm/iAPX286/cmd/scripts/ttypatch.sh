:
#	iAPX286 @(#)ttypatch.sh	1.8
# 	Initial Version  - Mark A. Carlson (unix_net!mark)	12/6/86
#   Modified not to make nodes unless necessary - Henry Seltzer 1-28-87

# This file configures the serial driver for the specified tty.
#

kernel=${SYSTEM:-"/system5"}
addr=0x3f8
int=4
tty=0
vector=0
mcr=0xc
# pflags=${PFLAGS:-"-k"}
pflags=${PFLAGS:-""}
count=1


mksd() {
#	echo "Serial port #$1 (/dev/tty$1) at address $2 and interrupt line #$3"
	if [ ! \( -c /dev/tty$1 \) ]			# M00 changed 1-28-87
	then
		rm -f /dev/tty$1 		#/dev/ttym$1 /dev/ttyM$1
		/etc/mknod /dev/tty$1 c 5 $1
#		/etc/mknod /dev/ttym$1 c 5 `expr $1 + 128`
#		/etc/mknod /dev/ttyM$1 c 5 `expr $1 + 192`
	fi					# M00 end changes
	/etc/patch $pflags $kernel +`expr $1 "*" 2` ctl $2
	/etc/patch $pflags $kernel +`expr $1 "*" 2` si_ints $3
	if [ "X"$4 = "X" ]
	then
	    /etc/patch $pflags $kernel +`expr $3 "*" 2` vector 0
	else
	    /etc/patch $pflags $kernel +`expr $3 "*" 2` vector $4
	fi
	if [ "X"$5 = "X" ]
	then
	    /etc/patch $pflags $kernel +`expr $1 "*" 2` mcr 0xc
	else
	    /etc/patch $pflags $kernel +`expr $1 "*" 2` mcr $5
	fi
}

dump() {
	/etc/patch $pflags $kernel +`expr $1 "*" 2` si_ints
	/etc/patch $pflags $kernel +`expr $1 "*" 2` ctl 
	/etc/patch $pflags $kernel +`expr $1 "*" 2` mcr 
#	/etc/patch $pflags $kernel +`expr $1 "*" 2` vector
	}

mknone() {
	/etc/patch $pflags $kernel +`expr $1 "*" 2` ctl 0
	/etc/patch $pflags $kernel +`expr $1 "*" 2` si_ints 0
	/etc/patch $pflags $kernel +`expr $1 "*" 2` vector 0
	/etc/patch $pflags $kernel +`expr $1 "*" 2` mcr 0xc
	}

if [ $# -eq 0 ]					# M00 changed 1-28-87
then
	  echo "		Usage:  $0 [ -k ][ -d ][ -c ][ -t<tty#> ][ -n<#ttys> ] [ -i<interrupt> ] [ -a<addr> ] [ -v<vector addr> ] [ -m<modem ctl value> ]"
		exit 1 				# M00 end changes
fi
while [ $# -gt 0 ]
do
	case $1 in
	-d )
		addr=""
		int=""
		vector=0
		;;
	-c )
		addr=0
		int=0
		vector=0
		;;
	-k )
		pflags=$1
		;;
	-n[0-9]* )
		count=`expr "$1" : '-n\([0-9]*\)'`
		;;
	-t[0-9]* )
		tty=`expr "$1" : '-t\([0-9]*\)'`
		;;
	-i[3457] )
		int=`expr "$1" : '-i\([3457]*\)'`
		;;
	-m* )
		mcr=`expr "$1" : '-m\(.*\)'`
		;;
	-v* )
		vector=`expr "$1" : '-v\(.*\)'`
		;;
	-a* )
		addr=`expr "$1" : '-a\(.*\)'`
		;;
	* )						# M00
		echo "Illegal flag argument '$1'"
		exit 1
		;;
	esac
	shift
done

max=`expr $tty + $count`
for i in 0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15
do
    if [ "X"$int = "X" ]
    then
	if [ $tty -le $i ]
	then
	    if [ $max -gt $i ]
	    then
		echo "int, address for tty$i :"
		dump $i 
	    else
		exit 0
	    fi
	fi
    else
	if [ $tty -le $i ]
	then
	    if [ $max -gt $i ]
	    then
		if [ $int -eq 0 ]
		then
		    mknone $i 
		else
		    echo "making device $i "
		    saddr=`expr  \( $i - $tty \) "*" 8 + $addr` 
		    mksd $i $saddr $int	$vector $mcr
		fi
	    else
		exit 0
	    fi
	fi
    fi
done
