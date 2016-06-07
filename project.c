#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

char var_value[11][100];
char var_name[11];//변수 선언을 위한 배열
int number_of_var = 0;
char input[1000];
char data[1000];
int p = 0;
char r[1000]; //연산의 결과를 저장할 배열
char op;

/*소수점과 부호 구현을 위한 부분*/
bool left_possitive_num = true;
bool right_possitive_num = true;
bool left_has_point = false;
bool right_has_point = false;
int pos_of_left_point = 0;
int pos_of_right_point = 0;
char left[60], right[60];
char left_after_point[60], right_after_point[60];

bool compare();//두 수를 어느것이 큰지 비교해주는 함수
bool check_var(); //변수를 정의할 때 중복되는지 체크하는 함수
void set_var(); //변수를 선언해주는 함수
void read_var(); //연산을 하다가 변수를 읽어올 때 사용할 함수
void show_var(); //VAR명령을 처리하는 함수
void load_var(); //load명령을 처리하는 함수
void save_var(); //save명령을 처리하는 함수
void input_string();//엔터를 치기전 까지 문자를 입력받는 함수
int check_error(); //error체크를 하면서 입력받은 명령의 종류를 처리해주는 함수

/*작업중인 함수들*/
void copy_left(int s, int e);
void copy_right(int s, int e);
void write_new_num(); //연산한 결과를 수식에 써주는 함수
void change_equation(); //수식을 정리해주는 함수
void ps_cal(); //연산하는 함수를 호출해주는 함수
void set_clear(); //거의 모든 전역변수 초기화
void remove_data(int n); //data배열을 n번째 요소부터 초기화
void remove_input(int n);
void remove_r(int n);
void do_calculate(); //정리된 수식을 계산하는 함수
void print_result();//,를 찍어주기 위한 함수
int number_of_op_for_data(); //입력받은 수식의 연산자의갯수를 조사하는 함수
int number_of_op_for_input(); //입력받은 수식의 연산자의갯수를 조사하는 함수
int number_of_num_for_input(); //입력받은 수식의 연산자의갯수를 조사하는 함수

/*사칙연산 함수들*/
void plus(); //덧셈
void minus(); //뺄셈
void multiple(); //곱셈
void divide(); //나눗셈
void modular(); //나머지
void swap();

void swap(){
	int t;
	for(int i = 0 ; i < 60; i++){
		t = left[i];
		left[i] = right[i];
		right[i] = t;
	}
	t = left_possitive_num;
	left_possitive_num = right_possitive_num;
	right_possitive_num = t;
}

void plus() //덧셈 함수
{
    if(left_possitive_num == true && right_possitive_num == false){
		right_possitive_num = true;
		minus();
		return;
	}
	else if(left_possitive_num == false && right_possitive_num == true){
		swap();
		right_possitive_num = true;
		minus();
		return;
	}
	else if(left_possitive_num == false && right_possitive_num == false){
		r[0] = '-';
		printf("-");
	}
	
    int left_rev[100] = { 0 }, right_rev[100] = { 0 };
    int idx = 1;
    int len = 0;
    for (int i = strlen(left) - 1; i >= 0; i--)
        left_rev[idx++] = left[i] - '0';
    idx = 1;
    for (int i = strlen(right) - 1; i >= 0; i--)
        right_rev[idx++] = right[i] - '0';
    if (strlen(right) > strlen(left))
        len = strlen(right) + 1;
    else
        len = strlen(left) + 1;
    int r_rev[101] = { 0 };
    for (int i = 1; i <= len; i++) {
        r_rev[i] += left_rev[i] + right_rev[i];
        if (r_rev[i] >= 10) {
            r_rev[i + 1] += r_rev[i] / 10;
            r_rev[i] %= 10;
        }
    }
    //초등학교때 덧셈하듯이 각 자리를 더해주고 필요하면 자릴수를 올려줌
    idx = 0;
    for (int i = len; i >= 1; i--) {
        if (idx == 0 && r_rev[i] == 0 && i != 1)
            continue;
        r[idx++] = r_rev[i] + '0';
    }
}

