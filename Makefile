run:
	lua j2l.lua && cd src && make && clear && ./bin/release/SkyVault

build:
	lua j2l.lua && cd src && make

clean:
	cd src && make clean
