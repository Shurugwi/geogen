#include "Compiler.hpp"

#include "../Grammar/output/GeoGenScriptLexer.h"
#include "../Grammar/output/GeoGenScriptParser.h"

#include "../antlr3/antlr3.h"

using namespace std;
using namespace geogen;
using namespace geogen::compiler;

Compiler::Compiler(){}

CompiledScript Compiler::CompileScript(std::string& code)
{
	pANTLR3_INPUT_STREAM input;
	pGeoGenScriptLexer lex;
	pANTLR3_COMMON_TOKEN_STREAM tokens;
	pGeoGenScriptParser parser;

	input = antlr3StringStreamNew((pANTLR3_UINT8)code.c_str(), ANTLR3_ENC_8BIT, code.length(), (pANTLR3_UINT8)"");
	lex = GeoGenScriptLexerNew(input);	
	tokens = antlr3CommonTokenStreamSourceNew(ANTLR3_SIZE_HINT, TOKENSOURCE(lex));
	//pANTLR3_LIST list = antlr3ListNew(5);
	//tokens->getTokensList(tokens, 0, 1, list);
	parser = GeoGenScriptParserNew(tokens);

	GeoGenScriptParser_script_return r = parser->script(parser);

	pANTLR3_BASE_TREE tree = r.tree;

	/*ExprTreeEvaluator eval;
	int rr = eval.run(tree);
	cout << "Evaluator result: " << rr << '\n';*/

	parser->free(parser);
	tokens->free(tokens);
	lex->free(lex);
	input->close(input);


	return CompiledScript();
}