EXE=json_to_html.exe

all:
	$(CXX) -std=c++11 -Isrc -o $(EXE) src/json_to_html.cpp 
clean:
	rm -rf *.exe *.o x64 Debug Release *.sdf *.ilk *.pdb src/Debug src/Release src/x64
	