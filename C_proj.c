#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <math.h>
#include <string.h>
#include <ctype.h>

#define INITIAL_SIZE 20
#define INCREASE 2
#define ARR_SIZE 52
#define HALF_ARR 26
#define MAX_INPUT 5
#define ASCII_a 97
#define ASCII_A 65
int input_size = INITIAL_SIZE;

typedef struct point{ //point initializer
    double x;
    double y;
}p;

typedef struct line{ //line initializer
    double slope;
    double y_inter;
}l;

typedef struct parallel{ //x initializer
    double x_val;
}x;

p* set_p(double p1, double p2){ // passes in values x,y and returns a pointer to p structure
    p* p_ = (p*)malloc(sizeof(p));
    if(p_!=NULL) {
        p_->x = p1;
        p_->y = p2;
    }
    return p_;
}

l* set_l(double slope, double y_inter) { // passes in values slope,y_inter and returns a pointer to l structure
    l* l_ = (l*) malloc(sizeof(l));
    if (l_ != NULL) {
        l_->slope = slope;
        l_->y_inter = y_inter;
    }
    return l_;
}

x* set_x(double x_val) { // passes in values x and returns a pointer to x structure
    x* x_ = (x*) malloc(sizeof(x));
    if (x_ != NULL) {
        x_->x_val = x_val;
    }
    return x_;
}

char* read(FILE* expected_input) { //reads an input a letter at a time for either user file or stdin and returns string
    int index = 0, c, increment_size, max = INITIAL_SIZE;
    input_size = INITIAL_SIZE;
    char *input = (char *) malloc(max * sizeof(char));
    if (input != NULL) {
        for (c = fgetc(expected_input); c != '\n' && c != EOF; c = fgetc(expected_input)) { //reads until end of line or file
            if (index == max - 1) { // if input is too large for initial size
                increment_size =  max * INCREASE ;
                input_size += increment_size;
                input = (char *) realloc(input, increment_size * sizeof(char)); //reallocate more memory to read input
                if (input == NULL)
                    return NULL;
                max = max * INCREASE;
            }
            input[index++] = c; //place he letter into the array
        }
        input[index] = '\0'; //place\0 at the end
    }
    return input;
}

char** parse(char* input) { //takes in a string and returns ** array of strings
    int i, current_command = 0;
    char **commands = (char **) malloc(MAX_INPUT * sizeof(input));
    if (commands != NULL) {
        commands[current_command] = input; //pointer set to first letter in array
        for (i = 0; i < input_size; i++) {
            if (input[i] == ',' || input[i] == ' ') { //searches for space or comma
                input[i] = '\0';                        // switches to \0 to create string
                commands[++current_command] = input + i + 1; //by pointer arithmetic have new pointer point to
                //first letter of next "word"
            }
        }
    }
    return commands;
}
int var_hash(char c) { //"hashes" or assigns an integer value to each letter to map it into an array
    if (c >= 'a' && c <= 'z')
        return c - ASCII_a;
    else if (c >= 'A' && c <= 'Z')
        return c - ASCII_A + HALF_ARR;
}

void delete_p(char name, p** names ){ //takes in var_name and list of points and deletes/frees the variable
    if (names[var_hash(name)] != NULL) {//and reassigns to NULL
        free(names[var_hash(name)]);
        names[var_hash(name)] = NULL;
        printf("Variable deleted\n");
    }
}
void delete_l(char name, l** names ) { //same method as above
    if (names[var_hash(name)] != NULL) {
        free(names[var_hash(name)]);
        names[var_hash(name)] = NULL;
        printf("Variable deleted\n");
    }
}

void delete_x(char name, x** names ) { //same method as above
    if (names[var_hash(name)] != NULL) {
        free(names[var_hash(name)]);
        names[var_hash(name)] = NULL;
        printf("Variable deleted\n");
    }
}

