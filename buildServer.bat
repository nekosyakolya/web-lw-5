set labDir=%CD%
cd /d c:\Program Files (x86)\Microsoft Visual Studio\2017\Community\Common7\IDE\
devenv "%labDir%\src\server\server.sln" /build Release
