#include "compiler/compiler.hpp"

inline std::string get_file_content(std::string_view name) {
	std::ifstream ifs(name.data());
	std::stringstream ss{};
	ss << ifs.rdbuf();
	return ss.str();
}

int main(int argc, char **argv) {
	compiler<false> c(argc, argv);
	c.run();
	return 0;
}
