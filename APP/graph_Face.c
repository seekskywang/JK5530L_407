/**
**
		曲线页面
		
*/
#include "DIALOG.h"
#include  "gui.h"
#include "my_register.h" 
#include "sysflag.h"
#include "key.h"

WM_HWIN GhWin;

extern GUI_CONST_STORAGE GUI_FONT GUI_FontHZ16;
extern GUI_CONST_STORAGE GUI_FONT GUI_FontSONG16;
extern GUI_CONST_STORAGE GUI_FONT GUI_FontSONG12;
extern GUI_CONST_STORAGE GUI_FONT GUI_FontSONGB12;
extern GUI_CONST_STORAGE GUI_FONT GUI_FontSONGB16;
extern GUI_CONST_STORAGE GUI_FONT GUI_FontHZ12;

#define ID_WINDOW_GRAPH (GUI_ID_USER + 0x80)

#define ID_BUTTON_25 (GUI_ID_USER + 0x8B)
#define ID_BUTTON_26 (GUI_ID_USER + 0x8C)
#define ID_BUTTON_27 (GUI_ID_USER + 0x8D)
#define ID_BUTTON_28 (GUI_ID_USER + 0x8E)
#define ID_BUTTON_29 (GUI_ID_USER + 0x8F)

#define ID_GRAPH_0   (GUI_ID_USER + 0x92)

#define ID_TimerTimeGH  5


static const GUI_WIDGET_CREATE_INFO _aDialogCreateGraph[] = {
	
  { WINDOW_CreateIndirect, "Window", ID_WINDOW_GRAPH,   0, 	0, 	 480,	272, 0, 0x0, 0 },
  
  { BUTTON_CreateIndirect, "Button", ID_BUTTON_25, 		85,       232, 70, 45, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "Button", ID_BUTTON_26, 		85+79*1,  232, 70, 45, 0, 0x0, 0},
  { BUTTON_CreateIndirect, "Button", ID_BUTTON_27, 		85+79*2,  232, 70, 45, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "Button", ID_BUTTON_28, 		85+79*3,  232, 70, 45, 0, 0x0, 0},
  { BUTTON_CreateIndirect, "Button", ID_BUTTON_29, 		85+79*4,  232, 70, 45, 0, 0x0, 0 },
  
  { GRAPH_CreateIndirect,   "Graph",  ID_GRAPH_0, 		100,  36,  365,	180, 0, 0x0, 0 },
	
};
	static struct SGraphData SGhListG[] = {
		
		{1,0, T_Text ,"V(V)", "V(V)", 0,0,&GUI_FontSONGB16,		10, 55,	0,	0 },
		{2,0, T_TRect ,"",	"",	0x0000FF7F,  0x0000FF7F,   0,	50, 62,	70,	64 },
		{3,0, T_Text ,"I(A)",	"I(A)",	0, 0,&GUI_FontSONGB16,	10, 85,	0,	0 },
		{4,0, T_TRect ,"",	"",	GUI_ORANGE,  GUI_ORANGE,	0,	50, 92,	70,	94 },
		
	};
 
