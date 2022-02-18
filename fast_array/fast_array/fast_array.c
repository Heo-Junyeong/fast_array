/**
* @ author : junyeong heo
\brief
** These functions are the data structure API
** of the high-speed insertion array.
** The time complexity of the stack insertion algorithm of
** the existing array is O(n), but this array is O(1).
*/

#include "fast_array.h"

// math api
float gaussian_random(float average, float stdev)
{
	float v1, v2, s, temp;

	do {
		v1 = 2 * ((float)rand() / RAND_MAX) - 1;      // range = [-1.0, 1.0]
		v2 = 2 * ((float)rand() / RAND_MAX) - 1;      // range = [-1.0, 1.0]
		s = v1 * v1 + v2 * v2;
	} while (s >= 1 || s == 0);

	s = sqrt((-2 * log(s)) / s);

	temp = v1 * s;
	temp = (stdev * temp) + average;

	return temp;
}

// Functions Implementation
void zeros(dtype* arr, const size_t size)
{
	int i;
	for (i = 0; i < size; i++) arr[i] = 0;
}
void zeros_pidx(dtype* arr, const size_t size, pIdx idx)
{
	int i;
	for (i = idx; i < idx + size; i++) arr[i] = 0;
}
void ones(dtype* arr, const size_t size)
{
	int i;
	for (i = 0; i < size; i++) arr[i] = 1;
}
void ones_pidx(dtype* arr, const size_t size, pIdx idx)
{
	int i;
	for (i = idx; i < idx + size; i++) arr[i] = 1;
}
void rands(dtype* arr, const size_t size, float average, float stdev)
{
	int i;
	for (i = 0; i < size; i++) arr[i] = gaussian_random(average, stdev);
}
void rands_pidx(dtype* arr, const size_t size, float average, float stdev, pIdx idx)
{
	int i;
	for (i = idx; i < idx + size; i++) arr[i] = gaussian_random(average, stdev);
}
void sin_(dtype* arr, const size_t size, float f0, float fs, float phase)
{
	int i;
	for (i = 0; i < size; i++) arr[i] = (dtype)sin(PI2 * (f0 / fs) * i + DEG2RAD(phase));
}
void sin_pidx(dtype* arr, const size_t size, float f0, float fs, float phase, pIdx idx)
{
	int i;
	for (i = idx; i < idx + size; i++) arr[i] = (dtype)sin(PI2 * (f0 / fs) * i + DEG2RAD(phase));
}
void cos_(dtype* arr, const size_t size, float f0, float fs, float phase)
{
	int i;
	for (i = 0; i < size; i++) arr[i] = (dtype)cos(PI2 * (f0 / fs) * i + DEG2RAD(phase));
}
void cos_pidx(dtype* arr, const size_t size, float f0, float fs, float phase, pIdx idx)
{
	int i;
	for (i = idx; i < idx + size; i++) arr[i] = (dtype)cos(PI2 * (f0 / fs) * i + DEG2RAD(phase));
}


dtype dot_product(dtype* a, dtype* b, pIdx size)
{
	dtype sum = 0; int i;
	for (i = 0; i < size; i++)  sum += a[i] * b[i];
	return sum;
}
dtype dot_product_pidx(dtype* arr1, dtype* arr2, const size_t size, pIdx ptr_idx)
{
	dtype sum = 0; int i;
	for (i = ptr_idx; i < ptr_idx + size; i++) sum += arr1[i] * arr2[i];
	return sum;
}
dtype dot_product_pidx_debug(dtype* arr1, dtype* arr2, const size_t size, pIdx ptr_idx)
{
	dtype sum = 0; int i;
	for (i = ptr_idx; i < ptr_idx + size; i++)
	{
		sum += arr1[i] * arr2[i];
		printf("arr[%d] = %.4f * arr2[%d]= %.4f = %.4f \n", i, arr1[i], i, arr2[i], sum);
	}
	return sum;
}
dtype dot_product_dpidx(dtype* arr1, dtype* arr2, const  size_t size, pIdx ptr_idx1, pIdx ptr_idx2)
{
	dtype sum = 0; int i, j;
	for (i = ptr_idx1, j = ptr_idx2; i < ptr_idx1 + size; i++, j++) sum += arr1[i] * arr2[j];
	return sum;
}
dtype dot_product_dpidx_debug(dtype* arr1, dtype* arr2, const size_t size, pIdx ptr_idx1, pIdx ptr_idx2)
{
	dtype sum = 0; int i, j;
	for (i = ptr_idx1, j = ptr_idx2; i < ptr_idx1 + size; i++, j++)
	{
		sum += arr1[i] * arr2[j];
		printf("arr[%d] = %.4f * arr2[%d]= %.4f = %.4f \n", i, arr1[i], i, arr2[j], sum);
	}
	return sum;
}
dtype dot_product4(dtype* arr1, dtype* arr2, const size_t size, pIdx ptr_idx1, pIdx ptr_idx2)
{
	dtype sum = 0; int i, j;
	for (i = ptr_idx1, j = ptr_idx2; i < ptr_idx1 + size; i++, j++) sum += arr2[i] * arr1[j];
	return sum;
}