void minus() //뺄셈
{
    if(left_possitive_num == true && right_possitive_num == false){
   		right_possitive_num = true;
   		plus();
   		return;
   }
   else if(left_possitive_num == false && right_possitive_num == true){
   		right_possitive_num = false;
   		plus();
   		return;
   }
   else if(left_possitive_num == false && right_possitive_num == false){
   		r[0] = '-';
   }
   
   int big = 0;
   for(int i = 0; i < 60 ;i++){
   		if(left[i] > right[i]){
   			big = 1;
   			break;
   		}
   		else if(left[i] < right[i]){
   			big = 2;
   			break;
   		}
   }
   if(big == 2){
   		swap();
   		if(r[0] == '-')
   			r[0] = 0;
   		else
   			r[0] = '-';
   }
   if(r[0] == '-')
   		printf("-");
    int left_rev[100] = { 0 }, right_rev[100] = { 0 };
    int idx = 1;
    int len = 0;
    if(!compare()) {
        printf("-");
        swap();
    }
    for (int i = strlen(left) - 1; i >= 0; i--)
        left_rev[idx++] = left[i] - '0';
    idx = 1;
    for (int i = strlen(right) - 1; i >= 0; i--)
        right_rev[idx++] = right[i] - '0';
    if (strlen(right) > strlen(left))
        len = strlen(right) + 1;
    else
        len = strlen(left) + 1;
    int r_rev[101] = { 0 };
    for (int i = 1; i <= len; i++)
        r_rev[i] += left_rev[i] - right_rev[i];
    for (int i = len; i >= 1; i--) {
        if (r_rev[i]<0)
        {
            r_rev[i] = 10 + r_rev[i];
            r_rev[i + 1] -= 1;
        }
    }
    idx = 0;
    for (int i = len; i >= 1; i--) {
        if (idx == 0 && r_rev[i] == 0 && i != 1)
            continue;
        r[idx++] = r_rev[i] + '0';
    }
    //1.각 배열 자리수 끼리 뺄셈 연산
    //2.각 배열자리를  스캔
    //3.만약 각 자리의 숫자가 음수이면
    //부호를 바꿔주고 윗 자리수의 값을 1을 뺀다
    //4.다시 배열 순서를 stack을 활용하여 다시 바꾼다.
}


void divide()
{
    int n=1,int_r[100]={0},float_r[9],len=1,idx=0;

    while(compare() || (!strcmp(left, right)))
    {
        minus();
        int_r[0]++;
        if(int_r[0]==10)
        {
            for(int i=0;i<len;i++)
            {
                if(int_r[i]>=10){
                    int_r[i+1]++;
                    int_r[i]-=10;
                }

            }

            if(int_r[len]==1)
                len++;

        }
        strcpy(left,r);
        remove_r(0);
    }

    for (int i = len; i >= 0; i--) 
        r[idx++]=int_r[i]+'0';
}      



void modular()
{
    char right_origin[100];
    strcpy(right_origin, right);
    while(compare()) {
        int idx = strlen(right);
        right[idx] = '0';
    }
    right[strlen(right) - 1] = '\0';
    while(compare() || !strcmp(left, right))
    {
        minus();
        strcpy(left, r);
        remove_r(0);
        while(!compare() && strcmp(right, right_origin))
            right[strlen(right) - 1] = '\0';
    }
    strcpy(r, left);
}

/******************************************Do not edit******************************************************/

