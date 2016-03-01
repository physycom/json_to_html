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

#define _CRT_SECURE_NO_WARNINGS

#include <fstream>
#include <stdexcept>
#include <string>
#include "jsoncons/json.hpp"
#include "header_html.h"

using namespace std;

#define MAJOR_VERSION           2
#define MINOR_VERSION           1

void usage(char* progname) {
  // Usage
  std::cout << "Usage: " << progname << " -i [input.json] -o [output.html] -[m/p] -u [undersampling] -s" << std::endl;
  std::cout << "\t- [input.json] UNIBO style GPS .json file to parse" << std::endl;
  std::cout << "\t- [output.html] html script to display route in Google Maps (only)" << std::endl;
  std::cout << "\t- -m markers mode / -p polyline mode" << std::endl;
  std::cout << "\t- [undersampling] a positive integer representing the undersampling factor" << std::endl;
  std::cout << "\t- -s optional flag for short point description (timestamp only)" << std::endl;
  exit(1);
}

int main(int argc, char** argv) {
  std::cout << "Json_to_Html (for Google Maps only) v" << MAJOR_VERSION << "." << MINOR_VERSION << std::endl;

  // Parsing command line
  std::string input_name, output_name, mode;
  std::string html_header, html_footer;
  size_t undersampling = 1;
  bool verbose = true;
  if (argc > 2) { /* Parse arguments, if there are arguments supplied */
    for (int i = 1; i < argc; i++) {
      if ((argv[i][0] == '-') || (argv[i][0] == '/')) {       // switches or options...
        switch (tolower(argv[i][1])) {
        case 'i':
          input_name = argv[++i];
          break;
        case 'o':
          output_name = argv[++i];
          break;
        case 'm':
          html_header = header_marker;
          html_footer = footer_marker;
          break;
        case 'p':
          html_header = header_poly;
          html_footer = footer_poly;
          break;
        case 'u':
          undersampling = (size_t)atoi(argv[++i]);
          break;
        case 's':
          verbose = false;
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
  else {
    std::cout << "ERROR: No flags specified. Read usage and relaunch properly." << std::endl;
    usage(argv[0]);
  }


  // Safety checks for file manipulations
  ofstream output_file;
  ifstream input_file;

  if (input_name.size() > 5) {
    if (input_name.substr(input_name.size() - 5, 5) != ".json") {
      std::cout << input_name << " is not a valid .json file. Quitting..." << std::endl;
      exit(2);
    }
  }
  else {
    std::cout << input_name << " is not a valid .json file. Quitting..." << std::endl;
    exit(22);
  }

  input_file.open(input_name.c_str());
  if (!input_file.is_open()) {
    cout << "FAILED: Input file " << input_name << " could not be opened. Quitting..." << std::endl;
    exit(222);
  }
  else { cout << "SUCCESS: file " << input_name << " opened!\n"; }
  input_file.close();

  if (output_name.size() > 5) {
    if (output_name.substr(output_name.size() - 5, 5) != ".html") {
      std::cout << output_name << " is not a valid .html file. Quitting..." << std::endl;
      exit(3);
    }
  }
  else {
    std::cout << output_name << " is not a valid .html file. Quitting..." << std::endl;
    exit(33);
  }

  output_file.open(output_name.c_str());
  if (!output_file.is_open()) {
    cout << "FAILED: Output file " << output_name << " could not be opened. Quitting..." << std::endl;
    exit(333);
  }
  else { cout << "SUCCESS: file " << output_name << " opened!" << std::endl; }

  if (html_header == "" || html_footer == "") {
    std::cout << "Display mode not specified. Quitting..." << std::endl;
    usage(argv[0]);
  }

  if (undersampling == 0) {
    std::cout << "WARNING: Undersampling factor is 0, set it to 1" << std::endl;
  }

  // Parsing JSON gps database and create a local copy 
  // to shorten code
  jsoncons::json gps_records = jsoncons::json::parse_file(input_name);
  std::vector<jsoncons::json *> gps_records_copy;
  if (gps_records.is_array()) {
    for (size_t k = 0; k < gps_records.size(); k++) {
      gps_records_copy.push_back(&(gps_records[k]));
    }
  }
  else if (gps_records.is_object()) {
    for (auto it = gps_records.begin_members(); it != gps_records.end_members(); it++) {
      gps_records_copy.push_back(&(it->value()));
    }
  }
  else {
    std::cout << "BAD json, not array nor object...?!?!" << std::endl;
    exit(321);
  }

  std::cout << "sixe ptr " << sizeof(gps_records_copy[0])*gps_records_copy.size() << "    size j " << sizeof(gps_records[0])*gps_records_copy.size() << std::endl;

  // Generating HTML document
  output_file << html_header;
  for (size_t i = 0; i < gps_records_copy.size(); ++i) {
    if (i % undersampling) continue;
    std::string tooltip(to_string(i));
    if (verbose) {
      if (gps_records_copy[i]->has_member("date"))
        tooltip = "date: " + gps_records_copy[i]->at("date").as<std::string>();
      if (gps_records_copy[i]->has_member("alt"))
        tooltip += "<br />altitude: " + gps_records_copy[i]->at("alt").as<std::string>();
      if (gps_records_copy[i]->has_member("heading"))
        tooltip += "<br />heading: " + gps_records_copy[i]->at("heading").as<std::string>();
      if (gps_records_copy[i]->has_member("speed"))
        tooltip += "<br />speed: " + gps_records_copy[i]->at("speed").as<std::string>();
      if (gps_records_copy[i]->has_member("enabling"))
        tooltip += "<br />enabling: " + gps_records_copy[i]->at("enabling").as<std::string>();
      if (gps_records_copy[i]->has_member("tracking_glonass")) {
        tooltip += "<br />glonass sats (used/seen): " + gps_records_copy[i]->at("using_glonass").as<std::string>() + " / " + gps_records_copy[i]->at("tracking_glonass").as<std::string>();
        tooltip += "<br />gps sats (used/seen): " + gps_records_copy[i]->at("using_gps").as<std::string>() + " / " + gps_records_copy[i]->at("tracking_gps").as<std::string>();
        tooltip += "<br />total sats (used/seen): " + gps_records_copy[i]->at("using_total").as<std::string>() + " / " + gps_records_copy[i]->at("tracking_total").as<std::string>();
      }
      if (gps_records_copy[i]->has_member("fix"))
        tooltip += "<br />fix: " + gps_records_copy[i]->at("fix").as<std::string>();
    }
    output_file
        << "["
        << std::fixed << std::setprecision(6)
        << (gps_records_copy[i]->has_member("lat") ? gps_records_copy[i]->at("lat").as<double>() : 90.0)
        << ","
        << (gps_records_copy[i]->has_member("lon") ? gps_records_copy[i]->at("lon").as<double>() : 0.0)
        << ",'<p>"
        << tooltip
        << "</p>']"
        << (i != gps_records_copy.size() - 1 ? ',' : ' ')
        << "\n";
  } 
  output_file << html_footer;
  output_file.close();

  return 0;
}

