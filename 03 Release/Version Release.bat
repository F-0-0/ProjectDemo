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
set Folder0="V %AppSoftwareVersion% %Date:~3,4%_%Date:~8,2%_%Date:~11,2%"

rmdir /s /q %Folder0%
mkdir %Folder0%

set MapFile="..\01 Source\Listings\ProjectDemo.map"
set HexFile="..\01 Source\Objects\ProjectDemo.hex"
set AxfFile="..\01 Source\Objects\ProjectDemo.axf"

set MapFileName="ProjectDemo"
set HexFileName="ProjectDemo"
set AxfFileName="ProjectDemo"

copy %MapFile% %Folder0%\%Folder1%\%MapFileName%_V%AppSoftwareVersion%".map"
copy %HexFile% %Folder0%\%Folder1%\%HexFileName%_V%AppSoftwareVersion%".hex"
copy %AxfFile% %Folder0%\%Folder1%\%AxfFileName%_V%AppSoftwareVersion%".axf"

pause
