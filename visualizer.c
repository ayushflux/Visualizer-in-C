#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>
#include <raylib.h>
#include <raymath.h>
#include <stddef.h>
#include <stdbool.h>

#define MAX_TEXT_LENGTH 256


int screenWidth = 800;
int screenHeight = 450;
int scale = 35;
int back_state = 0;
Color color = DARKBLUE;
Color color2 = BLACK;
int graph_scale=10;
int scale_x=10;
int scale_y=10;

void bgrndClr();


#define MAX 100
enum Type{NUMBER, VARIABLE, OPERATOR, LEFT_BRACKET, RIGHT_BRACKET, FUNCTION};
typedef struct{
    enum Type type;
    char data;
    double value;
    int sign;
}Token;
struct STACK{
    int top;
    Token items[MAX];
}stack;
Token Invalid={OPERATOR,'I',404,-1};
void stack_init(struct STACK *s){
    s->top=-1;
}
double sin(double x){
    return(sinf(x));
}
double cos(double x){
    return(cosf(x));
}
double tan(double x){
    return(tanf(x));
}
double arcsin(double x){
        return(asinf(x));
}
double arccos(double x){
    return(acosf(x));
}
double arctan(double x){
    return(atanf(x));
}
double log2(double x){
    return(log2f(x));
}
double log10(double x){
    return(log10f(x));
}
double log(double x){
    return(logf(x));
}
double unity(double x){
    return(x);
}
double (*operations[])(double)={sin, cos, tan, arcsin, arccos, arctan, log2, log10, log, unity};
int fn_index(char *a) {
    if (!strcmp(a, "sin")) return 0;
    else if (!strcmp(a, "cos")) return 1;
    else if (!strcmp(a, "tan")) return 2;
    else if (!strcmp(a, "arcsin")) return 3;
    else if (!strcmp(a, "arccos")) return 4;
    else if (!strcmp(a, "arctan")) return 5;
    else if (!strcmp(a, "log2")) return 6;
    else if (!strcmp(a, "log10")) return 7;
    else if (!strcmp(a, "log")) return 8;
    return 9;
}

void stack_push(struct STACK *s, Token t){
    if(s->top>=MAX-1){
        printf("Limit Reached\n");
        return;
    }else{
        s->items[++(s->top)]=t;
        return;
    }
}
int stack_size(struct STACK *s){
    return((s->top)+1);
}
Token stack_pop(struct STACK *s){
    if(s->top==-1){
        return(Invalid);
    }else{
        return(s->items[(s->top)--]);
    }
}
Token stack_top(struct STACK *s){
    return (s->items[s->top]);
}

struct Evaluate{    //evaluator stack
    int top;
    double data[100];
}evaluate;

void evaluate_init(struct Evaluate *s){ //initializing the stack
    s->top=-1;
}

void evaluate_push(struct Evaluate *s, double value){   //pushing to stack
    if(s->top==MAX-1){
        printf("Maximum Limit Reached\n");
    }
    else{
        s->data[++(s->top)]=value;

    }
}
bool evaluate_full(struct Evaluate *s){ //checking for overflow
    if(s->top==MAX-1){
        return true;
    }
    return false;
}
bool evaluate_empty(struct Evaluate *s){    //checking if evaluate is empty
    if(s->top==-1){
        return true;
    }
    return false;
}
double evaluate_pop(struct Evaluate *s){    //popping the top of evaluating and adjusting the top
    return(s->data[(s->top)--]);
}
int evaluate_size(struct Evaluate *s){  //evaluate stack size
    return(s->top+1);
}