void print_result() {
    bool is_r_minus_num = false;
    bool does_r_have_point = false;
    int pos_point;
    if(r[0] == '-')
        is_r_minus_num = true;
    for(int i = 0; i < strlen(r); i++)
        if(r[i]=='.')
            does_r_have_point = true;

    char to_print_thingsi_rev[100];
    int idx = 0,n=0,pass_point=0,pass_point_yes=0,k=0; 
    for(int i = strlen(r)-1 ; i>= is_r_minus_num;i--){
        to_print_thingsi_rev[idx++] = r[i];
        if(pass_point_yes==1)
            k++;
        if(i==is_r_minus_num){
            to_print_thingsi_rev[idx]='\0';
            continue;
        }
        if(r[i-1]=='.'){
            to_print_thingsi_rev[idx++]='.';
            i--;
            pass_point_yes=1;
            n=0;
        }


        else if(idx==4*n+3&&pass_point_yes==0){
            to_print_thingsi_rev[idx++]=',';
            n++;
        }

        else if(k==3*n+3&&pass_point_yes==1){
            to_print_thingsi_rev[idx++]=',';
            n++;
        }

    }
    if(is_r_minus_num)
        printf("-");   

    for(int i = strlen(to_print_thingsi_rev)-1; i>=0 ;i--)
        printf("%c", to_print_thingsi_rev[i]);


}

void do_calculate()
{
    int cycle = number_of_op_for_data();
    while (cycle) {
        set_clear();
        int s, e;
        p = strlen(data) - 1;
        e = p;
        while (data[p] != ' ' && p >= 0)
            p--;
        s = p + 1;
        copy_right(s, e);
        op = data[--p];
        while (!(data[p] >= '0' && data[p] <= '9'))
            p--;
        e = p;
        while (data[p] != ' ' && p >= 0)
            p--;
        s = p + 1;
        p++;
        copy_left(s, e);
        ps_cal();
        write_new_num();
        cycle--;
    }
    int len = strlen(data);
    for (int i = 0; i <= len; i++)
        r[i] = data[i];
}

int number_of_op_for_data()
{
    int value_to_return = 0;
    for (int i = 1; i < strlen(data); i++)
        if ((data[i] == '*' || data[i] == '+' || data[i] == '/' || data[i] == '-' || data[i] == '%') && data[i - 1] == ' ' && data[i + 1] == ' ')
            value_to_return++;
    return value_to_return;
}

int number_of_op_for_input()
{
    int value_to_return = 0;
    for (int i = 1; i < strlen(input); i++)
        if ((input[i] == '*' || input[i] == '+' || input[i] == '/' || input[i] == '-' || input[i] == '%') && input[i - 1] == ' ' && input[i + 1] == ' ')
            value_to_return++;
    return value_to_return;
}

int number_of_num_for_input()
{
    int value_to_return = 0;
    int idx = 0;
    while (idx < strlen(input)) {
        bool flag = false;
        while ((input[idx] >= '0' && input[idx] <= '9') || input[idx] == '.' || (input[idx] >= 'A' && input[idx] <= 'Z') || (input[idx] >= 'a' && input[idx] <= 'z')) {
            flag = true;
            idx++;
            if ((input[idx] >= 'A' && input[idx] <= 'Z') || (input[idx] >= 'a' && input[idx] <= 'z'))
                break;
        }
        if (flag)
            value_to_return++;
        while (!((input[idx] >= '0' && input[idx] <= '9') || input[idx] == '.' || (input[idx] >= 'A' && input[idx] <= 'Z') || (input[idx] >= 'a' && input[idx] <= 'z'))) {
            if(input[idx + 1] != '\0')
                idx++;
            else
                break;
        }
    }
    return value_to_return;
}

void show_var()
{
    int i;
    if (number_of_var == 0)
        printf("정의된 변수 없음 \n");
    for (i = 0; i < number_of_var; i++) {
        printf("%c = ", var_name[i]);
        for (int j = 0; j <= strlen(var_value[i]); j++)
            r[j] = var_value[i][j];
        print_result();
        printf("\n");
    }
}

void load_var()
{
    FILE *save;
    save = fopen("var.txt", "r");
    if (save == NULL) {
        printf("error : file doesn't exit. \n");
        return;
    }
    char name, value[70];
    while (fscanf(save, "%c = %s\n", &name, value) != EOF) {
        input[0] = name;
        input[1] = ' ';
        input[2] = '=';
        input[3] = ' ';
        for (int i = 0; i <= strlen(value); i++)
            input[i + 4] = value[i];
        check_error();
    }
    fclose(save);
}

void save_var()
{
    FILE *save;
    save = fopen("var.txt", "w");
    for (int i = 0; i < number_of_var; i++) {
        fprintf(save, "%c = ", var_name[i]);
        fprintf(save, "%s \n", var_value[i]);
    }
    fclose(save);
}

