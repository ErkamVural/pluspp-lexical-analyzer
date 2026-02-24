#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#define KEYWORD_NUMBER 6
#define OPERATOR_NUMBER 3

const char *keywords[KEYWORD_NUMBER] = {"number", "write", "repeat", "times", "and", "newline"};
const char *operators[OPERATOR_NUMBER] = {":=", "+=", "-="}; // The two character long operator list can be added later.

FILE *input, *output;
int current_char;
int current_char_index = 0;
int current_line = 1;

void get_char() {
    current_char = fgetc(input);
    if (current_char == '\n') {
        current_line++;
        current_char_index = 0;
    } else {
        current_char_index++;
    }
}

void write_token(char *token, char *value) { // Write token in output.lx
    if (value == NULL) {
        fprintf(output, "%s\n", token);
    } else {
        fprintf(output, "%s(%s)\n", token, value);
    }
}

void skip_comment() {
    get_char();

    while (current_char != EOF) {
        if (current_char == '*') {
            return; // end of comment
        }
        get_char();
    }

    printf("The comment could not be closed!\n");
    exit(1);
}

int is_keyword(char *str) { // Check char array is keyword if yes return 1 else return 0
    for (int i = 0; i < KEYWORD_NUMBER; i++) {
        if (strcmp(str, keywords[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

int is_operator() { // Get second char and check they are operator if yes return 1 else return 0 
    int first_char = current_char;
    int second_char = fgetc(input);
    current_char_index++;
    
    if (second_char == EOF) {
        return 0;
    }
    
    char op[3];
    op[0] = first_char;
    op[1] = second_char;
    op[2] = '\0';
    
    for (int i = 0; i < OPERATOR_NUMBER; i++) {
        if (strcmp(op, operators[i]) == 0) {
            write_token("Operator", op);
            return 1;
        }
    }
    
    ungetc(second_char, input);
    current_char_index--;
    return 0;
}

void get_string() { // Get alpha and check it is keyword or identifier
    char string[21];
    int i = 0;
    string[i++] = current_char;
    get_char();

    while (isalnum(current_char) || current_char == '_') {
        if (i < 20) {
            string[i++] = current_char;
        } else {
            printf("String cannot be greater than 20 characters at line %d!", current_line);
            exit(1);
        }
        get_char();
    }
    
    ungetc(current_char, input);
    current_char_index--;
    string[i] = '\0';
    
    if (is_keyword(string)) {
        write_token("Keyword", string);
    } else {
        write_token("Identifier", string);
    }
}

void get_number() {
    char number[101];
    int i = 0;
    number[i++] = current_char;
    get_char();

    while (isdigit(current_char)) {
        if (i < 100) {
            number[i++] = current_char;
        } else {
            printf("Number too long at line %d!\n", current_line);
            exit(1);
        }
        get_char();
    }

    if (!(isspace(current_char) || current_char == ';' || current_char == EOF)) {
        printf("Invalid number format number cannot contain '%c' at %d.line and %d.char!", current_char, current_line, current_char_index);
        exit(1);
    }

    ungetc(current_char, input);
    current_char_index--;
    number[i] = '\0';

    if (strcmp(number, "-") == 0) {
        printf("Error: Invalid number format (lone minus) at line %d. Negative numbers cannot have space after minus sign.\n", current_line);
        exit(1);
    }

    write_token("IntConstant", number);
}

void get_string_constant() { // When current char equal '"' get string constant if there no second '"' exit
    int current_line_copy = current_line;
    int current_index_copy = current_char_index;
    char string[101];
    int i = 0;
    
    string[i++] = current_char;
    get_char();
    
    while (current_char != EOF && current_char != '"') {
        if (i >= 100) { // Bound Control
            printf("Error: String constant too long (max 100 chars) starting at line %d!\n", current_line_copy);
            exit(1);
        }
        string[i++] = current_char;
        get_char();
    }
    
    if (current_char != '"') {
        printf("The String starting at %d. row %d index was not closed!\n", current_line_copy, current_index_copy);
        exit(1);
    }
    
    string[i++] = current_char;
    string[i] = '\0';
    
    write_token("StringConstant", string);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s filename\n", argv[0]);
        return 1;
    }

    char input_file[100], output_file[100];
    sprintf(input_file, "%s.plus", argv[1]);
    sprintf(output_file, "%s.lx", argv[1]); // Create new .lx with argument name

    input = fopen(input_file, "r");
    if (!input) {
        printf("Error opening input file");
        exit(1);
    }

    output = fopen(output_file, "w");
    if (!output) {
        printf("Error creating output file");
        fclose(input);
        exit(1);
    }

    get_char();

    while (current_char != EOF) {
        if (isspace(current_char)) {
            // Skips if block and gets next char
        } else if (current_char == '*') {
            skip_comment();
        } else if (isalpha(current_char)) {
            get_string();
        } else if (is_operator()) {
            // make action in is_operator()
        } else if (isdigit(current_char) || current_char == '-') {
            get_number();
        } else if (current_char == '"') {
            get_string_constant();
        } else if (current_char == ';') {
            write_token("EndOfLine", NULL);
        } else if (current_char == '{') {
            write_token("OpenBlock", NULL);
        } else if (current_char == '}') {
            write_token("CloseBlock", NULL);
        } else {
            printf("Invalid character '%c' detected at %d.line %d.char!\n", current_char, current_line, current_char_index);
            exit(1);
        }
        get_char(); // get next char
    }
    
    fclose(input);
    fclose(output);
    return 0;
}