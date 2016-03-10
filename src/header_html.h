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

const std::string html_header =
R"(
<!DOCTYPE html>
<html> 
  <head> 
    <meta http-equiv="content-type" content="text/html; charset=UTF-8" /> 
    <title>
      Display TRIP 
    </title> 
    <style>
      html, body, #map-canvas {
        height: 100%;
        margin: 0px;
        padding: 0px
      }
      #panel {
        position: absolute;
        top: 5px;
        right: 0%;
        margin-left: -180px;
        z-index: 5;
        background-color: #fff;
        padding: 5px;
        border: 1px solid #999;
      }
    </style>
    <script type="text/javascript"  src="https://maps.googleapis.com/maps/api/js?v=3.exp"></script>
<!-- Remote Physycom libraries -->
    <script type="text/javascript" src="https://raw.githubusercontent.com/physycom/ruler/master/markerwithlabel.js"></script>
    <script type="text/javascript" src="https://raw.githubusercontent.com/physycom/ruler/master/ContextMenu.js"></script>
    <link rel="stylesheet" type="text/css" href="http://cdn.rawgit.com/physycom/ruler/master/ruler_map.css">
    <script type="text/javascript" src="https://raw.githubusercontent.com/physycom/ruler/master/ruler_map.js"></script>       
  </head> 

    <body>
    <div id="map" style="width: 100%; height: 100%;"></div>
    
    <script type="text/javascript">
)";