void input_string()
{
    char c;
    int i = 0;
    while (1) {
        c = getchar();
        if (c == '\n' || c == EOF) {
            input[i] = '\0';
            break;
        }
        else
            input[i++] = c;
    }
}

void change_equation()
{
    int i, x;
    int len_input;
    bool need_change_num = false, to_start_change = false, pass_this = true; //복합연산 체크를 위해서
    len_input = strlen(input);
    p = 0;
    remove_data(0);
    for (i = 0; i < len_input; i++) {
        data[p++] = input[i];
        if (data[p - 1] >= 'A' && data[p] <= 'Z') {
            p--;
            read_var();
        }
        if (!pass_this && input[i] == ' ')
            to_start_change = true;
        if (need_change_num == true && input[i] == ' ' && pass_this) { //연산자 뒤에 처음 오는 공백은 아직 두번째 숫자를 입력받기 전이므로 pass
            pass_this = false;
            set_clear();
        }
        if (to_start_change == true) {
            //요부분 나중에 함수로 교체하자...!
            //재사용하기 괜찮을듯 하다....!
            int s, e;
            p -= 2;
            e = p;
            //while ((data[p] >= '0' && data[p] <= '9') || data[p] == '.')
            while (data[p] != ' ' && p >= 0)
                p--;
            s = p + 1;
            copy_right(s, e);
            op = data[--p];
            while (!(data[p] >= '0' && data[p] <= '9'))
                p--;
            e = p;
            //while ((data[p] >= '0' && data[p] <= '9') || data[p] == '.')
            while (data[p] != ' ' && p >= 0)
                p--;
            s = p + 1;
            p++;
            copy_left(s, e);
            ps_cal();
            write_new_num();
            to_start_change = false;
            need_change_num = false;
            pass_this = true;
            data[p++] = ' ';
        }
        if (input[i] == '*' || input[i] == '/' || input[i] == '%') //*, /, % 연산자가 있으므로 우선적으로 계산할 필요가 있음
            need_change_num = true;
    }
}

void write_new_num()
{
    int i = 0;
    int len = 0;
    remove_data(p);
    len = strlen(r);
    for (i = 0; i < len; i++)
        data[p++] = r[i];
    set_clear();
}

void ps_cal()
{
    if (op == '+')
        plus(); //덧셈함수 호출
    else if (op == '-')
        minus(); //마이너스 함수 호출
    else if (op == '*')
        multiple(); //곱셈함수 호출
    else if (op == '/')
        divide(); //나눗셈 함수 호출
    else if (op == '%')
        modular(); //나머지연산 함수 호출
}

void copy_right(int s, int e)
{
    int i, idx = 0;
    if (data[s] == '-') {
        right_possitive_num = false;
        s++;
    }
    for (i = s; i <= e; i++) {
        if (!right_has_point)
            right[idx++] = data[i];
        else
            right_after_point[idx++] = data[i];
        if (data[i] == '.') {
            right[idx - 1] = '\0';
            pos_of_right_point = idx;
            right_has_point = true;
            idx = 0;
        }
    }
}

void copy_left(int s, int e)
{
    int i, idx = 0;
    if (data[s] == '-') {
        left_possitive_num = false;
        s++;
    }
    for (i = s; i <= e; i++) {
        if (!left_has_point)
            left[idx++] = data[i];
        else
            left_after_point[idx++] = data[i];
        if (data[i] == '.') {
            left[idx - 1] = '\0';
            pos_of_left_point = idx;
            left_has_point = true;
            idx = 0;
        }
    }
}

void set_clear()
{
    int i;
    int len = strlen(left);
    for (i = 0; i <= len; i++)
        left[i] = '\0';
    len = strlen(right);
    for (i = 0; i <= len; i++)
        right[i] = '\0';
    len = strlen(r);
    for (i = 0; i <= len; i++)
        r[i] = '\0';
    len = strlen(left_after_point);
    for (i = 0; i <= len; i++)
        left_after_point[i] = '\0';
    len = strlen(right_after_point);
    for (i = 0; i <= len; i++)
        right_after_point[i] = '\0';
    left_possitive_num = true;
    right_possitive_num = true;
    left_has_point = false;
    right_has_point = false;
    pos_of_left_point = 0;
    pos_of_right_point = 0;//매번 초기화
}