char check_arr(char name, p** list_p, l** list_l, x** list_x) { //takes in a var_name and all 3 arrays and returns x,l,p
    if (list_p[var_hash(name)] != NULL)                         //function checks which type the given variable is
        return 'p';
    else if (list_l[var_hash(name)] != NULL)
        return 'l';
    else if (list_x[var_hash(name)] != NULL)
        return 'x';
    else                                            //and return 'n' for NULL if it hasn't been set yet
        return 'n';
}

void print(char name, p** list_p, l** list_l,x** list_x ){ //takes in variable name and all 3 variable storage arrays
    if (check_arr(name,list_p, list_l, list_x)== 'p')       //and check's where it's located before printing a value
        printf("( %lf , %lf )\n", list_p[var_hash(name)]->x, list_p[var_hash(name)]->y);
    else if (check_arr(name,list_p, list_l, list_x)== 'l')
        printf("y=%lfx+%lf\n",  list_l[var_hash(name)]->slope, list_l[var_hash(name)]->y_inter);
    else if (check_arr(name,list_p, list_l, list_x)== 'x')
        printf("x=%lf\n",  list_x[var_hash(name)]->x_val);
    else if(check_arr(name,list_p, list_l, list_x)== 'n') //or if it's empty prints that the variable doesn't exist in memory
        printf("variable either deleted or not initialized\n");
}

void line_from_points(p* p1, p* p2, l** list_l, char name ){ //takes in 2 pointer of type p and the line storage array and name
    double slope, y_inter;
    slope = (p1->y - p2->y)/(p1->x - p2->x); //finds slope
    y_inter = p1->y - (slope*p1->x);   //evaluates y_inter by plugging into y=mx+b
    list_l[var_hash(name)] = set_l(slope, y_inter); //sets a new line of name "name"
}

void intersection(char line1, char line2, char point_name, l** list_l, p** list_p){
    double a = list_l[var_hash(line2)]->slope - list_l[var_hash(line1)]->slope; //equivalent to setting 2 line formulas equal and
    double b = list_l[var_hash(line1)]->y_inter - list_l[var_hash(line2)]->y_inter;//^^ x's on one side and y_inter on the other
    double x = b/a;//divide slope by y_inter to isolate x
    double y = (list_l[var_hash(line1)]->slope * x) + (list_l[var_hash(line1)]->y_inter);// plug in x to solve for y
    list_p[var_hash(point_name)] = set_p(x,y); //set a new point x,y
}

double distance_p(p* p1,p* p2){ //evaluates distance between 2 points with basic distance formula
    double result;
    result = sqrt(pow(p2->x - p1->x,2) + pow(p2->y - p1->y,2));
    return result;
}
double distance_l(l* l1, l* l2) { //evaluates distance between 2 parallel line from given formula
    if (l1->slope == l2->slope && l1->y_inter != l2->y_inter){
        double result;
        result = fabs(l1->y_inter - l2->y_inter) / sqrt(1 + pow(l1->slope, 2));
        return result;
    }
    else
        printf("lines not parallel - Invalid input\n");
}
double distance_l_p(p* p, l* l){ //evaluates distance between point and nearest point on line
    double result;
    result = fabs((l->slope*p->x)+(-1*p->y)+l->y_inter)/sqrt(pow(l->slope,2)+1);
    return result;
}

double distance_x(x* x1, x* x2){ //evaluates  horizontal distance between 2 x= lines
    double result;
    result = fabs(x1->x_val - x2->x_val);
    return result;
}

double distance_p_x(p* p, x* x){ //evaluates horizontal distance between point and x= line
    double result;
    result = fabs(x->x_val - p->x);
    return result;
}

