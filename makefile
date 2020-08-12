mk: main.cpp mikilib.cpp
	g++-9 -std=c++17 $^ -o $@ -g