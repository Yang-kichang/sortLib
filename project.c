
//***************주의********************입력방법: (숫자) 공백 (연산자) 공백 (숫자)**************************************************

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

char var_value[11][100];
char var_name[11];
int number_of_var = 0;
bool flag1 = false;

/*두 수의 대소를 비교하는 함수*/
bool compare_func(char first[], char second[]);// 입력받은 두 케이스의 크기를를 비교하는 함수.

											   /*변수와 관련된 부분*/
bool check_var(char name);//변수가 이미 존재하는지 검사
void set_var(char input[]);//변수를 설정하는 함수
void read_var(char data[], int p);//변수를 읽어주는 함수
void show_var();//저장된  변수를 보여주는 함수
void load_var();//변수를 불러오는 함수
void save_var();//변수를 설정한 수 저장하는 함수


				/*입력된 데이터 체크하는 부분*/
void input_string(char input[]);//데이터를 입력받는 함수
int check_error(char input[]);//입력에 에러를 체크해주는 함수
int number_of_op(char target[]);//연산자를 저장하는 함수
int number_of_num(char target[]);//수치를 저장하는 함수

								 /*사칙연산을 위한 부분*/
bool ps_cal(char left[], char right[], char op, char result[]);//연산처리를 하는 함수
void plus(char left[], char right[], char result[]);//덧셈처리함수
void minus(char left[], char right[], char result[]);//뺄셈처리함수
void multiple(char left[], char right[], char result[]);//곱셈처리함수
bool divide(char left[], char right[], char result[]);//나눗셈처리함수
bool modular(char left[], char right[], char result[]);//나머지연산 처리함수
bool dose_it_have_point(char target[], char after_point[]);//소수점 체크함수
														   /*연산과 상관없는 부분*/
void print_result(char[]);//결과를 화면에 출력해주는 함수
void remove_zero(char[]);//배열에 값과 관계없는 '0'값을 제거해주는함수

						 /*초기화와 관련된 부분*/
void clear_all(char[], int value, int target_size);//memset과 동일한 기능을 하는 함수
void clear_after_n(char[], int value, int from, int target_szie);//배열에서 arr[n] 이후에 있는 값들을 제거하는 함수

																 /*연산 우선순위 & 연산을 처리하는 부분*/
void get_ans(char input[], char result[]);//답을 구하는 함수
int get_num(char from[], char target[], int p);//스트림에서 숫자를 끌어오는 함수.
bool next_op(char input[], int p);//다음 연산자가 무엇인지 체크하는 함수

								  /*for remove_zero()*/
bool is_there_point_after_me(char[], int);
bool is_there_point_before_me(char[], int);
bool is_there_any_num_before_me(char[], int);
bool is_there_any_num_after_me(char[], int);

