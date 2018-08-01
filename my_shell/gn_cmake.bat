cd ..
REM gn gen out/config --ide=json --json-ide-script=../../gn/gn_to_cmake.py

REM cd out/config
REM cmake .

REM 作者：var_rain
REM 链接：https://www.jianshu.com/p/4225f4342d18

REM ## 在skia目录下执行 ## 
REM 构建静态库 > 
REM gn gen out/Static 
REM ## 构建动态库 
REM ## 因为Windows平台的编译器在导出动态库的时候会检查方法定义的通用性 
REM ## 所以编译过程中会报出 C4251 和 C4275 的警告, 只是警告而已, 
REM 所以我们在此处加上 extra_cflags=[\"/wd4251\",\"/wd4275\"] 参数来忽略此警告 > 

gn gen out/Shared --args="is_component_build=true is_official_build=true extra_cflags=[\"/wd4251\",\"/wd4275\"]" 

REM ## 构建Debug版本 > 
REM gn gen out/Debug 

REM ## 构建Release版本 > 
REM gn gen out/Release --args="is_debug=false" 

REM ## 当然还可以直接构建VisualStudio的.sln文件, 使用VisualStudio直接导入, 
REM 但是呢,经过我反复的试验, 我不推荐使用VisualStudio编译, 因为这玩意会报出很多莫名其妙的错误信息. > 
REM gn gen out/sln --ide=vs

ninja -C out/Shared
