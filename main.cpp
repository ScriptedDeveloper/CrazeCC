#include "lexer/lexer.hpp"
#include "parser/parser.hpp"
#include "code_gen/code_gen.hpp"

int main(int argc, char **argv) {
	if(argc == 1) {
		std::cerr << "No source files specified." << std::endl << "./" << argv[0] << " <source_file>" << std::endl;
		std::exit(1);
	}
	auto file_name = argv[1];
	lexer l(file_name);
	l.parse();
	parser p(std::move(l.lexer_vec));
	code_generator c(p.ast_vec, file_name);
	c.init();
	return 0;
}
