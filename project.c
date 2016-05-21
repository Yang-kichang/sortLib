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
char left[60]={0}, right[60]={0};
char r[1000]; //연산의 결과를 저장할 배열

bool check_var(); //변수를 정의할 때 중복되는지 체크하는 함수
void set_var(); //변수를 선언해주는 함수
void read_var(); //연산을 하다가 변수를 읽어올 때 사용할 함수
void write_new_num(char op, char last_op); //복합 연산시 숫자를 바꿔줄 함수
void show_var(); //VAR명령을 처리하는 함수
void load_var(); //load명령을 처리하는 함수
void save_var(); //save명령을 처리하는 함수
void change_equation(); //수식을 정리해주는 함수
void input_string();
int check_error(); //error체크를 하면서 입력받은 명령의 종류를 처리해주는 함수
void ps_op(char op); //문자열로 처리를 바꿔주면 void로
int pow(int x, int y);

//사칙연산 함수들
void plus(); //덧셈
void minus(); //뺄셈
void multiple(); //곱셈
void divide(); //나눗셈
void modular(); //나머지


int main()
{
	printf("Start...\n");
	while (1) {
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
			set_var();
		else if (flag == 2)
			return 0;
		else { //연산을 처리하는 부분
			printf("= ");
			change_equation();
			//수식 정리 완료
			for (int i = 0; i < p; i++) {
				if (!(data[i] >= '0' && data[i] <= '9'))
					printf(" %c ", data[i]);
				else
					printf("%c", data[i]);
			}
			printf("\n");
		}
	}
	return 0;
}

bool check_var()
{
	int idx, i;
	bool r = false;
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

void set_var()
{
	int i;
	int idx = 0;
	if (check_var()) {
		int p_var;
		for (p_var = 0; p_var < number_of_var; p_var++)
			if (var_name[p_var] == input[0])
				break;
		for (i = 0; ; i++)
			if (input[i] >= '0' && input[i] <= '9')
				break;
		for (; i < strlen(input); i++)
			var_value[p_var][idx++] = input[i];
	}
	else {
		var_name[number_of_var] = input[0];
		for (i = 0; ; i++)
			if (input[i] >= '0' && input[i] <= '9')
				break;
		for (; i < strlen(input); i++)
			var_value[number_of_var][idx++] = input[i];
		number_of_var++;
	}
}

void read_var()
{
	int i, idx;
	for (idx = 0; idx < number_of_var; idx++)
		if (var_name[idx] == data[p])
			break;
	for (i = 0; i < strlen(var_value[idx]); i++)
		data[p++] = var_value[idx][i];
}

void write_new_num(char op, char last_op)
{
	int i = 0;
	char rev_r[1000] = { 0, };
	int len = 0;

	//r = ps_op(op);
	//r을 문자열로 만들고 사칙연산 함수 결과를 r에 저장해야함
	while (r > 0) {
		rev_r[len++] = r % 10 + '0';
		r /= 10;
	}
	for (i = len - 1; i >= 0; i--) {
		data[++p] = rev_r[i];
	}
	data[++p] = last_op;
	p += 1;
}

int check_error()
{
	int i, j;
	int number_of_equal = 0;
	bool error = false;
	for (i = 0; i < strlen(input); i++) {
		if (input[i] == '=')
			number_of_equal++;
		if (!(input[i] == ' ' || input[i] == '*' || input[i] == '/' || input[i] == '%' || input[i] == '+' || input[i] == '-' || (input[i] >= '0' && input[i] <= '9'))) {
			error = true;
			for (j = 0; j < number_of_var; j++) {
				if (input[i] == var_name[j])
					error = false;
			}
		}
	}
	if (number_of_equal == 1)
		return 1; //변수선언
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
	else if (strcmp(input, "end") == 0)
		return 2; //종료
	else if (error == true) {
		if (strlen(input) == 1 && (input[0] >= 'A' && input[0] <= 'Z')) {
			printf("= undefined.\n");
			return 0;
		}
		return -1; //error출력
	}
	return 3; //연산하기
}

void show_var()
{
	int i;
	if (number_of_var == 0) {
		printf("정의된 변수 없음 \n");
	}
	for (i = 0; i < number_of_var; i++) {
		printf("%c = ", var_name[i]);
		puts(var_value[i]);
	}
}

void load_var()
{
	printf("This is load_var() \n");
}

void save_var()
{
	printf("This is save_var() \n");
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

void ps_op(char op)
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

void change_equation()
{
	int i,x;
	int p_10;
	int len_input;
	char op, last_op;
	bool need_change_num = false, to_start_change = false; //복합연산 체크를 위해서
	bool is_it_num = true;
	len_input = strlen(input);
	p = 0;
	for (i = 0; i < len_input; i++) {

		if ((input[i] == ' '))
			continue;	//작업의 편의를 위해 공백을 제거한다
		data[p++] = input[i];
		if (data[p - 1] >= 'A' && data[p] <= 'Z') {
			p--;
			read_var();
		}
		if (need_change_num == true && (input[i] == '*' || input[i] == '/' || input[i] == '%' || input[i] == '+' || input[i] == '-')) {
			to_start_change = true;
			last_op = input[i];
			p -= 2;
			for(x=0;x<60;x++){
				left[x] = 0;
				right[x] = 0;
			}
		}
		if (to_start_change == true) {
			p_10 = 0;
			while (data[p] >= '0' && data[p] <= '9') {
				//  right[60] += (data[p--] - '0') * pow(10, p_10++); 수정필요
			}
			p_10 = 0;
			op = data[p--];
			while (data[p] >= '0' && data[p] <= '9') {
				//  left[60] += (data[p--] - '0') * pow(10, p_10++); 수정필요
			}
			write_new_num(op, last_op);
			to_start_change = false;
			need_change_num = false;
		}
		if (input[i] == '*' || input[i] == '/' || input[i] == '%')
			need_change_num = true;
	}
}
int pow(int x, int y)
{
	int i;
	int r = 1;
	for (i = 0; i < y; i++)
		r *= x;
	return r;
}

void plus() //덧셈 함수
{
}
void minus() //뺄셈
{
}
void multiple()
{
}
void divide()
{
}
void modular()
{
}
