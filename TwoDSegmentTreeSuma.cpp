#include <iostream>
#include <vector>

using namespace std;

class SegmentTree {
private:
    vector<int> tree;
    int n;

    int left(int node) { return 2 * node + 1; }
    int right(int node) { return 2 * node + 2; }
    int mid(int l, int r) { return l + (r - l) / 2; }

    void construir(int node, int start, int end, const vector<int>& values) {
        if (start == end) {
            tree[node] = values[start];
            return;
        }
        int m = mid(start, end);
        construir(left(node), start, m, values);
        construir(right(node), m + 1, end, values);
        tree[node] = tree[left(node)] + tree[right(node)];
    }

    int consulta(int node, int start, int end, int l, int r) {
        if (r < start || end < l) return 0;
        if (l <= start && end <= r) return tree[node];
        int m = mid(start, end);
        return consulta(left(node), start, m, l, r) +
               consulta(right(node), m + 1, end, l, r);
    }

    void actualizar(int node, int start, int end, int idx, int value) {
        if (start == end) {
            tree[node] = value;
        } else {
            int m = mid(start, end);
            if (idx <= m)
                actualizar(left(node), start, m, idx, value);
            else
                actualizar(right(node), m + 1, end, idx, value);
            tree[node] = tree[left(node)] + tree[right(node)];
        }
    }

public:
    SegmentTree(const vector<int>& arr) {
        n = arr.size();
        tree.resize(4 * n, 0);
        construir(0, 0, n - 1, arr);
    }

    int suma(int l, int r) {
        return consulta(0, 0, n - 1, l, r);
    }

    void actualizar(int idx, int value) {
        actualizar(0, 0, n - 1, idx, value);
    }
};

class TwoDSegmentTree {
private:
    vector<SegmentTree*> tree;
    int n;

    int left(int node) { return 2 * node + 1; }
    int right(int node) { return 2 * node + 2; }
    int mid(int l, int r) { return l + (r - l) / 2; }

    void construir(int node, int start, int end, const vector<vector<int>>& matrix) {
        if (start == end) {
            tree[node] = new SegmentTree(matrix[start]);
        } else {
            int m = mid(start, end);
            construir(left(node), start, m, matrix);
            construir(right(node), m + 1, end, matrix);

            vector<int> merged(matrix[0].size(), 0);
            for (int col = 0; col < matrix[0].size(); col++) {
                merged[col] = tree[left(node)]->suma(col, col) + tree[right(node)]->suma(col, col);
            }
            tree[node] = new SegmentTree(merged);
        }
    }

    int consulta(int node, int start, int end, int x1, int x2, int y1, int y2) {
        if (x2 < start || end < x1) return 0;
        if (x1 <= start && end <= x2) return tree[node]->suma(y1, y2);
        int m = mid(start, end);
        return consulta(left(node), start, m, x1, x2, y1, y2) +
               consulta(right(node), m + 1, end, x1, x2, y1, y2);
    }

    void actualizarElemento(int node, int start, int end, int x, int y, int value) {
        if (start == end) {
            tree[node]->actualizar(y, value);
        } else {
            int m = mid(start, end);
            if (x <= m)
                actualizarElemento(left(node), start, m, x, y, value);
            else
                actualizarElemento(right(node), m + 1, end, x, y, value);

            vector<int> merged(tree[0]->suma(0, 0), 0);
            for (int col = 0; col < merged.size(); col++) {
                merged[col] = tree[left(node)]->suma(col, col) + tree[right(node)]->suma(col, col);
            }
            delete tree[node];
            tree[node] = new SegmentTree(merged);
        }
    }

public:
    TwoDSegmentTree(const vector<vector<int>>& matrix) {
        n = matrix.size();
        tree.resize(4 * n, nullptr);
        construir(0, 0, n - 1, matrix);
    }

    int suma(int x1, int y1, int x2, int y2) {
        return consulta(0, 0, n - 1, x1, x2, y1, y2);
    }

    void actualizar(int x, int y, int value) {
        actualizarElemento(0, 0, n - 1, x, y, value);
    }
};

int main() {
    vector<vector<int>> matrix = {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9}
    };
    TwoDSegmentTree tree(matrix);

    cout << "Suma total: " << tree.suma(0, 0, 2, 2) << endl;
    tree.actualizar(1, 1, 10);
    cout << "Suma despues de actualizar: " << tree.suma(0, 0, 2, 2) << endl;

    return 0;
}
