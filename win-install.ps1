$CurrentDir = "" + (Get-Location)

$BinaryDir = $CurrentDir + "/lib"
$IncludeDir = $CurrentDir + "/include"

[Environment]::SetEnvironmentVariable("SEAL_INCLUDE", $IncludeDir, [System.EnvironmentVariableTarget]::User)
[Environment]::SetEnvironmentVariable("SEAL_BIN_DIR", $BinaryDir,  [System.EnvironmentVariableTarget]::User)