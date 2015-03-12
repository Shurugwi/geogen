/** \file
 *  This C header file was generated by $ANTLR version 3.5.1
 *
 *     -  From the grammar source file : C:\\Users\\matej_000\\Source\\Repos\\geogen\\src\\GeoGen\\Grammar\\GeoGenScriptDecls.g
 *     -                            On : 2015-03-09 21:33:55
 *     -           for the tree parser : GeoGenScriptDeclsTreeParser
 *
 * Editing it, at least manually, is not wise.
 *
 * C language generator and runtime by Jim Idle, jimi|hereisanat|idle|dotgoeshere|ws.
 *
 *
 * The tree parser 
GeoGenScriptDecls

has the callable functions (rules) shown below,
 * which will invoke the code for the associated rule in the source grammar
 * assuming that the input stream is pointing to a token/text stream that could begin
 * this rule.
 *
 * For instance if you call the first (topmost) rule in a parser grammar, you will
 * get the results of a full parse, but calling a rule half way through the grammar will
 * allow you to pass part of a full token stream to the parser, such as for syntax checking
 * in editors and so on.
 *
 * The parser entry points are called indirectly (by function pointer to function) via
 * a parser context typedef pGeoGenScriptDecls, which is returned from a call to GeoGenScriptDeclsNew().
 *
 * The methods in pGeoGenScriptDecls are  as follows:
 *
 *  - 
 void
      pGeoGenScriptDecls->script(pGeoGenScriptDecls)
 *  - 
 void
      pGeoGenScriptDecls->metadata(pGeoGenScriptDecls)
 *  - 
 MetadataValue*
      pGeoGenScriptDecls->metadataKeyValueCollection(pGeoGenScriptDecls)
 *  - 
 MetadataValue*
      pGeoGenScriptDecls->metadataList(pGeoGenScriptDecls)
 *  - 
 GeoGenScriptDecls_metadataKeyValuePair_return
      pGeoGenScriptDecls->metadataKeyValuePair(pGeoGenScriptDecls)
 *  - 
 MetadataValue*
      pGeoGenScriptDecls->metadataKeyValueValue(pGeoGenScriptDecls)
 *  - 
 void
      pGeoGenScriptDecls->declaration(pGeoGenScriptDecls)
 *  - 
 void
      pGeoGenScriptDecls->enumDeclaration(pGeoGenScriptDecls)
 *  - 
 map<String, int>
      pGeoGenScriptDecls->enumValues(pGeoGenScriptDecls)
 *  - 
 void
      pGeoGenScriptDecls->functionDeclaration(pGeoGenScriptDecls)
 *  - 
 CodeBlock*
      pGeoGenScriptDecls->block(pGeoGenScriptDecls)
 *  - 
 CodeBlock*
      pGeoGenScriptDecls->statement(pGeoGenScriptDecls)
 *  - 
 CodeBlock*
      pGeoGenScriptDecls->blockStatement(pGeoGenScriptDecls)
 *  - 
 CodeBlock*
      pGeoGenScriptDecls->variableDeclaration(pGeoGenScriptDecls)
 *  - 
 CodeBlock*
      pGeoGenScriptDecls->globalVariableDeclaration(pGeoGenScriptDecls)
 *  - 
 CodeBlock*
      pGeoGenScriptDecls->yieldStatement(pGeoGenScriptDecls)
 *  - 
 CodeBlock*
      pGeoGenScriptDecls->returnStatement(pGeoGenScriptDecls)
 *  - 
 CodeBlock*
      pGeoGenScriptDecls->whileStatement(pGeoGenScriptDecls)
 *  - 
 CodeBlock*
      pGeoGenScriptDecls->forStatement(pGeoGenScriptDecls)
 *  - 
 CodeBlock*
      pGeoGenScriptDecls->initExpression(pGeoGenScriptDecls)
 *  - 
 CodeBlock*
      pGeoGenScriptDecls->ifStatement(pGeoGenScriptDecls)
 *  - 
 void
      pGeoGenScriptDecls->switchStatement(pGeoGenScriptDecls)
 *  - 
 void
      pGeoGenScriptDecls->normalCase(pGeoGenScriptDecls)
 *  - 
 void
      pGeoGenScriptDecls->defaultCase(pGeoGenScriptDecls)
 *  - 
 GeoGenScriptDecls_expression_return
      pGeoGenScriptDecls->expression(pGeoGenScriptDecls)
 *  - 
 CodeBlock*
      pGeoGenScriptDecls->coordinateExpression(pGeoGenScriptDecls)
 *  - 
 CodeBlock*
      pGeoGenScriptDecls->callExpression(pGeoGenScriptDecls)
 *  - 
 GeoGenScriptDecls_indexAccessExpression_return
      pGeoGenScriptDecls->indexAccessExpression(pGeoGenScriptDecls)
 *  - 
 CodeBlock*
      pGeoGenScriptDecls->conditionalOperatorExpression(pGeoGenScriptDecls)
 *  - 
 CodeBlock*
      pGeoGenScriptDecls->lvalueExpression(pGeoGenScriptDecls)
 *  - 
 CodeBlock*
      pGeoGenScriptDecls->lvalueIndexAccessExpression(pGeoGenScriptDecls)
 *  - 
 CodeBlock*
      pGeoGenScriptDecls->collectionLiteral(pGeoGenScriptDecls)
 *  - 
 CodeBlock*
      pGeoGenScriptDecls->collectionLiteralItem(pGeoGenScriptDecls)
 *  - 
 void
      pGeoGenScriptDecls->label(pGeoGenScriptDecls)
 *  - 
 GeoGenScriptDecls_stringLiteral_return
      pGeoGenScriptDecls->stringLiteral(pGeoGenScriptDecls)
 *
 * The return type for any particular rule is of course determined by the source
 * grammar file.
 */
