run:
	cd src && make && ln -sf ~/Documents/SkyVault/src/bin/release/SkyVault ~/Documents/SkyVault/SkyVault && clear && ./bin/release/SkyVault 

build:
	cd src && make && ln -sf ~/Documents/SkyVault/src/bin/release/SkyVault ~/Documents/SkyVault/SkyVault

clean:
	cd src && make clean
