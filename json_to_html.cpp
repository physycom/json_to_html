#include <fstream>
#include <stdexcept>
#include <string>
#include "jsoncons/json.hpp"

using namespace std;

#define MAJOR_VERSION           1
#define MINOR_VERSION           0

const std::string header = 
"<!DOCTYPE html>                                                              \n\
<html>                                                                        \n\
<head>                                                                        \n\
  <meta http-equiv=\"content-type\" content=\"text/html; charset=UTF-8\" />   \n\
  <title>Google Maps Multiple Markers</title>                                 \n\
  <script src=\"http://maps.google.com/maps/api/js?sensor=false\"             \n\
          type=\"text/javascript\"></script>                                  \n\
</head>                                                                       \n\
<body>                                                                        \n\
  <div id=\"map\" style=\"width: 1200px; height: 600px;\"></div>              \n\
                                                                              \n\
  <script type=\"text/javascript\">                                           \n\
    var locations = [                                                         \n\
                                                                              \n\
";

const std::string footer = 
"                                                                             \n\
    ];                                                                        \n\
                                                                              \n\
    var map = new google.maps.Map(document.getElementById('map'), {           \n\
      zoom: 17,                                                               \n\
      center: new google.maps.LatLng(locations[0][0], locations[0][1]),       \n\
      mapTypeId: google.maps.MapTypeId.ROADMAP                                \n\
    });                                                                       \n\
                                                                              \n\
    var infowindow = new google.maps.InfoWindow();                            \n\
                                                                              \n\
    var marker, i;                                                            \n\
                                                                              \n\
    for (i = 0; i < locations.length; i++) {                                  \n\
      marker = new google.maps.Marker({                                       \n\
        position: new google.maps.LatLng(locations[i][0], locations[i][1]),   \n\
        map: map                                                              \n\
      });                                                                     \n\
                                                                              \n\
      google.maps.event.addListener(marker,'mouseover',(function(marker, i){  \n\
        return function() {                                                   \n\
          infowindow.setContent(locations[i][0]);                             \n\
          infowindow.open(map, marker);                                       \n\
        }                                                                     \n\
      })(marker, i));                                                         \n\
    }                                                                         \n\
  </script>                                                                   \n\
</body>                                                                       \n\
</html>                                                                       \n\
";

int main(int argc, char** argv)
{
// Usage
  std::cout << "Json_to_Html (for Google Maps only) v" << MAJOR_VERSION << "." << MINOR_VERSION << std::endl;
  std::cout << "Usage: " << argv[0] << " -i [input.json] -o [output.html]" << std::endl;
  std::cout << "\t- [input.json] UNIBO style GPS .json file to parse" << std::endl;
  std::cout << "\t- [output.html] html script to display route in Google Maps (only)" << std::endl;

// Parsing command line
  std::string input_name, output_name;
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
        default:    // no match...
          std::cout << "Flag \"" << argv[i] << "\" not recognized. Quitting..." << std::endl;
          exit(1);
        }
      }
      else {
        std::cout << "Flag \"" << argv[i] << "\" not recognized. Quitting..." << std::endl;
        exit(11);
      }
    }
  }
  else { std::cout << "No flags specified. Read usage and relaunch properly." << std::endl; exit(111); }

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
    cout << "FAILED: Input file " << input_name << " could not be opened." << endl;
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
    cout << "FAILED: Output file " << output_name << " could not be opened." << endl;
    cout << "Hit ENTER to close.\n"; cin.get();
    exit(333);
  }
  else { cout << "SUCCESS: file " << output_name << " opened!\n"; }

// Preparing HTML document
  output_file << header;
  jsoncons::json gps_records = jsoncons::json::parse_file(input_name);
  for (size_t i = 0; i < gps_records.size(); ++i){
    try{
      output_file << "[" << std::fixed << std::setprecision(6) << gps_records[i]["lat"].as<double>() << "," << gps_records[i]["lon"].as<double>() << "]" << (i!=gps_records.size()-1?',':' ') << "\n";
    }
    catch (const std::exception& e){
      std::cerr << e.what() << std::endl;
    }
  }
  output_file << footer;
  output_file.close();

  return 0;
}