void remove_data(int n)
{
    int i, len = strlen(data);
    for (i = n; i < len; i++)
        data[i] = '\0';
}

void remove_input(int n)
{
    int i, len = strlen(input);
    for (i = n; i < len; i++)
        input[i] = '\0';
}

void remove_r(int n)
{
    int i, len = strlen(r);
    for (i = n; i < len; i++)
        r[i] = '\0';
}

bool check_var()
{
    int idx, i;
    bool r = false; //같은 이름을 가진 변수가 존재하지 않으면 false
    for (idx = 0; idx < number_of_var; idx++)
        if (var_name[idx] == input[0]) {
            r = true;
            break;
        }
    if (r == true) {
        int len = strlen(var_value[idx]);
        for (i = 0; i < len; i++)
            var_value[idx][i] = '\0';
    }
    return r;
}
/*
true : left is bigger than right
false : right is bigger than left
 */
bool compare()
{
    bool r = true;
    int len_of_left = strlen(left);
    int len_of_right = strlen(right);
    if (!left_possitive_num && !right_possitive_num)
        r = !r;
    if (left_possitive_num != right_possitive_num) {
        if (left_possitive_num)
            return r;
        else
            return !r;
    }
    if (len_of_left > len_of_right)
        return r;
    else if (len_of_left < len_of_right)
        return !r;
    for (int i = 0; i < len_of_left; i++) {
        if (left[i] > right[i])
            return r;
        else if (left[i] < right[i])
            return !r;
    }
}

void set_var()
{
    int idx = 0;
    if (input[0] >= 'a' && input[0] <= 'z')
        input[0] -= ('a' - 'A');
    int idx_of_var_to_set = number_of_var;
    if (check_var()) {
        int i;
        for (i = 0; i < number_of_var; i++)
            if (var_name[i] == input[0])
                break;
        idx_of_var_to_set = i;
    }
    else
        number_of_var++;

    int i;
    var_name[idx_of_var_to_set] = input[0];
    for (i = 0; i < strlen(input); i++)
        if (input[i] >= '0' && input[i] <= '9')
            break;
    i = 4;
    remove_data(0);
    for (; i < strlen(input); i++) {
        data[idx++] = input[i];
    }
    remove_input(0);
    for (i = 0; i < strlen(data); i++)
        input[i] = data[i];
    remove_data(0);
    change_equation();
    do_calculate();
    for (i = 0; i < strlen(r); i++)
        var_value[idx_of_var_to_set][i] = r[i];
    set_clear();
}

void read_var()
{
    int i, idx;
    if (data[p] >= 'a' && data[p] <= 'z')
        data[p] -= ('a' - 'A');
    for (idx = 0; idx < number_of_var; idx++)
        if (var_name[idx] == data[p])
            break;
    for (i = 0; i < strlen(var_value[idx]); i++)
        data[p++] = var_value[idx][i];
}

int main()
{
    printf("Start...\n");
    while (1) {
        set_clear();
        remove_data(0);
        printf("(input)");
        input_string();
        int flag = check_error();
        if (flag == -1) {
            printf("= error \n");
            continue;
        }
        else if (flag == 0) //command를 이미 처리했으므로
            continue;
        else if (flag == 1) //변수 선언하는 부분
            continue;
        else if (flag == 2)
            return 0;
        else { //연산을 처리하는 부분
            printf("= ");
            change_equation();
            set_clear();
            //수식 정리 완료 
            //수식은 data 배열에 저장되있음
            do_calculate();
            //정리한 수식을 계산
            print_result();
            printf("\n");
        }
    }
    return 0;
}

