#include "lexer/lexer.hpp"
#include "parser/parser.hpp"
#include "code_gen/code_gen.hpp"
#include "exception/exception.hpp"
#include "preprocessor/preprocessor.hpp"

int main(int argc, char **argv) {
	std::string file_name{}, output{};
	if(argc != 1) {
		if(argc == 1) {
			std::cerr << "No source files specified." << std::endl << "./" << argv[0] << " <source_file>" << std::endl;
			std::exit(1);
		} else if(argc <= 3 && std::string_view(argv[1]) != "-d") {
			std::cerr << "Expected output file! Specifiy with -o" << std::endl;
			std::exit(1);
		}	
		file_name = argv[1];
		output = argv[3];
	} else {
		file_name = "asd";
		output = "erd";
	}
	lexer l(file_name);
	l.parse();
	preprocessor prep(l.lexer_vec);
	prep.init();
	parser p(std::move(l.lexer_vec));
	code_generator c(p.ast_vec, output);
	int ret = c.init();
	if(ret != code_generator::GENERATE_SUCCESS)
		exception(ret, -1);
	return 0;
}