int op_prec(char c){        //operator precedence
    if(c=='^')return(3);
    if(c=='*'||c=='/')return(2);
    if(c=='+'||c=='-')return(1);
    return 0;
}
int present(char* a, char b){   //substring checking as strchr returns pointers (just to be safe)
    if(strchr(a,b)!=NULL){
        return 1;
    }return 0;
}
void DrawFunction(float drawProgress, char* a);
double parser(char* dr, double sub){ //parsing the equation string --pending: MODULARIZATION
    Token store[100];
    char sr[strlen(dr)+5];  //work-around to avoid some errors
    sr[0]='(';
    sr[1]='0';
    sr[2]='+';
    sr[3]='(';
    int i,j;
    for(i=4,j=0;j<strlen(dr);i++,j++){
        sr[i]=dr[j];
    }
    sr[i++]=')';
    sr[i++]=')';
    sr[i]='\0';
    char temp[15];
    int sr_pos=0;
    int prev=0;
    int count_token=0;
    while(sr[sr_pos]!='\0'){
        int cut_point=prev;
        while(isdigit(sr[sr_pos])||sr[sr_pos]=='.'){
            cut_point++;
            sr_pos++;
        }
        if(prev!=sr_pos){
            
            int tok_data_pos=0;
            store[count_token].type=NUMBER;
            for(int i=prev;i<sr_pos;i++,tok_data_pos++){
                temp[tok_data_pos]=sr[i];
            }
            temp[tok_data_pos]='\0';
            store[count_token].value=strtod(temp,NULL);
            store[count_token].data=' ';
            prev=sr_pos;
            count_token++;
            
        }
        if(present("+-*/^",sr[sr_pos])){
            store[count_token].type=OPERATOR;
            store[count_token].data=sr[sr_pos];

            count_token++;
            sr_pos++;
        }
        cut_point=sr_pos;
        if(sr[sr_pos]==' ')sr_pos++;
        if(isalpha(sr[sr_pos])){
            if(isalpha(sr[sr_pos+1])){  
                int inc;  
            while(isalpha(sr[++cut_point])){inc++;}
                if(cut_point!=sr_pos){
                    char sample_2[10];
                    int tok_data_pos=0;
                    for(int i=sr_pos; i<cut_point;i++){
                        sample_2[tok_data_pos++]=sr[i];
                    }
                    sample_2[tok_data_pos]='\0';
                    store[count_token].type=FUNCTION;
                    store[count_token].sign=fn_index(sample_2);
                    store[count_token++].data=store[count_token].sign+'0';
                    sr_pos=cut_point;
                }}else{

            store[count_token].type=VARIABLE;
            store[count_token].data=sr[sr_pos];
            store[count_token].sign=1;
            count_token++;
            sr_pos++;}
        }
        if(sr[sr_pos]=='('){
            int tempor=sr_pos+2;
            bool unary=false;
            while(sr[tempor]!=')'){
                if((sr[sr_pos+1]=='-'||sr[sr_pos+1]=='+')&&(isalpha(sr[tempor])||isdigit(sr[tempor])))unary=true;
                tempor++;
            }
            if(unary){
                if(isalpha(sr[sr_pos+2])){
                    store[count_token].type=VARIABLE;
                    store[count_token].data=sr[sr_pos+2];
                    if(sr[sr_pos+1]=='-'){
                        
                        store[count_token].sign=-1;
                    }else{
                        store[count_token].sign=1;
                    }
                    
                    count_token++;
                    sr_pos=tempor+1;


                }else{
                    char sample[10];
                    store[count_token].type=NUMBER;
                    for(int k=sr_pos+2,l=0;k<tempor;k++,l++){
                        sample[l]=sr[k];
                    }
                    if(sr[sr_pos+1]=='-'){
                        store[count_token].value=-1*atoi(sample);
                    }else{
                        store[count_token].value=atoi(sample);
                    }
                    store[count_token].sign=1;
                    store[count_token].data=' ';
                    count_token++;
                    sr_pos=tempor+1;
                }
            }else{
                store[count_token].type=LEFT_BRACKET;
                store[count_token].data='(';
                count_token++;
                sr_pos++;}


            }
        
        if(sr[sr_pos]==')'){
            store[count_token].type=RIGHT_BRACKET;
            store[count_token].data=')';
            count_token++;
            sr_pos++;
        }
        prev=sr_pos;
    }
    stack_init(&stack);
    Token answer[count_token];
    int answer_token=0;
    int bkt=0;
    for(int i=0;i<count_token;i++){
        if(store[i].type==NUMBER||store[i].type==VARIABLE){
            answer[answer_token++]=store[i];
        }
        if(store[i].type==OPERATOR){
            while(stack.top!=-1&&(stack_top(&stack)).type==OPERATOR&&(op_prec(stack_top(&stack).data)>=op_prec((store[i]).data))){
            answer[answer_token++]=stack_pop(&stack);
        }        stack_push(&stack, store[i]);

        }
        if(store[i].type==FUNCTION){
            stack_push(&stack,store[i]);
        }

        if(store[i].type==LEFT_BRACKET){
            stack_push(&stack,store[i]);
            bkt++;
        }
        if(store[i].type==RIGHT_BRACKET){
            Token iterate=stack_pop(&stack);
            while(iterate.type!=LEFT_BRACKET||(iterate.type==OPERATOR&&iterate.value==404)){
                answer[answer_token++]=iterate;
                iterate=stack_pop(&stack);
            }
        }

    }
    while(stack.top!=-1){
        answer[answer_token++]=stack_pop(&stack);
    }

    //assigning the variable its values
    for(int i=0;i<count_token-(2*bkt);i++){
        if(answer[i].type==VARIABLE){
            answer[i].value=sub;
            }
    }
    for(int i=0;i<count_token-(2*bkt);i++){
        if(answer[i].type==VARIABLE){
            answer[i].value*=answer[i].sign;
        }
    }

    //evaluating the RPN
    evaluate_init(&evaluate);
    for(int i = 0; i < answer_token; i++) {
        if(answer[i].type == LEFT_BRACKET || answer[i].type == RIGHT_BRACKET) continue;
        if(answer[i].type==NUMBER||answer[i].type==VARIABLE){
            evaluate_push(&evaluate, answer[i].value);
        }
        if(answer[i].type==FUNCTION){
            double temp_1=evaluate_pop(&evaluate);
            evaluate_push(&evaluate, operations[answer[i].sign](temp_1));
        }
        if(answer[i].type==OPERATOR){
            if(evaluate_size(&evaluate)>=2){
            double temp_1=evaluate_pop(&evaluate);
            double temp_2=evaluate_pop(&evaluate);
            if(answer[i].data=='+'){
                evaluate_push(&evaluate, temp_1+temp_2);
            }
            else if(answer[i].data=='-')
            {
                evaluate_push(&evaluate, temp_2-temp_1);
            }else if(answer[i].data=='*'){
                evaluate_push(&evaluate, temp_2*temp_1);
            }else if(answer[i].data=='/'){
                evaluate_push(&evaluate, temp_2/temp_1);
            }else if(answer[i].data=='^'){
                evaluate_push(&evaluate, pow(temp_2,temp_1));
            }
            
        }}
    }

    return(evaluate_pop(&evaluate));

}

        
int main()
{       
    float drawProgress = 0.0;
    float drawSpeed = 0.1f;
    InitWindow(screenWidth, screenHeight, "Visualizer in C");
    SetConfigFlags(FLAG_VSYNC_HINT | FLAG_MSAA_4X_HINT);
    char text[MAX_TEXT_LENGTH + 1] = ""; // Buffer to store text
    int textLength = 0; // Current text length

    Texture2D darkMode = LoadTexture("dm.png");
    Texture2D darkMode2 = LoadTexture("dm2.png");

    Rectangle dmbtn = {10, screenHeight - 10 - darkMode.height, darkMode.width, darkMode.height};
    Rectangle dmbtn2 = {10, screenHeight - 10 - darkMode2.height, darkMode2.width, darkMode2.height};

    int A = 1;
    int redraw =0;
    // main loop//
    while (A)
    {
        int key = GetCharPressed();
        while (key > 0) {
            if (textLength < MAX_TEXT_LENGTH) {
                text[textLength] = (char)key;
                text[++textLength] = '\0'; 
            }
            key = GetCharPressed();
            if(key==KEY_ENTER)break; 
        }
        if (IsKeyPressed(KEY_BACKSPACE) && textLength > 0) {
            textLength--;
            text[textLength] = '\0';
        }
        
        float wheel = GetMouseWheelMove();
        if (wheel > 0) { 
            scale_x += 2; 
            scale_y += 2;}
         else if (wheel < 0) {  
            if (scale_x > 2) scale_x -= 2; 
            if (scale_y > 2) scale_y -= 2; }


           BeginDrawing();
        bgrndClr(darkMode, darkMode2, dmbtn, dmbtn2);
        DrawText("f(x):", 20, 20, 20, DARKGRAY);
        DrawText(text, 20, 50, 20, color2); // Display text
        DrawText("press [delete] to exit", screenWidth-230, screenHeight-30, 20, color2); // Display text
        DrawText("Scroll Mouse up for Zoom In", 10, screenHeight-60, 20, GRAY); // Display text
        DrawText("Scroll Mouse Down for Zoom Out", 10, screenHeight-30, 20, GRAY); // Display text

        if (CheckCollisionPointRec(GetMousePosition(), dmbtn) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            back_state++;
        }
        for (float i = GetScreenWidth() / 2.0; i >= 0; i = i - scale)
        {   if(i!=GetScreenWidth() / 2.0)DrawText(TextFormat("%d",(int)((i - screenWidth / 2.0) * scale_x / scale)), i, screenHeight/2.0+2,12,RED)  ;
            DrawLine(i, 0, i, screenHeight, Fade(DARKGRAY, 0.3f));
        }
        for (float i = GetScreenWidth() / 2.0; i < GetScreenWidth(); i = i + scale)
        {   if(i!=GetScreenWidth() / 2.0)DrawText(TextFormat("%d",(int)((i - screenWidth / 2.0) * scale_x / scale)), i, screenHeight/2.0+2,12,RED)  ;
            
            DrawLine(i, 0, i, screenHeight, Fade(DARKGRAY, 0.3f));
        }
        for (float i = screenHeight / 2.0f; i < screenHeight; i = i + scale)
        {    
            
            if(i!=GetScreenHeight()/2.0)DrawText(TextFormat("%d", (int)((screenHeight / 2.0 - i) * scale_y / scale)),
            GetScreenWidth() / 2 + 2, i, 12, RED);
            DrawLine(0, i, GetScreenWidth(), i, Fade(DARKGRAY, 0.3f));
        }
        for (float i = screenHeight / 2.0f; i >= 0; i = i - scale)
        {
            DrawText(TextFormat("%d", (int)((screenHeight / 2.0 - i) * scale_y / scale)),
            GetScreenWidth() / 2 + 2, i, 12, RED);
            DrawLine(0, i, GetScreenWidth(), i, Fade(DARKGRAY, 0.3f));
        }
        
        DrawFunction(drawProgress,text);
        drawProgress += drawSpeed; 
        if(IsKeyPressed(KEY_R)){
            drawProgress=0;
        }
      
        EndDrawing();
        A = !IsKeyPressed(KEY_DELETE);
    }
}

