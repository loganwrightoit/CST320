/***********************************************************
* Author:                   Logan Wright
* Date Created:             02/06/2015
* Last Modification Date:   02/06/2015
* Lab Number:               CST 320 Lab 2
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
#include <vector>

class Parser
{

    public:

        Parser();
        ~Parser();

        bool parse(std::vector<Tokenizer::Token> tokens);

    private:

        // FUNCTION → TYPE Identifier ( ARG_LIST ) COMPOUND_STMT
        bool function();

        // ARG_LIST → ARG ARG_LIST2 | λ
        bool arg_list();

        // ARG_LIST2 → , ARG ARG_LIST2 | λ
        bool arg_list2();

        // ARG → TYPE Identifier
        bool arg();

        // DECLARATION → TYPE IDENT_LIST ;
        bool declaration();

        // TYPE → int | float | bool | string
        bool type();

        // IDENT_LIST → Identifier IDENT_LIST2
        bool ident_list();

        // IDENT_LIST2 → = EXPRESSION IDENT_LIST3 | IDENT_LIST3
        bool ident_list2();

        // IDENT_LIST3 → , IDENT_LIST | λ
        bool ident_list3();

        // STATEMENT → FOR_STMT | WHILE_STMT | EXPRESSION ; | IF_STMT | COMPOUND_STMT | DECLARATION | ;
        bool statement();

        // FOR_STMT → for ( EXPRESSION ; OPT_EXPR ; OPT_EXPR ) STATEMENT
        bool for_stmt();

        // OPT_EXPR → EXPRESSION | λ
        bool opt_expr();

        // WHILE_STMT → while ( EXPRESSION ) STATEMENT
        bool while_stmt();

        // IF_STMT → if ( EXPRESSION ) STATEMENT ELSEPART
        bool if_stmt();

        // ELSEPART → else STATEMENT | λ
        bool elsepart();

        // COMPOUND_STMT → { STMT_LIST }
        bool compound_stmt();

        // STMT_LIST → STATEMENT STMT_LIST | λ
        bool stmt_list();

        // EXPRESSION → Identifier = EXPRESSION | RVALUE | true | 1
        bool expression();

        // RVALUE → MAG RVALUE2
        bool rvalue();

        // RVALUE2 → COMPARE MAG RVALUE2 | λ
        bool rvalue2();

        // COMPARE → == | < | > | <= | >= | !=
        bool compare();

        // MAG → TERM MAG2
        bool mag();

        // MAG2 → + TERM MAG2 | - TERM MAG2 | λ
        bool mag2();

        // TERM → FACTOR TERM2
        bool term();

        // TERM2 → * FACTOR TERM2 | / FACTOR TERM 2 | λ
        bool term2();

        // FACTOR → ( EXPRESSION ) | - FACTOR | + FACTOR | Identifier | Integer | Float | String
        bool factor();

        // Helper function that returns true if equality exists at current position, incrementing iterator if true
        bool equals(Tokenizer::TokenType type);

        // Helper function that returns true if equality exists at current position, incrementing iterator if true
        bool equals(char* input);

        // Prints debug code showing function progress -> token
        void debug(char* msg);

        // Whether debug code should be printed to console
        bool doDebug;

        // Stores current position in tokens vector
        std::vector<Tokenizer::Token>::iterator token, end;

};