static void _cbDialogGraph(WM_MESSAGE * pMsg) {
	
	WM_HWIN hItem;
	int NCode;
	int Id,MAX_LEN = 0;
	int i;
	char *winName;
	
	static struct SListData slistG[] = {
		
		{T_Button ,"内阻测试","I.R. TEST",  	0x00B22222 , 0,		ID_BUTTON_25 },
		{T_Button , "程控电源", "POWER",  0x00B22222 , 0, ID_BUTTON_26 },
		{T_Button ,"程控负载","LOAD",  	0x00B22222 , 0, 	ID_BUTTON_27 },
		{T_Button ,"充放电", "C&DC", 	0x00B22222 , 0, 	ID_BUTTON_28 },
		{T_Button ,"系统设置", "SYSTEM", 	0x00B22222 , 0, 	ID_BUTTON_29 },
	
	};
	
	GRAPH_SCALE_Handle hScaleH ;
	GRAPH_SCALE_Handle hScaleV ;
 	GRAPH_DATA_Handle  IData,VData;

	switch (pMsg->MsgId) {
	  	
		case WM_PAINT: {
			
			GUI_SetColor(0x00AFEEEE);

			GUI_FillRoundedRect(0,0,480,33,5);

			GUI_SetColor(GUI_BLACK);

			GUI_SetFont(&GUI_FontSONGB16);

			GUI_UC_SetEncodeUTF8();

			GUI_SetTextMode(GUI_TM_TRANS);//设置文本模式为底色透明
			
			winName = ( FLAG_LANG == 1 ) ? "GRAPH" : "曲线图" ; 

			GUI_DispStringAt( winName , (480 - GUI_GetStringDistX( winName))/2, 10);//SET
			
			for( i = 0; i < sizeof( SGhListG )/sizeof( SGhListG[0]) ; i++  ){

				bush_Graph_String( FLAG_LANG, SGhListG[i] );
			}
			
		}
		break;

		case WM_TIMER:{
			if(WM_GetTimerId(pMsg->Data.v) == ID_TimerTimeGH){	
				
				//GRAPH_DATA_YT_AddValue(VData,DISS_Voltage*10);
				//GRAPH_DATA_YT_AddValue(IData,DISS_Current*10);
				
				WM_RestartTimer(pMsg->Data.v, 500);//复位定时器数值越大刷新时间越短
						
			}	
			break;
		}
		break;	
		
		case WM_KEY:
		
		
			
			break;
			
		case WM_INIT_DIALOG:{
				
			hItem = pMsg->hWin;
			
			WINDOW_SetBkColor(hItem, 0x004A4655);

			WM_CreateTimer( hItem ,ID_TimerTimeGH , 20 , 0);//创建本窗口定时器
			
			MAX_LEN = sizeof( slistG )/sizeof( slistG[0] );

			bush_WinObj( FLAG_LANG, pMsg ,slistG, MAX_LEN);

			hItem = WM_GetDialogItem(pMsg->hWin, ID_GRAPH_0);

			GRAPH_SetBorder(hItem,23, 5, 5, 10);  //设置边框的尺寸
			
			hScaleV = GRAPH_SCALE_Create(20, GUI_TA_RIGHT, GRAPH_SCALE_CF_VERTICAL, 20);
			
			//设置标签字体颜色
			GRAPH_SCALE_SetTextColor(hScaleV,GUI_RED);
				
			//将标签添加到垂直方向
			GRAPH_AttachScale( hItem, hScaleV); 
			
			//创建和增加水平范围尺度标
			hScaleH = GRAPH_SCALE_Create(173, GUI_TA_HCENTER , GRAPH_SCALE_CF_HORIZONTAL, 50);
			
			GRAPH_SetGridDistY(hItem,20);
			//GRAPH_SetGridFixedX(hItem,25);

			//设置字体颜色
			GRAPH_SCALE_SetTextColor( hScaleH,GUI_DARKGREEN); 

			GRAPH_SCALE_SetFactor(hScaleV, 0.1);
			//添加到水平方向
			GRAPH_AttachScale( hItem, hScaleH );
			
			VData = GRAPH_DATA_YT_Create(GUI_GREEN, 500, 0, 0);//

			IData = GRAPH_DATA_YT_Create(GUI_RED, 500, 0, 0);

			GRAPH_AttachData(hItem,VData);

			GRAPH_AttachData(hItem,IData);	//为绘图控件添加数据对象	

			GRAPH_SetGridVis(hItem,1);    //启用网格绘制  1/0
		}
		break;
    default:

			WM_DefaultProc(pMsg);
		break;
	 }
}



WM_HWIN CreateWGraph(void);
WM_HWIN CreateWGraph(void) {

	GhWin = GUI_CreateDialogBox(_aDialogCreateGraph, GUI_COUNTOF(_aDialogCreateGraph), _cbDialogGraph, WM_HBKWIN, 0, 0);
	
	return GhWin;	
}

void KEY_GRAPH_INPUT(char *strx){
	
	
}

void KEY_DOWN_GRAPH(int handle )
{


}

void Graph_ent( )
{
	if( FLAG_INPUTAUTH ){
		
		FLAG_INPUTAUTH = 0;
		Graph_value_correct();
		
	}else{

		FLAG_INPUTAUTH = 1;
	}
	
}

void Graph_delete_input( )
{


}

void Graph_value_correct( )
{


}