void bgrndClr(Texture2D darkMode, Texture2D darkMode2, Rectangle dmbtn, Rectangle dmbtn2)
{
    if (back_state % 2)
    {
        ClearBackground(BLACK);
        DrawLine(0, screenHeight / 2.0f, GetScreenWidth(), screenHeight / 2.0f, GRAY);
        DrawLine(GetScreenWidth() / 2.0f, 0, GetScreenWidth() / 2.0f, screenHeight, GRAY);
        DrawTexture(darkMode2, dmbtn2.x, dmbtn2.y, WHITE);
        color = YELLOW;
        color2 = WHITE;
    }
    else
    {
        ClearBackground(WHITE);
        DrawLine(0, screenHeight / 2.0f, GetScreenWidth(), screenHeight / 2.0f, GRAY);
        DrawLine(GetScreenWidth() / 2.0f, 0, GetScreenWidth() / 2.0f, screenHeight, GRAY);
        DrawTexture(darkMode, dmbtn.x, dmbtn.y, WHITE);
        color = DARKBLUE;
        color2= BLACK;

    }
}

void DrawFunction(float drawProgress, char* a) {
    for (float x = -drawProgress; x <= drawProgress; x += 0.01f) {  
        float y=parser(a,x);
        DrawPixel(screenWidth / 2 + x*scale/scale_x ,screenHeight / 2 - y*scale/scale_y   , color);

    }
}
