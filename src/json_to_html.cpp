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

#define CAUSE_IGNITION_ON                0x88
#define CAUSE_IGNITION_OFF               0x08
#define CAUSE_COURSE                     0x84

#define MAJOR_VERSION                    3
#define MINOR_VERSION                    0

void usage(char* progname) {
  // Usage
  std::cout << "Usage: " << progname << " -i [input.json] -o [output.html] -u [undersampling] -s" << std::endl;
  std::cout << "\t- [input.json] UNIBO style GPS .json file to parse" << std::endl;
  std::cout << "\t- [output.html] html script to display route in Google Maps (only)" << std::endl;
  std::cout << "\t- [undersampling] a positive integer representing the undersampling factor" << std::endl;
  std::cout << "\t- -s optional flag for short point description (timestamp only)" << std::endl;
  exit(1);
}

int main(int argc, char** argv) {
  std::cout << "Json_to_Html (for Google Maps only) v" << MAJOR_VERSION << "." << MINOR_VERSION << std::endl;

  // Parsing command line
  std::string input_name, output_name, mode;
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

  if (undersampling == 0) {
    std::cout << "WARNING: Undersampling factor is 0, set it to 1" << std::endl;
  }

  // Parsing JSON gps database and create a local vector of pointers 
  jsoncons::json gps_records = jsoncons::json::parse_file(input_name);
  std::vector<jsoncons::json *> gps_records_copy;
  std::vector<std::vector<jsoncons::json *>> trips;
  int old_counter = 0;
  try {
    if (gps_records.is_array()) {
      for (size_t k = 0; k < gps_records.size(); k++) {
        if (
          (k > 0 && gps_records[k].has_member("enabling") && gps_records[k]["enabling"].as<string>() == "ignition_on")
          ||
          (k > 0 && gps_records[k].has_member("cause") && gps_records[k]["cause"].as<int>() == CAUSE_IGNITION_ON)
          ||
          (gps_records[k].has_member("global_index") && gps_records[k]["global_index"].as<int>() < old_counter)
          ) {
          old_counter = 0;
          trips.push_back(gps_records_copy);
          gps_records_copy.clear();
        }
        if (gps_records[k].has_member("global_index")) old_counter = gps_records[k]["global_index"].as<int>();
        gps_records_copy.push_back(&(gps_records[k]));
      }
      trips.push_back(gps_records_copy);
    }
    else if (gps_records.is_object()) {
      for (auto it = gps_records.begin_members(); it != gps_records.end_members(); it++) {
        if (
          (it != gps_records.begin_members() && it->value().has_member("enabling") && it->value()["enabling"].as<string>() == "ignition_on")
          ||
          (it != gps_records.begin_members() && it->value().has_member("cause") && it->value()["cause"].as<int>() == CAUSE_IGNITION_ON)
          ||
          (it->value().has_member("global_index") && it->value()["global_index"].as<int>() < old_counter)
          ) {
          old_counter = 0;
          trips.push_back(gps_records_copy);
          gps_records_copy.clear();
        }
        if (it->value().has_member("global_index")) old_counter = it->value()["global_index"].as<int>();
        gps_records_copy.push_back(&(it->value()));
      }
      trips.push_back(gps_records_copy);
    }
    else {
      std::cout << "BAD json, not array nor object...?!?!" << std::endl;
      exit(321);
    }
  }
  catch (std::exception &e) {
    std::cout << e.what() << std::endl;
    exit(5);
  }

  // Generating HTML document
  output_file << html_header;
  for (size_t i = 0; i < trips.size(); i++) {
    output_file << "\t\t\tvar Trajectory_trip_" << i << ";" << endl;
    output_file << "\t\t\tvar Markers_trip_" << i << " = [];" << endl;
  }
  output_file << endl << "\t\t\tvar map;" << endl << endl;
  output_file << "\t\t\tfunction initialize(){" << endl;
  for (size_t i = 0; i < trips.size(); i++) {
    output_file << endl << "\t\t\t\tvar Locations_trip_" << i << " = [" << endl;
    // start of gps points 
    for (size_t j = 0; j < trips[i].size(); j++) {
      if (j % undersampling) continue;
      std::string tooltip(to_string(j));
      if (verbose) {
        if (trips[i][j]->has_member("date"))
          tooltip = "date: " + trips[i][j]->at("date").as<std::string>();
        if (trips[i][j]->has_member("alt"))
          tooltip += "<br />altitude: " + trips[i][j]->at("alt").as<std::string>();
        if (trips[i][j]->has_member("heading"))
          tooltip += "<br />heading: " + trips[i][j]->at("heading").as<std::string>();
        if (trips[i][j]->has_member("speed"))
          tooltip += "<br />speed: " + trips[i][j]->at("speed").as<std::string>();
        if (trips[i][j]->has_member("enabling"))
          tooltip += "<br />cause: " + trips[i][j]->at("enabling").as<std::string>();
        if (trips[i][j]->has_member("tracking_glonass")) {
          tooltip += "<br />glonass sats (used/seen): " + trips[i][j]->at("using_glonass").as<std::string>() + " / " + trips[i][j]->at("tracking_glonass").as<std::string>();
          tooltip += "<br />gps sats (used/seen): " + trips[i][j]->at("using_gps").as<std::string>() + " / " + trips[i][j]->at("tracking_gps").as<std::string>();
          tooltip += "<br />total sats (used/seen): " + trips[i][j]->at("using_total").as<std::string>() + " / " + trips[i][j]->at("tracking_total").as<std::string>();
        }
        if (trips[i][j]->has_member("fix"))
          tooltip += "<br />fix: " + trips[i][j]->at("fix").as<std::string>();
      }
      output_file
        << "["
        << std::fixed << std::setprecision(6)
        << (trips[i][j]->has_member("lat") ? trips[i][j]->at("lat").as<double>() : 90.0)
        << ","
        << (trips[i][j]->has_member("lon") ? trips[i][j]->at("lon").as<double>() : 0.0)
        << ",'<p>"
        << tooltip
        << "</p>']"
        << (j != trips[i].size() - 1 ? ',' : ' ')
        << endl;
    }
    // end of gps points
    output_file << "\t\t\t\t]" << endl;
  }
  output_file << R"(
        map = new google.maps.Map(document.getElementById('map'), {
          zoom: 17,
          center : new google.maps.LatLng(Locations_trip_0[1][0], Locations_trip_0[1][1]),
          mapTypeId : google.maps.MapTypeId.ROADMAP
        });
        
        var infowindow = new google.maps.InfoWindow();

        var Marker, i;
)";
  for (size_t i = 0; i < trips.size(); i++) {
    output_file << R"(
/////// TRIP )" << i << R"(
        for(i = 0; i<Locations_trip_)" << i << R"(.length; i++) {
          var marker_url;
          if ( Locations_trip_)" << i << R"([i][2].search("first_last") != -1 )
            marker_url = 'http://maps.gpsvisualizer.com/google_maps/icons/circle/blue.png';
          else if ( Locations_trip_)" << i << R"([i][2].search("rdp_engine") != -1 )
            marker_url = 'http://maps.gpsvisualizer.com/google_maps/icons/circle/green.png';
          else if ( Locations_trip_)" << i << R"([i][2].search("smart_restore") != -1 )
            marker_url = 'http://maps.gpsvisualizer.com/google_maps/icons/circle/yellow.png';
          else if ( Locations_trip_)" << i << R"([i][2].search("ignition_on") != -1 )
            marker_url = 'http://maps.gpsvisualizer.com/google_maps/icons/circle/white.png';
          else if ( Locations_trip_)" << i << R"([i][2].search("ignition_off") != -1 )
            marker_url = 'http://maps.gpsvisualizer.com/google_maps/icons/circle/black.png';
          else
            marker_url = 'http://maps.gpsvisualizer.com/google_maps/icons/circle/green.png';
          
          Marker = new google.maps.Marker({
            position: new google.maps.LatLng( Locations_trip_)" << i << R"([i][0], Locations_trip_)" << i << R"([i][1] ),
            map: map,
            zIndex: Locations_trip_)" << i << R"([i][1],
            icon: marker_url
          });
          Markers_trip_)" << i << R"(.push(Marker);

          google.maps.event.addListener(Marker, 'click', (function(marker, i) {
            return function() {
              infowindow.setContent(Locations_trip_)" << i << R"([i][2]);
              infowindow.open(map, marker);
            }
          })(Marker, i));
        }
)";
    output_file << R"(
        var PolyPath_trip_)" << i << R"( = [];
        for (i = 0; i<Locations_trip_)" << i << R"(.length; i++) {
          PolyPath_trip_)" << i << R"(.push(new google.maps.LatLng(Locations_trip_)" << i << R"([i][0], Locations_trip_)" << i << R"([i][1]))
        }
 				Trajectory_trip_)" << i << R"( = new google.maps.Polyline({
  					path: PolyPath_trip_)" << i << R"(,
	  				geodesic: true,
		  			strokeColor: '#cc0000',
			  		strokeOpacity: 1.,
				  	strokeWeight: 2
				});
        Trajectory_trip_)" << i << R"(.setMap(map);
)";
  }
  output_file << endl << "\t\t\t\truler_map = new RulerMap(map)" << endl;
  output_file << "\t\t}" << endl;
  for (size_t i = 0; i < trips.size(); i++) {
    output_file << R"(
    function toggle_trip_)" << i << R"((){
      Trajectory_trip_)" << i << R"(.setMap(Trajectory_trip_)" << i << R"(.getMap() ? null : map);
      for (i = 0 ; i<Markers_trip_)" << i << R"(.length ; i++) {
        var mark = Markers_trip_)" << i << R"([i];
        mark.setMap(mark.getMap() ? null : map);
      }
    }
)";
  }
  output_file << R"(
		google.maps.event.addDomListener(window, 'load', initialize);

    </script>
    <div id = "panel">
)";
  for (size_t i = 0; i < trips.size(); i++) {
    output_file << "\t\t\t<button onclick = \"toggle_trip_" << i << "()\" style = \"background-color:#cc0000; color:#000000\">Trip " << i << "</button>" << endl;
  }
  output_file << R"(    </div>
  </body>
</html>
)";
  output_file.close();

  return 0;
}

