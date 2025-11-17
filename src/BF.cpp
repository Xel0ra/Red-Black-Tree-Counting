#include <iostream>
#include <vector>
#include <cmath>
#include <string>

using namespace std;
/*  
生成所有可能的结构： 生成 $N$ 个节点的所有可能的二叉树形态。
生成所有可能的染色： 对每一种形态，生成所有可能的红黑染色方案。
验证每一种方案： 检查每一种“形态 + 染色”的组合是否满足红黑树的所有5条性质。
计数： 统计所有通过验证的树的数量。
O(8^N/sqrt(N)) limit N=6
*/

// 模数
const int MOD = 1000000007;

// 节点颜色
enum Color { RED, BLACK };

// 树节点结构
struct Node {
    Color color;
    Node *left;
    Node *right;
    
    // 构造函数
    Node(Node* l = nullptr, Node* r = nullptr) : color(RED), left(l), right(r) {}
};

// -----------------------------------------------------------------
// 阶段 3: 验证函数
// -----------------------------------------------------------------

/**
 * 验证性质 4: 红色节点必须有两个黑色的子节点 (NULL 视为黑色)
 */
bool check_prop4(Node* node) {
    if (node == nullptr) {
        return true; // NULL 节点是黑色的, 满足
    }

    if (node->color == RED) {
        // 检查左子节点
        if (node->left != nullptr && node->left->color == RED) {
            return false;
        }
        // 检查右子节点
        if (node->right != nullptr && node->right->color == RED) {
            return false;
        }
    }

    // 递归检查子树
    return check_prop4(node->left) && check_prop4(node->right);
}

/**
 * 验证性质 5: 所有路径黑高相同
 * 返回:
 * - 0:  如果 node 是 NULL
 * - -1: 如果该子树违反了性质 5
 * - h:  如果该子树有效, 其黑高为 h
 */
int get_black_height(Node* node) {
    if (node == nullptr) {
        return 0; // NULL 叶子节点的黑高为 0
    }

    int left_bh = get_black_height(node->left);
    int right_bh = get_black_height(node->right);

    // 检查1: 子树是否已经失效?
    if (left_bh == -1 || right_bh == -1) {
        return -1;
    }

    // 检查2: 左右子树的黑高是否相等?
    if (left_bh != right_bh) {
        return -1;
    }

    // 计算并返回当前节点的黑高
    int my_bh = left_bh + (node->color == BLACK ? 1 : 0);
    return my_bh;
}

/**
 * 验证一个带颜色的树是否是合法的红黑树
 */
bool isValidRBTree(Node* root) {
    if (root == nullptr) {
        return true; // 一个空树是合法的 (虽然我们这里 N >= 1)
    }

    // 1. 验证性质 2: 根节点必须是黑色
    if (root->color == RED) {
        return false;
    }

    // 2. 验证性质 4: 红色节点的孩子必须是黑色
    if (!check_prop4(root)) {
        return false;
    }

    // 3. 验证性质 5: 所有路径黑高相同
    if (get_black_height(root) == -1) {
        return false;
    }

    // 性质 1 (红或黑) 和 性质 3 (NULL是黑) 已经隐含满足
    return true;
}


// -----------------------------------------------------------------
// 阶段 1: 生成所有可能的树形态
// -----------------------------------------------------------------

/**
 * 递归生成所有具有 n 个内部节点的二叉树 *形态* (不含颜色)
 * 返回一个包含所有形态的根节点的 vector
 */
vector<Node*> generate_shapes(int n) {
    vector<Node*> shapes_list;
    if (n == 0) {
        // 基本情况: 0 个节点, 只有 NULL
        shapes_list.push_back(nullptr);
        return shapes_list;
    }

    // 递归情况: 1 个根 + k 个左子树 + (n-1-k) 个右子树
    for (int k = 0; k < n; ++k) {
        vector<Node*> left_shapes = generate_shapes(k);
        vector<Node*> right_shapes = generate_shapes(n - 1 - k);

        // 组合所有可能的左右子树
        for (Node* l_shape : left_shapes) {
            for (Node* r_shape : right_shapes) {
                Node* new_root = new Node(l_shape, r_shape);
                shapes_list.push_back(new_root);
            }
        }
    }
    return shapes_list;
}

// -----------------------------------------------------------------
// 阶段 2: 应用染色方案
// -----------------------------------------------------------------

/**
 * 辅助函数: 收集一棵树中的所有 N 个节点 (按中序遍历)
 */
void get_nodes_in_order(Node* node, vector<Node*>& nodes) {
    if (node == nullptr) {
        return;
    }
    get_nodes_in_order(node->left, nodes);
    nodes.push_back(node);
    get_nodes_in_order(node->right, nodes);
}

/**
 * 辅助函数: 清理 (删除) 一棵树
 */
void delete_tree(Node* node) {
    if (node == nullptr) {
        return;
    }
    delete_tree(node->left);
    delete_tree(node->right);
    delete node;
}


int main() {
    int n;
    cin >> n;

    if (n == 0) {
        cout << 1 << endl; // 只有 NULL 树
        return 0;
    }
    
    // 阶段 1: 生成所有 N 个节点的树形态
    vector<Node*> all_shapes = generate_shapes(n);

    long long total_valid_trees = 0;

    // 遍历每一种形态
    for (Node* shape : all_shapes) {
        
        // 辅助: 获取该形态的 N 个节点
        vector<Node*> nodes_in_order;
        get_nodes_in_order(shape, nodes_in_order);

        // 阶段 2: 遍历所有 2^N 种染色方案
        // (i 是一个 N 位的 bitmask, 0=RED, 1=BLACK)
        long long num_colorings = (1LL << n);
        
        for (long long i = 0; i < num_colorings; ++i) {
            
            // 应用第 i 种染色方案
            for (int j = 0; j < n; ++j) {
                if ((i >> j) & 1) {
                    nodes_in_order[j]->color = BLACK;
                } else {
                    nodes_in_order[j]->color = RED;
                }
            }

            // 阶段 3: 验证
            if (isValidRBTree(shape)) {
                total_valid_trees = (total_valid_trees + 1) % MOD;
            }
        }
    }

    cout << total_valid_trees << endl;

    // 清理内存 (非常重要, 否则会内存泄漏)
    for (Node* shape : all_shapes) {
        delete_tree(shape);
    }

    return 0;
}