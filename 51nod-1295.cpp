//可持久化字典树
#include<iostream>
#include<algorithm>
#include<stdio.h>
#include<string.h>
#include<math.h>
using namespace std;
const int N=50010;
const int Key=29;//总的位数
const int inf=0x3f3f3f3f;

int W[N],rt[N];
struct Tire{//可持久化字典树
    struct Node{
        int ch[2],cnt;
    }node[N*40];
    int sz;//总节点数
    void init(){
        sz=1; rt[0]=node[0].cnt=0;
        memset(node[0].ch,0,sizeof(node[0].ch));
    }
    int update(int o,int w){
        int u,now;
        u=now=sz++;
        node[now]=node[o];//当前节点等于上一个节点,然后更新
        for(int i=Key;i>=0;i--){
            int v=(w>>i)&1;
            int nxt=sz++;
            node[nxt]=node[node[now].ch[v]];//先新建节点,这样建不会错
            node[nxt].cnt++;
            node[now].ch[v]=nxt;
            now=nxt;
        }
        return u;
    }
    int query(int L,int R,int w){//查询w异或[L,R]里面的数最大是多少
        int ans=0;
        for(int i=Key;i>=0;i--){
            int v=((w>>i)&1)^1;
            int cnt=node[node[R].ch[v]].cnt-node[node[L].ch[v]].cnt;
            if(cnt) ans|=(1<<i);
            else v=v^1;
            L=node[L].ch[v], R=node[R].ch[v];
        }
        return ans;
    }
}tire;
int main(){
#ifdef gh546
freopen("a.in","r",stdin);
#endif // gh546
    int n,q; read(n); read(q);
    for(int i=1;i<=n;i++) read(W[i]);
    tire.init();
    for(int i=1;i<=n;i++){
        rt[i]=tire.update(rt[i-1],W[i]);
    }
    while(q--){
        int x,L,R; read(x); read(L); read(R);
        printf("%d\n",tire.query(rt[L-1],rt[R],x));
    }
    return 0;
}

