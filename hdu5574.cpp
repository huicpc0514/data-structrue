#include<stdio.h>
#include<iostream>
#include<string.h>
#include<algorithm>
#include<set>
using namespace std;
template <class T>
void read(T &ret){
    int f=1; char ch=getchar();
    ret=0;
    while(ch<'0'||ch>'9'){if(ch=='-') f=-f; ch=getchar();}
    while(ch>='0'&&ch<='9'){ret=ret*10+ch-'0'; ch=getchar();}
    ret*=f;
}
const int N=100010;
struct Edge{
    int to,next;
}edge[N*2];
int head[N],tot;
int fa[N],siz[N],dep[N];
int top[N],son[N];
int st[N],ed[N],id[N],tim;//p[u]表示u被访问的时间,fp[t]表示t时刻访问u
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
            siz[u]+=siz[v]; //统计以u为根的树的节点数
            if(son[u]==-1||siz[v]>siz[son[u]])
                son[u]=v; //更新重儿子
        }
    }
}
void getpos(int u,int sp){//得到时间戳,sp表示重链顶端的点
    top[u]=sp; st[u]=++tim; id[tim]=u;
    if(son[u]!=-1){
        getpos(son[u],sp);
        for(int i=head[u];i!=-1;i=edge[i].next){
            int v=edge[i].to;
            if(v!=son[u]&&v!=fa[u]) //去掉重儿子和父亲节点
                getpos(v,v);
        }
    }
    ed[u]=tim;
}
int n;
#define lson l,mid,o<<1
#define rson mid+1,r,o<<1|1
struct Node{
    int val,col;
}node[N<<2];
int col[N];
void build(int l,int r,int o){
    node[o].val=0;
    if(l==r){
        node[o].col=col[id[l]]; return;
    }
    int mid=(l+r)>>1;
    build(lson); build(rson);
    node[o].col=1;
}
void pushdown(int o){
    if(node[o].val){
        int ls=o<<1, rs=ls+1;
        node[ls].val+=node[o].val;
        node[rs].val+=node[o].val;
        node[o].val=0;
    }
}
void update(int l,int r,int o,int L,int R,int val){
    if(L<=l&&r<=R){
        node[o].val+=val; return;
    }
    int mid=(l+r)>>1;
    if(L<=mid) update(lson,L,R,val);
    if(R>mid) update(rson,L,R,val);
}
int query(int l,int r,int o,int pos){
    if(l==r) return node[o].val;
    pushdown(o);
    int mid=(l+r)>>1;
    if(pos<=mid) return query(lson,pos);
    return query(rson,pos);
}
int lca(int u,int v){
    while(top[u]!=top[v]){
        if(dep[top[u]]<dep[top[v]]) swap(u,v);
        u=fa[top[u]];
    }
    return dep[u]<dep[v]?u:v;
}
void tupdate(int u,int v,int w){
    while(top[u]!=top[v]){
        if(dep[top[u]]<dep[top[v]]) swap(u,v);
        update(1,n,1,st[top[u]],st[u],w);
        u=fa[top[u]];
    }
    if(dep[u]>dep[v]) swap(u,v);
    update(1,n,1,st[u],st[v],w);
}
set<int>s[N];
set<int>::iterator it;
int getcolor(int l,int r,int o,int L,int R){
    if(!node[o].col) return 0;
    if(l==r) return node[o].col;
    int mid=(l+r)>>1;
    int ans=0;
    if(R>mid) ans=getcolor(rson,L,R);
    if(ans==0&&L<=mid) ans=getcolor(lson,L,R);
    return ans;
}
int tcolor(int u){
    while(top[u]!=top[1]){
        int ans=getcolor(1,n,1,st[top[u]],st[u]);
        if(ans) return ans;
        u=fa[top[u]];
    }
    return getcolor(1,n,1,st[1],st[u]);
}
void solve(int u,int c,int op){
    if(op==-1) s[c].erase(st[u]);
    it=lower_bound(s[c].begin(),s[c].end(),st[u]);
    int pre=0, nxt=0;
    if(it!=s[c].end()){
        nxt=lca(u,id[*it]);
    }
    if(it!=s[c].begin()){
        --it;
        pre=lca(u,id[*it]);
    }
    if(!pre&&!nxt) tupdate(1,u,op);
    else{
        if(st[pre]<st[nxt]) pre=nxt;
        tupdate(pre,u,op);
        tupdate(pre,pre,-op);
    }
    if(op==1) s[c].insert(st[u]);
}
void del(int l,int r,int o,int L,int R){
    if(!node[o].col) return;
    if(l==r){
        solve(id[l],node[o].col,-1);
        node[o].col=0; return;
    }
    int mid=(l+r)>>1;
    if(L<=mid) del(lson,L,R);
    if(R>mid) del(rson,L,R);
    node[o].col=node[o<<1].col||node[o<<1|1].col;
}
void add(int l,int r,int o,int pos,int c){
    if(l==r){
        solve(id[l],c,1); node[o].col=c;
        return;
    }
    int mid=(l+r)>>1;
    if(pos<=mid) add(lson,pos,c);
    else add(rson,pos,c);
    node[o].col=1;
}
int main(){
#ifdef gh546
freopen("a.in","r",stdin);
#endif // gh546
    int TAT; read(TAT);
    for(int cas=1;cas<=TAT;cas++){
        read(n); init();
        for(int i=1;i<=n;i++) s[i].clear();
        for(int i=1;i<n;i++){
            int u,v; read(u); read(v);
            addedge(u,v); addedge(v,u);
        }
        for(int i=1;i<=n;i++) read(col[i]);
        tim=0;
        dfs(1,-1,1); getpos(1,1);
        build(1,n,1);
        for(int i=1;i<=n;i++){
            solve(i,col[i],1);
        }
        printf("Case #%d:\n",cas);
        int q; read(q);
        while(q--){
            int op,u; read(op); read(u);
            if(op==0){
                int c; read(c);
                del(1,n,1,st[u],ed[u]);
                add(1,n,1,st[u],c);
            }
            else{
                int c=tcolor(u);
                int flag=0;
                if(c){
                    it=lower_bound(s[c].begin(),s[c].end(),st[u]);
                    if(it==s[c].end()||(*it)>ed[u]) flag=1;
                }
                printf("%d\n",query(1,n,1,st[u])+flag);
            }
        }
    }
    return 0;
}
