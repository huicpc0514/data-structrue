//n个数的排序,q次询问,每次询问[l,r]内有多少对数满足pi整除pj
//首先满足整除共nlogn对,计算l~r可以用1-r减1~l-1,但是会多算在[1,l-1],[l,r]这样的情况
//x=1~r减1-l-1, y=分别在[1,l-1],[l,r]
//ans=x-y 离线处理,对于所有L==i的情况,查询sum[R]-sum[L-1],然后把x[i]的倍数更新
//所有R==i的情况,查询sum[R]-sum[L-1]
#include<stdio.h>
#include<string.h>
#include<iostream>
#include<algorithm>
#include<vector>
#include<queue>
#include<stack>
#include<math.h>
using namespace std;
const int N=2e5+10;
int sum[N],n,q;
int lowbit(int x){return x&(-x);}
void update(int pos,int val){
    for(int i=pos;i<=n;i+=lowbit(i))
        sum[i]+=val;
}
int query(int pos){
    int ans=0;
    for(int i=pos;i;i-=lowbit(i)){
        ans+=sum[i];
    }
    return ans;
}
struct Node{
    int l,r,id;
    bool operator <(const Node &a)const{
        return l<a.l;
    }
}t1[N],t2[N];
int cmp(Node a,Node b){
    return a.r<b.r;
}
int a[N],ans[N],pos[N];
int main()
{
    while(scanf("%d%d",&n,&q)!=EOF){
        for(int i=1;i<=n;i++){
            scanf("%d",&a[i]);
            pos[a[i]]=i;
        }
        for(int i=1;i<=q;i++){
            scanf("%d%d",&t1[i].l,&t1[i].r); t1[i].id=i;
            t2[i]=t1[i];
        }
        sort(t1+1,t1+q+1);
        sort(t2+1,t2+q+1,cmp);
        int L=1,R=1;
        for(int i=1;i<=n;i++){
            while(L<=q&&t1[L].l==i){
                int id=t1[L].id;
                ans[id]-=(query(t1[L].r)-query(t1[L].l-1));
                L++;
            }
            for(int x=a[i];x<=n;x+=a[i]) update(pos[x],1);
            while(R<=q&&t2[R].r==i){
                int id=t2[R].id;
                ans[id]+=(query(t2[R].r)-query(t2[R].l-1));
                R++;
            }
        }
        for(int i=1;i<=q;i++) printf("%d\n",ans[i]);
    }
    return 0;
}
