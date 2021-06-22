#ifndef _SYSFLAG_H_
#define _SYSFLAG_H_
#endif

#include  "gui.h"
#include "my_register.h" 
#include "key.h"
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define T_Button 0   	//按钮
#define T_THref 1    	//文本带框链接
#define T_Text 2    	//文本
#define T_TLISTBOX 3    //下拉框
#define T_TRect  4      //矩形
#define T_LBox_Text 5   //下拉框里的链接

#define WIN_KEYUP 8    
#define WIN_KEYDOWN 2    	
#define WIN_KEYLEFT 4    
#define WIN_KEYRIGHT  6      

struct SListData{
	
	vu8 d_Type;    							//窗口小工具类型
	char d_SName[20];   						//显示时的文字
	char d_EName[20];  						//英文显示
	//const GUI_FONT GUI_UNI_PTR* d_Font; 	//字体
	vu32 d_Color;    						//显示时的颜色
	vu32 d_BKColor; 						//背景颜色
	int d_ID;        						//控件ID
};

struct SGraphData{
		
	int d_ID;         						//当前绘制的ID
	int focusType;	 						//是否选中
	int t_Type;     						//类型 为后续添加其他形状
	char f_SName[20];   						//显示时的文字
	char f_EName[20];  						//英文显示
	// vu8 Width,Heigh;                        //所占区域的长度及高度
	vu32 d_FColor   , d_BKColor; 			//字体颜色   背景颜色
	// int d_onFColor , d_onFBKColor;  		//选中时的字体颜色  选中时的背景颜色
	const GUI_FONT GUI_UNI_PTR* d_Font; 	//字体
	vu16 d_x, d_y, d_x1, d_y1;  				//矩形坐标	
	
};

///当前页面焦点布局
typedef struct winIdList
{
	int winFlag;    /*  页面标记  */
	int Lines;      /*  行  */
	int Columns;    /*  列  */
	
}winIdLists;

char *show_times_char( vu32 _r );

extern int get_nextindex( int id,const int idList[], int handle );

extern int *get_id_index( struct SGraphData temp_dList[], int dataLen);

//  WM对象  小工具 ----  列表化绘制
extern void bush_WinObj(vu8 langType ,WM_MESSAGE * pMsg , struct SListData sList[], int MAX_LEN /*, const GUI_FONT * pNewFont*/);

//  WM对象  小工具 ----  单个绘制
extern void bush_SingleWin(vu8 langType ,WM_MESSAGE * pMsg ,struct SListData sData ,const GUI_FONT * pNewFont );

//  列表绘制
extern void bush_Graph_StringList(vu8 langType , struct SGraphData SGData[], int MAX_LEN);

//  单个绘制
extern void bush_Graph_String(vu8 langType , struct SGraphData SGData/* , const GUI_FONT * pNewFont*/ );

//  获取当前焦点的顺序值   ---  数据列表 
extern int *focus_FunLocation( struct SGraphData datalist[], int dlen, const int focusList[], int flen );

extern int return_vlaueNum( char temp[], int dot_loc );

/* void __cbSetButton( WM_MESSAGE * pMsg); */

extern void Write_Flash_Bytes();  //写入

extern void Read_Flash_Bytes();	 //读取

extern char *Read_Data_Parse(vu8 v_f);

extern int Box_Data_Location( int k_d, char k_v[], vu8 type);

extern void OnOff_GPOI_ResetSet( vu8 _type, vu8 _value );
extern void Off_GPOI_ResetSet();

//trig按键
extern void trig_power_charge();
extern void trig_load_discharge();
extern void trig_Cdc_charge();
extern void trig_R_charge();

//删除所有窗体
extern void delteWindow(void);	

//数据校准
extern void Power_value_correct();   
extern void R_value_correct();  
extern void Load_value_correct();
extern void Cdc_value_correct();
extern void Graph_value_correct();
extern void Sysset_value_correct();
extern void Sysinfo_value_correct();

//数字按键
extern void key_input_window(char *strx);
extern void KEY_POWER_INPUT(char *strx);
extern void KEY_IRT_INPUT(char *strx);
extern void KEY_LOAD_INPUT(char *strx);	
extern void KEY_CDC_INPUT(char *strx);	
extern void KEY_GRAPH_INPUT(char *strx);	
extern void KEY_SYSSET_INPUT(char *strx);
extern void KEY_SYSINFO_INPUT(char *strx);

//确认键
extern void ent_input_win();
extern void Power_ent();
extern void R_ent();
extern void Load_ent();
extern void Cdc_ent();
extern void Graph_ent();
extern void Sysset_ent();
extern void Sysinfo_ent();

//上下左右按键 
extern void key_down_window( vu8 direction );
extern void KEY_DOWN_IRT( int handle );
extern void KEY_DOWN_POWER( int handle );
extern void KEY_DOWN_LOAD( int handle );
extern void KEY_DOWN_CDC( int handle );
extern void KEY_DOWN_GRAPH( int handle );
extern void KEY_DOWN_SYSSET( int handle );
extern void KEY_DOWN_SYSINFO( int handle );

//删除输入字符
extern void key_delete_input();
extern void Power_delete_input(); 
extern void R_delete_input();		
extern void Load_delete_input();
extern void Cdc_delete_input();	
extern void Graph_delete_input();	
extern void Sysset_delete_input();
extern void Sysinfo_delete_input();





















				
