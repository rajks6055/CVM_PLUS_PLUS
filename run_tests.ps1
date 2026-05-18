# Define our folders and executable
$TestFile = "tests\test_all.cvm"
$OutputDir = "outputs"
$OutputFile = "$OutputDir\test_results.txt"
$Compiler = ".\cvm.exe"

# 1. Check if output directory exists, if not, create it
If (!(Test-Path $OutputDir)) {
    New-Item -ItemType Directory -Force -Path $OutputDir | Out-Null
    Write-Host "Created missing outputs directory." -ForegroundColor Yellow
}

# 2. Run the compiler and redirect output ( > ) to the text file
Write-Host "Running CVM++ Test Suite..." -ForegroundColor Cyan
& $Compiler $TestFile > $OutputFile

# 3. Read the output back to the console so you can see it succeeded
Write-Host "Done! Output saved to $OutputFile" -ForegroundColor Green
Write-Host "--- TEST RESULTS ---"
Get-Content $OutputFile