// [The "BSD license"]
// Copyright (c) 2005-2009 Jim Idle, Temporal Wave LLC
// http://www.temporal-wave.com
// http://www.linkedin.com/in/jimidle
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
// 1. Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
// 3. The name of the author may not be used to endorse or promote products
//    derived from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
// IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
// OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
// IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
// INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
// NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
// THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#ifndef	_GeoGenScriptDecls_H
#define _GeoGenScriptDecls_H
/* =============================================================================
 * Standard antlr3 C runtime definitions
 */
#include    <antlr3.h>

/* End of standard antlr 3 runtime definitions
 * =============================================================================
 */

	#include <memory>
	#include <limits>	

	#undef max

	#include "../../compiler/UnexpectedTokenException.hpp"
	#include "../../compiler/UnexpectedCharacterException.hpp"
	#include "../../compiler/InvalidEscapeSequenceException.hpp"
	#include "../../compiler/ReadOnlyExpressionWriteException.hpp"
	#include "../../compiler/InvalidSymbolDefinitionException.hpp"
	#include "../../compiler/SymbolRedefinitionException.hpp"
	#include "../../compiler/MetadataKeyRedefinitionException.hpp"
	#include "../../compiler/InvalidReturnException.hpp"
	#include "../../compiler/InvalidContinueException.hpp"
	#include "../../compiler/InvalidBreakException.hpp"
	#include "../../runtime/CompiledScript.hpp"
	#include "../../runtime/CodeBlock.hpp"
	#include "../../runtime/MetadataBoolean.hpp"
	#include "../../runtime/MetadataIdentifier.hpp"
	#include "../../runtime/MetadataKeyValueCollection.hpp"
	#include "../../runtime/MetadataList.hpp"
	#include "../../runtime/MetadataNumber.hpp"
	#include "../../runtime/MetadataString.hpp"
	#include "../../runtime/MetadataValue.hpp"
	#include "../../runtime/ScriptFunctionDefinition.hpp"
	#include "../../corelib/EnumTypeDefinition.hpp"
	#include "../../corelib/ArrayTypeDefinition.hpp"
	#include "../../compiler/Compiler.hpp"
	#include "../../runtime/instructions/CallGlobalInstruction.hpp"
	#include "../../runtime/instructions/BreakInstruction.hpp"
	#include "../../runtime/instructions/CallBlockInstruction.hpp"
	#include "../../runtime/instructions/CallGlobalInstruction.hpp"
	#include "../../runtime/instructions/CallMemberInstruction.hpp"
	#include "../../runtime/instructions/ContinueInstruction.hpp"
	#include "../../runtime/instructions/DeclareGlobalValueInstruction.hpp"
	#include "../../runtime/instructions/DeclareLocalValueInstruction.hpp"
	#include "../../runtime/instructions/IfInstruction.hpp"
	#include "../../runtime/instructions/LoadConstBooleanInstruction.hpp"
	#include "../../runtime/instructions/LoadConstNumberInstruction.hpp"
	#include "../../runtime/instructions/LoadConstStringInstruction.hpp"
	#include "../../runtime/instructions/LoadMemberValueInstruction.hpp"
	#include "../../runtime/instructions/LoadNullInstruction.hpp"
	#include "../../runtime/instructions/LoadScopeReferenceInstruction.hpp"
	#include "../../runtime/instructions/LoadScopeValueInstruction.hpp"
	#include "../../runtime/instructions/PopInstruction.hpp"
	#include "../../runtime/instructions/StoreGlobalValueInstruction.hpp"
	#include "../../runtime/instructions/StoreMemberValueInstruction.hpp"
	#include "../../runtime/instructions/StoreReferenceInstruction.hpp"
	#include "../../runtime/instructions/StoreScopeValueInstruction.hpp"
	#include "../../runtime/instructions/WhileInstruction.hpp"
	#include "../../runtime/instructions/YieldAsNamedInstruction.hpp"
	#include "../../renderer/Renderer.hpp"

	using namespace std;
	using namespace geogen;
	using namespace geogen::compiler;
	using namespace geogen::corelib;
	using namespace geogen::runtime;	
		
	void handleTreeWalkerError (pANTLR3_BASE_RECOGNIZER recognizer, pANTLR3_UINT8 * tokenNames);


