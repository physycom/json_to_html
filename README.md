### Installation
**Make** and a **C++11** compatible compiler are required. Clone the repo and type ``make all`` in your favourite shell.

Uses jsoncons library (https://github.com/danielaparker/jsoncons).


### Usage
```
json2html.exe -i input.json -o output.html -m [or] -p
```
where *input.json* must be an existing and valid .json file while *output.html* is the name of the output web page. To select the display mode choose one: *-m* for markers and *-p* for polylines.

### Input Sample

Either array-style:
```
[
    {
        "your_key1":"your_value",
        "your_key2":"your_value",
        "lat" : 44.498950,
        "lon" : 11.353606
    },
    {
        "your_key1":"your_value",
        "your_key2":"your_value",
        "lat" : 44.498595,
        "lon" : 11.354939
    },
    {
        "your_key1":"your_value",
        "your_key2":"your_value",
        "lat" : 44.498257,
        "lon" : 11.355894
    }
]
```
or object-style:
```
{
    "gps_record_0000001":
    {
        "your_field":"your_content",
        "lat":44.5021611,
        "lon":11.3648926
    },
    "gps_record_0000002":
    {
        "another_field":"another_content",
        "lat":44.5021381,
        "lon":11.3648768
    }
}

Brought to you with :heart: by:

&copy; _Physics of Complex Systems Laboratory - Physics and Astronomy Department - University of Bologna_
