// This module contains relevant functions specific to lexing TEMP<Elixir> tokens.

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "tokenfuns.h"

/*
Takes an Integ object (defined in './tokens.h') and returns the dynamically allocated string 
"Int(<num>)" where <num> is the int value stored in the Integ object. This function 
requires that the returned string be freed by the caller.
*/
static char* disp_int(Integ integer) {
    // Extract integer's value.
    const int num = integer.num;

    // Calculate the number of digits in the integer.
    const int int_str_size = ((int)(ceil(log10(num)) + 1));

    // Allocate memory for these digits plus the "Int()" characters.
    char* int_str = malloc((int_str_size + 5) * sizeof(char));

    // Error if memory allocation fails.
    if (int_str == NULL) {
        // Create an error message including the integer.
        char* err_str = malloc ((int_str_size + 29) * sizeof(char));
        sprintf(err_str, "Error allocating memory for %d.", num);
        perror(err_str);

        // Free the error message and exit the program.
        free(err_str);
        exit(EXIT_FAILURE);
    }
    // Create and return the dynamically allocated string.
    sprintf(int_str, "Int(%d)", num);
    return int_str;
}

// Free the internal <name> of a Vari object.
void free_varname(Vari var) {
    free(var.name);
}

/*
Takes a Vari object (defined in './tokens.h') and returns the dynamically allocated string 
"Vari(<name>)" where <name> is the string value stored in the Vari object. This function 
requires that the returned string be freed by the caller.
*/
static char* disp_var(Vari var) {
    // Extract the variable's value.
    char* name = var.name;

    // Calculate the size of the Vari's name.
    const int var_str_size = strlen(name) + 1;

    // Allocate memory for this name plus the "Var()" characters.
    char* var_str = malloc((var_str_size + 5) * sizeof(char));

    // Error if memory allocation fails.
    if (var_str == NULL) {
        // Create an error message including the name.
        char* err_str = malloc((var_str_size + 31) * sizeof(char));
        sprintf(err_str, "Error allocating memory for \"%s\".", name);
        perror(err_str);

        // Free the error message and exit the program.
        free(err_str);
        exit(EXIT_FAILURE);
    }
    // Create and return the dynamically allocated string.
    sprintf(var_str, "Var(%s)", name);
    return var_str;
}

// Integers that split tokens into categories in the 'disp_token' below.
// Categories are labeled in the Token enum in './tokens.h'.
static enum TokenSiphons {
    KeywrdSip = 2,
    PrimSip = KeywrdSip + 1,
    PrimOpSip = PrimSip + 5,
    VarSip = PrimOpSip + 2,
    GroupSip = VarSip + 2,
    NothingSip = GroupSip + 1
};

/*
Takes a Token value (defined in './tokens.h') and a pointer to an object and returns the string 
representation of that token. The object is used for tokens that contain data (e.g. Int(12)). 
Returned strings that contain this data must be freed by the caller. Assume that a given token 
is paired with a pointer to the correct object.
*/
char* disp_token(Token tok, void* obj) {
    // Assign the Token value as an int.
    const int tok_num = tok;

    // Use the Token Siphons to narrow the Token value into a category. Switch statements 
    // then return the correct string at each category.
    if (tok_num >= KeywrdSip) {
        if (tok_num >= PrimSip) {
            if (tok_num >= PrimOpSip) {
                if (tok_num >= VarSip) {
                    if (tok_num >= GroupSip) {
                        if (tok_num >= NothingSip) {
                            // Token does not fit any category.
                            perror("Not a token.");
                            exit(EXIT_FAILURE);
                        }
                        return "Nothing";
                    }
                    switch (tok_num) {
                        case LeftPar: return "LeftPar";
                        case RightPar: return "RightPar";
                        default: 
                            perror("Not a grouper.");
                            exit(EXIT_FAILURE);
                    }
                }
                switch (tok_num) {
                    // Use obj to return a dynamically allocated string.
                    case Var: return disp_var(*((Vari*)obj));
                    case Bind: return "Bind";
                    default: 
                        perror("Not a var token.");
                        exit(EXIT_FAILURE);
                }
            }
            switch (tok_num) {
                case Plus: return "Plus";
                case Minus: return "Minus";
                case Mult: return "Mult";
                case Div: return "Div";
                default: 
                    perror("Not a primitive operator.");
                    exit(EXIT_FAILURE);
            }
        }
        switch (tok_num) {
            // Use obj to return a dynamically allocated string.
            case Int: return disp_int(*((Integ*)obj));
            default:
                perror("Not a primitive type.");
                exit(EXIT_FAILURE);
        }
    }
    switch (tok_num) {
        case Let: return "Let";
        case Now: return "Now";
        default: 
            perror("Not a keyword.");
            exit(EXIT_FAILURE);
    }   
}