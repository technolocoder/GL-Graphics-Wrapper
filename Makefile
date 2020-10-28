include_path = include/
src_path = src/
build_path = build/
lib_path = lib/

object_files = $(build_path)Camera.o $(build_path)Mesh.o $(build_path)Model.o $(build_path)Program.o $(build_path)Shader.o $(build_path)Texture.o 

.PHONY : build
build : $(object_files)
	ar crf $(lib_path)GL_Wrapper.a $(object_files)

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