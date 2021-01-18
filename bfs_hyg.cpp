//程序描述：基于盲目搜索策略的宽度优先搜索方法

#include <iostream>
#include <string>
#include <cstring>
#include <cmath>
#include <vector>
#include <queue>
#include <set>
using namespace std;

#define N 9       //九宫格总数字

//数组定义：0~9阶乘定义
const int jc[N + 1] = { 1,1,2,6,24,120,720,5040,40320,362880 };    //0-9的阶乘

//数组定义，移动规则（分别对应空格右移、下移、上移、左移）
const int zero_move[4][2] = { {0,1}, {1,0},{0,-1},{-1,0} };

//结构体定义：状态结构体
typedef struct Status
{
    int array[N];   //状态数组
    int hash;       //存储某一状态的哈希索引
    int pos;        //0(空格)当前位置
    int step;       //记录移动步数
}Node;


//函数功能：康托展开
//函数参数：某一状态序列
//函数返回：康托展开值
int cantor(int arr[N])
{
    int i, j;
    int sum = 0;
    for (i = 0; i < N; i++)
    {
        int nmin = 0;
        for (j = i + 1; j < N; j++)
        {
            if (arr[i] > arr[j])
                nmin++;
        }
        sum += (nmin * jc[N - i - 1]);

    }

    return sum;
}

//函数功能：交换数组
void swap(int* array, int i, int j)
{
    int t = array[i];
    array[i] = array[j];
    array[j] = t;
}


//函数功能：以矩阵形式打印状态数组
void printArray(int* array)
{
    for (int i = 0; i < N; i++)
    {
        if (i % 3 == 0)
            cout << "\n";
        cout << array[i] << " ";
    }
    cout << endl;
}


//函数功能：复制数组
void copyArray(int source[N], int target[N])
{
    for (int i = 0; i < N; i++)
    {
        target[i] = source[i];
    }

}

//vector<Node> vec_node;
//函数功能：宽度优先搜索
//函数参数：初始状态数组，源状态哈希索引，目标状态哈希索引
//函数返回：搜索步数(空格移动次数)
int bfs(int array_source[N], int source_Hash, int target_Hash)
{
    if (source_Hash == target_Hash)  //消除同一状态
        return 0;

    set<int> setHash;
    queue<vector<Node>> queue_Open;  //Open表
    vector<Node> vec_Node;



    Node now_Status;   //当前状态
    Node next_Status;  //下一状态

    copyArray(array_source, now_Status.array);

    //确定状态中空格（0）的位置
    int pos = 0;
    for (int i = 0; i < N; i++)
    {
        if (array_source[i] == 0)
            break;
        pos++;
    }

    //初始化当前状态结构体
    now_Status.hash = source_Hash;
    now_Status.pos = pos;
    now_Status.step = 0;
    next_Status.step = 0;
    

    //把当前结点放入Open表中
    vec_Node.push_back(now_Status);
    queue_Open.push(vec_Node);
    setHash.insert(now_Status.hash);

    //Open表不为空则继续搜索
    while (!queue_Open.empty())
    {
        //从Open表中删除第一个状态
        // printf("%s\n","进入while循环" );
        vec_Node = queue_Open.front();
        
        queue_Open.pop();
        now_Status = vec_Node[vec_Node.size()-1];
        // printf("%s\n", "printArray(now_Status.array);");
        // printArray(now_Status.array);
        // printf("%s\n","vec_Node[i]");
        //  for(int i = 0; i < vec_Node.size(); i++)
        //     {
        //         printArray(vec_Node[i].array);
        //     }       
        //对Open表中的每一个状态判断空格右、下、上、左移动
        for (int i = 0; i < 4; i++)
        {
            int offsetX = 0, offsetY = 0;
            //固定某一坐标，移动另一坐标
            offsetX = (now_Status.pos % 3 + zero_move[i][0]);
            offsetY = (now_Status.pos / 3 + zero_move[i][1]);

            //边界判断通过后空格进行移动
            if (offsetX >= 0 && offsetX < 3 && offsetY < 3 && offsetY >= 0)
            {
                copyArray(now_Status.array, next_Status.array);//空格移动复制
                next_Status.step = now_Status.step;
               
                next_Status.step++;
                swap(next_Status.array, now_Status.pos, offsetY * 3 + offsetX);

                //移动后的状态哈希索引及空格位置
                next_Status.hash = cantor(next_Status.array);
                next_Status.pos = (offsetY * 3 + offsetX);

                int begin = setHash.size();
                setHash.insert(next_Status.hash);
                int end = setHash.size();

                vec_Node.push_back(next_Status);
                if (next_Status.hash == target_Hash)
                {       
                    for(int i = 0; i < vec_Node.size(); i++)
                    {
                        printArray(vec_Node[i].array);
                    }        
                    printf("%ld\n",  vec_Node.size());
                    return next_Status.step;
                }

                if (end > begin)
                {
                    queue_Open.push(vec_Node);
                }
                vec_Node.pop_back();

            }
        }

    }
    return -1;
}

//函数功能：求逆序数
//函数参数：表示状态的数字序列
int inversion(int array[N])
{
    int sum = 0;
    for (int i = 0; i < N; ++i)
    {
        for (int j = i + 1; j < N; ++j)
        {
            if (array[j] < array[i] && array[j] != 0)  //不与0比较
            {
                sum++;
            }
        }
    }
    return sum;

}

//主程序入口
int main()
{
    string str_source;   //源状态字符串
    string str_target;   //目标状态字符串

    // cout << "请输入初始状态序列：";
    // cin >> str_source;
    // cout << endl;
    // cout << "请输入目标状态序列：";
    // cin >> str_target;
    // cout << endl;
    str_source = "132405678";
    str_target = "123804765";

    //源int数组和目标int数组
    int array_source[N];
    int array_target[N];

    for (int i = 0; i < 9; i++)
    {
        if (str_source.at(i) >= '0' && str_source.at(i) <= '8')
        {
            array_source[i] = str_source.at(i) - '0';
        }
        else
        {
            array_source[i] = 0;
        }
        if (str_target.at(i) >= '0' && str_target.at(i) <= '8')
        {
            array_target[i] = str_target.at(i) - '0';
        }
        else
        {
            array_target[i] = 0;
        }
    }

    //源状态哈希和目标状态哈希
    int sHash, tHash;
    sHash = cantor(array_source);
    tHash = cantor(array_target);

    // printf(" %d,%d\n",sHash,tHash);
    // printArray(array_source);
    // printArray(array_source);

    //求初始状态和目标状态的逆序数
    int inver_source = inversion(array_source);
    int inver_target = inversion(array_target);

    //具有同奇或同偶排列的八数码才能移动可达，否则不可达
    if ((inver_source + inver_target) % 2 == 0)
    {
        int step = bfs(array_source, sHash, tHash);
        cout << "从初始状态到目标状态空格0的最小移动步数为："<<step << endl; //输出步数     
    }
    else
    {
        cout << "无法从初始状态到达目标状态!" << endl;
    }

    return 0;
}

