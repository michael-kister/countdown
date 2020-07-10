/*********************************************************************
 * The idea is to store a tree using arrays.
 * Of course, for maximal generality, you would use a struct, with
 * left and right nodes; however, since we know that at most the tree
 * will have six leaves, we can slightly optimize.
 *
 * [a]  [b]  [c]  [d]  [e]  [f]    
 *  |____|    |    |    |    |    (+,a,b) : 00110000
 *  |         |____|____|    |    (-,c,e) : 01001010
 *  |_________|    |         |    (*,a,c) : 10101000
 *  |______________|         |    (/,a,d) : 11100100
 *  |________________________|    (+,a,f) : 00100001
 *  |
 * [y]
 *
 * Note that a char has 8 bits, which is enough to encode one layer of
 * the tree, by uniquely identifying the operator in question, and the
 * two numbers which are undergoing the operation. You can have at
 * most 5 operations, so theoretically, you could encode the whole
 * tree in an array of 5 characters.
 *
 *
 * In terms of solutions, right now I'm iterating over all possible
 * trees, and then checking to see that they're connected, and if they
 * produce a good answer.
 *
 * Perhaps a smarter approach would be to iterate backwards, so that I
 * can be assured that I'm only choosing valid trees.
 * Another benefit of this approach is that if I group the trees by
 * the number of branches, they I may be able to pursue the simplest
 * solutions first, and then increasingly complicated solutions.
 *
 *
 * https://stackoverflow.com/questions/14900693/enumerate-all-full-labeled-binary-tree?noredirect=1&lq=1
 *
 *
 * We iterate over branches by specifying the number of leaves, and
 * then identifying the possible numbers, and then creating all trees
 * of the correct size with those leaves, while always pursuing diff-
 * erent combinations of operations.
 *
 * On one hand, I'd like to eliminate 
 *********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <time.h>
#include <limits.h>
#include <string.h>

#define BYTE_TO_BINARY_PATTERN "%c%c%c%c%c%c%c%c"
#define BYTE_TO_BINARY(byte)  \
  (byte & 0x80 ? '1' : '0'), \
  (byte & 0x40 ? '1' : '0'), \
  (byte & 0x20 ? '1' : '0'), \
  (byte & 0x10 ? '1' : '0'), \
  (byte & 0x08 ? '1' : '0'), \
  (byte & 0x04 ? '1' : '0'), \
  (byte & 0x02 ? '1' : '0'), \
  (byte & 0x01 ? '1' : '0')

void bprint(int m) {
    printf(BYTE_TO_BINARY_PATTERN" "BYTE_TO_BINARY_PATTERN" "BYTE_TO_BINARY_PATTERN" "BYTE_TO_BINARY_PATTERN"\n",
	   BYTE_TO_BINARY(m>>24), BYTE_TO_BINARY(m>>16), BYTE_TO_BINARY(m>>8), BYTE_TO_BINARY(m));
}

// number of numbers for round
#define N 6

#define N0 1
#define N1 2
#define N2 4
#define N3 8
#define N4 16
#define N5 32

#define __NUM_MASK__ 63

#define ADD 64
#define SUB 128
#define MUL 256
#define DIV 512

#define __OP_MASK__ 960
#define __OP_FLAG__ 1024


int digit_count(int n) {
    int count = 0;
    while (n != 0) {
        n /= 10;
        ++count;
    }
    return count;
}

// global variables
int n_codes[] = {N5, N4, N3, N2, N1, N0};
int o_codes[] = {ADD, SUB, MUL, DIV};
int __numbers__[N];
int __workspace__[N];
int __target__;
int __solution__[N];
int __lvars__[N];
int __rvars__[N];
int location[N];
#define BUFFER_SIZE 64
char formula[BUFFER_SIZE];

// prototypes for setting the numbers
int set_numbers(void);

char choices[32];

int ask_user(int *n_large, int *n_small) {

    int num_large, num_small, num_wrong, i;
    
    printf("What would you like? (Choose 6 of [b/l])\n");
    scanf("%s", choices);
  
    for (i = 0; i < N; ++i) {
	if (choices[i] == 'b' || choices[i] == 'B') {
	    ++num_large;
	} else if (choices[i] == 's' || choices[i] == 'S') {
	    ++num_small;
	} else {
	    ++num_wrong;
	}
    }

    if (num_wrong > 0) {
	printf("I told you to choose between 'b' and 's'.\n");
	printf("You're getting %d extra big numbers!\n", num_wrong);
	num_large += num_wrong;
    }

    if (num_large > 4) {
	printf("You can only have 4 big ones---so that is what you'll have.\n");
	num_large = 4;
	num_small = 2;
    }

    printf("You've chosen %d big numbers, and %d small numbers.\n",
	   num_large, num_small);
    printf("Your numbers are:\n");

    return 0;

}

int set_numbers(void) {

    return 0;
}

// prototypes for solution
int print_solution(int n);
char get_op(int c);
int do_op(int c, int ii);
int which_vars(int c, int *i, int *j);
int which_leaves[N];
int is_leaf_available[N];
int leaf_recursion(int i_leaf, int max_leaf, int i_start);
int tree_recursion(int i_branch, int num_leaf);
int evaluate(int num_op);



int large_ones[4]  = {25, 50, 75, 100};
int small_ones[20] = {1,1,2,2,3,3,4,4,5,5,6,6,7,7,8,8,9,9,10,10};

int i_large[6];
int i_small[6];

/**
 * 
 * -------------------------------------------------------------------
 */
