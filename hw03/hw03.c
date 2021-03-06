// EE3980 HW03 Heap Sort
// 105061110, 周柏宇
// 2020/03/25

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#define METHOD 4	// algorithm index
					// 0: selection sort
					// 1: insertion sort
					// 2: bubble sort
					// 3: shaker sort
					// 4: heap sort
int N;								// input size
char **data;						// input data
char **A;							// array to be sorted
int R = 500;						// number of repetitions
char algNames[][10] = {				// names of sorting algorithms available
	"Selection", "Insertion", "Bubble", "Shaker", "Heap"
};
void (*Sort[5])(char **list, int n); // to store pointer of sorting algorithms

void readInput(void);					// read all inputs
void printArray(char **A);				// print the content of array A
void copyArray(char **data, char **A);	// copy data to array A
double GetTime(void);					// get local time in seconds
void SelectionSort(char **list, int n);	// in-place selection sort
void InsertionSort(char **list, int n);	// in-place insertion sort
void BubbleSort(char **list, int n);	// in-place bubble sort
void ShakerSort(char **list, int n);	// in-place shaker sort
void HeapSort(char **list, int n);		// in-place heap sort
void Heapify(char **list, int i, int n); // enforce max heap property
void swap(char **a, char **b);			// swap two words
void freeMemory(char **list, int n);	// free allocated memory

int main(void)
{
	int i;									// index
	double t;								// local time
	
	readInput();							// store inputs in array data
	t = GetTime();							// get local time
	A = (char **)malloc(sizeof(char *) * N);	// allocate memory for copying
	Sort[0] = SelectionSort;				// store function pointer in array
	Sort[1] = InsertionSort;
	Sort[2] = BubbleSort;
	Sort[3] = ShakerSort;
	Sort[4] = HeapSort;
	for (i = 0; i < R; i++) {
		copyArray(data, A);					// copy the data to array A
		(*Sort[METHOD])(A, N);				// execute the sorting algorithm
		if (i == 0) {						// if it is the first execution
			printArray(A);					// print out the sorted array
		}
	}
	t = (GetTime() - t) / R;				// calculate the average run time
	printf("%s Sort:\n", algNames[METHOD]);	// print out the algorithm name
	printf("  N = %d\n", N);				// print out the input size
	printf("  CPU time = %.5e seconds\n", t);	// print out average CPU time
	freeMemory(data, N);					// free array data
	free(A);								// free array A

	return 0;
}

void readInput(void)			// read all inputs
{
	int i;						// index
	char tmpWord[1000];			// store input temporarily

	scanf("%d", &N);			// input number of entries
	data = (char **)malloc(sizeof(char *) * N);	// allocate memory for pointers
	for (i = 0; i < N; i++) {
		scanf("%s", tmpWord);	// input a word
		// allocate memory just enough to fit the word
		data[i] = (char *)malloc(sizeof(char) * (strlen(tmpWord) + 1));
		strcpy(data[i], tmpWord);	// transfer the input to array
	}
}

void printArray(char **A)				// print the content of array A
{
	int i;								// index

	for (i = 0; i < N; i++) {
		printf("%d %s\n", i + 1, A[i]);	// print the index and array content
	}
}

void copyArray(char **data, char **A)	// copy data to array A
{
	int i;								// index

	for (i = 0; i < N; i++) {
		A[i] = data[i];					// copy content from array data to A
	}
}

double GetTime(void)						// get local time in seconds
{
	struct timeval tv;						// variable to store time

	gettimeofday(&tv, NULL);				// get local time

	return tv.tv_sec + 1e-6 * tv.tv_usec;	// return local time in seconds
}


void SelectionSort(char **list, int n)	// in-place selection sort
{
	int i, j, k;						// index

	for (i = 0; i < n; i++) {
		j = i;							// initialize j with i
		for (k = i + 1; k < n; k++) {
			if (strcmp(list[k], list[j]) < 0) {	// if word at k is smaller
				j = k;					// store the index at j
			}
		}
		swap(&list[i], &list[j]);		// swap the words at index i and j
	}
}

void InsertionSort(char **list, int n)	// in-place insertion sort
{
	int j, i;							// index
	char *tmp;							// temporary char pointer

	for (j = 1; j < n; j++) {		// assume list[0 : j - 1] already sorted
		tmp = list[j];				// copy the word at index j
		i = j - 1;					// initialize i with j - 1
		// repeat until list[i] is smaller 
		while ((i >= 0) && (strcmp(tmp, list[i]) < 0)) {
			list[i + 1] = list[i];	// fill the previous word with current word
			i--;					// move on to the next word
		}
		list[i + 1] = tmp;			// fill the word list[j] at index i + 1
	}
}

void BubbleSort(char **list, int n)				// in-place bubble sort
{
	int i, j;									// index

	for (i = 0; i < n - 1; i++) {
		for (j = n - 1; j > i; j--) {			// list[0 : i - 1] is sorted
			if (strcmp(list[j], list[j - 1]) < 0) {	// if right word is smaller
				swap(&list[j], &list[j - 1]);	// swap the right and left word
			}
		}
	}
}

void ShakerSort(char **list, int n)	// in-place shaker sort
{
	int j;							// index
	int l = 0;						// left bound
	int r = n - 1;					// right bound

	while (l <= r) {				// while there are elements between bounds
		for (j = r; j > l; j--) {	// move the smallest word to the left
			if (strcmp(list[j], list[j - 1]) < 0) {	// if right word is smaller
				swap(&list[j], &list[j - 1]);	// swap the right and left word
			}
		}
		l++;						// close the bound on the left
		for (j = l; j < r; j++) {	// move the biggest word to the right
			if (strcmp(list[j], list[j + 1]) > 0) {	// if left word is bigger
				swap(&list[j], &list[j + 1]);	// swap the right and left word
			}
		}
		r--;						// close the bound on the right
	}
}

void HeapSort(char **list, int n)	// in-place heat sort
{
	int i;							// index

	for (i = n / 2 - 1; i >= 0; i--) {	// make the list a max heap
		Heapify(list, i, n);
	}
	for (i = n - 1; i > 0; i--) {
		swap(&list[0], &list[i]);		// move maximum to the end
		Heapify(list, 0, i);	// restore max heap property to unsorted part
	}
}

void Heapify(char **list, int i, int n)	// enforce max heap property
{
	int j;								// index
	int done;							// loop flag
	char *tmpWord;						// temporary word

	j = 2 * (i + 1) - 1;				// initialize j to be lchild of i
	tmpWord = list[i];					// copy root element
	done = 0;
	while ((j <= n - 1) && (!done)) {
		// let list[j] to be the larger child
		if ((j < n - 1) && (strcmp(list[j], list[j + 1]) < 0)) {
			j++;
		}
		if (strcmp(tmpWord, list[j]) > 0) {
			done = 1;					// exit if root is larger
		}
		else {
			list[(j + 1) / 2 - 1] = list[j]; // replace j's parent with list[j]
			j = 2 * (j + 1) - 1;		// move j to its lchild
		}
	}
	list[(j + 1) / 2 - 1] = tmpWord;	// move original root to proper place
}

void swap(char **a, char **b)	// swap two words
{
	char *tmp;					// temporary char pointer

	tmp = *a;					// copy word at address a
	*a = *b;					// change word at a to word at b
	*b = tmp;					// change word at b to original word at a
}

void freeMemory(char **list, int n)	// free allocated memory
{
	int i;							// index

	for (i = 0; i < n; i++) {
		free(list[i]);				// free the memory that stores the words
	}
	free(list);						// free the memory that stores the pointers
}