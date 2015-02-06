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

        // ARG_LIST → ARG | ARG_LIST , ARG
        bool arg_list();

        // ARG → TYPE Identifier
        bool arg();

        // DECLARATION → TYPE IDENT_LIST ;
        bool declaration();

        // TYPE → int | float
        bool type();

        // IDENT_LIST → Identifier , IDENT_LIST Identifier
        bool ident_list();

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

        // STMT_LIST → STMT_LIST STATEMENT | λ
        bool stmt_list();

        // EXPRESSION → Identifier = EXPRESSION | RVALUE
        bool expression();

        // RVALUE → RVALUE COMPARE MAG | MAG
        bool rvalue();

        // COMPARE → == | < | > | <= | >= | !=
        bool compare();

        // MAG → MAG + TERM | MAG - TERM | TERM
        bool mag();

        // TERM → TERM * FACTOR | TERM / FACTOR | FACTOR
        bool term();

        // FACTOR → ( EXPRESSION ) | - FACTOR | + FACTOR | Identifier | TYPE
        bool factor();

        // Helper function that returns true if equality exists, while also incrementing iterator
        bool isNextType(Tokenizer::TokenType type);

        // Helper function that returns true if equality exists, while also incrementing iterator
        bool isNextValue(char* input);

        // Stores current position in tokens vector
        std::vector<Tokenizer::Token>::iterator pos;

};

