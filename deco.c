/*--------------------------------------------------------------------*/
/* deco.c                                                             */
/* Auhtor: Christian Ronda                                            */
/*--------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

/*--------------------------------------------------------------------*/

enum State_Type {BASE, COMMENT_START, COMMENT, QUOTE, COMMENT_END, BACKSLASH};

enum State_Type handleBaseState(int c)
{
    enum State_Type state;
    if(c == '/'){
        state = COMMENT_START;
    }
    else if(c == '\'' || c == '\"'){
        putchar(c);
        state = QUOTE;
    }
    else{
        putchar(c);
        state = BASE;
    }
    return state;
}

/*--------------------------------------------------------------------*/

enum State_Type handleComment_StartState(int c){
    enum State_Type state;
    if(c == '*'){
        state = COMMENT;
        putchar(' ');
    }
    else if(c == '\n'){
        putchar('/');
        putchar(c);
        state = BASE;
    }
    else{
        putchar('/');
        state = COMMENT_START;
    }
    return state;
}

/*--------------------------------------------------------------------*/

enum State_Type handleCommentState(int c){
    enum State_Type state;
    if(c == '\n'){
        putchar('\n');
        state = COMMENT;
    }
    else if (c == '*'){
        state = COMMENT_END;
    }
    else{
        state = COMMENT;
    }
    return state;
}

/*--------------------------------------------------------------------*/

enum State_Type handleComment_EndState(int c){
    enum State_Type state;
    if (c == '/'){
        state = BASE;
    }
    else if (c == '*'){
        state = COMMENT_END;
    }
    else if(c == '\n'){
        putchar('\n');
        state = COMMENT;
    }
    else{
        state = COMMENT;
    }
    return state;
}

/*--------------------------------------------------------------------*/

enum State_Type handleQuoteState(int c){
    enum State_Type state;
    if (c == '\'' || c == '\"'){
        putchar(c);
        state = BASE;
    }
    else if(c == '\\'){
        putchar(c);
        state = BACKSLASH;
    }
    else{
        putchar(c);
        state = QUOTE;
    }
    return state;
}

/*--------------------------------------------------------------------*/

enum State_Type handleBackSlashState(int c){
    enum State_Type state;
    putchar(c);
    state = QUOTE;
    return state;
}

/*takes a input file a prints out the contents replacing comments with spaces
 * and giving an error message if the comment has no end                    */
int main(void){
    int c;
    enum State_Type state = BASE;
    int line = 1;
    int possible_error;
    while((c = getchar()) != EOF){
        if (c == '\n')
            line++;
        switch (state) {
            case BASE:
                state = handleBaseState(c);
                break;
            case COMMENT_START:
                possible_error = line;
                state = handleComment_StartState(c);
                break;
            case COMMENT:
                state = handleCommentState(c);
                break;
            case COMMENT_END:
                state = handleComment_EndState(c);
                break;
            case QUOTE:
                state = handleQuoteState(c);
                break;
            case BACKSLASH:
                state = handleBackSlashState(c);
                break;
            default: break;
        }
    }
    if (state == 4 || state == 2){
        fprintf(stderr, "Error: %d: unterminated comment\n", possible_error);
        exit(EXIT_FAILURE);
    }
    return 0;
}