int main(int argc, char** argv) {

    __numbers__[0] = 100;
    __numbers__[1] = 75;
    __numbers__[2] = 50;
    __numbers__[3] = 4;
    __numbers__[4] = 1;
    __numbers__[5] = 7;

    __target__ = 391;

    int i, j;
    
    srand(time(0));

    int which;
    int num_large = 3;
    int num_small = 3;

    // to select, you want to get

    for (i = 0; i < num_large; ++i) {
	which = rand() % (4 - i);
	__numbers__[i] = large_ones[which];
	for (j = which; j < (4 - i); ++j) {
	    large_ones[j] = large_ones[j+1];
	}
    }

    for (i = 0; i < num_small; ++i) {
	which = rand() % (20 - i);
	__numbers__[i+num_large] = small_ones[which];
	for (j = which; j < (20 - i); ++j) {
	    small_ones[j] = small_ones[j+1];
	}
    }

    __numbers__[0] = 100;
    __numbers__[1] = 75;
    __numbers__[2] = 50;
    __numbers__[3] = 4;
    __numbers__[4] = 1;
    __numbers__[5] = 7;

    printf("\n");
    for (i = 0; i < N; ++i)
	printf("%4d  ", __numbers__[i]);
    printf("\n\n");

    // save things at the end, to avoid confusion.
    for (i = 0; i < N; ++i) {
	is_leaf_available[i] = 1;
	__workspace__[i] = __numbers__[i];
	location[i] = 0;
    }
    
    for (i = 5; i < 6; ++i) {
    	leaf_recursion(0, i+1, 0);
    }

    return 0;  
  
}

/********************************************************************/
/*                         UX FUNCTIONS                             */
/********************************************************************/










/********************************************************************/
/*                      SOLUTION FUNCTIONS                          */
/********************************************************************/