/******************************************************************************
**                          FUNCTION IMPLEMENTAION
**                          PRINT ARRAY
*******************************************************************************/
void print_arrf_vect(dtype* ptr, const size_t size, pIdx ptr_idx)
{
	int i;
	for (i = ptr_idx; i < ptr_idx + size; i++) printf("%.6f ", ptr[i]);
	printf("\n");
}
void print_arrs_vect(short* ptr, const size_t size, pIdx ptr_idx)
{
	int i;
	for (i = ptr_idx; i < ptr_idx + size; i++) printf("%d ", ptr[i]);
	printf("\n");
}
void print_arrf(dtype* arr, const size_t size)
{
	int i;
	for (i = 0; i < size; i++) printf("%.6f ", arr[i]);
	printf("\n");
}
void print_arrs(short* arr, const size_t size)
{
	int i;
	for (size_t i = 0; i < size; i++) printf("%d ", arr[i]);
	printf("\n");
}


/******************************************************************************
**                          FUNCTION IMPLEMENTAION
**                          ADAPTIVE ALGORITHM
*******************************************************************************/
dtype least_mean_square(dtype* x, dtype* h, dtype* desired, dtype* y, dtype adapt_rate, dtype error,
	const int n_coeffecients, const int n_output_samples)
{
	/*
	* Arguments
	- x : Pointer to input samples
	- h : Pointer to the coefficient array
	- desired : Pointer to the desired output array
	- y : Pointer to filtered output array
	- adapt_rate : Adaptation rate error Initial error (mu)
	- n_coeffocients : Number of coefficients
	- n_output_samples : Number of output samples
	
	Description
	-	The least_mean_square implements an LMS adaptive filter.
		Given an actual input signal and a desired input signal,
		the filter produces an output signal, the final coefficient values,
		and returns the final output error signal.
	*/

	int i, j;
	dtype sum = 0.0f;
	for (i = 0; i < n_output_samples; i++)
	{
		for (j = 0; j < n_coeffecients; j++) h[j] = h[j] + (adapt_rate * error * x[i + j - 1]); // w(n+1) = w(n) + \mu*x(n)*e(n)
		for (j = 0; j < n_coeffecients; j++) sum += h[j] * x[i + j];
		y[i] = sum;
		error = desired[i] - sum;
	}
	return error;
}

dtype fast_least_mean_square(dtype* x, dtype* h, dtype* desired, dtype* y, dtype adapt_rate, dtype error,
	const int n_coeffecients, const int n_output_samples, pIdx x_idx, pIdx h_idx, pIdx d_idx, pIdx y_idx)
{
	/*
	* Arguments
	- x : Pointer to input samples
	- h : Pointer to the coefficient array
	- desired : Pointer to the desired output array
	- y : Pointer to filtered output array
	- adapt_rate : Adaptation rate error Initial error
	- n_coeffocients : Number of coefficients
	- n_output_samples : Number of output samples
	- x_idx : Index of fast array x
	- h_idx : Index of fast array h
	- d_idx : Index of fast array d
	- y_idx : Index of fast array y

	Description
	-	The least_mean_square implements an LMS adaptive filter.
		Given an actual input signal and a desired input signal,
		the filter produces an output signal, the final coefficient values,
		and returns the final output error signal.
	*/

	int i, j;
	dtype sum = 0.0f;
	for (i = y_idx; i < n_output_samples + y_idx; i++)
	{
		for (j = h_idx; j < n_coeffecients + h_idx; j++) h[j] = h[j] + (adapt_rate * error * x[i + j - 1]); // w(n+1) = w(n) + \mu*x(n)*e(n)
		for (j = h_idx; j < n_coeffecients + h_idx; j++) sum += h[j] * x[i + j];
		y[i] = sum;
		error = desired[i] - sum;
	}
	return error;
}

/******************************************************************************
**                          FUNCTION IMPLEMENTAION
**                          OPEARTION
*******************************************************************************/
void autocor(dtype* __restrict r, const dtype* __restrict x, const int autocor_len, const int lag)
{
	/*
	* Arguments
	- r : Pointer to output array of autocorrelation of length audocor_len.
	- x : Pointer to input array of length nx+nr. Input data must be padded with nr consecutive zeros at the beginning.
	- autocor_len : Length of autocorrelation vector
	- lag : Length of lag

	Description
	-	This routine performs the autocorrelation of the input array x.
		It is assumed that the length of the input array, x, is a multiple of 2 
		and the length of the output array, r, is a multiple of 4.
		The assembly routine computes 4 output samples at a time.
		It is assumed that input vector x is padded with nr no of zeros in the beginning.
	*/
	int i, k;
	dtype sum;
	for (i = 0; i < lag; i++)
	{
		sum = 0;
		for (k = lag; k < autocor_len + lag; k++) sum += x[k] * x[k-i];
		r[i] = sum;
	}
}

void fast_autocor(dtype* __restrict r, const dtype* __restrict x, const int autocor_len, const int lag, pIdx r_idx, pIdx x_idx)
{
	/*
	* Arguments
	- r : Pointer to output array of autocorrelation of length audocor_len.
	- x : Pointer to input array of length nx+nr. Input data must be padded with nr consecutive zeros at the beginning.
	- autocor_len : Length of autocorrelation vector
	- lag : Length of lag

	Description
	-	This routine performs the autocorrelation of the input array x.
		It is assumed that the length of the input array, x, is a multiple of 2
		and the length of the output array, r, is a multiple of 4.
		The assembly routine computes 4 output samples at a time.
		It is assumed that input vector x is padded with nr no of zeros in the beginning.
	*/
	int i, k;
	dtype sum;
	for (i = 0; i < lag; i++)
	{
		sum = 0;
		for (k = lag; k < autocor_len + lag; k++) sum += x[k] * x[k - i];
		r[i] = sum;
	}
}