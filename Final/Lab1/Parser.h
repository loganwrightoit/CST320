/***********************************************************
* Author:                   Logan Wright
* Date Created:             02/06/2015
* Last Modification Date:   03/04/2015
* Lab Number:               CST 320 Final Project
* Filename:                 Parser.h
*
* Overview:
*   Parses a file using a Mini-C grammar.
*
* Input:
*   None.
*
* Output:
*   None.
************************************************************/

#pragma once
#include "Tokenizer.h"
#include "SymbolTable.h"
#include <vector>

class Parser
{

    public:

        Parser();
        ~Parser();

        /**********************************************************************
        *   bool parse(std::vector<Tokenizer::Token> tokens)
        *
        *      Purpose: Parses a list of tokens using mini-c grammar rules.
        *
        *        Entry: tokens - a list of Tokenizer::Token's.
        *
        *         Exit: Boolean true if file parses completely.
        *
        *               Boolean false if parser error encountered.
        **********************************************************************/

        bool parse(std::vector<Tokenizer::Token> tokens);

    private:

        // FUNCTION → TYPE Identifier ( ARG_LIST ) COMPOUND_STMT
        bool function(int level);

        // ARG_LIST → ARG ARG_LIST2 | λ
        bool arg_list(int level);

        // ARG_LIST2 → , ARG ARG_LIST2 | λ
        bool arg_list2(int level);

        // ARG → TYPE Identifier
        bool arg(int level);

        // DECLARATION → TYPE IDENT_LIST ;
        bool declaration(int level);

        // TYPE → int | float | bool | string
        bool type(int level);

        // IDENT_LIST → Identifier IDENT_LIST2
        bool ident_list(int level);

        // IDENT_LIST2 → ( EXPRESSION ) IDENT_LIST3 | = EXPRESSION IDENT_LIST3 | IDENT_LIST3
        bool ident_list2(int level);

        // IDENT_LIST3 → , IDENT_LIST | λ
        bool ident_list3(int level);

        // STATEMENT → FOR_STMT | WHILE_STMT | EXPRESSION ; | STREAM_STMT ; | IF_STMT | COMPOUND_STMT | DECLARATION | ;
        bool statement(int level);

        // FOR_STMT → for ( DECLARATION OPT_EXPR ; OPT_EXPR ) STATEMENT
        bool for_stmt(int level);

        // OPT_EXPR → EXPRESSION | λ
        bool opt_expr(int level);

        // WHILE_STMT → while ( EXPRESSION ) STATEMENT
        bool while_stmt(int level);

        // IF_STMT → if ( EXPRESSION ) STATEMENT ELSEPART
        bool if_stmt(int level);

        // ELSEPART → else STATEMENT | λ
        bool elsepart(int level);

        // COMPOUND_STMT → { STMT_LIST }
        bool compound_stmt(int level);

        // STMT_LIST → STATEMENT STMT_LIST | λ
        bool stmt_list(int level);

        // EXPRESSION → Identifier = EXPRESSION | RVALUE
        bool expression(int level);

        // STREAM_STMT → cout << EXPRESSION OSTREAM_LIST | cin >> Identifier
        bool stream_stmt(int level);

        // OSTREAM_LIST → << EXPRESSION OSTREAM_LIST | λ
        bool ostream_list(int level);

        // RVALUE → MAG RVALUE2
        bool rvalue(int level);

        // RVALUE2 → COMPARE MAG RVALUE2 | λ
        bool rvalue2(int level);

        // COMPARE → == | < | > | <= | >= | !=
        bool compare(int level);

        // MAG → TERM MAG2
        bool mag(int level);

        // MAG2 → + TERM MAG2 | - TERM MAG2 | λ
        bool mag2(int level);

        // TERM → FACTOR TERM2
        bool term(int level);

        // TERM2 → * FACTOR TERM2 | / FACTOR TERM2 | % FACTOR TERM2 | λ
        bool term2(int level);

        // FACTOR → ( EXPRESSION ) | - FACTOR | + FACTOR | Identifier FACTOR2 | VALUE
        bool factor(int level);

        //FACTOR2 → ++ | -- | λ
        bool factor2(int level);

        // VALUE → Integer | Float | String | Boolean
        bool value(int level);

        // Helper function that returns true if equality exists at current position, incrementing iterator if true
        bool equals(Tokenizer::TokenType type);

        // Helper function that returns true if equality exists at current position, incrementing iterator if true
        bool equals(char* input);

        // Prints syntax error with expected token value
        void expected(char* expected);

        // Prints syntax error with expected token value
        void error(char* error);

        // Adds symbol to symbol table
        void addSymbol(Symbol::Use use);

        // Stores current position in tokens vector
        std::vector<Tokenizer::Token>::iterator token, end;

        // Stores last type for IDENT_LIST declarations
        Symbol::Type lastType;

        // Stores symbols
        SymbolTable symbolTable;

        // Stores output for parser tree in pair <indent, string>
        std::vector<std::pair<int, std::string>> output;

        // Adds parse branch for output later
        void addBranch(int level, std::string inStr);

        // Removes last branch from parse tree
        void popBranch();

        // State used when defining identifiers
        bool definition;

};