int check_error()
{
    if (input[0] >= 'a' && input[0] <= 'z' && strlen(input) == 1)
        input[0] -= ('a' - 'A');
    int i, j;
    int number_of_equal = 0;
    bool error = false;
    for (i = 0; i < strlen(input); i++) {
        if (input[i] == '=')
            number_of_equal++;
        if (!(input[i] == ' ' || input[i] == '*' || input[i] == '/' || input[i] == '%' || input[i] == '+' || input[i] == '-' || (input[i] >= '0' && input[i] <= '9') || input[i] == '.')) {
            error = true;
            for (j = 0; j < number_of_var; j++) {
                if (input[i] == var_name[j])
                    error = false;
            }
        }
        if (input[i] == '/' && input[i + 1] == ' ' && input[i + 2] == '0' && (input[i + 3] == ' ' || input[i + 3] == '\0')) {
            printf("error : divide by zero \n");
            return 0;
        }
    }
    if (number_of_equal == 1) {
        set_var();
        return 1; //변수선언
    }
    else if (strcmp(input, "clear") == 0) {
        system("clear");
        return 0;
    }
    else if (strcmp(input, "load") == 0) {
        load_var();
        return 0;
    }
    else if (strcmp(input, "save") == 0) {
        save_var();
        return 0;
    }
    else if (strcmp(input, "VAR") == 0) {
        show_var();
        return 0;
    }
    else if (strcmp(input, "end") == 0) {
        return 2; //종료
    }
    else if (error == true) {
        if (strlen(input) == 1 && (input[0] >= 'A' && input[0] <= 'Z')) {
            printf("= undefined.\n");
            return 0;
        }
        return -1; //error출력
    }
    if (number_of_op_for_input() + 1 != number_of_num_for_input())
        return -1;
    return 3; //연산하기
}

void multiple()
{
    int tmp[101][101] = { 0 };
    int left_rev[100] = { 0 }, right_rev[100] = { 0 };
    int idx = 1;
    int len = 0;
    /*
       자릿수를 맞춰주기 편하려고 left와 right 숫자를 거꾸로 뒤집어줌
       계산의 편의를 위해 int형 사용
     */

    strcat(right, right_after_point);
    strcat(left, left_after_point);

    for (int i = strlen(left) - 1; i >= 0; i--)
        left_rev[idx++] = left[i] - '0';

    idx = 1;

    for (int i = strlen(right) - 1; i >= 0; i--)
        right_rev[idx++] = right[i] - '0';

    for (int i = 1; i <= strlen(right); i++) { //right의 길이만큼 1의 자리부터 시작
        int j;
        for (j = 1; j < i; j++)
            tmp[i][j] = 0;
        for (j = i; j < strlen(left) + i; j++) {
            tmp[i][j] += (right_rev[i]) * (left_rev[j - i + 1]); //right의 각 자리에 있는 수와 left를 곱해줌 
            if (tmp[i][j] >= 10) { //자릿수를 올려주는 부분
                tmp[i][j + 1] += tmp[i][j] / 10;
                tmp[i][j] %= 10;
            }
        }
        if (j - 1 > len)
            len = j;
    }
    //마치 초등학교때 각 자리마다 곱셈을 하는것 처럼
    int r_rev[101] = { 0 };
    for (int i = 1; i <= len; i++) { //각 자릿수마다 값을 알기위해서 더해줌
        int sum = 0;
        for (int j = 1; j <= strlen(right); j++) {
            sum += tmp[j][i];
        }
        r_rev[i] += sum;
        if (r_rev[i] >= 10) {
            r_rev[i + 1] += r_rev[i] / 10;
            r_rev[i] %= 10;
        }
    }
    //초등학교때 곱셈한 것을 다시 더하는것 처럼
    int pos = strlen(left_after_point) + strlen(right_after_point);
    idx = 0;
    if (left_possitive_num != right_possitive_num)
        r[idx++] = '-';
    for (int i = len; i >= 1; i--) {
        if ((idx == 0 || (idx == 1 && r[0] == '-')) && r_rev[i] == 0 && i != 1)
            continue;
        if (i == pos)
            r[idx++] = '.';
        r[idx++] = r_rev[i] + '0';
    }
}
