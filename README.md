# json_to_html
A command line tool which extracts the gps coordinates (latitude and longitude) from a .json archive and creates a .html to display the trajectory in Google Maps.

### Installation
**Make** and a **C++11** compatible compiler are required. Clone the repo and type ``make all`` in your favourite shell.

### Usage
```
json2html.exe -i input.json -o output.html -m [or] -p
```
where *input.json* must be an existing and valid .json file while *output.html* is the name of the output web page. To select the display mode choose one: *-m* for markers and *-p* for polylines.

### Input Sample

```
{
    "gps_record_0000001":
    {
        "your_field":your_content
        "lat":44.5021611,
        "lon":11.3648926
    },
    "gps_record_0000002":
    {
        "another_field":another_content
        "lat":44.5021381,
        "lon":11.3648768
    },
}
```

Brought to you with :heart: by:

&copy; _Complex Systems Laboratory - Physics and Astronomy Department - University of Bologna_
