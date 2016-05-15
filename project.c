#include <stdio.h>
#include <stdbool.h>
#include <string.h>

char data[1000];
int p = 0;
int tmp_n1, tmp_n2;
char last_op;
//복합연산시 숫자를 바꿔주기 위한 변수
//우선은 정수로 변환해서 계산 나중에는 문자열로 처리할것!

void ps_change_n(char op);

int main()
{
	int i;
	char in;
	char op;
	bool need_change_num = false, before_op = false, to_start_change = false; //복합연산 체크를 위해서
	while ((in = getchar()) != '\n') {
		if ((in != ' '))
			data[p++] = in;
		if (need_change_num == true && (in == '*' || in == '/' || in == '%' || in == '+' || in == '-')) {
			to_start_change = true;
			last_op = in;
			p -= 2;
			tmp_n1 = 0;
			tmp_n2 = 0;
		}
		if (to_start_change == true) {
			while (data[p] != '*' && data[p] && '/' && data[p] != '%') {
				tmp_n2 += data[p--] - '0';
				tmp_n2 *= 10;
			}
			op = data[p--];
			while(data[p] >= '0' && data[p] <= '9') {
				tmp_n1 += data[p--] - '0';
				tmp_n1 *= 10;
			}
			ps_change_n(op);
			to_start_change = false;
			need_change_num = false;
		}
		if (in == '*' || in == '/' || in == '%')
			need_change_num = true;
	}
	for (i = 0; i < p; i++) {
		if(!(data[i] >= '0' && data[i] <= '9'))
			printf(" %c ", data[i]); 
		else
			printf("%c", data[i]);
	}
	printf("\n");

	return 0;
}

void ps_change_n(char op)
{
	int i = 0;
	int r;
	char rev_r[1000] = { 0, };
	int len = 0;

	if (op == '*') r = tmp_n1 * tmp_n2;
	else if (op == '/') r = tmp_n1 / tmp_n2;
	else r = tmp_n1 % tmp_n2;
	while (r > 0) {
		rev_r[len++] = r % 10 + '0';
		r /= 10;
	}
	for(i = len - 1; i >= 0; i--) {
		data[++p] = rev_r[i];
	}
	data[++p] = last_op;
	p += 1;
}
