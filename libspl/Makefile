
all:
	cc -O2 buildit.c -o buildit
	./buildit "-c=-pipe -fPIC -fasynchronous-unwind-tables -O0 -DDEBUG -g -I." "-l=-Wl,-soname,libspld.so -shared -export-dynamic -pthread -Wl,-z,defs -Wl,-z,now -Wl,-z,relro" -tlib -pdebug libspld
	g++ -Wl,-rpath='$ORIGIN':. -Wl,-z,defs -Wl,-z,now -Wl,-z,relro -DDEBUG -g -L. -lspld -I. -o testit testit.cpp
	./buildit "-c=-pipe -fPIC -fasynchronous-unwind-tables -O2 -DNDEBUG -I." "-l=-Wl,-soname,libspl.so -shared -export-dynamic -pthread -Wl,-z,defs -Wl,-z,now -Wl,-z,relro" -tlib -prelease libspl

check:
	./testit

install:
	cp -u libspl.so /usr/lib
	chmod a+r /usr/lib/libspl.so
	cp -u libspld.so /usr/lib
	chmod a+r /usr/lib/libspld.so
	cp -ru spl/ /usr/include
	chmod -R a+r /usr/include/spl
	ldconfig

uninstall:
	rm /usr/lib/libspl.so
	rm /usr/lib/libspld.so
	ldconfig
	
clean:
	rm -f bin/*.o
	rm -f release/*.o
	rm -f debug/*.o
	rm -f libspl.so
	rm -f libspld.so
	rm -f testit
	rm -f libspl.tar.gz

dist:
	rm -f Makefile
	rm -f bin/*.o
	rm -f release/*.o
	rm -f debug/*.o
	rm -f libspl.so
	rm -f libspld.so
	rm -f testit
	rm -f libspl.tar.gz
	tar -czvf libspl.tar.gz *

