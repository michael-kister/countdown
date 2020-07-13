#include <stdio.h>
#include <string.h>

char CALC_BUF[64];

int digit_count(int n) {
    int count = 0;
    while (n != 0) {
        n /= 10;
        ++count;
    }
    return count;
}

int calculate(char *expr) {

    int i, j;
    int len = strlen(expr);
    
    int nums[] = {0,0};
    int lens[] = {0,0};

    int i_start = 0;
    int i_num = 0;

    char op;
    int shift = 0;
    int status = 0;

    int num_par = 0;
    int num_op = 0;
    for (i = 0; i < len; ++i) {
	switch (expr[i]) {
	case '(': ++num_par; break;
	case ')': --num_par; break;
	case '+': ++num_op; break;
	case '-': ++num_op; break;
	case '*': ++num_op; break;
	case '/': ++num_op; break;
	default : break;
	}
    }

    if (num_par != 0) {
	printf("\nUnmatched parentheses. Fail to calculate.\n\n");
	return -1;
    }
    
    printf("\n");
    for (i = 0; i < num_op; ++i) {
	for (j = 0; j < len; ++j) {

	    switch (expr[j]) {
	    case '(':
		i_start = j;
		++num_par;
		nums[0] = nums[1] = 0;
		lens[0] = lens[1] = 0;
		i_num = 0;
		break;
	    case ')':
		--num_par;
		switch (op) {
		case '+': nums[0] += nums[1]; break;
		case '-': nums[0] -= nums[1]; break;
		case '*': nums[0] *= nums[1]; break;
		case '/': nums[0] /= nums[1]; break;
		}
		shift = 3 + lens[0] + lens[1];
		sprintf(expr+i_start, "%d%s", nums[0],
			expr+i_start+shift);
		printf(" = %s\n", expr);
		
		--i_num;
		nums[0] = nums[1] = 0;
		lens[0] = lens[1] = 0;
		status = 1;
		break;
	    case '+': op = '+'; ++i_num; break;
	    case '-': op = '-'; ++i_num; break;
	    case '*': op = '*'; ++i_num; break;
	    case '/': op = '/'; ++i_num; break;
	    default:
		nums[i_num] *= 10;
		nums[i_num] += expr[j] - '0';
		lens[i_num] += 1;
		break;
	    }

	    if (i_num > 1) {
		printf("\nUnmatched parentheses. Fail to calculate.\n\n");
		return -1;
	    }

	    if (status) {
		len = strlen(expr);
		status = 0;
		break;
	    }
	}
    }
    printf("\n");

    nums[0] = 0;
    for (i = 0; i < len; ++i) {
	nums[0] *= 10;
	nums[0] += expr[i] - '0';
    }

    return nums[0];
}
