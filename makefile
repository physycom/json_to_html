EXE=json_to_html.exe

all:
	$(CXX) -std=c++11 -I. -o $(EXE) json_to_html.cpp 
