run:
	./j2l && cd src && make && clear && ./bin/release/SkyVault

build:
	./j2l && cd src && make

clean:
	cd src && make clean
