# Compile and run test.cpp, then display the results

& g++ $cppFile -o testing\test testing\test.cpp testing\simulator.cpp

# Check if compilation was successful
if ($LASTEXITCODE -eq 0) {
    $xy_points = & .\testing\test
    $xy_points | Out-File -FilePath "testing/path.csv" -Encoding utf8
    python testing\show_path.py
}
