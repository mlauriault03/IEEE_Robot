# Compile and run test.cpp, then display the results

$outputFile = "testing/path.csv"

& g++ $cppFile -o testing\test testing\test.cpp testing\simulator.cpp

# Check if compilation was successful
if ($LASTEXITCODE -eq 0) {
    $xy_points = & .\testing\test

    $encoding = New-Object System.Text.UTF8Encoding $False
    [System.IO.File]::WriteAllLines("$PSScriptRoot\$outputFile", $xy_points, $encoding)

    python testing\show_path.py
}
