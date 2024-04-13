@echo off

@REM 操作字符串（x是开始位置，y是取得字符数）
@REM echo %time:~x,y%

@REM 跳转到Public_IF.h所在的目录
set Path0=%cd%
cd "..\01 Source\Code\Public\" 

@echo off & setlocal enabledelayedexpansion
set lineFlag=0
for /f "delims=*" %%i in (Public_IF.h) do (
set /a lineFlag+=1
  if !lineFlag!==17 ( @REM 版本号所在行数 17行
    set AppSoftwareVersionLineContent=%%i 
  )
)
@REM 截取版本号
set AppSoftwareVersion=%AppSoftwareVersionLineContent:~27,5%

@REM echo %AppSoftwareVersionLineContent%
@REM echo %AppSoftwareVersion%
@REM pause

cd %Path0%
set Folder0="Ver%AppSoftwareVersion%_%Date:~3,4%_%Date:~8,2%_%Date:~11,2%"
set Folder1="APP"

rmdir /s /q %Folder0%
mkdir %Folder0%\%Folder1%

set AppMapFile="..\01 Source\App\Listings\BS9x44AMxx_Demo.map"
set AppHexFile="..\01 Source\App\Objects\BS9x44AMxx_Demo.hex"
set AppAxfFile="..\01 Source\App\Objects\BS9x44AMxx_Demo.axf"

set AppMapFileName="App_Demo"
set AppHexFileName="App_Demo"
set AppAxfFileName="App_Demo"

copy %AppMapFile% %Folder0%\%Folder1%\%AppMapFileName%_%AppSoftwareVersion%".map"
copy %AppHexFile% %Folder0%\%Folder1%\%AppHexFileName%_%AppSoftwareVersion%".hex"
copy %AppAxfFile% %Folder0%\%Folder1%\%AppAxfFileName%_%AppSoftwareVersion%".axf"

pause
