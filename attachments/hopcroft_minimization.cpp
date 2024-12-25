#include <iostream> // 包含输入输出流的头文件
#include <vector>   // 包含向量容器的头文件
#include <set>      // 包含集合容器的头文件
#include <cstring>  // 包含C风格字符串处理函数的头文件
using namespace std; // 使用标准命名空间，避免每次调用标准库函数时都需要加std::

int zf; // 用于存储输入文件中定义的字符集的大小
char zfj[101]; // 字符数组，用于存储这些字符

// NFA节点结构体定义
struct Node {
    int s; // 状态编号
    bool flag; // 标记一个DFA节点集合是否包含NFA结束态，即表示DFA中是否是一个结束态
    Node(int ss, bool f) { // 构造函数
        s = ss; // 初始化状态编号
        flag = f; // 初始化标记
    }
};

// DFA边结构体定义
struct Edge {
    int to; // 边的目标状态
    char c; // 边对应的字符
    Edge(int y, char z) { // 构造函数
        to = y; // 初始化目标状态
        c = z; // 初始化字符
    }
};

// 存最小化DFA边集结构体定义
struct Edge_dfa {
    int from; // 边的起始状态
    int to; // 边的目标状态
    char c; // 边对应的字符
    Edge_dfa(int x, int y, char z) { // 构造函数
        from = x; // 初始化起始状态
        to = y; // 初始化目标状态
        c = z; // 初始化字符
    }
};

// 重载<运算符，用于比较Edge_dfa对象，以便在集合中排序
bool operator<(const Edge_dfa &x, const Edge_dfa &y) {
    if (x.from < y.from)
        return true;
    else if (x.to < y.to)
        return true;
    else
        return x.c < y.c;
}

vector<Node> V; // DFA节点集
vector<Edge> edge[101]; // DFA边集，使用数组存储，索引对应状态编号
int v, e; // 分别存储节点数和边数
int t = 2; // 默认可分为N和A两个集合
vector<int> eq; // 存储等价关系集合
set<Edge_dfa> Ed; // 存储最小化DFA边集
int p[101]; // 记录状态属于哪个集合

// split函数用于在Hopcroft算法中分割状态集合
void split(int x, char c) {
    int s = eq[x]; // 获取当前集合编号x对应的状态集合
    int a[101]; // 存储吃完字符达到的状态集
    int b[101]; // 对应a中分割的吃字符前的状态
    memset(a, 0, sizeof(a)); // 初始化a数组
    memset(b, 0, sizeof(b)); // 初始化b数组
    int i, j, k;
    for (i = 0; i < v; i++) { // 遍历集合中的状态
        if ((s >> i) & 1) { // 如果状态i属于集合s
            for (j = 0; j < edge[i].size(); j++) { // 遍历状态i的出边
                if (edge[i][j].c == c) { // 如果出边的字符是c
                    int v = edge[i][j].to; // 获取目标状态
                    a[p[v]] |= (1 << v); // 更新a数组，表示状态v属于集合p[v]
                    b[p[v]] |= (1 << i); // 更新b数组，表示状态i属于集合p[v]
                }
            }
        }
    }
    int ll = t; // 记录当前集合的数量
    for (i = 0; i < ll; i++) { // 遍历所有集合
        if (b[i] == eq[x]) break; // 如果找到匹配的集合，则跳出循环
        if (a[i] && i != x) { // 如果集合i在吃完字符c后与集合x相同，且i不等于x
            eq.push_back(b[i]); // 将b[i]作为新的集合加入eq
            for (k = 0; k < v; k++) // 更新状态集合映射p
                if ((b[i] >> k) & 1)
                    p[k] = t;
            eq[x] &= (~b[i]); // 更新集合x
            t++; // 增加集合计数
        }
    }
}

// Hopcroft函数用于执行Hopcroft算法，将DFA的状态集合分割成不相交的等价类
void Hopcroft() {
    int N = 0, A = 0; // 分别表示非接受状态集合和接受状态集合
    for (int i = 0; i < v; i++) { // 分成N和A两个集合
        if (V[i].flag) { // 如果状态i是接受状态
            A |= (1 << V[i].s); // 将状态i加入接受状态集合A
            p[V[i].s] = 1; // 标记状态i属于集合1
        } else {
            N |= (1 << V[i].s); // 将状态i加入非接受状态集合N
            p[V[i].s] = 0; // 标记状态i属于集合0
        }
    }
    if (N == 0) return; // 如果没有非接受状态，则算法结束
    eq.push_back(N); // 将非接受状态集合加入等价关系集合
    eq.push_back(A); // 将接受状态集合加入等价关系集合
    int i, j;
    int l;
    while (1) {
        for (i = 0; i < zf; i++) { // 对于每个输入字符
            l = t; // 记录当前集合的数量
            for (j = 0; j < l; j++)
                split(j, zfj[i]); // 分割状态集合
        }
        if (l == t) break; // 如果集合数量没有变化，则算法结束
    }
}

// put_status函数用于打印等价状态集合的信息
void put_status(int x, int i) {
    int j;
    printf("状态："); // 打印状态集合的开始
    for (int j = 0; j < v; j++) // 遍历所有状态
        if ((x >> j) & 1) // 如果状态j属于集合x
            printf("q%d ", j); // 打印状态编号
    printf("为等价状态，可合并，记作状态T%d\n", i); // 打印等价状态集合编号
}

// main函数是程序的入口点
int main() {
    cin >> zf; // 从标准输入读取字符集大小
    for (int i = 0; i < zf; i++) // 从标准输入读取字符集
        cin >> zfj[i];
    cin >> v; // 从标准输入读取节点数
    for (int i = 0; i < v; i++) { // 从标准输入读取节点信息
        int x, y;
        cin >> x >> y; // 从标准输入读取状态编号和是否是接受状态
        bool flag = (y == 1); // 如果是接受状态，设置标志位
        V.push_back(Node(x, flag)); // 将节点加入节点集
    }
    cin >> e; // 从标准输入读取边数
    for (int i = 0; i < e; i++) { // 从标准输入读取边信息
        int x, y;
        char z;
        cin >> x >> y >> z; // 从标准输入读取边的起始状态、目标状态和字符
        edge[x].push_back(Edge(y, z)); // 将边加入边集
    }
    Hopcroft(); // 执行Hopcroft算法
    for (int i = 0; i < t; i++) // 打印等价状态集合信息
        put_status(eq[i], i);
    cout<<"共"<<t<<"个等价类"<<endl;
    for (int i = 0; i < v; i++) { // 合并边集
        for (int j = 0; j < edge[i].size(); j++) {
            Ed.insert(Edge_dfa(p[i], p[edge[i][j].to], edge[i][j].c)); // 将边加入最小化DFA边集
        }
    }
    bool type[t][127];
    for (int i = 0; i < t; ++i) {
        for (int j = 0; j < 127; ++j) {
            type[i][j] = false;
        }
    }
    for (set<Edge_dfa>::iterator it = Ed.begin(); it != Ed.end(); it++) // 打印最小化DFA的边i
        if(type[it->from][int(it->c)]==false) {
            printf("状态T%d -> 状态T%d,当吃入字符%c\n", it->from, it->to, it->c);
            type[it->from][int(it->c)]=true;//每一种状态转移只需输出一次
        }
    return 0; // 程序结束
}