#ifdef __cplusplus
extern "C" {
#endif

// Forward declare the context typedef so that we can use it before it is
// properly defined. Delegators and delegates (from import statements) are
// interdependent and their context structures contain pointers to each other
// C only allows such things to be declared if you pre-declare the typedef.
//
typedef struct GeoGenScriptDecls_Ctx_struct GeoGenScriptDecls, * pGeoGenScriptDecls;



#ifdef	ANTLR3_WINDOWS
// Disable: Unreferenced parameter,							- Rules with parameters that are not used
//          constant conditional,							- ANTLR realizes that a prediction is always true (synpred usually)
//          initialized but unused variable					- tree rewrite variables declared but not needed
//          Unreferenced local variable						- lexer rule declares but does not always use _type
//          potentially unitialized variable used			- retval always returned from a rule
//			unreferenced local function has been removed	- susually getTokenNames or freeScope, they can go without warnigns
//
// These are only really displayed at warning level /W4 but that is the code ideal I am aiming at
// and the codegen must generate some of these warnings by necessity, apart from 4100, which is
// usually generated when a parser rule is given a parameter that it does not use. Mostly though
// this is a matter of orthogonality hence I disable that one.
//
#pragma warning( disable : 4100 )
#pragma warning( disable : 4101 )
#pragma warning( disable : 4127 )
#pragma warning( disable : 4189 )
#pragma warning( disable : 4505 )
#pragma warning( disable : 4701 )
#endif
typedef struct GeoGenScriptDecls_metadataKeyValuePair_return_struct
{
    pANTLR3_BASE_TREE       start;
    pANTLR3_BASE_TREE       stop;
    String name;
    MetadataValue* value;
}
    GeoGenScriptDecls_metadataKeyValuePair_return;



typedef struct GeoGenScriptDecls_expression_return_struct
{
    pANTLR3_BASE_TREE       start;
    pANTLR3_BASE_TREE       stop;
    CodeBlock* returnCodeBlock;
    CodeBlock* refCodeBlock;
}
    GeoGenScriptDecls_expression_return;



typedef struct GeoGenScriptDecls_indexAccessExpression_return_struct
{
    pANTLR3_BASE_TREE       start;
    pANTLR3_BASE_TREE       stop;
    CodeBlock* returnCodeBlock;
    CodeBlock* refCodeBlock;
}
    GeoGenScriptDecls_indexAccessExpression_return;



typedef struct GeoGenScriptDecls_stringLiteral_return_struct
{
    pANTLR3_BASE_TREE       start;
    pANTLR3_BASE_TREE       stop;
    String value;
    int line;
    int pos;
}
    GeoGenScriptDecls_stringLiteral_return;




/* globalAttributeScopeDecl(scope)
 */
/* makeScopeSet()
 */
 /** Definition of the BlockScope scope variable tracking
 *  structure. An instance of this structure is created by calling
 *  GeoGenScriptDecls_BlockScopePush().
 */
typedef struct  GeoGenScriptDecls_BlockScope_SCOPE_struct
{
    /** Function that the user may provide to be called when the
     *  scope is destroyed (so you can free pANTLR3_HASH_TABLES and so on)
     *
     * \param POinter to an instance of this typedef/struct
     */
    void    (ANTLR3_CDECL *free)	(struct GeoGenScriptDecls_BlockScope_SCOPE_struct * frame);

    /* =============================================================================
     * Programmer defined variables...
     */
    int breakCodeBlockLevel;
    int continueCodeBlockLevel;

    /* End of programmer defined variables
     * =============================================================================
     */
}
    GeoGenScriptDecls_BlockScope_SCOPE, * pGeoGenScriptDecls_BlockScope_SCOPE;


/** Context tracking structure for 
GeoGenScriptDecls

 */
struct GeoGenScriptDecls_Ctx_struct
{
    /** Built in ANTLR3 context tracker contains all the generic elements
     *  required for context tracking.
     */
    pANTLR3_TREE_PARSER	    pTreeParser;

    /* globalAttributeScopeDef(scope)
     */
    /** Pointer to the  BlockScope stack for use by pGeoGenScriptDecls_BlockScopePush()
     *  and pGeoGenScriptDecls_BlockScopePop()
     */
    pANTLR3_STACK pGeoGenScriptDecls_BlockScopeStack;
    ANTLR3_UINT32 pGeoGenScriptDecls_BlockScopeStack_limit;
    /** Pointer to the top of the stack for the global scope pGeoGenScriptDecls_BlockScopeStack
     */
    pGeoGenScriptDecls_BlockScope_SCOPE    (*pGeoGenScriptDecls_BlockScopePush)(struct GeoGenScriptDecls_Ctx_struct * ctx);
    pGeoGenScriptDecls_BlockScope_SCOPE    pGeoGenScriptDecls_BlockScopeTop;




     void
     (*script)	(struct GeoGenScriptDecls_Ctx_struct * ctx);

     void
     (*metadata)	(struct GeoGenScriptDecls_Ctx_struct * ctx);

     MetadataValue*
     (*metadataKeyValueCollection)	(struct GeoGenScriptDecls_Ctx_struct * ctx);

     MetadataValue*
     (*metadataList)	(struct GeoGenScriptDecls_Ctx_struct * ctx);

     GeoGenScriptDecls_metadataKeyValuePair_return
     (*metadataKeyValuePair)	(struct GeoGenScriptDecls_Ctx_struct * ctx);

     MetadataValue*
     (*metadataKeyValueValue)	(struct GeoGenScriptDecls_Ctx_struct * ctx);

     void
     (*declaration)	(struct GeoGenScriptDecls_Ctx_struct * ctx);

     void
     (*enumDeclaration)	(struct GeoGenScriptDecls_Ctx_struct * ctx);

     map<String, int>
     (*enumValues)	(struct GeoGenScriptDecls_Ctx_struct * ctx);

     void
     (*functionDeclaration)	(struct GeoGenScriptDecls_Ctx_struct * ctx);

     CodeBlock*
     (*block)	(struct GeoGenScriptDecls_Ctx_struct * ctx);

     CodeBlock*
     (*statement)	(struct GeoGenScriptDecls_Ctx_struct * ctx);

     CodeBlock*
     (*blockStatement)	(struct GeoGenScriptDecls_Ctx_struct * ctx);

     CodeBlock*
     (*variableDeclaration)	(struct GeoGenScriptDecls_Ctx_struct * ctx);

     CodeBlock*
     (*globalVariableDeclaration)	(struct GeoGenScriptDecls_Ctx_struct * ctx);

     CodeBlock*
     (*yieldStatement)	(struct GeoGenScriptDecls_Ctx_struct * ctx);

     CodeBlock*
     (*returnStatement)	(struct GeoGenScriptDecls_Ctx_struct * ctx);

     CodeBlock*
     (*whileStatement)	(struct GeoGenScriptDecls_Ctx_struct * ctx);

     CodeBlock*
     (*forStatement)	(struct GeoGenScriptDecls_Ctx_struct * ctx);

     CodeBlock*
     (*initExpression)	(struct GeoGenScriptDecls_Ctx_struct * ctx);

     CodeBlock*
     (*ifStatement)	(struct GeoGenScriptDecls_Ctx_struct * ctx);

     void
     (*switchStatement)	(struct GeoGenScriptDecls_Ctx_struct * ctx);

     void
     (*normalCase)	(struct GeoGenScriptDecls_Ctx_struct * ctx);

     void
     (*defaultCase)	(struct GeoGenScriptDecls_Ctx_struct * ctx);

     GeoGenScriptDecls_expression_return
     (*expression)	(struct GeoGenScriptDecls_Ctx_struct * ctx);

     CodeBlock*
     (*coordinateExpression)	(struct GeoGenScriptDecls_Ctx_struct * ctx);

     CodeBlock*
     (*callExpression)	(struct GeoGenScriptDecls_Ctx_struct * ctx);

     GeoGenScriptDecls_indexAccessExpression_return
     (*indexAccessExpression)	(struct GeoGenScriptDecls_Ctx_struct * ctx);

     CodeBlock*
     (*conditionalOperatorExpression)	(struct GeoGenScriptDecls_Ctx_struct * ctx);

     CodeBlock*
     (*lvalueExpression)	(struct GeoGenScriptDecls_Ctx_struct * ctx);

     CodeBlock*
     (*lvalueIndexAccessExpression)	(struct GeoGenScriptDecls_Ctx_struct * ctx);

     CodeBlock*
     (*collectionLiteral)	(struct GeoGenScriptDecls_Ctx_struct * ctx);

     CodeBlock*
     (*collectionLiteralItem)	(struct GeoGenScriptDecls_Ctx_struct * ctx);

     void
     (*label)	(struct GeoGenScriptDecls_Ctx_struct * ctx);

     GeoGenScriptDecls_stringLiteral_return
     (*stringLiteral)	(struct GeoGenScriptDecls_Ctx_struct * ctx);
    // Delegated rules

    const char * (*getGrammarFileName)();
    void            (*reset)  (struct GeoGenScriptDecls_Ctx_struct * ctx);
    void	    (*free)   (struct GeoGenScriptDecls_Ctx_struct * ctx);

    	geogen::runtime::CompiledScript* compiledScript;
    	geogen::runtime::CodeBlock* rootCodeBlock;
    	
    	pANTLR3_BASE_TREE_ADAPTOR	adaptor;
    	pANTLR3_VECTOR_FACTORY		vectors;
    	
    	bool isInFunction;
    	bool isInLoop;
    	int codeBlockLevel;
    	std::vector<String> lines;

};

// Function protoypes for the constructor functions that external translation units
// such as delegators and delegates may wish to call.
//
ANTLR3_API pGeoGenScriptDecls GeoGenScriptDeclsNew         (
pANTLR3_COMMON_TREE_NODE_STREAM
 instream);
ANTLR3_API pGeoGenScriptDecls GeoGenScriptDeclsNewSSD      (
pANTLR3_COMMON_TREE_NODE_STREAM
 instream, pANTLR3_RECOGNIZER_SHARED_STATE state);

/** Symbolic definitions of all the tokens that the 
tree parser
 will work with.
 * \{
 *
 * Antlr will define EOF, but we can't use that as it it is too common in
 * in C header files and that would be confusing. There is no way to filter this out at the moment
 * so we just undef it here for now. That isn't the value we get back from C recognizers
 * anyway. We are looking for ANTLR3_TOKEN_EOF.
 */
#ifdef	EOF
#undef	EOF
#endif
#ifdef	Tokens
#undef	Tokens
#endif
#define EOF      -1
#define AS      4
#define BLOCK      5
#define BREAK      6
#define CASE      7
#define COLLECTION      8
#define COLLECTION_ITEM      9
#define COLON      10
#define COMMA      11
#define COMMENT      12
#define CONDITION_EXPRESSION      13
#define CONTINUE      14
#define COORDINATE      15
#define DECLARATIONS      16
#define DEFAULT      17
#define ELSE      18
#define ENUM      19
#define ESCAPED      20
#define FALSE_LIT      21
#define FOR      22
#define FUNCTION      23
#define GLOBAL      24
#define IDENTCHAIN      25
#define IDENTIFIER      26
#define IF      27
#define INCREMENT_EXPRESSION      28
#define INITIALIZATION_EXPRESSION      29
#define INTEGER      30
#define LEFT_BRACKET      31
#define LEFT_CURLY_BRACKET      32
#define LEFT_SQUARE_BRACKET      33
#define METADATA      34
#define NUMBER      35
#define OPERATOR_ASSIGN      36
#define OPERATOR_ASSIGN_AND      37
#define OPERATOR_ASSIGN_DIVIDE      38
#define OPERATOR_ASSIGN_LSHIFT      39
#define OPERATOR_ASSIGN_MINUS      40
#define OPERATOR_ASSIGN_MODULO      41
#define OPERATOR_ASSIGN_OR      42
#define OPERATOR_ASSIGN_PLUS      43
#define OPERATOR_ASSIGN_RSHIFT      44
#define OPERATOR_ASSIGN_TIMES      45
#define OPERATOR_ASSIGN_XOR      46
#define OPERATOR_BIT_AND      47
#define OPERATOR_BIT_OR      48
#define OPERATOR_BIT_XOR      49
#define OPERATOR_CALL      50
#define OPERATOR_DECREMENT      51
#define OPERATOR_DECREMENT_POST      52
#define OPERATOR_DECREMENT_PRE      53
#define OPERATOR_DIVIDE      54
#define OPERATOR_DOT      55
#define OPERATOR_EQUAL_TO      56
#define OPERATOR_EXPR_IF      57
#define OPERATOR_GREATER_THAN      58
#define OPERATOR_GREATER_THAN_OR_EQUAL_TO      59
#define OPERATOR_INCREMENT      60
#define OPERATOR_INCREMENT_POST      61
#define OPERATOR_INCREMENT_PRE      62
#define OPERATOR_IS      63
#define OPERATOR_LESS_THAN      64
#define OPERATOR_LESS_THAN_OR_EQUAL_TO      65
#define OPERATOR_LSHIFT      66
#define OPERATOR_MINUS      67
#define OPERATOR_MINUS_UN      68
#define OPERATOR_MODULO      69
#define OPERATOR_NOT      70
#define OPERATOR_NOT_EQUAL_TO      71
#define OPERATOR_PLUS      72
#define OPERATOR_PLUS_UN      73
#define OPERATOR_RELATIVE      74
#define OPERATOR_REL_AND      75
#define OPERATOR_REL_OR      76
#define OPERATOR_RSHIFT      77
#define OPERATOR_SUBSCRIPT      78
#define OPERATOR_TIMES      79
#define PARAMETERS      80
#define QUOTE      81
#define RETURN      82
#define RIGHT_BRACKET      83
#define RIGHT_CURLY_BRACKET      84
#define RIGHT_SQUARE_BRACKET      85
#define SCRIPT      86
#define SEMICOLON      87
#define SIMPLE_COLLECTION      88
#define STRING      89
#define SWITCH      90
#define TRUE_LIT      91
#define VALUES      92
#define VAR      93
#define WHILE      94
#define WHITESPACE      95
#define YIELD      96
#ifdef	EOF
#undef	EOF
#define	EOF	ANTLR3_TOKEN_EOF
#endif

#ifndef TOKENSOURCE
#define TOKENSOURCE(lxr) lxr->pLexer->rec->state->tokSource
#endif

/* End of token definitions for GeoGenScriptDecls
 * =============================================================================
 */
/** } */

#ifdef __cplusplus
}
#endif

#endif

/* END - Note:Keep extra line feed to satisfy UNIX systems */