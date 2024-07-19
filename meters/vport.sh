socat -d -d PTY,link=/dev/ttyV0,raw,echo=0 PTY,link=/dev/ttyV1,raw,echo=0 &
socat TCP4:192.168.1.100:2001 /dev/ttyV0,raw,echo=0,crnl

