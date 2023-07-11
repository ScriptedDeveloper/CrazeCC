#include "lexer/lexer.hpp"
#include "parser/parser.hpp"

int main(int argc, char **argv) {
	if(argc == 1) {
		std::cerr << "No source files specified." << std::endl << "./" << argv[0] << " <source_file>" << std::endl;
		std::exit(1);
	}
	lexer l(argv[1]);
	l.parse();
	parser p(std::move(l.lexer_vec));
	return 0;
}
