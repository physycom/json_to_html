/* Copyright 2015-2017 - Marco Di Cristina, Alessandro Fabbri, Mattia Guidetti, Stefano Sinigardi */

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

#include <iostream>
#include <jsoncons/json.hpp>
#include <physycom/jsontohtml.hpp>

using namespace std;

constexpr int MAJOR = 4;
constexpr int MINOR = 1;

#define CAUSE_IGNITION_ON                0x88
#define CAUSE_IGNITION_OFF               0x08
#define CAUSE_COURSE                     0x84

void usage(char* progname)
{
  // Usage
  string pn(progname);
  cerr << "Usage: " << pn.substr(pn.find_last_of("/\\")+1) << " -i [input.json] -o [output.html] -j [config.json] -u [undersampling] -s -e" << endl;
  cerr << "\t- [input.json] Physycom GNSS .json format file to parse" << endl;
  cerr << "\t- [output.html] html script to display route in Google Maps (only)" << endl;
  cerr << "\t- [undersampling] a positive integer representing the undersampling factor" << endl;
  cerr << "\t- -s optional flag for short point description (timestamp only)" << endl;
  cerr << "\t- -e to export the image as [output.png]" << endl;
  cerr << "\t- [config.json] configuration json. Sample:" << endl;
  cerr << R"({
  "tag 1" :
  {
    "filename" : "file1.json",
    "style" : "style1", // [ )";
  for (const auto &s : allowed_styles) cout << s << ( s==allowed_styles.back() ? "" : " | ");
  cerr << R"( ]
    "color" : "color1"  // in the hex literal form FF0000
  }
}
)";
}

int main(int argc, char** argv)
{
  cout << "json_to_html v" << MAJOR << "." << MINOR << endl;

  string input_name = "", output_name = "map.html", json_config = "";
  int undersampling = 1;
  bool verbose = true, export_map = false;
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
        case 'u':
          undersampling = (int)stoi(argv[++i]);
          break;
        case 's':
          verbose = false;
          break;
        case 'j':
          json_config = argv[++i];
          break;
        case 'e':
          export_map = true;
          break;
        default:    // no match...
          cerr << "ERROR: Flag \"" << argv[i] << "\" not recognized. Quitting..." << endl;
          usage(argv[0]);
          exit(1);
        }
      }
      else {
        cerr << "ERROR: Flag \"" << argv[i] << "\" not recognized. Quitting..." << endl;
        usage(argv[0]);
        exit(1);
      }
    }
  }
  else {
    cerr << "ERROR: No flags specified. Read usage and relaunch properly." << endl;
    usage(argv[0]);
    exit(1);
  }

  jsoncons::json jconf;
  if( json_config == "" && input_name != "")
  {
    jsoncons::json jspec;
    jspec["filename"] = input_name;
    jconf["Trip"] = jspec;
  }
  else if ( json_config != "" && input_name == "" )
  {
    jconf = jsoncons::json::parse_file(json_config);
  }
  else
  {
    cerr << "ERROR: flag -i and -j are exclusive." << endl;
    exit(2);
  }

  try
  {
    json_to_html<jsoncons::json> j2h(export_map, verbose, undersampling);
    for(auto file : jconf.object_range())
    {
      j2h.push_file(file.value()["filename"].as<string>(), [&j2h]()
      {
        int old_counter = 0;
        vector<jsoncons::json> temp_trip;
        for (size_t k = 0; k < j2h.records.size(); k++)
        {
          if (
            (k > 0 && j2h.records[k].has_member("enabling") && j2h.records[k]["enabling"].as<string>() == "ignition_on")
            ||
            (k > 0 && j2h.records[k].has_member("cause") && j2h.records[k]["cause"].as<int>() == CAUSE_IGNITION_ON)
            ||
            (j2h.records[k].has_member("global_index") && j2h.records[k]["global_index"].as<int>() < old_counter)
            )
          {
            old_counter = 0;
            j2h.trips.push_back(temp_trip);
            temp_trip.clear();
          }
          if (j2h.records[k].has_member("global_index")) old_counter = j2h.records[k]["global_index"].as<int>();
          temp_trip.push_back(j2h.records[k]);
        }
        j2h.trips.push_back(temp_trip);
      });
      j2h.init_specs(string(file.key()), file.value());
    }
    j2h.dump_html(output_name);
  }
  catch (exception &e)
  {
    cerr << "EXC: " << e.what() << endl;
    exit(1);
  }

  return 0;
}
