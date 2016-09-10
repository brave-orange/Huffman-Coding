#pragma once
#include <UIlib.h>
#include<String.h>
#include<windows.h>

using namespace DuiLib;

#ifdef _DEBUG
#   ifdef _UNICODE
#       pragma comment(lib, "DuiLib_ud.lib")
#   else
#       pragma comment(lib, "DuiLib_d.lib")
#   endif
#else
#   ifdef _UNICODE
#       pragma comment(lib, "DuiLib_u.lib")
#   else
#       pragma comment(lib, "DuiLib.lib")
#   endif
#endif
int m,s1,s2;
typedef struct{
	unsigned int weight;
	unsigned int parent,lchild,rchild;
}
HTNode,*HuffmanTree; //动态分配数组存储哈夫曼树
typedef char * HuffmanCode; //动态分配数组存储哈夫曼编码表


void Select(HuffmanTree HT,int n) 
{ 
	int i,j;
	for(i = 1;i <= n;i++)
		if(!HT[i].parent){s1 = i;break;}
		for(j = i+1;j <= n;j++)
			if(!HT[j].parent){s2 = j;break;}
			for(i = 1;i <= n;i++) 
				if((HT[s1].weight>HT[i].weight)&&(!HT[i].parent)&&(s2!=i))
					s1=i; 
				for(j = 1;j <= n;j++)  
					if((HT[s2].weight>HT[j].weight)&&(!HT[j].parent)&&(s1!=j))
						s2=j; 
};

void HuffmanCoding(HuffmanTree &HT, HuffmanCode HC[], int *w, int n)
{ 
	//w存放n个字符的权值(均>0)，构造哈夫曼树HT， 
	//并求出n个字符的哈夫曼编码HC 
	int i,j;
	char *cd;
	int p;
	int cdlen;
	if (n<=1) return;
	m=2*n-1;
	HT = (HuffmanTree)malloc((m+1) * sizeof(HTNode)); //0号单元未用 
	for (i=1;i<=n;i++)
	{
		//初始化 
		HT[i].weight=w[i-1];
		HT[i].parent=0;
		HT[i].lchild=0;
		HT[i].rchild=0;
	} 
	for (i=n+1; i<=m; i++)
	{ //初始化
		HT[i].weight=0;
		
		HT[i].parent=0;
		HT[i].lchild=0;
		HT[i].rchild=0;
	} 

	for (i=n+1; i<=m; i++ )
	{   
		//建哈夫曼树 
		//在HT[1..i-1]中选择parent为0且weight最小的两个结点， 
		//其序号分别为s1和s2。
		Select(HT,i-1);
		HT[s1].parent = i;
		HT[s2].parent = i;
		HT[i].lchild = s1;
		HT[i].rchild = s2; 
		HT[i].weight = HT[s1].weight + HT[s2].weight; 
		
	}

	//------无栈非递归遍历哈夫曼树，求哈夫曼编码
	cd = (char *)malloc(n*sizeof(char));//分配求编码的工作空间
	p = m; 
	cdlen = 0; 
	for (i=1; i<=m; ++i) //遍历哈夫曼树时用作结点状态标志
		HT[i].weight = 0; 
	while (p) { 
		if (HT[p].weight==0) 
		{ // 向左 
			HT[p].weight = 1; 
			if (HT[p].lchild != 0) 
			{
				p = HT[p].lchild; 
				cd[cdlen++] ='0'; 
			}  
			else if (HT[p].rchild == 0) 
			{ 
				//登记叶子结点的字符的编码
				HC[p] = (char *)malloc((cdlen+1) * sizeof(char)); 
				cd[cdlen] ='\0';
				strcpy(HC[p], cd); // 复制编码(串) 
			}  
		} 
		
		else if (HT[p].weight==1) 
		{ 
			// 向右 
			HT[p].weight = 2; 
			if (HT[p].rchild != 0) 
			{
				p = HT[p].rchild; cd[cdlen++] ='1'; 
			}
		}
		else
		{
			//HT[p].weight==2，退回退到父结点，编码长度减1
			HT[p].weight = 0; p = HT[p].parent;
			--cdlen;
		}
	}
	
};

