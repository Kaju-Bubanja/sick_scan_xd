REM 
REM Run sick_scan on ROS-2 Windows with simple test server
REM 

if exist "c:\dev\ros2_foxy\local_setup.bat" ( call C:\dev\ros2_foxy\local_setup.bat )
if exist "c:\opt\ros\foxy\x64\setup.bat" ( call c:\opt\ros\foxy\x64\setup.bat )
set PATH=c:\vcpkg\installed\x64-windows\bin;%PATH%

pushd ..\..\..\..
call .\install\setup.bat
rem start "ros2 echo cloud" ros2 topic echo /cloud
start "rviz2" rviz2 -d ./src/sick_scan_xd/test/emulator/config/rviz_emulator_cfg_ros2.rviz
@timeout /t 5

REM 
REM Run test server
REM 

start "test_server" ros2 run sick_scan test_server ./src/sick_scan_xd/tools/test_server/config/test_server_cola.launch
@timeout /t 1

REM 
REM Run sick_scan on ROS-2 Windows
REM 

ros2 run sick_scan sick_generic_caller ./src/sick_scan_xd/launch/sick_tim_240.launch hostname:=127.0.0.1 port:=2112 sw_pll_only_publish:=False

@pause
popd
