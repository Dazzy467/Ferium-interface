DEBUG_FLAG = -fdiagnostics-color=always -g -DDEBUG
RELEASE_FLAG = -static -s -O3 -Wl,--subsystem,windows
DLL_PATH = C:\Program Files (x86)\wxWidgets\lib\gcc_x64_dll

DEBUG_INCLUDE_PATH = 	-I "include" \
						-I "C:\Program Files (x86)\wxWidgets\include" \
						-I "C:\Program Files (x86)\wxWidgets\lib\gcc_x64_dll\mswud"

LINKING_DEBUG = 	-L "C:\Program Files (x86)\wxWidgets\lib\gcc_x64_dll" \
					-l "wxmsw32ud_core" \
					-l "wxbase32ud"

RELEASE_INCLUDE_PATH = 	-I "include" \
						-I "C:\Program Files (x86)\wxWidgets\include" \
						-I "C:\Program Files (x86)\wxWidgets\lib\gcc_x64_dll\mswu"

LINKING_RELEASE = 	-L "C:\Program Files (x86)\wxWidgets\lib\gcc_x64_dll" \
					-l "wxmsw32u_core" \
					-l "wxbase32u"

DEBUG_INCLUDE_PATH_STATIC = 	-I "include" \
								-I "C:\Program Files (x86)\wxWidgets-static\include" \
								-I "C:\Program Files (x86)\wxWidgets-static\lib\gcc_x64_lib\mswud"
								
RELEASE_INCLUDE_PATH_STATIC = 	-I "include" \
								-I "C:\Program Files (x86)\wxWidgets-static\include" \
								-I "C:\Program Files (x86)\wxWidgets-static\lib\gcc_x64_lib\mswu"

LINKING_DEBUG_STATIC = 		-L "C:\Program Files (x86)\wxWidgets-static\lib\gcc_x64_lib" \
							-l "wxmsw32ud_core" \
							-l "wxbase32ud" \
							-lwxmsw32ud_aui \
							-lwxpngd \
							-lwxzlibd \
							-lwxjpegd \
							-lwxtiffd \
							-lwxscintillad \
							-lwxregexud \
							-lwxexpatd \
							-L "C:\msys64\ucrt64\lib" \
							-lkernel32 \
							-luser32 \
							-lgdi32 \
							-lwinspool \
							-lcomdlg32 \
							-ladvapi32 \
							-lshell32 \
							-lole32 \
							-loleaut32 \
							-luuid \
							-lcomctl32 \
							-lwsock32 \
							-lodbc32 \
							-lshlwapi \
							-lversion \
							-luxtheme \
							-loleacc \

LINKING_RELEASE_STATIC = 	-L "C:\Program Files (x86)\wxWidgets-static\lib\gcc_x64_lib" \
							-l "wxmsw32u_core" \
							-l "wxbase32u" \
							-lwxmsw32u_aui \
							-lwxpng \
							-lwxzlib \
							-lwxjpeg \
							-lwxtiff \
							-lwxscintilla \
							-lwxregexu \
							-lwxexpat \
							-L "C:\msys64\ucrt64\lib" \
							-lkernel32 \
							-luser32 \
							-lgdi32 \
							-lwinspool \
							-lcomdlg32 \
							-ladvapi32 \
							-lshell32 \
							-lole32 \
							-loleaut32 \
							-luuid \
							-lcomctl32 \
							-lwsock32 \
							-lodbc32 \
							-lshlwapi \
							-lversion \
							-luxtheme \
							-loleacc \


all: debug release
debug: build/debug/debug.exe build/debug/wxbase32ud_gcc_custom.dll build/debug/wxmsw32ud_core_gcc_custom.dll
release: build/release/release.exe build/debug/wxbase32u_gcc_custom.dll build/debug/wxmsw32u_core_gcc_custom.dll

debug-static: build/debug-static/debug.exe
release-static: build/release-static/release.exe


build/debug/wxbase32ud_gcc_custom.dll: build/debug/debug.exe
	xcopy /D /Y "$(DLL_PATH)\wxbase32ud_gcc_custom.dll" "build\debug\"

build/debug/wxmsw32ud_core_gcc_custom.dll: build/debug/debug.exe
	xcopy /D /Y "$(DLL_PATH)\wxmsw32ud_core_gcc_custom.dll" "build\debug\"

build/debug/wxbase32u_gcc_custom.dll: build/release/release.exe
	xcopy /D /Y "$(DLL_PATH)\wxbase32u_gcc_custom.dll" "build\release\"

build/debug/wxmsw32u_core_gcc_custom.dll: build/release/release.exe
	xcopy /D /Y "$(DLL_PATH)\wxmsw32u_core_gcc_custom.dll" "build\release\"

build/debug/debug.exe: src/main.cpp
	g++ $(DEBUG_FLAG) $< $(DEBUG_INCLUDE_PATH) $(LINKING_DEBUG) -o $@
	
build/release/release.exe: src/main.cpp
	g++ $(RELEASE_FLAG) $< $(RELEASE_INCLUDE_PATH) $(LINKING_RELEASE) -o $@

build/debug-static/debug.exe: src/main.cpp
	g++ $(DEBUG_FLAG) $< $(DEBUG_INCLUDE_PATH_STATIC) $(LINKING_DEBUG_STATIC) -o $@

build/release-static/release.exe: src/main.cpp
	g++ $(RELEASE_FLAG) $< $(RELEASE_INCLUDE_PATH_STATIC) $(LINKING_RELEASE_STATIC) -o $@