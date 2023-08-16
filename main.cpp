#include "compiler/compiler.hpp"

int main(int argc, char **argv) {
	compiler<false> c(argc, argv);
	c.run();
	return 0;
}
