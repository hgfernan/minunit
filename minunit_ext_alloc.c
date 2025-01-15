/**
 * Sample code for *ext() macros, with memory allocation
 * Created on Wed Jan 15 17:31:07 2025
 * 
 * @author: hilton
 **/

#include "minunit.h"
#include <stdlib.h> /* RAND_MAX, calloc(), free(), srand(), rand() */

#define UNUSED(x) (void)(x)

#define MU_TEST_EXT(method_name, param) static void method_name(void* param)
#define MU_TEST_SUITE_EXT(suite_name, param) static void suite_name(void*, param)

/*  Test setup and teardown function pointers */
static void (*minunit_setup_ext)(void* param) = NULL;
static void (*minunit_teardown_ext)(void* param) = NULL;

/*  Test setup and teardown parameter pointers */
static void *minunit_setup_parm = NULL;
static void *minunit_teardown_parm = NULL;

/* static double tolerance = 0.15; */

struct double_duo {
	size_t length;
	double* left;
	double* right;
};

void test_setup_ext(void* param) {
	size_t ind;
	size_t mean = RAND_MAX / 2;
	struct double_duo* duo = (struct double_duo*)param;
	
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
	struct double_duo* duo = (struct double_duo*)param;
	
	free(duo->left);
	free(duo->right);
}

/*  Configure setup and teardown functions, extended */
#define MU_SUITE_CONFIGURE_EXT(setup_fun, setup_parm, teardown_fun, teardown_parm) MU__SAFE_BLOCK(\
	minunit_setup_ext = setup_fun;\
	minunit_teardown_ext = teardown_fun;\
	minunit_setup_parm = setup_parm;\
	minunit_teardown_parm = teardown_parm;\
)

/*  Test runner */
#define MU_RUN_TEST_EXT(test, parm) MU__SAFE_BLOCK(\
	if (minunit_real_timer==0 && minunit_proc_timer==0) {\
		minunit_real_timer = mu_timer_real();\
		minunit_proc_timer = mu_timer_cpu();\
	}\
	if (minunit_setup_ext) (*minunit_setup_ext)(minunit_setup_parm);\
	minunit_status = 0;\
	test(parm);\
	minunit_run++;\
	if (minunit_status) {\
		minunit_fail++;\
		printf("F");\
		printf("\n%s\n", minunit_last_message);\
	}\
	(void)fflush(stdout);\
	if (minunit_teardown_ext) (*minunit_teardown_ext)(minunit_teardown_parm);\
)

MU_TEST_EXT(test_assert_start_eq, param) {
	struct double_duo* duo = (struct double_duo*)param;
	mu_assert_double_eq(duo->left[0], duo->right[0]);
}

MU_TEST_EXT(test_assert_equiv, param) {
	struct double_duo* duo = (struct double_duo*)param;
	size_t count = 0;
	size_t ind;
	
	for (ind = 0; ind < duo->length; ind++) {
		count += (size_t)(duo->left[ind] == duo->right[ind]);
	}
	
	printf("count ");
	
	mu_assert(duo->length == count, "The arrays are not equal");
}

MU_TEST_SUITE(test_suite_whole) {
	struct double_duo duo = {10, (double*)NULL, (double*)NULL};
	
	MU_SUITE_CONFIGURE_EXT(test_setup_ext, &duo, test_teardown_ext, &duo);

	MU_RUN_TEST_EXT(test_assert_start_eq, &duo);
	
	MU_RUN_TEST_EXT(test_assert_equiv, &duo);
}

int main() {
	MU_RUN_SUITE(test_suite_whole);
	MU_REPORT();
	return MU_EXIT_CODE;
}