int tree_recursion(int i_branch, int num_leaf) {
    
  int i, j, k;

    int status;
    
    // loop over the starting branch
    for (i = 0; i < num_leaf-1; ++i) {

	// in the very first one, we can only start at one
	if (i_branch == 0 && i > 0)
	    break;

	// if not the first, then check that it's not a good target
	if (i > 0 && is_leaf_available[i])
	    continue;
	
	// loop over the targeted branch
	for (j = i+1; j < num_leaf; ++j) {

	    // check that it's available
	    if (is_leaf_available[j]) {

		// make sure no one else tries to land here
		is_leaf_available[j] = 0;

		// loop over different operations
		for (k = 0; k < 4; ++k) {

		    // put it in backwards for the printing
		    __solution__[(num_leaf-1)-i_branch-1] =
		        n_codes[which_leaves[i]] +
			n_codes[which_leaves[j]] +
			o_codes[k];

		    __lvars__[(num_leaf-1)-i_branch-1] = which_leaves[i];
		    __rvars__[(num_leaf-1)-i_branch-1] = which_leaves[j];

		    //if (__numbers__[which_leaves[i]] < __numbers__[which_leaves[j]]) {

		    __solution__[(num_leaf-1)-i_branch-1] += __OP_FLAG__;
		    
		    if (i_branch+1 < num_leaf-1) {
			status = tree_recursion(i_branch+1, num_leaf);
		    } else {
			status = print_solution(num_leaf-1);
		    }
		    
		    __solution__[(num_leaf-1)-(i_branch+1)] -= __OP_FLAG__;

		    //} else {
		    
		    if (i_branch+1 < num_leaf-1) {
			status = tree_recursion(i_branch+1, num_leaf);
		    } else {
			status = print_solution(num_leaf-1);
		    }
		      
		    //}

		    if (status == 0) 
		      	break;
		}

		// make the leaf available again
		is_leaf_available[j] = 1;

		if (status == 0)
		    return 0;
	    }
	}
    }
    return 1;
}

int leaf_recursion(int i_leaf, int max_leaf, int i_start) {

    int i;
    
    // loop over which is the i_leaf'th variable to use
    for (i = i_start; i < N - max_leaf + i_leaf + 1; ++i) {

	// let them know which i_leaf'th variable
	which_leaves[i_leaf] = i;

	// if we can do more leaves
	if (i_leaf+1 < max_leaf) {

	    // start on i_leaf+1'th leaf, starting at i+1
	    leaf_recursion(i_leaf+1, max_leaf, i+1);
	} else {
	    
	    // do the tree recursion!
	    tree_recursion(0, max_leaf);
	}
    }
    return 0;
}

char get_op(int c) {
    switch(c & __OP_MASK__) {
    case ADD : return '+';
    case SUB : return '-';
    case MUL : return '*';
    case DIV : return '/';
    default: return '?';
    }
    return 0;
}

int print_solution(int n) {

    int i, j, ix, iy, i_end = 0;

    int n_shift = 4;

    int e = evaluate(n);

    //printf("\n>>> Prepare to print (%i)\n", e);
    
    if (e != __target__)
    //if (e != 399)
    	return -1;

    // do a check to try and eliminate some repeats
    for (i = 0; i < N-2; ++i) {

	// if you have two totally separate adjacent branches
	if (0 == (__solution__[i] & __solution__[i+1] & __NUM_MASK__)) {

	    // only continue if the bigger one is first
	    if ((__solution__[i]&__NUM_MASK__) <
		(__solution__[i+1]&__NUM_MASK__))
		return -1;
	}
    }
    
    // iterate over the vector of solutions, starting with the last
    for (i = n-1; i >= 0; --i) {

	// identify which variables you're looking at
	//which_vars(__solution__[i], &ix, &iy);

	ix = __lvars__[i];
	iy = __rvars__[i];
	
	
	// starting at the end, and going down until you get to the
	// one who is substituted, shift them all by some number
	for (j = i_end; j > location[ix]; --j)
	    formula[j+n_shift] = formula[j];

	// having done so, you should now shift the locations of any
	// variables that were already to the right of our target
	for (j = 0; j < N; ++j) {
	    if (location[j] > location[ix]) {
		location[j] += n_shift;
	  }
	}

	// first, put in an opening parenthesis
	formula[location[ix]+0] = '(';

	// now you need to deal with the possibility that things
	// are going to be done backwards...
	if (__solution__[i] & __OP_FLAG__) {

	    formula[location[ix]+1] = '0' + iy;
	    formula[location[ix]+2] = get_op(__solution__[i]);
	    formula[location[ix]+3] = '0' + ix;

	    location[ix] += 3;
	    location[iy] = location[ix] - 2;
	    
	    formula[location[ix]+1] = ')';
	    
	} else {
	
	    formula[location[ix]+1] = '0' + ix;
	    formula[location[ix]+2] = get_op(__solution__[i]);
	    formula[location[ix]+3] = '0' + iy;

	    location[ix] += 1;
	    location[iy] = location[ix] + 2;

	    formula[location[iy]+1] = ')';
	}

	// make sure to account for recent changes in the total
	i_end += n_shift;
	
    }
    
    // now you need to plug in the numbers
    for (i = 0; i < N; ++i) {

	// check that it's one we care about
	if (location[i] > 0) {

	    // calculate how far we need to shift
	    n_shift = digit_count(__numbers__[i]) - 1;

	    // shift rightwards the rest of the equation
	    for (j = i_end; j > location[i]; --j)
		formula[j+n_shift] = formula[j];

	    // move indices of other numbers
	    for (j = 0; j < N; ++j)
		if (location[j] > location[i])
		    location[j] += n_shift;

	    // make note of where the end moves
	    i_end += n_shift;

	    // actually insert the number
	    sprintf(formula + location[i], "%d%s",
		    __numbers__[i], formula+location[i]+n_shift+1);
	    
	}
    }
    
    // display the formula
    printf("%d = %s\n", e, formula);

    // empty out the buffer
    memset(formula, 0, BUFFER_SIZE);

    // reset all the locations
    for (i = 0; i < N; ++i)
	location[i] = 0;

    return e == __target__ ? 0 : -1;
}

