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
char left[60], right[60];
char r[1000]; //연산의 결과를 저장할 배열

char op;

bool check_var(); //변수를 정의할 때 중복되는지 체크하는 함수
void set_var(); //변수를 선언해주는 함수
void read_var(); //연산을 하다가 변수를 읽어올 때 사용할 함수
void show_var(); //VAR명령을 처리하는 함수
void load_var(); //load명령을 처리하는 함수
void save_var(); //save명령을 처리하는 함수
void input_string();
int check_error(); //error체크를 하면서 입력받은 명령의 종류를 처리해주는 함수

				   //작업중인 함수들
void copy_left(int s, int e);
void copy_right(int s, int e);
void write_new_num(); //복합 연산시 숫자를 바꿔줄 함수
void change_equation(); //수식을 정리해주는 함수
void ps_cal(); //문자열로 처리를 바꿔주면 void로
void set_clear(); //left, right, r 배열 초기화
void remove_data(int n); //data배열 초기화

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

			//printf("\n");
		}
	}
	return 0;
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
			while (data[p] >= '0' && data[p] <= '9')
				p--;
			s = p + 1;
			copy_right(s, e);
			op = data[--p];
			p -= 2;
			e = p;
			while (data[p] >= '0' && data[p] <= '9')
				p--;
			s = p + 1;
			p++;
			copy_left(s, e);
			ps_cal();
			write_new_num();
			to_start_change = false;
			need_change_num = false;
			pass_this = true;
		}
		if (input[i] == '*' || input[i] == '/' || input[i] == '%') //*, /, % 연산자가 있으므로 우선적으로 계산할 필요가 있음
			need_change_num = true;
	}
	puts(data);
}

void write_new_num()
{
	int i = 0;
	int len = 0;

	remove_data(p);
	len = strlen(r);
	for (i = 0; i < len; i++)
		data[p++] = r[i];
	data[p++] = ' ';
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
	for (i = s; i <= e; i++)
		right[idx++] = data[i];
}

void copy_left(int s, int e)
{
	int i, idx = 0;
	for (i = s; i <= e; i++)
		left[idx++] = data[i];
}

void set_clear()
{
	int i;
	for (i = 0; i < strlen(left); i++)
		left[i] = '\0';
	for (i = 0; i < strlen(right); i++)
		right[i] = '\0';
	for (i = 0; i < strlen(r); i++)
		r[i] = '\0';
}

void remove_data(int n)
{
	int i, len = strlen(data);
	for (i = n; i < len; i++)
		data[i] = '\0';
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

void plus() //덧셈 함수
{
	//for testing
	strcpy(r, "testing plus");
}
void minus() //뺄셈
{
	//for testing
	strcpy(r, "testing minus!");
}
void multiple()
{
	//for testing
	strcpy(r, "testing multiple!");
}
void divide()
{
	//for testing
	strcpy(r, "testing divide!");
}
void modular()
{
	//for testing
	strcpy(r, "testing modular!");
}
