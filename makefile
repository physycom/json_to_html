EXE=json2html.exe

all:
	g++ -std=c++11 -I. -o $(EXE) json_to_html.cpp 
