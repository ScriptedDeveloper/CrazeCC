#pragma once
#include <iostream>

namespace AST_preprocessor {
	class include {
		public:
			include() {

			};
			virtual ~include() {
			};
			inline void define_keyword() {
				__defined_keyword = true;
			}
			inline bool defined_keyword() {
				return __defined_keyword;
			}
		private:
			bool __defined_keyword{false};
	};
};
