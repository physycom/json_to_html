#include <fstream>
#include <stdexcept>
#include <string>
#include "jsoncons/json.hpp"

//using jsoncons::json;

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

int main()
{
  std::ofstream html_script;
  html_script.open("./gps_script.html");
  html_script << header;

  jsoncons::json gps_records = jsoncons::json::parse_file("./gps.json");
  for (size_t i = 0; i < gps_records.size(); ++i){
    try{
      html_script << "[" << std::fixed << std::setprecision(6) << gps_records[i]["lat"].as<double>() << "," << gps_records[i]["lon"].as<double>() << "]" << (i!=gps_records.size()-1?',':' ') << "\n";
    }
    catch (const std::exception& e){
      std::cerr << e.what() << std::endl;
    }
  }

  html_script << footer;
  html_script.close();

  return 0;
}

