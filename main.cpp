#include "compiler/compiler.hpp"
#include "tests/test.hpp"

int main(int argc, char **argv) {
	compiler<false> c(argc, argv);
	c.run();
	return 0;
}
