/**
 * Sample code for *ext() macros, with memory allocation
 * Created on Wed Jan 15 17:31:07 2025
 * 
 * @author: hilton
 **/

#include "minunit.h"
#include <stdlib.h> /* RAND_MAX, calloc(), free(), srand(), rand() */

/* static double tolerance = 0.15; */

struct two_vecs {
	size_t length;
	double* left;
	double* right;
};

void test_setup_ext(void* param) {
	size_t ind;
	size_t mean = RAND_MAX / 2;
	struct two_vecs* duo = (struct two_vecs*)param;
	
	duo->left  = (double*)calloc(duo->length, sizeof(double));
	duo->right = (double*)calloc(duo->length, sizeof(double));
	
	srand(duo->length);
	duo->left[0] = duo->right[0] = 0.0;
	
	for (ind = 1; ind < duo->length; ind++) {
		duo->left[ind]  = 
			(duo->left[ind - 1] + (rand() - mean)) / RAND_MAX;
		duo->right[ind] = 
			(duo->right[ind - 1] + (rand() - mean)) / RAND_MAX;
	}
}

void test_teardown_ext(void* param) {
	struct two_vecs* duo = (struct two_vecs*)param;
	
	free(duo->left);
	free(duo->right);
}

MU_TEST_EXT(test_assert_start_eq, param) {
	struct two_vecs* duo = (struct two_vecs*)param;
	mu_assert_double_eq(duo->left[0], duo->right[0]);
}

MU_TEST_EXT(test_assert_equiv, param) {
	struct two_vecs* duo = (struct two_vecs*)param;
	size_t count = 0;
	size_t ind;
	
	for (ind = 0; ind < duo->length; ind++) {
		count += (size_t)(duo->left[ind] == duo->right[ind]);
	}
	
	printf("count ");
	
	mu_assert(duo->length == count, "The arrays are not equal");
}

MU_TEST_SUITE(test_suite_whole) {
	struct two_vecs duo = {10, (double*)NULL, (double*)NULL};
	
	MU_SUITE_CONFIGURE_EXT(test_setup_ext, &duo, test_teardown_ext, &duo);

	MU_RUN_TEST_EXT(test_assert_start_eq, &duo);
	
	MU_RUN_TEST_EXT(test_assert_equiv, &duo);
}

int main() {
	MU_RUN_SUITE(test_suite_whole);
	MU_REPORT();
	return MU_EXIT_CODE;
}

