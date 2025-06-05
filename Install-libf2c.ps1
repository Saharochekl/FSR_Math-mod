<#
.SYNOPSIS
    Скачивает и собирает libf2c под Windows-MinGW и кладёт в выбранный PREFIX
.EXAMPLE
    PS> .\Install-libf2c.ps1 -Prefix "C:\dev\thirdparty\f2c"
#>

param(
    [string]$Prefix = "C:\local"
)

# ───── sanity checks ─────
if (-not (Get-Command gcc  -ErrorAction SilentlyContinue)) { Write-Error "gcc (MinGW) not found" ; exit 1 }
if (-not (Get-Command make -ErrorAction SilentlyContinue)) { Write-Error "make not found"        ; exit 1 }

$Temp = New-Item -ItemType Directory -Path ([System.IO.Path]::GetTempPath()) -Name ("f2c_" + [guid]::NewGuid())
Write-Host "[libf2c] Working dir: $Temp"

# ───── download and unpack ─────
$Zip  = "$Temp\libf2c.zip"
Invoke-WebRequest -Uri "https://www.netlib.org/f2c/libf2c.zip" -OutFile $Zip
Expand-Archive -LiteralPath $Zip -DestinationPath $Temp

Push-Location "$Temp\libf2c"
Write-Host "[libf2c] Building…"
& make -f makefile.u

# ranlib (optional for MinGW)
if (Get-Command ranlib -ErrorAction SilentlyContinue) { & ranlib libf2c.a }

# ───── install ─────
$newInc = Join-Path $Prefix "include"
$newLib = Join-Path $Prefix "lib"
$newInc, $newLib | ForEach-Object { if (-not (Test-Path $_)) { New-Item -ItemType Directory -Path $_ | Out-Null } }

Copy-Item -Force f2c.h    $newInc
Copy-Item -Force libf2c.a $newLib
Write-Host "[libf2c] Installed to $Prefix"

Pop-Location
Remove-Item -Recurse -Force $Temp