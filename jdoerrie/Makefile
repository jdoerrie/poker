dirs:
	mkdir -p out/Debug/
	mkdir -p out/Release/
	cd out/Debug && cmake -DCMAKE_BUILD_TYPE=Debug ../..
	cd out/Release && cmake -DCMAKE_BUILD_TYPE=Release ../..

rel:
	cd out/Release && make

dbg:
	cd out/Debug && make

clean:
	rm -r out/
