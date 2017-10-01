#include<bits/stdc++.h>
using namespace std;

char str[100],term[100],teststr[100];
int sz,mxp,trans[(1<<20)][2],q0;
bool follow[20][20],vis[(1<<20)];
queue<int> q;
struct node
{
    int pos;
    char ch;
    node *left;
    node *right;
    bool nullable;
    bool firstpos[20];
    bool lastpos[20];
};

node *pofex[100],*root;

node *new_node(char dt)
{
    node *nd= new node;
    nd->left=NULL;
    nd->right=NULL;
    nd->nullable=(dt=='*'||dt=='#');
    nd->pos=-1;
    nd->ch=dt;
    int i;
    for(i=0;i<20;i++)
    {
        nd->firstpos[i]=false;
        nd->lastpos[i]=false;
    }
    return nd;
}

int priority_instack(char chk)
{
switch(chk)
{
case '.': return 5;
case '+': return 3;
case '(': return 1;
default: return 0;
}
}
int priority_instring(char chk)
{
switch(chk)
{
case '.': return 4;
case '+': return 2;
case '(': return 6;
default: return 0;
}
}

void to_postfix()
	{
	    int k=0,i,j=0;
	char stk[100],top=0;
	for(i=0;str[i]!='\0';i++)
		{
		if(str[i]=='0'||str[i]=='1'||str[i]=='*')
			{
				//pfex[k++]=str[i];
				pofex[k]=new_node(str[i]);
				if(str[i]!='*')
				{
				    pofex[k]->pos=j;
				    term[j]=str[i];
				    j++;
				}
				k++;
			}
		else
			{
			if(str[i]!=')')
					{
					while(top>0&&priority_instack(stk[top-1])>priority_instring(str[i]))
						{
							//pfex[k++]=stk[top-1];
							pofex[k++]=new_node(stk[top-1]);
						top--;
						}
					stk[top++]=str[i];
					}
			else
					{
					while(stk[top-1]!='(')
					{
					//pfex[k++]=stk[top-1];
						pofex[k++]=new_node(stk[top-1]);
						top--;
					}
					top--;
					}
			}
		}
		while(top>0)
        {
		//pfex[k++]=stk[top-1];
		pofex[k++]=new_node(stk[top-1]);
        top--;
        }
        pofex[k]=new_node('#');
        pofex[k]->pos=j;
        term[j]='#';
        j++;
        k++;
        pofex[k++]=new_node('.');
        sz=k;
        mxp=j;
}

void parsetree_gen()
{
    int k=0,i,top=0;
	node *stk[100];
    for (i = 0; i < mxp; i++)
		for (int j = 0; j < mxp; j++)
			follow[i][j] = false;

    for(i=0;i<sz;i++)
    {
    if(pofex[i]->ch=='+')
    {
        pofex[i]->right=stk[top-1];
        top--;
        pofex[i]->left=stk[top-1];
        top--;
        pofex[i]->nullable=(pofex[i]->left->nullable)||(pofex[i]->right->nullable);
        for (int j = 0; j < mxp; j++)
			{
				pofex[i]->firstpos[j] = pofex[i]->left->firstpos[j] || pofex[i]->right->firstpos[j];
				pofex[i]->lastpos[j] = pofex[i]->left->lastpos[j] || pofex[i]->right->lastpos[j];
			}

    }
    else if(pofex[i]->ch=='.')
    {
        pofex[i]->right=stk[top-1];
        top--;
        pofex[i]->left=stk[top-1];
        top--;
        pofex[i]->nullable=(pofex[i]->left->nullable)&&(pofex[i]->right->nullable);
           for (int j = 0; j < mxp; j++)
			{
			    if(pofex[i]->left->nullable)
				pofex[i]->firstpos[j] = pofex[i]->left->firstpos[j] || pofex[i]->right->firstpos[j];
				else
                pofex[i]->firstpos[j] = pofex[i]->left->firstpos[j];
				if(pofex[i]->right->nullable)
				pofex[i]->lastpos[j] = pofex[i]->left->lastpos[j] || pofex[i]->right->lastpos[j];
				else
                pofex[i]->lastpos[j] = pofex[i]->right->lastpos[j];
			}
        for (int j = 0; j < mxp; j++)
				if (pofex[i]->left->lastpos[j])
					for (int k = 0; k < mxp; k++)
						follow[j][k] = follow[j][k] || pofex[i]->right->firstpos[k];

    }
    else if(pofex[i]->ch=='0'||pofex[i]->ch=='1'||pofex[i]->ch=='#')
    {
        pofex[i]->firstpos[pofex[i]->pos]=true;
        pofex[i]->lastpos[pofex[i]->pos]=true;
    }
    else if(pofex[i]->ch=='*')
    {
        pofex[i]->left=stk[top-1];
        top--;
        for (int j = 0; j < mxp; j++)
			{
				pofex[i]->firstpos[j] = pofex[i]->left->firstpos[j];
				pofex[i]->lastpos[j] = pofex[i]->left->lastpos[j];
			}
        for (int j = 0; j < mxp; j++)
				if (pofex[i]->lastpos[j])
					for (int k = 0; k < mxp; k++)
						follow[j][k] = follow[j][k] || pofex[i]->firstpos[k];
    }
    stk[top++]=pofex[i];
    }
    root = stk[0];
}

void dfa()
{
    int i,j,val;
    for(i=0;i<(1<<mxp);i++)
    {
        trans[i][0]=0;
        trans[i][1]=0;
        vis[i]=0;
    }
    val=0;
    for(i=0;i<mxp;i++)
    {
        if(root->firstpos[i])
            {val=val|(1<<i);
            }
    }
    q0=val;
    q.push(val);
    vis[val]=1;
    while(!q.empty())
    {
    val=q.front();
    q.pop();
    for(i=0;i<mxp;i++)
    {
        if(val&(1<<i))
            {
                if(term[i]!='#')
                {
                for(j=0;j<mxp;j++)
                    if(follow[i][j])
                    trans[val][term[i]-48]|=(1<<j);
                }
            }
    }
        if(!vis[trans[val][0]])
        {
            q.push(trans[val][0]);
            vis[trans[val][0]]=1;
        }
        if(!vis[trans[val][1]])
        {
            q.push(trans[val][1]);
            vis[trans[val][1]]=1;
        }
    }

}
bool accept()
{
	int i,cur_state = q0;

	for (i = 0; teststr[i]!='\0'; i++)
		cur_state = trans[cur_state][teststr[i] - '0'];

	return (cur_state & (1 << (mxp - 1)));
}
int main()
{
    int i;
    cout<<"Enter the regular expression:\n";
    cin>>str;
    to_postfix();

    //debug
    /*cout<<"PostFix of regular expression:\n";
    for(i=0;i<sz;i++)
    cout<<pofex[i]->ch;
    cout<<"\n";*/
    parsetree_gen();
    dfa();

    cout<<"Enter a string to be tested:\n";
    cin>>teststr;
    if(accept())
    {
        cout<<"String is accepted by dfa and so is of the given regular expression";
    }
    else
        cout<<"String is not accepted by dfa and so is not of the given regular expression";
    return 0;
}