int main()
{
	puts("Start...");
	while (1) {
		printf("(input)");
		char input[100] = { 0 };
		input_string(input);
		int flag = check_error(input);
		if (!flag)
			continue;
		else if (flag == 1)
			puts("= error");
		else if (flag == -1)
			return 0;
		else {
			char result[100] = { 0 };
			printf("= ");
			get_ans(input, result);
			if (number_of_op(result)) {
				char tmp[100];
				strcpy(tmp, result);
				clear_all(result, '\0', strlen(result));
				get_ans(tmp, result);
			}
			print_result(result);
		}
	}
}
bool compare_func(char first[], char second[]) {
	bool r = true;
	char tmp1[100] = { 0 }, tmp2[100] = { 0 };
	bool ddaihao_first = true;
	if (first[0] == '-')
		ddaihao_first = false;
	int idx = 0;
	for (int i = !ddaihao_first; i < strlen(first); i++)
		tmp1[idx++] = first[i];
	bool ddaihao_second = true;
	if (second[0] == '-')
		ddaihao_second = false;
	idx = 0;
	for (int i = !ddaihao_second; i < strlen(second); i++)
		tmp2[idx++] = second[i];
	char first_after_point[11] = { 0 };
	char second_after_point[11] = { 0 };
	dose_it_have_point(tmp1, first_after_point);
	dose_it_have_point(tmp2, second_after_point);
	if (strlen(first_after_point) > strlen(second_after_point)) {
		int diff = strlen(first_after_point) - strlen(second_after_point);
		for (int i = 0; i < diff; i++)
			strcat(second_after_point, "0");
	}
	else {
		int diff = strlen(second_after_point) - strlen(first_after_point);
		for (int i = 0; i < diff; i++)
			strcat(first_after_point, "0");
	}
	strcat(tmp1, first_after_point);
	strcat(tmp2, second_after_point);
	remove_zero(tmp1);
	remove_zero(tmp2);
	if (!ddaihao_first && !ddaihao_second)
		r = !r;
	else if (ddaihao_first != ddaihao_second) {
		if (ddaihao_first)
			return r;
		else
			return !r;
	}
	int len_of_first = strlen(tmp1);
	int len_of_second = strlen(tmp2);
	if (len_of_first > len_of_second)
		return r;
	else if (len_of_first < len_of_second)
		return !r;
	for (int i = 0; i < len_of_first; i++) {
		if (tmp1[i] > tmp2[i])
			return r;
		else if (tmp1[i] < tmp2[i])
			return !r;
	}
}
bool check_var(char name) {
	int idx, i;
	bool r = false;
	for (idx = 0; idx < number_of_var; idx++)
		if (var_name[idx] == name) {
			r = true;
			break;
		}
	if (r) {
		int len = strlen(var_value[idx]);
		for (i = 0; i < len; i++)
			var_value[idx][i] = '\0';
	}
	return r;
}
void set_var(char input[]) {
	if (input[0] >= 'a' && input[0] <= 'z')
		input[0] -= ('a' - 'A');
	int idx_var = number_of_var;
	if (check_var(input[0])) {
		int i;
		for (i = 0; i < number_of_var; i++)
			if (var_name[i] == input[0])
				break;
		idx_var = i;
	}
	else
		number_of_var++;
	int i;
	var_name[idx_var] = input[0];
	for (i = 0; i < strlen(input); i++)
		if (input[i] >= '0' && input[i] <= '9')
			break;
	i = 4;
	int idx = 0;
	char show_value[100] = { 0 };
	for (int i = 0; i<strlen(input); i++) {
		if ((input[i] >= '0'&&input[i] <= '9') || input[i] == '-' || input[i] == '.') {
			show_value[idx++] = input[i];
		}
	}
	if (flag1 == false) {
		printf("= ");
		print_result(show_value);
	}
	idx = 0;
	for (; i < strlen(input); i++)
		var_value[idx_var][idx++] = input[i];
	if (number_of_op(var_value[idx_var])) {
		char tmp[100] = { 0 };
		get_ans(var_value[idx_var], tmp);
		if (number_of_op(tmp)) {
			char tmp2[100] = { 0 };
			get_ans(tmp, tmp2);
			strcpy(tmp, tmp2);
		}
		strcpy(var_value[idx_var], tmp);
	}
}
void read_var(char data[], int p) {
	int i, idx;
	if (data[p] >= 'a' && data[p] <= 'z')
		data[p] -= ('a' - 'A');
	for (idx = 0; idx < number_of_var; idx++)
		if (var_name[idx] == data[p])
			break;
	for (i = 0; i < strlen(var_value[idx]); i++)
		data[p++] = var_value[idx][i];
}
void show_var() {
	int i;
	if (!number_of_var)
		puts("정의된 변수 없음");
	for (i = 0; i < number_of_var; i++) {
		printf("%c = ", var_name[i]);
		print_result(var_value[i]);
	}
}
void load_var() {
	FILE *save;
	save = fopen("var.txt", "r");
	if (save == '\0') {
		puts("error : file doesn't exit.");
		return;
	}
	char name, value[70];
	char input[100];
	while (fscanf(save, "%c = %s\n", &name, value) != EOF) {
		clear_all(input, 0, sizeof(input));
		input[0] = name;
		input[1] = ' ';
		input[2] = '=';
		input[3] = ' ';
		strcat(input, value);
		flag1 = true;
		check_error(input);
		flag1 = false;
	}
	fclose(save);
}
void save_var() {
	FILE *save;
	save = fopen("var.txt", "w");
	for (int i = 0; i < number_of_var; i++)
		fprintf(save, "%c = %s\n", var_name[i], var_value[i]);
	fclose(save);
}
void input_string(char input[]) {
	char c;
	int idx = 0;
	while (1) {
		c = getchar();
		if (c == '\n' || c == EOF) {
			input[idx] = '\0';
			return;
		}
		input[idx++] = c;
	}
}
int check_error(char input[]) {
	for (int i = 0; i < strlen(input); i++)
		if (input[i] >= 'a' && input[i] <= 'z')
			input[i] -= ('a' - 'A');
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
	}
	if (number_of_equal == 1) {
		if (number_of_var > 9) {
			puts("error: 변수의 개수를 초과하였습니다.");
			return 0;
		}
		set_var(input);
		return 0; //변수선언
	}
	else if (strcmp(input, "END") == 0) {
		return -1; //종료
	}
	else if (strcmp(input, "CLEAR") == 0) {
		system("clear");
		return 0;
	}
	else if (strcmp(input, "LOAD") == 0) {
		load_var();
		return 0;
	}
	else if (strcmp(input, "SAVE") == 0) {
		save_var();
		return 0;
	}
	else if (strcmp(input, "VAR") == 0) {
		show_var();
		return 0;
	}
	else if (error == true) {
		if (strlen(input) == 1 && (input[0] >= 'A' && input[0] <= 'Z')) {
			printf("= undefined.\n");
			return 0;
		}
		return 1; //error출력
	}
	if (number_of_op(input) + 1 != number_of_num(input))
		return 1;
	return 2; //연산하기
}
int number_of_op(char target[]) {
	int value_to_return = 0;
	for (int i = 1; i < strlen(target); i++)
		if ((target[i] == '*' || target[i] == '+' || target[i] == '/' || target[i] == '-' || target[i] == '%') && target[i - 1] == ' ' && target[i + 1] == ' ')
			value_to_return++;
	return value_to_return;
}
int number_of_num(char target[]) {
	int value_to_return = 0;
	int idx = 0;
	while (idx < strlen(target)) {
		bool flag = false;
		while ((target[idx] >= '0' && target[idx] <= '9') || target[idx] == '.' || (target[idx] >= 'A' && target[idx] <= 'Z') || (target[idx] >= 'a' && target[idx] <= 'z')) {
			flag = true;
			idx++;
			if ((target[idx] >= 'A' && target[idx] <= 'Z') || (target[idx] >= 'a' && target[idx] <= 'z'))
				break;
		}
		if (flag)
			value_to_return++;
		while (!((target[idx] >= '0' && target[idx] <= '9') || target[idx] == '.' || (target[idx] >= 'A' && target[idx] <= 'Z') || (target[idx] >= 'a' && target[idx] <= 'z'))) {
			if (target[idx + 1] != '\0')
				idx++;
			else
				break;
		}
	}
	return value_to_return;
}
bool ps_cal(char left[], char right[], char op, char result[]) {
	bool is_error = false;
	if (op == '+') {
		if (left[0] != '-' && right[0] != '-')
			plus(left, right, result);
		else if (left[0] == '-' && right[0] == '-') {
			result[0] = '-';
			char tmp1[70] = { 0 }, tmp2[70] = { 0 };
			for (int i = 1; i < strlen(left); i++)
				tmp1[i - 1] = left[i];
			for (int i = 1; i < strlen(right); i++)
				tmp2[i - 1] = right[i];
			plus(tmp1, tmp2, result);
		}
		else if (left[0] != '-' && right[0] == '-') {
			char tmp[70] = { 0 };
			for (int i = 1; i < strlen(right); i++)
				tmp[i - 1] = right[i];
			if (compare_func(left, tmp))
				minus(left, tmp, result);
			else {
				result[0] = '-';
				minus(tmp, left, result);
			}
		}
		else if (left[0] == '-' && right[0] != '-') {
			char tmp[70] = { 0 };
			for (int i = 1; i < strlen(left); i++)
				tmp[i - 1] = left[i];
			if (compare_func(tmp, right) && strcmp(tmp, right)) {
				result[0] = '-';
				minus(tmp, right, result);
			}
			else
				minus(right, tmp, result);
		}
	}
	else if (op == '-') {
		if (left[0] != '-' && right[0] != '-') {
			if (compare_func(left, right))
				minus(left, right, result);
			else {
				result[0] = '-';
				minus(right, left, result);
			}
		}
		else if (left[0] == '-' && right[0] == '-') {
			char tmp1[70] = { 0 }, tmp2[70] = { 0 };
			for (int i = 1; i < strlen(left); i++)
				tmp1[i - 1] = left[i];
			for (int i = 1; i < strlen(right); i++)
				tmp2[i - 1] = right[i];
			if (compare_func(tmp1, tmp2) && strcmp(tmp1, tmp2)) {
				result[0] = '-';
				minus(tmp1, tmp2, result);
			}
			else
				minus(tmp2, tmp1, result);
		}
		else if (left[0] != '0' && right[0] == '-') {
			char tmp[70] = { 0 };
			for (int i = 1; i < strlen(right); i++)
				tmp[i - 1] = right[i];
			plus(left, tmp, result);
		}
		else if (left[0] == '-' && right[0] != '-') {
			result[0] = '-';
			char tmp[70] = { 0 };
			for (int i = 1; i < strlen(left); i++)
				tmp[i - 1] = left[i];
			plus(tmp, right, result);
		}
	}
	else if (op == '*')
		multiple(left, right, result);
	else if (op == '/')
		is_error = divide(left, right, result);
	else
		is_error = modular(left, right, result);
	if (is_error)
		return false;
	return true;
}
inline void plus(char left[], char right[], char result[]) {
	char left_after_point[11] = { 0 };
	char right_after_point[11] = { 0 };
	dose_it_have_point(left, left_after_point);
	dose_it_have_point(right, right_after_point);
	if (strlen(left_after_point) > strlen(right_after_point)) {
		int diff = strlen(left_after_point) - strlen(right_after_point);
		for (int i = 0; i < diff; i++)
			strcat(right_after_point, "0");
	}
	else {
		int diff = strlen(right_after_point) - strlen(left_after_point);
		for (int i = 0; i < diff; i++)
			strcat(left_after_point, "0");
	}
	strcat(left, left_after_point);
	strcat(right, right_after_point);
	int left_rev[100] = { 0 }, right_rev[100] = { 0 };
	int idx = 1, len = strlen(left) + strlen(right);
	if (strlen(left) > strlen(right))
		len = strlen(left) + 1;
	else
		len = strlen(right) + 1;
	for (int i = strlen(left) - 1; i >= 0; i--)
		left_rev[idx++] = left[i] - '0';
	idx = 1;
	for (int i = strlen(right) - 1; i >= 0; i--)
		right_rev[idx++] = right[i] - '0';
	int result_rev[100] = { 0 };
	for (int i = 1; i <= len; i++) {
		result_rev[i] += left_rev[i] + right_rev[i];
		if (result_rev[i] >= 10) {
			result_rev[i + 1] += result_rev[i] / 10;
			result_rev[i] %= 10;
		}
	}
	int point_pos = strlen(left_after_point);
	idx = strlen(result);
	for (int i = len; i > 0; i--) {
		if ((idx == 0 || (idx == 1 && result[0] == '-')) && result_rev[i] == 0 && i != 1)
			continue;
		if (i == point_pos)
			result[idx++] = '.';
		result[idx++] = result_rev[i] + '0';
	}
}
inline void minus(char left[], char right[], char result[]) {
	char left_after_point[11] = { 0 };
	char right_after_point[11] = { 0 };
	dose_it_have_point(left, left_after_point);
	dose_it_have_point(right, right_after_point);
	if (strlen(left_after_point) > strlen(right_after_point)) {
		int diff = strlen(left_after_point) - strlen(right_after_point);
		for (int i = 0; i < diff; i++)
			strcat(right_after_point, "0");
	}
	else {
		int diff = strlen(right_after_point) - strlen(left_after_point);
		for (int i = 0; i < diff; i++)
			strcat(left_after_point, "0");
	}
	strcat(left, left_after_point);
	strcat(right, right_after_point);
	int left_rev[100] = { 0 }, right_rev[100] = { 0 };
	int idx = 1, len = strlen(left) + strlen(right);
	if (strlen(left) > strlen(right))
		len = strlen(left);
	else
		len = strlen(right);
	for (int i = strlen(left) - 1; i >= 0; i--)
		left_rev[idx++] = left[i] - '0';
	idx = 1;
	for (int i = strlen(right) - 1; i >= 0; i--)
		right_rev[idx++] = right[i] - '0';
	int result_rev[100] = { 0 };
	for (int i = 1; i <= len; i++) {
		result_rev[i] += left_rev[i] - right_rev[i];
		if (result_rev[i] < 0) {
			result_rev[i + 1] += -1;
			result_rev[i] = 10 + result_rev[i];
		}
	}
	int point_pos = strlen(left_after_point);
	idx = strlen(result);
	for (int i = len; i > 0; i--) {
		if (i == point_pos)
			result[idx++] = '.';
		result[idx++] = result_rev[i] + '0';
	}
}
inline void multiple(char first[], char second[], char r[]) {
	char tmp1[100] = { 0 }, tmp2[100] = { 0 };
	bool ddaihao_first = true;
	if (first[0] == '-')
		ddaihao_first = false;
	int idx = 0;
	for (int i = !ddaihao_first; i < strlen(first); i++)
		tmp1[idx++] = first[i];
	bool ddaihao_second = true;
	if (second[0] == '-')
		ddaihao_second = false;
	idx = 0;
	for (int i = !ddaihao_second; i < strlen(second); i++)
		tmp2[idx++] = second[i];
	//부호 정리해주기 완료
	int tmp[100][100] = { 0 };
	char first_after_point[11] = { 0 };
	char second_after_point[11] = { 0 };
	dose_it_have_point(tmp1, first_after_point);
	dose_it_have_point(tmp2, second_after_point);
	if (strlen(first_after_point) > strlen(second_after_point)) {
		int diff = strlen(first_after_point) - strlen(second_after_point);
		for (int i = 0; i < diff; i++)
			strcat(second_after_point, "0");
	}
	else {
		int diff = strlen(second_after_point) - strlen(first_after_point);
		for (int i = 0; i < diff; i++)
			strcat(first_after_point, "0");
	}
	strcat(tmp1, first_after_point);
	strcat(tmp2, second_after_point);
	idx = 1;
	int second_rev[100] = { 0 }, first_rev[100] = { 0 }, r_rev[100] = { 0 };
	for (int i = strlen(tmp2) - 1; i >= 0; i--)
		second_rev[idx++] = tmp2[i] - '0';
	idx = 1;
	for (int i = strlen(tmp1) - 1; i >= 0; i--)
		first_rev[idx++] = tmp1[i] - '0';
	int len = 0;
	for (int i = 1; i <= strlen(tmp2); i++) {
		int j;
		for (j = 1; j < i; j++)
			tmp[i][j] = 0;
		for (j = i; j < strlen(tmp1) + i; j++) {
			tmp[i][j] += (second_rev[i]) * (first_rev[j - i + 1]);
			if (tmp[i][j] >= 10) {
				tmp[i][j + 1] += tmp[i][j] / 10;
				tmp[i][j] %= 10;
			}
		}
		if (len < j)
			len = j;
	}
	for (int i = 1; i <= len; i++) {
		int sum = 0;
		for (int j = 1; j <= strlen(tmp2); j++)
			sum += tmp[j][i];
		r_rev[i] += sum;
		if (r_rev[i] >= 10) {
			r_rev[i + 1] += r_rev[i] / 10;
			r_rev[i] %= 10;
		}
	}
	int point_pos = strlen(first_after_point) + strlen(second_after_point);
	idx = 0;
	if (ddaihao_first != ddaihao_second)
		r[idx++] = '-';
	for (int i = len; i >= 1; i--) {
		if (((idx == 0) || (idx == 1 && r[0] == '-')) && r_rev[i] == 0 && i != 1)
			continue;
		if (i == point_pos)
			r[idx++] = '.';
		r[idx++] = r_rev[i] + '0';

	}
}
inline bool divide(char left[], char right[], char result[]) {
	char result_tmp[100] = { 0 },op[2]={'\0','\0'};
	remove_zero(right);
	if (!strcmp(right, "0"))
		return false;
	if (!strcmp(right, "1")) {
		strcpy(result, left);
		return true;
	}
	char tmp1[100] = { 0 }, tmp2[100] = { 0 };
	bool ddaihao_first = true;
	if (left[0] == '-')
		ddaihao_first = false;
	int idx = 0;
	for (int i = !ddaihao_first; i < strlen(left); i++) tmp1[idx++] = left[i];
	bool ddaihao_second = true;
	if (right[0] == '-')
		ddaihao_second = false;
	idx = 0;
	for (int i = !ddaihao_second; i < strlen(right); i++) tmp2[idx++] = right[i];
	//부호 정리해주기 완료
	char first_after_point[11] = { 0 };
	char second_after_point[11] = { 0 };
	dose_it_have_point(tmp1, first_after_point);
	dose_it_have_point(tmp2, second_after_point);
	if (strlen(first_after_point) > strlen(second_after_point)) {
		int diff = strlen(first_after_point) - strlen(second_after_point);
		for (int i = 0; i < diff; i++)
			strcat(second_after_point, "0");
	}
	else {
		int diff = strlen(second_after_point) - strlen(first_after_point);
		for (int i = 0; i < diff; i++)
			strcat(first_after_point, "0");
	}
	strcat(tmp1, first_after_point);
	strcat(tmp2, second_after_point);
	char tmp2_bak[100];
	strcpy(tmp2_bak, tmp2);
	char counter[100] = { 0 }, one[100] = { 0 };
	counter[0] = '0';
	one[0] = '1';
	while (compare_func(tmp1, tmp2) && strcmp(tmp2, "") && strlen(one))
	{
		clear_all(result_tmp, '\0', sizeof(result_tmp));
		bool flag = false;
		while (compare_func(tmp1, tmp2)) {
			strcat(tmp2, "0");
			strcat(one, "0");
			flag = true;
		}
		if (flag) {
			one[strlen(one) - 1] = '\0';
			tmp2[strlen(tmp2) - 1] = '\0';
		}
		while (compare_func(tmp1, tmp2)) {
			minus(tmp1, tmp2, result_tmp);
			remove_zero(result_tmp);
			strcpy(tmp1, result_tmp);
			clear_all(result_tmp, '\0', sizeof(result_tmp));
			plus(counter, one, result_tmp);
			strcpy(counter, result_tmp);
			clear_all(result_tmp, 0, strlen(result_tmp));
		}
		while (!compare_func(tmp1, tmp2) && strlen(tmp2) > 0) {
			tmp2[strlen(tmp2) - 1] = '\0';
			one[strlen(one) - 1] = '\0';
		}
	}
	if (strlen(tmp2)) {
		strcpy(result, counter);
		return true;
	}
	strcpy(tmp2, "0.");
	tmp2[strlen(tmp2)] = tmp2_bak[0];
	strcpy(one, "0.1");
	char zero[10] = { 0 };
	zero[0] = '0';
	while (compare_func(tmp1, tmp2) && strlen(one) < 10) {
		clear_all(result_tmp, '\0', sizeof(result_tmp));
		while (compare_func(tmp1, tmp2) && strlen(tmp1) < 10) {
			//puts(tmp1);
			//puts(tmp2);
			char ttmp[10] = { 0 };
			strcpy(ttmp, tmp2);
			minus(tmp1, ttmp, result_tmp);
			remove_zero(result_tmp);
			strcpy(tmp1, result_tmp);
			clear_all(result_tmp, '\0', sizeof(result_tmp));
			char tone[20] = { 0 };
			strcpy(tone, one);
			plus(counter, tone, result_tmp);
			strcpy(counter, result_tmp);
			clear_all(result_tmp, 0, strlen(result_tmp));
		}
		while (!compare_func(tmp1, tmp2) && strlen(tmp2) < 10) {
			char bak = tmp2[strlen(tmp2) - 1];
			clear_after_n(tmp2, '\0', 2, sizeof(tmp2));
			strcat(tmp2, zero);
			tmp2[strlen(tmp2)] = bak;
			bak = one[strlen(one) - 1];
			clear_after_n(one, '\0', 2, sizeof(one));
			strcat(one, zero);
			one[strlen(one)] = bak;
			strcat(zero, "0");
		}
	}
	if((left[0]=='-'&&right[0]!='-')||left[0]!='-'&&right[0]=='-')
		op[0]='-';
	
	result[0]=op[0];
	op[0]='\0';
	strcat(result, counter);
	return true;
}
inline bool modular(char left[], char right[], char r[]) {
	if (!strcmp(right, "0"))
		return false;
	char right_origin[100];
	strcpy(right_origin, right);
	bool flag = false;
	while (compare_func(left, right)) {
		right[strlen(right)] = '0';
		flag = true;
	}
	if (flag)
		right[strlen(right) - 1] = '\0';
	while (compare_func(left, right) || !strcmp(left, right))
	{
		minus(left, right, r);
		remove_zero(r);
		strcpy(left, r);
		clear_all(r, '\0', strlen(r));
		while (!compare_func(left, right) && strcmp(right, right_origin))
			right[strlen(right) - 1] = '\0';
	}
	strcpy(r, left);
}
bool dose_it_have_point(char target[], char after_point[]) {
	int len = strlen(target);
	int i;
	bool flag = false;
	for (i = 0; i < len; i++) {
		if (target[i] == '.') {
			target[i] = '\0';
			flag = true;
			break;
		}
	}
	i++;
	int idx = 0;
	for (; i < len; i++)
		after_point[idx++] = target[i];
	return flag;
}

