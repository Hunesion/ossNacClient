#!/bin/bash 

LANG_CONFIG="ko_KR.UTF-8"

NEEDEDFILES="/run/test2"
NEEDED="/usr/sbin/cups-browsed"
UNNEEDED=""

process_error()
{
#       echo "$1\n"
        echo "$1" > /var/tmp/nac/html/error.html
        iptables -F
        iptables -A OUTPUT -m tcp -p tcp --dport 8443 -j ACCEPT
        iptables -A OUTPUT -s 127.0.0.1 -j ACCEPT
        iptables -A OUTPUT -d 127.0.0.1 -j ACCEPT
        iptables -A OUTPUT -j DROP
        exit 1
}

if [ "$LANG" != "$LANG_CONFIG" ] ; then
        process_error "edit language to $LANG_CONFIG"
fi

for i in $NEEDEDFILES ; do
        if ! [ -e "$i" ]; then process_error "File $i is not present"; fi
done

for i in $NEEDED ; do
        ps -ef | grep -v grep |grep -qiw $i || $i &
done

for i in $UNNEEDED ; do
        ps -ef | grep -v grep |grep -qiw $i && process_error "$i is runninggg"
done

rm -f /var/tmp/nac/html/error.html
iptables -F