void Distance(char a, char b, p** p_list, l** l_list, x** x_list){
    char input3,input4;
    double final;
    input3 = check_arr(a, p_list, l_list, x_list);
    input4 = check_arr(b, p_list, l_list, x_list);
    if (input3 == 'p' && input4 == 'p') {
        final = distance_p(p_list[var_hash(a)], p_list[var_hash(b)]);
        printf("distance between points = %lf\n", final);
    }else if (input3 == 'l' && input4 == 'l') {
        if(l_list[var_hash(a)]->slope == l_list[var_hash(b)]->slope){
            final = distance_l(l_list[var_hash(a)], l_list[var_hash(b)]);
            printf("distance between lines = %lf\n", final);
        }else
            printf("lines not parellel. Cannot evaluate distance\n");
    }else if (input3 == 'l' && input4 == 'p') {
        final = distance_l_p(p_list[var_hash(b)], l_list[var_hash(a)]);
        printf("distance between line and point = %lf\n", final);
    }else if (input3 == 'p' && input4 == 'l') {
        final = distance_l_p(p_list[var_hash(a)], l_list[var_hash(b)]);
        printf("distance between line and point = %lf\n", final);
    }else if (input3 =='x' && input4 == 'x'){
        final = distance_x(x_list[var_hash(a)], x_list[var_hash(b)]);
        printf("distance between lines = %lf\n", final);
    }else if (input3 =='x' && input4 == 'p'){
        final = distance_p_x(p_list[var_hash(b)], x_list[var_hash(a)]);
        printf("distance between line1 and point = %lf\n", final);
    }else if (input3 == 'p' && input4 == 'x'){
        final = distance_p_x(p_list[var_hash(a)], x_list[var_hash(b)]);
        printf("distance between line1 and point = %lf\n", final);
    }else if((input3 =='x' && input4 == 'l') || (input3 =='l' && input4 == 'x'))
        printf("lines not parallel.Cannot evaluate distance\n");
    else if(input3 =='n' || input4 == 'n')
        printf("one or both variables uninitialized");

}

void area(char line_name, l** lines){ //takes in name in line and storage array
    double y_inter, x_inter;
    y_inter = lines[var_hash(line_name)]->y_inter;
    x_inter = fabs((-y_inter)/(lines[var_hash(line_name)]->slope)); //finds x_inter by setting y=0
    printf("the area of the triangle is %lf\n",(x_inter*y_inter)/2); //area of right triangle 1/2(base*height)
}

int check_digit(char* input){
    int i, number=1;
    for(i=0; i<strlen(input); i++){
        if(isdigit(input[i]))
            number=1;
        else if(input[i]=='.')
            continue;
        else{
            number=0;
            break;
        }
    }
    return number;
}

void free_mem(p** list_p, l** list_l, x** list_x){ //takes in all 3 storage arrays
    int i;                                  //in a for loop frees only the used spots and then frees arrays themselves
    for(i=0; i<ARR_SIZE; i++) {
        if (list_p[i] != NULL)
            free(list_p[i]);
        if (list_l[i] != NULL)
            free(list_l[i]);
        if (list_x[i] != NULL)
            free(list_x[i]);
    }

    if(list_p != NULL)
        free(list_p);
    if(list_l != NULL)
        free(list_l);
    if(list_x != NULL)
        free(list_x);
    printf("Memory cleared\n");
}

void free_com(char** commands){
    int i;
    for(i=0;i<MAX_INPUT; i++)
        free(commands[i]);
    free(commands);
    printf("commands freed");
}