class CDuiFrameWnd : public CWindowWnd, public INotifyUI
{
public:
    virtual LPCTSTR GetWindowClassName() const { return _T("DUIMainFrame"); }
    virtual void    Notify(TNotifyUI& msg) 
    {
        if(msg.sType == _T("click"))
        {
            if(msg.pSender->GetName() == _T("go")) 
            {

				CEditUI * edit1 = dynamic_cast <CEditUI*>(m_PaintManager.FindControl(_T("num")));
				char * num = (char *) edit1->GetText().GetData();
				char number[500];
				strcpy(number,num);
				int hafuman[100],a=0,j=0,q=0;
				char s[10];
				strcpy(s,"");
				for(int i=0;i<=strlen(number);i++)
				{
					char r;
					r=number[i];
					s[q]=r;
					q++;
					if(number[i]==32||number[i]=='\0')
					{
						hafuman[j]=atoi(s);
						q=0;
						j++;
						strcpy(s,"");
						
					}

					
				}
				HuffmanTree HT;
				HuffmanCode * HC;
				HC = (HuffmanCode *)malloc(j+1*sizeof(HuffmanCode));
				HuffmanCoding(HT,HC,hafuman,j+1);
				CDuiString str;
				CListUI* pList = static_cast<CListUI*>(m_PaintManager.FindControl(_T("ListDemo1")));
				
				
 
				// 添加List列表内容，必须先Add(pListElement)，再SetText
				for (int i = 1; i <= j; i++)
				{
					CListTextElementUI* pListElement = new CListTextElementUI;
					pListElement->SetTag(i);
					pList->Add(pListElement);
 
					str.Format(_T("%d"), i);
					pListElement->SetText(0, str);
					str.Format(_T("%d"), hafuman[i-1]);
					pListElement->SetText(1, str);
					str.Format(_T("%s"),HC[i]);
					pListElement->SetText(2, str);
				}
				
            }
			if(msg.pSender->GetName() == _T("minbtn"))
			{
				SendMessage(WM_SYSCOMMAND, SC_MINIMIZE, 0);
			}
			if(msg.pSender->GetName() == _T("closebtn"))
			{
				exit(0);
			}
        }
    }

    virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        LRESULT lRes = 0;

        if( uMsg == WM_CREATE ) 
        {
            m_PaintManager.Init(m_hWnd);

            CDialogBuilder builder;
            CControlUI* pRoot = builder.Create(_T("duilib.xml"), (UINT)0, NULL, &m_PaintManager);   // duilib.xml需要放到exe目录下
            ASSERT(pRoot && "Failed to parse XML");

            m_PaintManager.AttachDialog(pRoot);
            m_PaintManager.AddNotifier(this);   // 添加控件等消息响应，这样消息就会传达到duilib的消息循环，我们可以在Notify函数里做消息处理
            return lRes;
        }
        // 以下3个消息WM_NCACTIVATE、WM_NCCALCSIZE、WM_NCPAINT用于屏蔽系统标题栏
        else if( uMsg == WM_NCACTIVATE ) 
        {
            if( !::IsIconic(m_hWnd) ) 
            {
                return (wParam == 0) ? TRUE : FALSE;
            }

        }
        else if( uMsg == WM_NCCALCSIZE ) 
        {
            return 0;
        }
        else if( uMsg == WM_NCPAINT ) 
        {
            return 0;
        }

        if( m_PaintManager.MessageHandler(uMsg, wParam, lParam, lRes) ) 
        {
            return lRes;
        }

        return __super::HandleMessage(uMsg, wParam, lParam);
    }

protected:
    CPaintManagerUI m_PaintManager;
};





int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
    CPaintManagerUI::SetInstance(hInstance);
    CPaintManagerUI::SetResourcePath(CPaintManagerUI::GetInstancePath());   // 设置资源的默认路径（此处设置为和exe在同一目录）

    CDuiFrameWnd duiFrame;
    duiFrame.Create(NULL, _T("DUIWnd"), UI_WNDSTYLE_FRAME, WS_EX_WINDOWEDGE);
    duiFrame.CenterWindow();
    duiFrame.ShowModal();
    return 0;
}