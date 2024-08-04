CC  = gcc
CXX = g++
G = Ninja
J = 6
EXE = opengl_flight_simulator

ifeq ($(OS), Windows_NT)
RUN = build\$(EXE).exe
else
RUN = ./build/$(EXE)
endif

run: compile
	$(RUN)

compile: prepare
	cmake --build ./build -j $(J)
prepare: 
	cmake -B ./build -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=TRUE -DCMAKE_BUILD_TYPE:STRING=Debug "-DCMAKE_C_COMPILER:FILEPATH=$(CC)" "-DCMAKE_CXX_COMPILER:FILEPATH=$(CXX)" -G "$(G)"

clean:
	cd build & git clean -d -f -x