int main() {
    FILE *expected_input = stdin; //initializes the expected input to be the keyboard
    char *input, *instruction = "Please input a command\n\
     Command List:\n\
     SET<variable_name> <variable_type> <comma separated values>\n\
     DEL <variable_name>\n\
     PRINT <variable_name>\n\
     LINE <point_1> <point_2 > <lne >\n\
     INTERSECT <line_1 > <line_2 > <point >\n\
     DIST <variable_name1> <variable_name2>\n\
     AREA <line>\n\
     HALT\n\
     HELP\n";
    char *input1, *input2, name;
    char **commands;
    FILE *infile; //input file pointer

    printf("To input instructions manually (through the keyboard), please press enter\n");
    printf("To input instructions from a file, please type \"FILE\"\n");
    input1 = read(expected_input); //asks user if they wan to input by keyboard or file
    if (strcmp(input1, "FILE") == 0) {
        printf("please input the name of your file:");
        input2 = read(expected_input);
        infile = fopen(input2, "r");
        if (infile!=NULL) //checks for error accessing file
            expected_input = infile; //changed expected input from stdin to users specific file
    }
    printf("%s", instruction);
    input = read(expected_input);
    commands = parse(input);
    p **variable_name_p = (p **) malloc(ARR_SIZE * sizeof(p*)); //allocation of the 3 array**'s the save the variables
    l **variable_name_l = (l **) malloc(ARR_SIZE * sizeof(l*)); //inputted by the user
    x **variable_name_x = (x **) malloc(ARR_SIZE * sizeof(x*));
    if (variable_name_p != NULL && variable_name_l != NULL && variable_name_x != NULL) {
        do {
            if (strcmp(commands[0], "SET") == 0) {
                if (commands[2] != NULL && commands[2][0] == 'p' && strlen(commands[2])<2) { //makes sure second input = p and not null
                    if (commands[1] != NULL && commands[3] != NULL && commands[4] != NULL &&
                        check_digit(commands[3])==1 && check_digit(commands[4])==1) { //checks that rest of values exist
                        name = commands [1][0];
                        double num2 = strtod(commands[3], NULL); //string to digit function does as expected
                        double num3 = strtod(commands[4], NULL);
                        if (((name >= 'a' && name <= 'z') || (name >= 'A' && name <= 'Z')) && strlen(commands[1])<2){ //checks for valid variable name
                            if ((check_arr(name, variable_name_p, variable_name_l, variable_name_x)) == 'n') { //checks to make sure
                                variable_name_p[var_hash(name)] = set_p(num2, num3);//that the slot in memory is empty
                                printf("variable %c saved\n", name);
                            }
                            else
                                printf("Variable name already used. Please input a new variable name\n");
                        } else
                            printf("Variable name %s invalid\n", commands[1]);
                    } else
                        printf("INvalid input format or number valued invalid");

                } else if (commands[2] != NULL && commands[2][0] == 'l' && strlen(commands[2])<2) { //same checks and errors as before
                    if (commands[1] != NULL && commands[3] != NULL && commands[4] != NULL &&
                        check_digit(commands[3])==1 && check_digit(commands[4])==1) {
                        name = commands[1][0];
                        double num2 = strtod(commands[3], NULL);
                        double num3 = strtod(commands[4], NULL);
                        if (((name >= 'a' && name <= 'z') || (name >= 'A' && name <= 'Z')) && strlen(commands[1])<2) {
                            if ((check_arr(name, variable_name_p, variable_name_l, variable_name_x)) == 'n') {
                                variable_name_l[var_hash(name)] = set_l(num2, num3);
                                printf("variable %c saved\n", name);
                            }
                            else
                                printf("Variable name already used. Please input a new variable name\n");
                        }else
                            printf("Variable name %s invalid\n", commands[1]);
                    } else
                        printf("Please check that your input follows the following format:\n"
                               "SET<variable_name> <variable_type> <slope,y_intercept>\n");

                } else if (commands[2] != NULL && commands[2][0] == 'x' && strlen(commands[2])<2) { //same as earlier
                    if (commands[1] != NULL && commands[3] != NULL && check_digit(commands[3])==1) {
                        double num = strtod(commands[3], NULL);
                        if (((commands[1][0] >= 'a' && commands[1][0] <= 'z') ||
                             (commands[1][0] >= 'A' && commands[1][0] <= 'Z')) && strlen(commands[1])<2) {
                            if ((check_arr(commands[1][0], variable_name_p, variable_name_l, variable_name_x)) == 'n') {
                                variable_name_x[var_hash(commands[1][0])] = set_x(num);
                                printf("variable %c saved\n", commands[1][0]);
                            }
                            else
                                printf("Variable name already used. Please input a new variable name\n");
                        }
                        else
                            printf("variable name %s invalid\n", commands[1]);

                    }else
                        printf("Please check that your input follows the following format:\n"
                               "SET<variable_name> <variable_type> <x-value>\n");
                } else
                    printf("variable type %s invalid. must be p, x , or l\n",commands[2]);

            } else if (strcmp(commands[0], "DEL") == 0) {//checks for a type and deletes from the corresponding array
                if (commands[1]!= NULL) {
                    if (check_arr(commands[1][0], variable_name_p, variable_name_l, variable_name_x) == 'p')
                        delete_p(commands[1][0], variable_name_p);
                    else if (check_arr(commands[1][0], variable_name_p, variable_name_l, variable_name_x) == 'l')
                        delete_l(commands[1][0], variable_name_l);
                    else if (check_arr(commands[1][0], variable_name_p, variable_name_l, variable_name_x) == 'x')
                        delete_x(commands[1][0], variable_name_x);
                    printf("deletion successful\n");
                }
                else
                    printf("missing variable input\n");

            } else if (strcmp(commands[0], "PRINT") == 0){//prints the variable given from function above
                if (commands[1]!=NULL)
                    print(commands[1][0], variable_name_p, variable_name_l, variable_name_x);
                else
                    printf("missing variable input\n");

            } else if (strcmp(commands[0], "LINE") == 0) {
                if (commands[1]!=NULL && commands[2]!=NULL && commands[3]!=NULL) {

                    char input3, input4;
                    input3 = check_arr(commands[1][0], variable_name_p, variable_name_l, variable_name_x);
                    input4 = check_arr(commands[2][0], variable_name_p, variable_name_l, variable_name_x);
                    if (input3 == 'n')
                        printf("variable %c not defined\n", commands[1][0]); //checks if either of the variables are undefined
                    else if (input4 == 'n')
                        printf("variable %c not defined\n", commands[2][0]);
                    else if (input3 == 'p' && input4 == 'p') {
                        line_from_points(variable_name_p[var_hash(commands[1][0])],
                                         variable_name_p[var_hash(commands[2][0])],
                                         variable_name_l, commands[3][0]);
                    } else
                        printf("incompatible variable types\n");
                }else
                    printf("missing variable input\n");

            } else if (strcmp(commands[0], "INTERSECT") == 0) {
                if (commands[1]!=NULL && commands[2]!=NULL && commands[3]!=NULL) {
                    char input3 = check_arr(commands[1][0], variable_name_p, variable_name_l, variable_name_x);
                    char input4 = check_arr(commands[2][0], variable_name_p, variable_name_l, variable_name_x);
                    if (input3 == 'n' )
                        printf("variable %c not defined\n", commands[1][0]);
                    else if (input4 == 'n')
                        printf("variable %c not defined\n", commands[2][0]);
                    else if(input3=='l' && input4=='l'){
                        intersection(commands[1][0], commands[2][0], commands[3][0], variable_name_l, variable_name_p);
                    }
                    else
                        printf("one or both chosen variables not of type l\n");
                }
                else
                    printf("missing variables\n");

            } else if (strcmp(commands[0], "DIST") == 0) {
                if (commands[1] != NULL && commands[2] != NULL) {
                    if ((commands[1][0] >= 'a' && commands[1][0] <= 'z') || (commands[1][0] >= 'A' && commands[1][0] <= 'Z'))
                        Distance(commands[1][0], commands[2][0], variable_name_p, variable_name_l, variable_name_x);
                    else
                        printf("invalid variable name\n");
                }
                else
                    printf("missing variable(s)\n");

            } else if (strcmp(commands[0], "AREA") == 0) {
                if (commands[1] != NULL) { //makes sure that the input variable is of type line before executing
                    if (check_arr(commands[1][0], variable_name_p, variable_name_l, variable_name_x) == 'l')
                        area(commands[1][0], variable_name_l);
                    else
                        printf("variable must be of type line\n");
                } else
                    printf("Missing variable name\n");

            } else if (strcmp(commands[0], "HELP") == 0)
                printf("%s", instruction);
            else
                printf("Command '%s' not defined, Please input a valid command (CASE SENSITIVE)\n", commands[0]);

            if(feof(expected_input)) {
                printf("End of file reached. Exiting program\n");
                break;
            }
            input = read(expected_input);
            commands = parse(input);

        } while (strcmp(commands[0], "HALT") != 0); //while commands not == HALT, continue

    }
    free_mem(variable_name_p, variable_name_l, variable_name_x); //frees up memory before exiting the program
    free_com(commands);
    return 0;
}