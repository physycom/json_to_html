---
documentclass: physycomen
title:  "json_to_html"
author: "Di Cristina, Fabbri, Guidetti, Sinigardi"
---

<a href="http://www.physycom.unibo.it"> 
<div class="image">
<img src="https://cdn.rawgit.com/physycom/templates/697b327d/logo_unibo.png" width="90" height="90" alt="© Physics of Complex Systems Laboratory - Physics and Astronomy Department - University of Bologna"> 
</div>
</a>
<a href="https://travis-ci.org/physycom/json_to_html"> 
<div class="image">
<img src="https://travis-ci.org/physycom/json_to_html.png?branch=master" width="90" height="20" alt="Build Status"> 
</div>
</a>
<a href="https://ci.appveyor.com/project/cenit/json-to-html"> 
<div class="image">
<img src="https://ci.appveyor.com/api/projects/status/w7o0yyvsgfuv4bnv?svg=true" width="90" height="20" alt="Build Status"> 
</div>
</a>

### Purpose
This tool has been written in order to ease the visualization of large GNSS datasets, coming from our various analyses (algorithms, tests, ...).
It converts a .json file, written using our standard convention, into an html containing a marker at each location with a pop-up presenting non-gnss keys found in the input file.
It uses Google Maps as the underlying mapping source.

### Installation
**make** and a **C++11** compatible compiler are required. Clone the repo and type ``make``, it should be enough in most cases!   
There's also a **VS2015** solution avalaible.   
Contains [jsoncons](https://github.com/danielaparker/jsoncons) as a git submodule.

### Usage
```
json_to_html.exe -i input.json -o output.html -m [or] -p
```
where *input.json* must be an existing and valid .json file while *output.html* is the name of the output web page. To select the display mode choose one: *-m* for markers and *-p* for polylines.

More details about file formats is available [here](https://github.com/physycom/file_format_specifications/blob/master/formati_file.md).

