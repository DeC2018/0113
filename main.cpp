#include <iostream>
#include <vector>
#include <queue>
#include <string>
#include <sstream>
#include <algorithm>

using namespace std;

// Definition for a binary tree node.
struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode* left, TreeNode* right) : val(x), left(left), right(right) {}
};

class Solution {
 public:
  vector<vector<int>> pathSum(TreeNode* root, int targetSum) {
    vector<vector<int>> ans;
    vector<int> path;
    dfs(root, targetSum, path, ans);
    return ans;
  }

 private:
  void dfs(TreeNode* root, int targetSum, vector<int>& path,
           vector<vector<int>>& ans) {
    if (root == nullptr)
      return;
    path.push_back(root->val);
    if (root->val == targetSum && root->left == nullptr && root->right == nullptr) {
      ans.push_back(path);
    } else {
      dfs(root->left, targetSum - root->val, path, ans);
      dfs(root->right, targetSum - root->val, path, ans);
    }
    path.pop_back();
  }
};

// Helper function to build a binary tree from a level-order traversal
TreeNode* buildTree(const vector<string>& nodes) {
    if (nodes.empty() || nodes[0] == "null") return nullptr;

    TreeNode* root = new TreeNode(stoi(nodes[0]));
    queue<TreeNode*> q;
    q.push(root);

    int i = 1;
    while (!q.empty() && i < nodes.size()) {
        TreeNode* current = q.front();
        q.pop();

        if (i < nodes.size() && nodes[i] != "null") {
            current->left = new TreeNode(stoi(nodes[i]));
            q.push(current->left);
        }
        i++;

        if (i < nodes.size() && nodes[i] != "null") {
            current->right = new TreeNode(stoi(nodes[i]));
            q.push(current->right);
        }
        i++;
    }

    return root;
}

// Helper function to parse input string like "[5,4,8,11,null,13,4,7,2,null,null,5,1]"
vector<string> parseInput(const string& s) {
    vector<string> res;
    string t = s;
    t.erase(remove(t.begin(), t.end(), '['), t.end());
    t.erase(remove(t.begin(), t.end(), ']'), t.end());
    stringstream ss(t);
    string item;
    while (getline(ss, item, ',')) {
        // Remove whitespace
        item.erase(remove_if(item.begin(), item.end(), ::isspace), item.end());
        res.push_back(item);
    }
    return res;
}

// Helper function to print the result
void printResult(const vector<vector<int>>& result) {
    cout << "[";
    for (size_t i = 0; i < result.size(); ++i) {
        cout << "[";
        for (size_t j = 0; j < result[i].size(); ++j) {
            cout << result[i][j];
            if (j + 1 < result[i].size())
                cout << ",";
        }
        cout << "]";
        if (i + 1 < result.size())
            cout << ",";
    }
    cout << "]" << endl;
}

// Helper function to free the memory of a tree
void deleteTree(TreeNode* root) {
    if (!root) return;
    deleteTree(root->left);
    deleteTree(root->right);
    delete root;
}

int main() {
    struct TestCase {
        string tree;
        int targetSum;
    };
    vector<TestCase> tests = {
        {"[5,4,8,11,null,13,4,7,2,null,null,5,1]", 22},
        {"[1,2,3]", 5},
        {"[1,2]", 0}
    };

    Solution solution;

    for (const auto& test : tests) {
        cout << "Input: root = " << test.tree << ", targetSum = " << test.targetSum << endl;
        auto nodes = parseInput(test.tree);
        TreeNode* root = buildTree(nodes);
        vector<vector<int>> result = solution.pathSum(root, test.targetSum);
        cout << "Output: ";
        printResult(result);
        deleteTree(root);
    }

    return 0;
}
