#include <stdio.h>
#include <stdlib.h>

typedef struct MinHeapNode {

	char data;
	unsigned freq;
	struct MinHeapNode *left;
	struct MinHeapNode *right;

}MinHeapNode;

struct MinHeap {

	unsigned size;
	unsigned len;
	MinHeapNode** lst;

};

MinHeapNode* newNode(char data, unsigned freq)
{
	MinHeapNode* temp = (MinHeapNode*)malloc(sizeof(MinHeapNode));

	temp->left = NULL;
    temp->right = NULL;
	temp->data = data;
	temp->freq = freq;

	return temp;
}

struct MinHeap* createMinHeap(unsigned len)

{

	struct MinHeap* minHeap = (struct MinHeap*)malloc(sizeof(struct MinHeap));
	minHeap->size = 0;
	minHeap->len = len;
	minHeap->lst = (MinHeapNode**)malloc(minHeap->len * sizeof(MinHeapNode*));

	return minHeap;
}
void swapNode(MinHeapNode** a, MinHeapNode** b)

{
	MinHeapNode* temp = *a;
	*a = *b;
	*b = temp;
}

int comparegret(struct MinHeapNode* MinHeap1, struct MinHeapNode* MinHeap2){
	if (MinHeap1->freq < MinHeap2->freq)
	return 1;
	else{
		return 0;
	}
}


void minHeapify(struct MinHeap* minHeap, int i)

{

	int smallest = i;

	if (2*i+1<minHeap->size && comparegret(minHeap->lst[2*i+1],minHeap->lst[smallest]))
		smallest = 2*i+1;

	if (2*i+2 < minHeap->size && comparegret(minHeap->lst[2*i+2],minHeap->lst[smallest]))
		smallest = 2*i+2;

	if (smallest != i) {
		swapNode(&minHeap->lst[smallest], &minHeap->lst[i]);
		minHeapify(minHeap, smallest);
	}
}


MinHeapNode* extractMin(struct MinHeap* minHeap)

{

	MinHeapNode* temp = minHeap->lst[0];
	minHeap->lst[0] = minHeap->lst[minHeap->size - 1];

	--minHeap->size;
	minHeapify(minHeap, 0);

	return temp;
}
void insertMinHeap(struct MinHeap* minHeap,MinHeapNode* minHeapNode)
{

	++minHeap->size;
	int i = minHeap->size - 1;

	while (i && comparegret(minHeapNode, minHeap->lst[(i - 1) / 2])) {
		minHeap->lst[i] = minHeap->lst[(i - 1) / 2];
		i = (i - 1) / 2;
	}

	minHeap->lst[i] = minHeapNode;
}

void buildHeap(struct MinHeap* minHeap)

{

	int n = minHeap->size - 1;

	for (int i = (n - 1) / 2; i >= 0; --i)
		minHeapify(minHeap, i);
}

struct MinHeap* createAndbuildHeap(char data[],int freq[], int size)

{

	struct MinHeap* minHeap = createMinHeap(size);

	for (int i = 0; i < size; i++)
		minHeap->lst[i] = newNode(data[i], freq[i]);

	minHeap->size = size;
	buildHeap(minHeap);

	return minHeap;
}

MinHeapNode* HuffmanTree(char data[], int freq[], int size)

{
	MinHeapNode *left;
	MinHeapNode *right;
	MinHeapNode *top;

	struct MinHeap* minHeap = createAndbuildHeap(data, freq, size);
	while (!(minHeap->size == 1)) {
		left = extractMin(minHeap);
		right = extractMin(minHeap);

		top = newNode('$', left->freq + right->freq);

		top->left = left;
		top->right = right;

		insertMinHeap(minHeap, top);
	}

	return extractMin(minHeap);
}

void printCodes(MinHeapNode* root, int array[],int top, int *codes)

{
	if (root->left) {
		array[top] = 0;
		printCodes(root->left, array, top + 1, codes);
	}

	if (root->right) {
		array[top] = 1;
		printCodes(root->right, array, top + 1, codes);
	}
	

	if (!(root->left) && !(root->right)) {
        printf("%c: ", root->data);
		for (int i = 0; i < top; i++)
			printf("%d", array[i]);

		printf("\n");

        int index = root->data;
        codes[index] = 0;

        for (int i = 0; i < top; i++) {
            codes[index] = (codes[index] * 10 + array[i]);
        }
	}
}
void HuffmanCodes(char data[], int freq[], int size, int *codes)

{
	MinHeapNode* root = HuffmanTree(data, freq, size);
	int array[100], top = 0;
    
    printCodes(root, array, top, codes);
}

void compression(char input[], char output[]){
    FILE* ptr = fopen(input, "r");
    char ch;
    int i = 0;
    int f[128] = { 0 };
 
    while (!feof(ptr)) {
        ch = fgetc(ptr);
        f[ch]++;
        i++;
    }
   
    char array[128];
    int freq[128];
    int size = 0;

    for (i = 0; i < 128; i++) {
        if (f[i] != 0) {
            array[size] = i;
            freq[size] = f[i];
            size++;
        }
    }

    int codes[128] = { 0 };
	HuffmanCodes(array, freq, size, codes);
    
    rewind(ptr);
    FILE* out_ptr = fopen(output, "w");
    while (!feof(ptr)) {
        ch = fgetc(ptr);
        for (i = 0; i < size; i++) {
            if(ch == array[i]){
                int index = ch;
                fprintf(out_ptr, "%d", codes[index]);
            }
        }
    }

    fclose(ptr);
    fclose(out_ptr);
}
int main()
{   
    char input_file[20];
    printf("Enter input file name : ");
    scanf("%s",input_file);

    char output_file[20];
    printf("Enter output file name : ");
    scanf("%s",output_file);

    int mode;
    printf("Choose mode (0 for compression, 1 for decompression): ");
    scanf("%d", &mode);

    if(mode == 0){
        compression(input_file, output_file);
    }

	return 0;
}