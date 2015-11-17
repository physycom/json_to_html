/* Copyright 2015 - Marco Di Cristina, Alessandro Fabbri, Mattia Guidetti, Stefano Sinigardi */

/***************************************************************************
This file is part of json_to_html.

json_to_html is free software : you can redistribute it and / or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

json_to_html is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with json_to_html. If not, see <http://www.gnu.org/licenses/>.
***************************************************************************/

#include <fstream>
#include <stdexcept>
#include <string>
#include "jsoncons/json.hpp"
#include "header_html.h"

using namespace std;

#define MAJOR_VERSION           1
#define MINOR_VERSION           0

void usage(char* progname){
  // Usage
    std::cout << "Json_to_Html (for Google Maps only) v" << MAJOR_VERSION << "." << MINOR_VERSION << std::endl;
    std::cout << "Usage: " << progname << " -i [input.json] -o [output.html] -m [or] -p" << std::endl;
    std::cout << "\t- [input.json] UNIBO style GPS .json file to parse" << std::endl;
    std::cout << "\t- [output.html] html script to display route in Google Maps (only)" << std::endl;
    std::cout << "\t- -m markers mode / -p polyline mode" << std::endl;
    exit(1);
}

int main(int argc, char** argv){
// Parsing command line
  std::string input_name, output_name, mode;
  if (argc > 2){ /* Parse arguments, if there are arguments supplied */
    for (int i = 1; i < argc; i++){
      if ((argv[i][0] == '-') || (argv[i][0] == '/')){       // switches or options...
        switch (tolower(argv[i][1])){
        case 'i':
          input_name = argv[++i];
          break;
        case 'o':
          output_name = argv[++i];
          break;
    		case 'm':
    		  mode="marker";
    		  break;
    		case 'p':
    		  mode="poly";
    		  break;
        default:    // no match...
          std::cout << "ERROR: Flag \"" << argv[i] << "\" not recognized. Quitting..." << std::endl;
          usage(argv[0]);
        }
      }
      else {
        std::cout << "ERROR: Flag \"" << argv[i] << "\" not recognized. Quitting..." << std::endl;
        usage(argv[0]);
      }
    }
  }
  else { std::cout << "ERROR: No flags specified. Read usage and relaunch properly." << std::endl; usage(argv[0]); }

// Safety checks for file manipulations
  ofstream output_file;
  ifstream input_file;

  if( input_name.size() > 5 ){
    if( input_name.substr(input_name.size()-5,5)!=".json" ){
      std::cout << input_name << " is not a valid .json file. Quitting..." << std::endl;
      exit(2);
    }
  }
  else{
    std::cout << input_name << " is not a valid .json file. Quitting..." << std::endl;
    exit(22);
  }
  input_file.open(input_name.c_str());  
  if (!input_file.is_open()) {
    cout << "FAILED: Input file " << input_name << " could not be opened." << std::endl;
    cout << "Hit ENTER to close.\n"; cin.get();
    exit(222);
  }
  else { cout << "SUCCESS: file " << input_name << " opened!\n"; }
  input_file.close();

  if( output_name.size() > 5 ){
    if( output_name.substr(output_name.size()-5,5)!=".html" ){
      std::cout << output_name << " is not a valid .html file. Quitting..." << std::endl;
      exit(3);
    }
  }
  else{
    std::cout << output_name << " is not a valid .html file. Quitting..." << std::endl;
    exit(33);
  }
  output_file.open(output_name.c_str());
  if (!output_file.is_open()) {
    cout << "FAILED: Output file " << output_name << " could not be opened." << std::endl;
    cout << "Hit ENTER to close.\n"; cin.get();
    exit(333);
  }
  else { cout << "SUCCESS: file " << output_name << " opened!"<< std::endl; }
  
  if(!(mode=="marker"||mode=="poly")){
    std::cout<<"Display mode not specified. Quitting..."<< std::endl;
	exit(4);
  }
  
// Preparing HTML document
  if(mode=="marker")output_file << header_marker;
  if(mode=="poly")output_file << header_poly;
  jsoncons::json gps_records = jsoncons::json::parse_file(input_name);
  if(gps_records.type() == 2) //array-style
	  for (size_t i = 0; i < gps_records.size(); ++i){
	    try{
	      output_file << "[" << std::fixed << std::setprecision(6) << gps_records[i]["lat"].as<double>() << "," << gps_records[i]["lon"].as<double>() << "]" << (i!=gps_records.size()-1?',':' ') << "\n";
	    }
	    catch (const std::exception& e){
	      std::cerr << e.what() << std::endl;
	    }
	  }
	else if(gps_records.type() == 1) { //object-style
		int i = 0;
		for(auto rec = gps_records.begin_members(); rec != gps_records.end_members(); ++rec, ++i) {
			try{
	    	output_file << "[" << std::fixed << std::setprecision(6) << rec->value()["lat"].as<double>() << "," << rec->value()["lon"].as<double>() << "]" << (i!=gps_records.size()-1?',':' ') << "\n";
	  	}
		  catch (const std::exception& e){
		    std::cerr << e.what() << std::endl;
		  }
		}
	}
  if(mode=="marker")output_file << footer_marker;
  if(mode=="poly")output_file << footer_poly;
  output_file.close();

  return 0;
}

