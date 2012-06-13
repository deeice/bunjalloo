#!/bin/sh

g() {
    python <<EOF
import ConfigParser
c = ConfigParser.ConfigParser()
c.readfp(open("$DEVKITPRO/" + 'devkitProUpdate.ini'))
print c.get("$1", "$2")
EOF
}

. $(dirname $0)/shell_functions.sh

check_devkitpro
TEMP=$(getopt -o hut --long help,update,batch -- "$@")
if [ $? != 0 ] ; then
  die "Try '$0 --help' for more information"
fi
eval set -- "$TEMP"
UPDATE=N
BATCH=N
while true ; do
  case $1 in
    -u|--update ) UPDATE=Y ; shift ;;
    -t|--batch ) BATCH=Y ; shift ;;
    -h|--help )
    echo "Update devkitARM from the internet"
    echo ""
    echo "Usage: $0 [OPTION]... "
    echo "Example:"
    echo " $0 -ut"
    echo ""
    echo "Options available are:"
    echo "-u, --update             Update the configuration file first"
    echo "-t, --batch              Run in batch mode, ask no questions"
    echo "-h,--help                Show this message and exit"
    exit 0
    ;;
    --) shift ;  break ;;
    *) die "Internal error! " ;;
  esac
done

cd $DEVKITPRO
here=$(pwd)
url="http://devkitpro.sourceforge.net/devkitProUpdate.ini"
if [ "$UPDATE" = "Y" ] || [ ! -e devkitProUpdate.ini ] ; then
    wget $url || die "Unable to download devkitProUpdate.ini"
fi

url=$(g devkitProUpdate URL)
platform=$(uname -m)-linux.tar.bz2
actualversion=$($DEVKITARM/bin/arm-*-gcc --version | head -1 | sed 's/.*release \([0-9]\+\).*/\1/g')

for item in devkitARM libnds libndsfat maxmodds dswifi filesystem defaultarm7 ndsexamples
do
    dl=$(g $item File | sed "s/win32\.exe/$platform/g")
    v=$(g $item Version)
    if [ -e $dl ] ; then
        if [ "x$BATCH" = "xY" ] ; then
            echo "Skipping $item $v. Already installed"
            continue
        fi
        printf "Looks like you already have $item $v. Download and install anyway? [y/N]"
        read doit
        if [ "x$doit" != "xy" ] ; then
            continue
        fi
    fi
    wget $url/$dl

    case $item in
        devkitARM)
            if [ -d $item ] ; then
                echo "Move old $item $actualversion out of the way"
                mv $item ${item}_$actualversion
            fi
            touch $item-$v
            tar xf $dl
            ;;
        ndsexamples)
            mkdir -p $item-$v
            cd $item-$v
            tar xf ../$dl
            cd ..
            ;;
        *)
            touch $item-$v
            mkdir -p libnds
            cd libnds
            tar xf ../$dl
            cd ..
            ;;
    esac
done
