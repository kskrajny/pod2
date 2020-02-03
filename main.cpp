#include <iostream>
#include<bits/stdc++.h>
using namespace std;

class Data {
public:
    long long r[3];
    long long l[3];
    long long all;
    int my;
    int mod;
    int nr;
/*
        void showData() const {
            cout << "l :";
            for(int i=0;i<3;i++) {
                cout << " " << l[i];
            }
            cout << endl;
            cout << "r :";
            for(int i=0;i<3;i++) {
                cout << " " << r[i];
            }
            cout << endl;
            cout << all << endl;
        }
*/
    Data(long long x, int n) {
        r[x % 3] = 1;
        l[x % 3] = 1;
        r[(x+1)%3] = 0;
        r[(x+2)%3] = 0;
        l[(x+1)%3] = 0;
        l[(x+2)%3] = 0;
        all = 0;
        if (x % 3 == 0) all++;
        mod = x % 3;
        my = x % 3;
        nr = n;
    }

    Data() {
        for(int i=0;i<3;i++) {
            l[i] = 0;
            r[i] = 0;
        }
        all = 0;
        mod = 0;
        my = 0;
    }
};

Data update(const Data &left, const Data &right) {
    Data data;
    //      left.showData();
    //    right.showData();
    data.all = left.all + right.all;
    for (int i = 0; i < 3; i++) {
        data.l[i] = left.l[i] + right.l[(30 + i - left.mod) % 3];
        data.r[i] = right.r[i] + left.r[(30 + i - right.mod) % 3];
        data.all += left.r[i] * right.l[(6 - i) % 3];
    }
    data.mod = (left.mod + right.mod) % 3;
    return data;
}

class node
{
public:
    node *left, *right;
    int size;
    Data data;
};

int size(node *x)
{
    if(x == nullptr) return 0;
    else return x->size;
}

int left_size(node *x)
{
    if(x == nullptr) return 0;
    if(x->left == nullptr) return 0;
    else return x->left->size;
}

node *newNode(int value, int nr)
{
    node* x = (node*)malloc(sizeof(node));
    x->left = x->right = nullptr;
    x->data = Data(value, nr);
    x->size = 1;
    return x;
}

void update(node *x)
{
    x->size = size(x->left)+size(x->right)+1;
    int my = x->data.my;
    int nr = x->data.nr;
    Data help = Data(my, nr);
    //cout << "update " << x->data.nr << endl;
    if(x->left != nullptr) help = update(x->left->data, help);
    if(x->right != nullptr) help = update(help, x->right->data);
    x->data = help;
    x->data.my = my;
    x->data.nr = nr;
}

node *rightRotate(node *x)
{
    node *y = x->left;
    x->left = y->right;
    y->right = x;

    update(x);
    update(y);
    return y;
}

node *leftRotate(node *x)
{
    node *y = x->right;
    x->right = y->left;
    y->left = x;

    update(x);
    update(y);
    return y;
}

node *splay(node *root, int s)
{
    if (root == nullptr || left_size(root)+1 == s)
        return root;

    if (left_size(root) >= s)
    {
        if (left_size(root->left) >= s)
        {
            root->left->left = splay(root->left->left, s);
            root = rightRotate(root);
        }
        else if (left_size(root->left)+1 < s) // Zig-Zag (Left Right)
        {
            root->left->right = splay(root->left->right, s-left_size(root->left)-1);

            if (root->left->right != nullptr)
                root->left = leftRotate(root->left);
        }

        return (root->left == nullptr)? root: rightRotate(root);
    }
    else
    {
        s = s-left_size(root)-1;
        if (left_size(root->right) >= s)
        {
            root->right->left = splay(root->right->left, s);

            if (root->right->left != nullptr)
                root->right = rightRotate(root->right);
        }
        else if (left_size(root->right)+1 < s)
        {
            root->right->right = splay(root->right->right, s-left_size(root->right)-1);
            root = leftRotate(root);
        }

        return (root->right == nullptr)? root: leftRotate(root);
    }
}

node *left_insert(node *root, node *ins)
{
    if(root == nullptr) return root;
    if(left_insert(root->left, ins) == nullptr) root->left = ins;
    update(root);
    return root;
}

node *insert(node *root, int s, int value)
{
    node *newnode = newNode(value, s);

    if (root == nullptr) return newnode;

    if(s <= 1) return left_insert(root, newnode);

    if(s > size(root)) s = size(root)+1;

    s = s-1;
    root = splay(root, s);

    newnode->right = root->right;
    root->right = newnode;

    update(newnode);
    update(root);

    return root;
}

node * query(node *root, long long l, long long r) {
    root = splay(root, r+1);
    root = splay(root, l);
    //cout << root->right->data.nr << endl;
    root->right = splay(root->right, r-l+2);
    //cout << root->data.nr << " " << root->right->data.nr << endl;
    cout << root->right->left->data.all << endl;
    return root;
}

void preOrder(node *root)
{
    if (root != nullptr)
    {
        preOrder(root->left);
        printf("%d : %lld\n", root->data.nr, root->data.all);
        preOrder(root->right);
    }
}


int main() {
    ios::sync_with_stdio(false);
    cin.tie(NULL);
    int n, m, x, a, b;
    cin >> n >> m;
    node *root = nullptr;
    int c;
    for(int i=1;i<n+1;i++) {
        c = 1;
        cin >> x;
        root = insert(root, i, x%3);
        //preOrder(root);
        //cout << endl << endl;
    }
    //cout << root->data.all << endl;
    root = insert(root, 1, 0);
    root = insert(root,  n+2, 0);
    //cout << root->data.all << endl;
    for(int i=0;i<m;i++) {
        cin >> a >> b;
        root = query(root, a, b);
    }
    return 0;
}
/*
 * 10 4
1 0 10 2 30 3 2 1 2 9
 */