//���ڲ���1 �����������ߵ�ʱ�򣬰����б߶���תһ�μ��ɣ�û��ʲô�Ѷȡ�

//���ڲ���2 �����������ߵ�ʱ�򣬳���Ҫ������·���ϵĵ���Ϸ�ת������⣬
//��Ҫ����ת�����ϵĺ�ֽ�ߣ���Ϊ��Ҫ��ת�����ܷ�ת��ǵ�Ӱ�죻
//��ת�����ĸ��ߣ�������ߣ����ܵ������ĸ��׵ķ�ת��ǵ�Ӱ�죬����ֱ�Ӹ���תһ�ε�������Ӱ��

//���ڲ���3 �����������ߵ�ʱ��ͳ�������ϵĺڱߺ�sum���Ͳ��������ϵĺڱ�
#include<algorithm>
#include<cstdio>
#include<cmath>
#include<iostream>
#include<string.h>
using namespace std;
#define ll __int64
const int N=1e5+10;
#define lson l,mid,o<<1
#define rson mid+1,r,o<<1|1
#define ls o<<1
#define rs o<<1|1
struct Seg{
    int f[N<<2],sum[N<<2];
    void lazy(int l,int r,int o){
        sum[o]=r-l+1-sum[o];
        f[o]^=1;
    }
    void pushup(int o){
        sum[o]=sum[ls]+sum[rs];
    }
    void pushdown(int l,int r,int o){
        if(f[o]){
            int mid=(l+r)>>1;
            lazy(lson); lazy(rson); f[o]=0;
        }
    }
    void build(int l,int r,int o){
        f[o]=sum[o]=0;
        if(l==r) return;
        int mid=(l+r)>>1;
        build(lson); build(rson);
    }
    void update(int l,int r,int o,int L,int R){
        if(L<=l&&r<=R){
            lazy(l,r,o); return;
        }
        pushdown(l,r,o);
        int mid=(l+r)>>1;
        if(L<=mid) update(lson,L,R);
        if(R>mid) update(rson,L,R);
        pushup(o);
    }
    int query(int l,int r,int o,int L,int R){
        if(L<=l&&r<=R) return sum[o];
        pushdown(l,r,o);
        int mid=(l+r)>>1, ans=0;
        if(L<=mid) ans+=query(lson,L,R);
        if(R>mid) ans+=query(rson,L,R);
        return ans;
    }
}light,weight;
struct Edge{
    int to,next;
}edge[N*2];
int head[N],tot;
int fa[N],siz[N],dep[N];
int top[N],son[N];
int p[N],fp[N],tim;//p[u]��ʾu�����ʵ�ʱ��,fp[t]��ʾtʱ�̷���u
void init(){
    tot=0;
    memset(head,-1,sizeof(head));
    memset(son,-1,sizeof(son));
}
void addedge(int u,int v){
    edge[tot].to=v; edge[tot].next=head[u];
    head[u]=tot++;
}
void dfs(int u,int pre,int d){
    dep[u]=d; fa[u]=pre; siz[u]=1;
    for(int i=head[u];i!=-1;i=edge[i].next)
    {
        int v=edge[i].to;
        if(v!=pre){
            dfs(v,u,d+1);
            siz[u]+=siz[v]; //ͳ����uΪ�������Ľڵ���
            if(son[u]==-1||siz[v]>siz[son[u]])
                son[u]=v; //�����ض���
        }
    }
}
void getpos(int u,int sp){//�õ�ʱ���,sp��ʾ�������˵ĵ�
    top[u]=sp; p[u]=++tim; fp[tim]=u;

    if(son[u]==-1) return ;//�����Ҷ�ӽڵ㣬������ݹ�
    getpos(son[u],sp);
    for(int i=head[u];i!=-1;i=edge[i].next){
        int v=edge[i].to;
        if(v!=son[u]&&v!=fa[u]) //ȥ���ض��Ӻ͸��׽ڵ�
            getpos(v,v);
    }
}
void update(int u,int v,int k){
    int f1=top[u],f2=top[v];
    while(f1^f2){
        if(dep[f1]<dep[f2]) swap(u,v),swap(f1,f2);
        if(k){
            light.update(1,tim,1,p[f1],p[u]);
            weight.update(1,tim,1,p[f1],p[f1]);
            if(son[u]!=-1)
                weight.update(1,tim,1,p[son[u]],p[son[u]]);
        }else
            weight.update(1,tim,1,p[f1],p[u]);
        u=fa[f1]; f1=top[u];
    }
    if(dep[u]>dep[v]) swap(u,v);
    if(k){
        light.update(1,tim,1,p[u],p[v]);
        weight.update(1,tim,1,p[u],p[u]);
        if(son[v]!=-1) weight.update(1,tim,1,p[son[v]],p[son[v]]);
    }else{
        if(u==v) return;
        if(son[u]!=-1) weight.update(1,tim,1,p[son[u]],p[v]);
    }
}
int query(int u,int v){
    int ans=0, f1=top[u],f2=top[v];
    while(f1^f2){
        if(dep[f1]<dep[f2]) swap(u,v), swap(f1,f2);
        if(f1!=u) ans+=weight.query(1,tim,1,p[son[f1]],p[u]);
        ans+=(weight.query(1,tim,1,p[f1],p[f1])^light.query(1,tim,1,p[fa[f1]],p[fa[f1]]));
        u=fa[f1]; f1=top[u];
    }
    if(u==v) return ans;
    if(dep[u]>dep[v]) swap(u,v);
    if(son[u]!=-1) ans+=weight.query(1,tim,1,p[son[u]],p[v]);
    return ans;
}
int main(){
#ifdef gh546
freopen("a.in","r",stdin);
#endif // gh546
    int TAT; scanf("%d",&TAT);
    while(TAT--){
        init();
        int n,q; scanf("%d",&n);
        for(int i=1;i<n;i++){
            int u,v; scanf("%d%d",&u,&v);
            addedge(u,v); addedge(v,u);
        }
        dfs(1,0,0); tim=0; getpos(1,1);
        light.build(1,tim,1);
        weight.build(1,tim,1);
        scanf("%d",&q);
        while(q--){
            int op,u,v; scanf("%d%d%d",&op,&u,&v);
            if(op==3) printf("%d\n",query(u,v));
            else update(u,v,op-1);
        }
    }
}
