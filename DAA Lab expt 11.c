//Name : Vijyant
//Roll no. : 25/DA/063
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TREE_HT 100
#define DISTINCT_CHARS 256

// A Huffman Tree node
struct MinHeapNode {
    char data;
    unsigned frequency;
    struct MinHeapNode *left, *right;
};

// A Min Heap (Priority Queue) collection structure
struct MinHeap {
    unsigned size;
    unsigned capacity;
    struct MinHeapNode** array;
};

// Allocate a new tree node with given character and frequency
struct MinHeapNode* newMinHeapNode(char data, unsigned frequency) {
    struct MinHeapNode* temp = (struct MinHeapNode*)malloc(sizeof(struct MinHeapNode));
    temp->left = temp->right = NULL;
    temp->data = data;
    temp->frequency = frequency;
    return temp;
}

// Allocate a min heap with a specified capacity
struct MinHeap* createMinHeap(unsigned capacity) {
    struct MinHeap* minHeap = (struct MinHeap*)malloc(sizeof(struct MinHeap));
    minHeap->size = 0;
    minHeap->capacity = capacity;
    minHeap->array = (struct MinHeapNode**)malloc(minHeap->capacity * sizeof(struct MinHeapNode*));
    return minHeap;
}

// Swap two min heap nodes
void swapMinHeapNode(struct MinHeapNode** a, struct MinHeapNode** b) {
    struct MinHeapNode* t = *a;
    *a = *b;
    *b = t;
}

// Standard Min-Heapify function
void minHeapify(struct MinHeap* minHeap, int idx) {
    int smallest = idx;
    int left = 2 * idx + 1;
    int right = 2 * idx + 2;

    if (left < (int)minHeap->size && minHeap->array[left]->frequency < minHeap->array[smallest]->frequency)
        smallest = left;

    if (right < (int)minHeap->size && minHeap->array[right]->frequency < minHeap->array[smallest]->frequency)
        smallest = right;

    if (smallest != idx) {
        swapMinHeapNode(&minHeap->array[smallest], &minHeap->array[idx]);
        minHeapify(minHeap, smallest);
    }
}

// Extract the node with the minimum frequency value from heap
struct MinHeapNode* extractMin(struct MinHeap* minHeap) {
    struct MinHeapNode* temp = minHeap->array[0];
    minHeap->array[0] = minHeap->array[minHeap->size - 1];
    --minHeap->size;
    minHeapify(minHeap, 0);
    return temp;
}

// Insert a new node into the Min Heap
void insertMinHeap(struct MinHeap* minHeap, struct MinHeapNode* minHeapNode) {
    ++minHeap->size;
    int i = minHeap->size - 1;

    while (i && minHeapNode->frequency < minHeap->array[(i - 1) / 2]->frequency) {
        minHeap->array[i] = minHeap->array[(i - 1) / 2];
        i = (i - 1) / 2;
    }
    minHeap->array[i] = minHeapNode;
}

// Build the min heap from the populated array elements
void buildMinHeap(struct MinHeap* minHeap) {
    int n = minHeap->size - 1;
    for (int i = (n - 1) / 2; i >= 0; --i)
        minHeapify(minHeap, i);
}

// Check if this node is a leaf node
int isLeaf(struct MinHeapNode* root) {
    return !(root->left) && !(root->right);
}

// Iteratively combine the lowest subtrees to build the Huffman Tree
struct MinHeapNode* buildHuffmanTree(char data[], int freq[], int size) {
    struct MinHeapNode *left, *right, *top;
    struct MinHeap* minHeap = createMinHeap(size);

    for (int i = 0; i < size; ++i)
        minHeap->array[i] = newMinHeapNode(data[i], freq[i]);

    minHeap->size = size;
    buildMinHeap(minHeap);

    // Loop until the size of heap becomes 1
    while (minHeap->size != 1) {
        // Extract the two minimum frequency nodes from min heap
        left = extractMin(minHeap);
        right = extractMin(minHeap);

        // Create a new internal node with a frequency equal to the sum of the two nodes' frequencies
        // '#' is a special marker character used for internal nodes
        top = newMinHeapNode('#', left->frequency + right->frequency);

        top->left = left;
        top->right = right;

        insertMinHeap(minHeap, top);
    }

    // The remaining node is the root node and the tree is complete
    struct MinHeapNode* root = extractMin(minHeap);
    free(minHeap->array);
    free(minHeap);
    return root;
}

// Print Huffman codes from the root of the Huffman Tree via traversal
void printCodes(struct MinHeapNode* root, int arr[], int top) {
    // Assign 0 to left edge and recur
    if (root->left) {
        arr[top] = 0;
        printCodes(root->left, arr, top + 1);
    }

    // Assign 1 to right edge and recur
    if (root->right) {
        arr[top] = 1;
        printCodes(root->right, arr, top + 1);
    }

    // If this is a leaf node, it contains one of the input characters
    if (isLeaf(root)) {
        printf("  '%c': ", root->data);
        for (int i = 0; i < top; ++i)
            printf("%d", arr[i]);
        printf("\n");
    }
}

// Clean up allocated tree memory
void freeTree(struct MinHeapNode* root) {
    if (!root) return;
    freeTree(root->left);
    freeTree(root->right);
    free(root);
}

int main() {
    char str[] = "huffman coding data compression implementation";
    
    // Arrays to store unique characters and their counts
    char arr[DISTINCT_CHARS];
    int freq[DISTINCT_CHARS];
    int size = 0;

    // Direct frequency map computation
    int countMap[DISTINCT_CHARS] = {0};
    for (int i = 0; str[i] != '\0'; i++) {
        countMap[(unsigned char)str[i]]++;
    }

    // Collect all unique characters found in the string
    for (int i = 0; i < DISTINCT_CHARS; i++) {
        if (countMap[i] > 0) {
            arr[size] = (char)i;
            freq[size] = countMap[i];
            size++;
        }
    }

    printf("--- Input Text Stream ---\n\"%s\"\n\n", str);
    printf("--- Character Frequencies ---\n");
    for (int i = 0; i < size; i++) {
        printf("  '%c': %d\n", arr[i], freq[i]);
    }

    // Construct the Prefix Huffman Tree
    struct MinHeapNode* root = buildHuffmanTree(arr, freq, size);

    // Print generated Variable-Length Codes
    int codeArray[MAX_TREE_HT], top = 0;
    printf("\n--- Generated Huffman Codes ---\n");
    printCodes(root, codeArray, top);

    // Clean up
    freeTree(root);
    return 0;
}
