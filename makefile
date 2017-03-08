EXE=json_to_html.exe

all: dirtree
	$(CXX) -O3 -std=c++11 -Isrc/jsoncons/src -o bin/$(EXE) src/json_to_html.cpp

dirtree:
	@mkdir -p bin

clean:
	rm -f bin/*.exe

cleanall:
	rm -rf bin
