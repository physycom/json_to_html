$invocation = (Get-Variable MyInvocation).Value
$pwd = Split-Path $invocation.MyCommand.Path
echo "PS---> Working in: $pwd"
$converter = "json_to_html.exe"

if ( $args[0] -eq "-m" -or $args[0] -eq "-p" ){
    $FILES_INPUT = Get-ChildItem $pwd -Filter data\*.json
    foreach($file_in in $FILES_INPUT)
    {
        Write-Host "PS---> Parsing: $file_in"
        $filename = $file_in.ToString().Split('.')
        $file_out = "data\"+$filename[0]+".html"
        &$converter -i data\$file_in -o $file_out -p
    }
}
else {
    Write-Host $args[0] "is not a valid flag. Quitting..."
    Exit
}