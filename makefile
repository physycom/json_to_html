EXE=json_to_html.exe

all:
	$(CXX) -std=c++11 -Isrc -o $(EXE) src/json_to_html.cpp 
