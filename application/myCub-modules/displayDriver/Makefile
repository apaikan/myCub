
.PHONY: all install uninstall
all:	dispd

dispd:
	g++ -o dispd dispd.cpp  PCD8544.cpp -rdynamic -lwiringPi

install: dispd
	[ "`id -u`" = "0" ] || { echo "Must be run as root"; exit 1; }
	cp -f dispd /usr/local/sbin
	cp -f init-script /etc/init.d/servoblaster
	chmod 755 /etc/init.d/servoblaster
	update-rc.d servoblaster defaults 92 08
	/etc/init.d/servoblaster start

uninstall:
	[ "`id -u`" = "0" ] || { echo "Must be run as root"; exit 1; }
	[ -e /etc/init.d/servoblaster ] && /etc/init.d/servoblaster stop || :
	update-rc.d servoblaster remove
	rm -f /usr/local/sbin/dispd
	rm -f /etc/init.d/servoblaster

clean:
	rm -f dispd