void clear_all(char target[], int value, int target_size) {
	for (int i = 0; i < target_size; i++)
		target[i] = value;
}
void clear_after_n(char target[], int value, int from, int target_szie) {
	for (int i = from; i < target_szie; i++)
		target[i] = value;
}
void get_ans(char input[], char result[]) {
	char data[100] = { 0 };
	int idx_data = 0;
	bool is_left_Ready = false, is_right_Ready = false, is_op_Ready = false;
	bool not_now = true, always_do = false;
	char left[100] = { 0 }, right[100] = { 0 }, op;
	int first, finish = false;
	for (int i = 0; i < strlen(input); i++) {
		if (i == strlen(input) - 1)
			finish = true;
		idx_data = strlen(data);
		data[idx_data] = input[i];
		if ((data[idx_data] >= 'A' && data[idx_data] <= 'Z') || (data[idx_data] >= 'a' && data[idx_data] <= 'z')) {
			read_var(data, idx_data);
			idx_data = strlen(data);
		}
		if (data[idx_data] == '*' || data[idx_data] == '/' || data[idx_data] == '%')
			always_do = true;
		if (data[idx_data] == ' ' || input[i + 1] == '\0') {
			if (is_op_Ready) {
				if (input[i + 1] == '\0')
					get_num(data, right, idx_data);
				else
					get_num(data, right, idx_data - 1);
				is_right_Ready = true;
			}
			if (is_left_Ready && !is_op_Ready) {
				op = data[idx_data - 1];
				is_op_Ready = true;
			}
			if (!is_op_Ready) {
				first = get_num(data, left, idx_data - 1);
				is_left_Ready = true;
			}
		}
		if (is_right_Ready && is_left_Ready && is_op_Ready) {
			if (next_op(input, i + 1) || always_do) {
				ps_cal(left, right, op, result);
				clear_after_n(data, 0, first, sizeof(data));
				strcat(data, result);
				strcat(data, " ");
			}
			is_left_Ready = false; is_right_Ready = false; is_op_Ready = false; not_now = true; always_do = false;
			clear_all(left, 0, sizeof(left));
			clear_all(right, 0, sizeof(right));
			clear_all(result, 0, strlen(result));
			if (!finish) {
				i -= 1;
				data[strlen(data) - 1] = '\0';
			}
		}
	}
	if (data[strlen(data) - 1] == 32) //알수없는 공백 제거
		data[strlen(data) - 1] = 0;
	strcpy(result, data);
	bool have_point = false;
	int pos_point = 0;
}
int get_num(char from[], char target[], int p) {
	if (from[p] == '\0')
		p--;
	char tmp[100] = { 0 };
	int idx = 0;
	int r = 0;
	while (p >= 0 && from[p] != ' ') {
		tmp[idx++] = from[p];
		p--;
	}
	r = p + 1;
	idx = 0;
	for (int i = strlen(tmp) - 1; i >= 0; i--)
		target[idx++] = tmp[i];
	return r;
}
bool next_op(char input[], int p) {
	while ((input[p] != '+' && input[p] == '-' &&  input[p] == '*' && input[p] == '/' && input[p] == '%') && p < strlen(input))
		p++;
	if (input[p] == '*' || input[p] == '/' || input[p] == '%')
		return false;
	return true;
}
void print_result(char result[]) {
	remove_zero(result);
	bool is_r_minus_num = false;
	bool does_r_have_point = false;
	int pos_point;
	if (result[0] == '-')
		is_r_minus_num = true;
	for (int i = 0; i < strlen(result); i++)
		if (result[i] == '.')
			does_r_have_point = true;
	char to_print_thingsi_rev[100];
	int idx = 0, n = 0, pass_point = 0, pass_point_yes = 0, k = 0;
	for (int i = strlen(result) - 1; i >= is_r_minus_num; i--) {
		to_print_thingsi_rev[idx++] = result[i];
		if (pass_point_yes == 1)
			k++;
		if (i == is_r_minus_num) {
			to_print_thingsi_rev[idx] = '\0';
			continue;
		}
		if (result[i - 1] == '.') {
			to_print_thingsi_rev[idx++] = '.';
			i--;
			pass_point_yes = 1;
			n = 0;
		}
		else if (idx == 4 * n + 3 && pass_point_yes == 0) {
			to_print_thingsi_rev[idx++] = ',';
			n++;
		}
		else if (k == 3 * n + 3 && pass_point_yes == 1) {
			to_print_thingsi_rev[idx++] = ',';
			n++;
		}
	}
	if (is_r_minus_num)
		printf("-");

	for (int i = strlen(to_print_thingsi_rev) - 1; i >= 0; i--)
		printf("%c", to_print_thingsi_rev[i]);
	printf("\n");

}
void remove_zero(char target[]) {
	bool have_point = false;
	int pos_point;
	for (int i = strlen(target) - 1, j = 0; i >= 0; i--, j++) {
		if (target[i] == '.') {
			have_point = true;
			pos_point = j;
		}
	}
	if (have_point) {     //소수점 뒤 9자리까지만 출력 (반올림x)
		if (pos_point >= 10) {
			for (int i = strlen(target) - 1, j = 0; j<pos_point - 9; j++, i--)
				target[i] = 0;
		}
	}
	char tmp[100] = { 0 };
	int idx = 0;
	bool flag = false;
	if (target[0] == '-') {
		flag = true;
		tmp[0] = '-';
		idx++;
	}
	for (int i = flag; i < strlen(target); i++) {
		if (target[i] == '0' && is_there_point_after_me(target, i) && !is_there_any_num_before_me(target, i) && target[i + 1] != '.')
			continue;
		if (!is_there_point_after_me(target, i) && i != strlen(target) - 1 && target[i] == '0' && (idx == 0 || (idx == 1 && tmp[0] == '-')))
			continue;
		if (i != strlen(target) - 1 && target[i] == '0' && is_there_point_before_me(target, i) && !is_there_any_num_after_me(target, i))
			continue;
		tmp[idx++] = target[i];
	}
	strcpy(target, tmp);
}
bool is_there_point_after_me(char target[], int idx) {
	for (int i = idx + 1; i < strlen(target); i++)
		if (target[i] == '.')
			return true;
	return false;
}
bool is_there_point_before_me(char target[], int idx) {
	for (int i = idx - 1; i >= 0; i--)
		if (target[i] == '.')
			return true;
	return false;
}
bool is_there_any_num_before_me(char target[], int idx) {
	for (int i = idx - 1; i >= 0; i--)
		if (target[i] != '0' && target[i] != '-')
			return true;
	return false;
}
bool is_there_any_num_after_me(char target[], int idx) {
	for (int i = idx + 1; i < strlen(target); i++)
		if (target[i] != '0')
			return true;
	return false;
}
