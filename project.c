#include <stdio.h>
#include <stdbool.h>
#include <string.h>

char data[1000];
int p = 0;
int left_n, right_n;
//복합연산시 숫자를 바꿔주기 위한 변수
//우선은 정수로 변환해서 계산 나중에는 문자열로

void write_new_num(char op, char last_op);
int ps_op(char op); //문자열로 처리를 바꿔주면 void로
int pow(int x, int y);

int main()
{
	int i;
	int p_10;
	char input[1000];
	int len_input;
	char op, last_op;
	bool need_change_num = false, to_start_change = false; //복합연산 체크를 위해서
	gets(input);
	len_input = strlen(input);
	for (i = 0; i < len_input; i++) {
		if (true) {
			if ((input[i] == ' '))
				continue;	//작업의 편의를 위해 공백을 제거한다
			data[p++] = input[i];
			if (need_change_num == true && (input[i] == '*' || input[i] == '/' || input[i] == '%' || input[i] == '+' || input[i] == '-')) {
				to_start_change = true;
				last_op = input[i];
				p -= 2;
				left_n = 0;
				right_n = 0;
			}
			if (to_start_change == true) {
				p_10 = 0;
				while (data[p] >= '0' && data[p] <= '9') {
					right_n += (data[p--] - '0') * pow(10, p_10++);
				}
				p_10 = 0;
				op = data[p--];
				while (data[p] >= '0' && data[p] <= '9') {
					left_n += (data[p--] - '0') * pow(10, p_10++);
				}
				write_new_num(op, last_op);
				to_start_change = false;
				need_change_num = false;
			}

			if (input[i] == '*' || input[i] == '/' || input[i] == '%')
				need_change_num = true;
		}
	}
	for (i = 0; i < p; i++) {
		if (!(data[i] >= '0' && data[i] <= '9'))
			printf(" %c ", data[i]);
		else
			printf("%c", data[i]);
	}
	printf("\n");

	return 0;
}

void write_new_num(char op, char last_op)
{
	int i = 0;
	int r;
	char rev_r[1000] = { 0, };
	int len = 0;

	r = ps_op(op);
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

int ps_op(char op)
{
	if (op == '+')
		return left_n + right_n;
	else if (op == '-')
		return left_n - right_n;
	else if (op == '*')
		return left_n * right_n;
	else if (op == '/')
		return left_n / right_n;
	else if (op == '%')
		return left_n % right_n;
}
int pow(int x, int y)
{
	int i;
	int r = 1;
	for (i = 0; i < y; i++)
		r *= x;
	return r;
}
