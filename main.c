#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NODES 5000

// Definition for a binary tree node.
struct TreeNode {
    int val;
    struct TreeNode* left;
    struct TreeNode* right;
};

// Helper to create a new tree node
struct TreeNode* newNode(int val) {
    struct TreeNode* node = (struct TreeNode*)malloc(sizeof(struct TreeNode));
    node->val = val;
    node->left = NULL;
    node->right = NULL;
    return node;
}

// Helper to parse input string like "[5,4,8,11,null,13,4,7,2,null,null,5,1]" to array
int parseInput(const char* s, char arr[][16], int* n) {
    int i = 0, j = 0, k = 0;
    while (s[i] && (s[i] == ' ' || s[i] == '[')) ++i;
    while (s[i] && s[i] != ']') {
        if (s[i] == ',') {
            arr[k][j] = '\0';
            ++k; j = 0;
        } else if (s[i] != ' ') {
            arr[k][j++] = s[i];
        }
        ++i;
    }
    if (j > 0) {
        arr[k][j] = '\0';
        ++k;
    }
    *n = k;
    return k;
}

// Helper to build tree from level-order array
struct TreeNode* buildTree(char arr[][16], int n) {
    if (n == 0 || strcmp(arr[0], "null") == 0) return NULL;
    struct TreeNode** nodes = (struct TreeNode**)malloc(n * sizeof(struct TreeNode*));
    for (int i = 0; i < n; ++i) nodes[i] = NULL;
    struct TreeNode* root = newNode(atoi(arr[0]));
    nodes[0] = root;
    int idx = 1;
    for (int i = 0; i < n && idx < n; ++i) {
        if (!nodes[i]) continue;
        // left child
        if (idx < n && strcmp(arr[idx], "null") != 0) {
            nodes[i]->left = newNode(atoi(arr[idx]));
            nodes[idx] = nodes[i]->left;
        }
        idx++;
        // right child
        if (idx < n && strcmp(arr[idx], "null") != 0) {
            nodes[i]->right = newNode(atoi(arr[idx]));
            nodes[idx] = nodes[i]->right;
        }
        idx++;
    }
    free(nodes);
    return root;
}

// Helper to free the memory of a tree
void deleteTree(struct TreeNode* root) {
    if (!root) return;
    deleteTree(root->left);
    deleteTree(root->right);
    free(root);
}

// --- Solution code ---

int** result;
int max, size, target;

void travel(struct TreeNode* root, int sum, int* table, int len, int **returnColumnSizes){
    if(root == NULL) return;

    if(root->left == NULL && root->right == NULL){
        if (target == sum + root->val){
            table[len++] = root->val;
            (*returnColumnSizes)[size] = len;
            result[size] = (int*)malloc(sizeof(int) * len);
            for(int i = 0; i < len; i++)
                result[size][i] = table[i];
            size++;
            if(size == max){
                max *= 2;
                result = (int**)realloc(result, sizeof(int*) * max);
                *returnColumnSizes = (int*)realloc(*returnColumnSizes, sizeof(int) * max);
            }
        }
        return;
    }
    int temp = sum + root->val;
    table[len] = root->val;
    travel(root->left, temp, table, len + 1, returnColumnSizes);
    travel(root->right, temp, table, len + 1, returnColumnSizes);
}

int** pathSum(struct TreeNode* root, int targetSum, int* returnSize, int** returnColumnSizes){
    if(root == NULL) {
        *returnSize = 0;
        *returnColumnSizes = NULL;
        return NULL;
    }
    size = 0;
    max = 64;
    target = targetSum;

    result = (int**)malloc(sizeof(int*) * max);
    *returnColumnSizes = (int*)malloc(sizeof(int) * max);
    int* table = (int*)malloc(sizeof(int) * 1024);

    travel(root, 0, table, 0, returnColumnSizes);
    *returnSize = size;
    free(table);
    return result;
}

// Helper to print the result
void printResult(int** result, int returnSize, int* returnColumnSizes) {
    printf("[");
    for (int i = 0; i < returnSize; ++i) {
        printf("[");
        for (int j = 0; j < returnColumnSizes[i]; ++j) {
            printf("%d", result[i][j]);
            if (j + 1 < returnColumnSizes[i])
                printf(",");
        }
        printf("]");
        if (i + 1 < returnSize)
            printf(",");
    }
    printf("]\n");
}

int main() {
    struct {
        const char* tree;
        int targetSum;
    } tests[] = {
        {"[5,4,8,11,null,13,4,7,2,null,null,5,1]", 22},
        {"[1,2,3]", 5},
        {"[1,2]", 0}
    };
    int num_cases = sizeof(tests) / sizeof(tests[0]);

    for (int i = 0; i < num_cases; ++i) {
        printf("Input: root = %s, targetSum = %d\n", tests[i].tree, tests[i].targetSum);
        char arr[MAX_NODES][16];
        int n = 0;
        parseInput(tests[i].tree, arr, &n);
        struct TreeNode* root = buildTree(arr, n);

        int returnSize = 0;
        int* returnColumnSizes = NULL;
        int** res = pathSum(root, tests[i].targetSum, &returnSize, &returnColumnSizes);

        printf("Output: ");
        if (res)
            printResult(res, returnSize, returnColumnSizes);
        else
            printf("[]\n");

        for (int j = 0; j < returnSize; ++j)
            free(res[j]);
        free(res);
        free(returnColumnSizes);
        deleteTree(root);
    }
    return 0;
}
