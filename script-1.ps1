<#
  Build-и-установи libf2c (MinGW-gcc или MSVC) из подпапки libf2c рядом со скриптом.
  Пример MinGW:  .\install_libf2c.ps1 -Prefix "$PWD\thirdparty\f2c" -Toolchain mingw
#>

param(
    [string]$Prefix = "$PWD\thirdparty\f2c",
    [ValidateSet('auto','mingw','msvc')] [string]$Toolchain = 'auto'
)

# --------------------------------------------------------------------
# 1.  Проверяем исходники
# --------------------------------------------------------------------
$Here   = $PSScriptRoot
$SrcDir = Join-Path $Here 'libf2c'
if (-not (Test-Path "$SrcDir\makefile.u")) {
    Write-Error "Не вижу libf2c рядом со скриптом → $SrcDir"; exit 1
}

# --------------------------------------------------------------------
# 2.  Выбор toolchain
# --------------------------------------------------------------------
function Has($c){Get-Command $c -ErrorAction SilentlyContinue}
if ($Toolchain -eq 'auto') {
    $Toolchain = if (Has gcc  -and Has mingw32-make) { 'mingw' }
                 elseif (Has cl -and Has nmake)      { 'msvc'  }
                 else { Write-Error "gcc+mingw32-make или cl+nmake не найдены"; exit 1 }
}
Write-Host "[libf2c] toolchain = $Toolchain"

switch ($Toolchain) {
    'mingw' {
        $make    = 'mingw32-make'
        $libFile = 'libf2c.a'
        $mf      = 'makefile.u'
    }
    'msvc'  {
        $make    = 'nmake'
        $libFile = 'libf2c.lib'
        $mf      = 'makefile.vc'
    }
}
# --- гарантируем, что mingw32-make и gcc найдутся -------------------
if ($Toolchain -eq 'mingw') {
    try   { $gccDir = (Get-Command gcc  -ErrorAction Stop).Path | Split-Path }
    catch { Write-Error "gcc не найден в PATH"; exit 1 }

    try   { $makeDir = (Get-Command mingw32-make -ErrorAction Stop).Path | Split-Path }
    catch { $makeDir = $gccDir }   # mingw32-make обычно в том же bin

    foreach ($d in @($gccDir,$makeDir)) {
        if (-not ($Env:PATH -split ';' | Where-Object { $_ -ieq $d })) {
            $Env:PATH = "$d;$Env:PATH"
        }
    }
}

# --------------------------------------------------------------------
# 3.  Создаём lightweight-обёртки mv/cp/rm (только если их нет)
# --------------------------------------------------------------------
$wrapDir = Join-Path $SrcDir '.winwrap'
if (-not (Test-Path $wrapDir)) { New-Item $wrapDir -ItemType Directory | Out-Null }

$wrappers = @{
    'mv.bat' = '@echo off\r\nmove /Y %*'
    'cp.bat' = '@echo off\r\ncopy /Y %*'
    'rm.bat' = '@echo off\r\ndel  /F /Q %*'
}
foreach ($f in $wrappers.Keys){
    $path = Join-Path $wrapDir $f
    if (-not (Test-Path $path)) { $wrappers[$f] -replace '\\r\\n',"`r`n" | Set-Content -Encoding ascii $path }
}

if (-not ($env:PATH -split ';' | Where-Object { $_ -ieq $wrapDir })) {
    $env:PATH = "$wrapDir;$env:PATH"
}    # подсовываем в PATH прямо для этого процесса

$sysDir = Join-Path $SrcDir 'sys'
if (-not (Test-Path $sysDir)) { New-Item $sysDir -ItemType Directory | Out-Null }

$timesH = @'
#ifndef _SYS_TIMES_H_
#define _SYS_TIMES_H_
#include <time.h>
struct tms {
    clock_t tms_utime;   /* user CPU time    */
    clock_t tms_stime;   /* system CPU time  */
    clock_t tms_cutime;  /* user children    */
    clock_t tms_cstime;  /* system children  */
};
static inline clock_t times(struct tms *buf)
{
    clock_t c = clock();
    if (buf) { buf->tms_utime = c; buf->tms_stime = 0;
               buf->tms_cutime = buf->tms_cstime = 0; }
    return c;
}
#endif
'@
Set-Content -Encoding ascii (Join-Path $sysDir 'times.h') -Value $timesH

# --------------------------------------------------------------------
# 4.  Ручной “hadd” (f2c.h, signal1.h, sysdep1.h)
# --------------------------------------------------------------------
Copy-Item -Force "$SrcDir\signal1.h0" "$SrcDir\signal1.h"
Copy-Item -Force "$SrcDir\sysdep1.h0" "$SrcDir\sysdep1.h"
Copy-Item -Force "$SrcDir\arith.h0"   "$SrcDir\arith.h"   
Get-Content "$SrcDir\f2c.h0","$SrcDir\f2ch.add" | Set-Content "$SrcDir\f2c.h"

# --------------------------------------------------------------------
# 5.  Сборка
# --------------------------------------------------------------------
Push-Location $SrcDir
Copy-Item -Force $mf makefile        # подсовываем makefile.u → makefile
& $make "CC=gcc" "MV=mv" "CP=cp" "RM=rm" "CAT=type"
if (-not (Test-Path $libFile)) { Pop-Location; Write-Error "Build failed"; exit 1 }

# --------------------------------------------------------------------
# 6.  Установка
# --------------------------------------------------------------------
$incDir = Join-Path $Prefix 'include'; $libDir = Join-Path $Prefix 'lib'
$null = New-Item -Force -Type Directory $incDir, $libDir -ErrorAction SilentlyContinue
Copy-Item -Force "$SrcDir\f2c.h"       $incDir
Copy-Item -Force "$SrcDir\$libFile"    $libDir
Pop-Location
Write-Host "[libf2c] installed → $Prefix"