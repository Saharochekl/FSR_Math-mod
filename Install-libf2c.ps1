<#
    Build-и-установи libf2c из папки, лежащей рядом со скриптом.
    Пример:  .\build_libf2c.ps1  -Prefix  "C:\dev\thirdparty\f2c"  -Toolchain mingw
#>

param(
    [string]$Prefix = "C:\local\f2c",
    [ValidateSet("auto","mingw","msvc")]
    [string]$Toolchain = "auto"
)

# ── исходники  -------------------------------------------------------
$Here   = Split-Path -LiteralPath $MyInvocation.MyCommand.Path -Parent
$SrcDir = Join-Path $Here 'libf2c'
if (-not (Test-Path "$SrcDir\makefile.u")) {
    Write-Error "Папка libf2c с исходниками не найдена рядом со скриптом."; exit 1
}

# ── выбор toolchain  -------------------------------------------------
function Has($cmd) { Get-Command $cmd -ErrorAction SilentlyContinue }
if ($Toolchain -eq 'auto') {
    $Toolchain = if (Has gcc  -and Has mingw32-make) { 'mingw' }
                 elseif (Has cl -and Has nmake)      { 'msvc' }
                 else { Write-Error "MinGW или MSVC не обнаружены."; exit 1 }
}
Write-Host "[libf2c] toolchain = $Toolchain"

switch ($Toolchain) {
    'mingw' { $make = 'mingw32-make'; $libFile = 'libf2c.a';  $mf = 'makefile.mgc' }
    'msvc'  { $make = 'nmake';        $libFile = 'libf2c.lib';$mf = 'makefile.msc' }
}

# ── сборка  ----------------------------------------------------------
Push-Location $SrcDir
Copy-Item -Force $mf makefile            # подсовываем нужный makefile
& $make hadd                             # C++-дружелюбный f2c.h
& $make
if (-not (Test-Path $libFile)) { Write-Error "Build failed."; exit 1 }

# ── установка  -------------------------------------------------------
$inc = Join-Path $Prefix 'include';  $lib = Join-Path $Prefix 'lib'
$inc,$lib | ForEach-Object { if (-not (Test-Path $_)) { New-Item -ItemType Directory $_ | Out-Null } }

Copy-Item -Force f2c.h      $inc
Copy-Item -Force $libFile   $lib
Write-Host "[libf2c] installed -> $Prefix"

Pop-Location