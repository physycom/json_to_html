const std::string header_marker =
R"(
<!DOCTYPE html>                                                             
<html>                                                                       
<head>                                                                       
  <meta http-equiv="content-type" content="text/html; charset=UTF-8" />  
  <title>Google Maps Multiple Markers</title>                                
  <script src="http://maps.google.com/maps/api/js?sensor=false"            
          type="text/javascript"></script>                                 
</head>                                                                      
<body>                                                                       
  <div id="map" style="width: 1200px; height: 600px;"></div>             
                                                                             
  <script type="text/javascript">                                          
    var locations = [                                                        
                                                                            
)";

const std::string footer_marker = 
R"(                                                                            
    ];                                                                        
                                                                              
    var map = new google.maps.Map(document.getElementById('map'), {           
      zoom: 17,                                                               
      center: new google.maps.LatLng(locations[0][0], locations[0][1]),       
      mapTypeId: google.maps.MapTypeId.ROADMAP                                
    });                                                                       
                                                                              
    var infowindow = new google.maps.InfoWindow();                            
                                                                              
    var marker, i;                                                            
                                                                              
    for (i = 0; i < locations.length; i++) {                                  
      marker = new google.maps.Marker({                                       
        position: new google.maps.LatLng(locations[i][0], locations[i][1]),   
        map: map                                                              
      });                                                                     
                                                                              
      google.maps.event.addListener(marker,'mouseover',(function(marker, i){  
        return function() {                                                   
          infowindow.setContent(locations[i][0]);                             
          infowindow.open(map, marker);                                       
        }                                                                     
      })(marker, i));                                                         
    }                                                                         
  </script>                                                                   
</body>                                                                       
</html>                                                                       
)";

const std::string header_poly = 
R"(
<!DOCTYPE html>
<html> 
	<head> 
		<meta http-equiv="content-type" content="text/html; charset=UTF-8" /> 
		<title>
	numero di satelliti  </title> 
		<script src="https://maps.googleapis.com/maps/api/js?v=3.exp&signed_in=true">
		</script>
	</head> 
<body>
	<div id="map" style="width: 1900px; height: 1000px;"></div>
	<script type="text/javascript">
	function initialize() {

var locationstot = [

)";

const std::string footer_poly = 
R"(
];

	var locations=locationstot;
	
    var map = new google.maps.Map(document.getElementById('map'), {
      zoom: 17,
      center: new google.maps.LatLng(locations[1][0], locations[1][1]),
      mapTypeId: google.maps.MapTypeId.ROADMAP
    });

    var infowindow = new google.maps.InfoWindow();

	var poly_path = [];

	for (i = 0; i < locations.length; i++) {  
		poly_path.push(new google.maps.LatLng(locations[i][0], locations[i][1]))
	}
	
	
var Traiettorie = new google.maps.Polyline({
    path: poly_path,
    geodesic: true,
    strokeColor: '#FF0000',
    strokeOpacity: 1.,
    strokeWeight: 2
  });

  Traiettorie.setMap(map);
}

google.maps.event.addDomListener(window, 'load', initialize);


  </script>
</body>
</html>
)";