int evaluate(int num_op) {
    int i, out;
    for (i = 0; i < num_op; ++i) {
	//printf("performing operation.\n");
	//out = do_op(__solution__[i]);
        out = do_op(__solution__[i], i);
	if (out < 0)
	    break;
    }
    memcpy(__workspace__, __numbers__, 24);

    return out;
}

#define _X_ __workspace__[i]
#define _Y_ __workspace__[j]
 
int do_op(int c, int ii) {

    int i = __lvars__[ii];
    int j = __rvars__[ii];
    
    // see if we want to do _X_ = _Y_ @ _X_, where _Y_ > _X_
    if (c & __OP_FLAG__) {
	
	// if we're doing the reverse order, make sure
	// that the right operand is bigger
	if (_X_ >= _Y_ || _X_ == 0)
	    return -1;

	switch(c & __OP_MASK__) {
	case ADD: _X_ += _Y_;       break;
	case SUB: _X_  = _Y_ - _X_; break;
	case MUL:
	    if (_X_ == 1 || _Y_ == 1) { return -1;
	    } else { _X_ *= _Y_; break; }
	case DIV:
	    if (_X_ == 1 || _Y_ == 1 || _Y_ % _X_) { return -1;
	    } else { _X_  = _Y_ / _X_; break; }
	}
	
    } else {
	
	// if forward, make sure left operand is bigger
	if (_X_ <  _Y_ || _Y_ == 0)
	    return -1;
	
	switch(c & __OP_MASK__) {
	case ADD: _X_ += _Y_; break;
	case SUB: _X_ -= _Y_; break;
	case MUL:
	    if (_X_ == 1 || _Y_ == 1) { return -1;
	    } else { _X_ *= _Y_; break; }
	case DIV:
	    if (_X_ == 1 || _Y_ == 1 || _X_ % _Y_) { return -1;
	    } else { _X_ /= _Y_; break; }
	}
    }
    
    return _X_;
}

int which_vars(int c, int *i, int *j) {
    *i = -1;
    *j = -1;
    int n = 1; // 2^p
    int p = 0; // exponent
    while (*j < 0) {
	if (c & n) {
	    if (*i < 0) {
		*i = p;
	    } else {
		*j = p;
	    }
	}
	++p;
	n <<= 1;
    }
    return 0;
}

