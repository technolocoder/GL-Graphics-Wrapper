include_path = include/
src_path = src/
build_path = build/
lib_path = lib/
bin_path = bin/
test_path = test/

object_files = $(build_path)Camera.o $(build_path)Mesh.o $(build_path)Model.o $(build_path)Program.o $(build_path)Shader.o $(build_path)Texture.o 

.PHONY : run_test
run_test : $(bin_path)Test_Executable
	./$(bin_path)Test_Executable

$(bin_path)Test_Executable : $(build_path)Test_Executable.o
	g++ $(build_path)Test_Executable.o -L$(lib_path) -lGL_Wrapper -o$(bin_path)Test_Executable 

$(build_path)Test_Executable.o : $(lib_path)libGL_Wrapper
	g++ $(test_path)main.cpp -O3 -c -o$(build_path)Test_Executable.o -I$(include_path)

.PHONY : build_library
build_library : $(lib_path)libGL_Wrapper

$(lib_path)libGL_Wrapper : $(object_files)
	ar crf $(lib_path)libGL_Wrapper.a $(object_files)

$(build_path)Camera.o : $(src_path)Camera.cpp $(include_path)Camera.hpp
	g++ $(src_path)Camera.cpp -c -o$(build_path)Camera.o -O3 -I$(include_path)

$(build_path)Mesh.o : $(src_path)Mesh.cpp $(include_path)Mesh.hpp
	g++ $(src_path)Mesh.cpp -c -o$(build_path)Mesh.o -O3 -I$(include_path)

$(build_path)Model.o : $(src_path)Model.cpp $(include_path)Model.hpp
	g++ $(src_path)Model.cpp -c -o$(build_path)Model.o -O3 -I$(include_path)

$(build_path)Program.o : $(src_path)Program.cpp $(include_path)Program.hpp
	g++ $(src_path)Program.cpp -c -o$(build_path)Program.o -O3 -I$(include_path)

$(build_path)Shader.o : $(src_path)Shader.cpp $(include_path)Shader.hpp
	g++ $(src_path)Shader.cpp -c -o$(build_path)Shader.o -O3 -I$(include_path)

$(build_path)Texture.o : $(src_path)Texture.cpp $(include_path)Texture.hpp
	g++ $(src_path)Texture.cpp -c -o$(build_path)Texture.o -O3 -I$(include_path)

clean:
	rm -f $(object_files) $(lib_path)libGL_Wrapper.a $(bin_path)Test_Executable $(build_path)Test_Executable.o