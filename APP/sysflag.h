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

#define T_Button 0   	//��ť
#define T_THref 1    	//�ı���������
#define T_Text 2    	//�ı�
#define T_TLISTBOX 3    //������
#define T_TRect  4      //����
#define T_LBox_Text 5   //�������������

#define WIN_KEYUP 8    
#define WIN_KEYDOWN 2    	
#define WIN_KEYLEFT 4    
#define WIN_KEYRIGHT  6      

struct SListData{
	
	vu8 d_Type;    							//����С��������
	char d_SName[20];   						//��ʾʱ������
	char d_EName[20];  						//Ӣ����ʾ
	//const GUI_FONT GUI_UNI_PTR* d_Font; 	//����
	vu32 d_Color;    						//��ʾʱ����ɫ
	vu32 d_BKColor; 						//������ɫ
	int d_ID;        						//�ؼ�ID
};

struct SGraphData{
		
	int d_ID;         						//��ǰ���Ƶ�ID
	int focusType;	 						//�Ƿ�ѡ��
	int t_Type;     						//���� Ϊ�������������״
	char f_SName[20];   						//��ʾʱ������
	char f_EName[20];  						//Ӣ����ʾ
	// vu8 Width,Heigh;                        //��ռ����ĳ��ȼ��߶�
	vu32 d_FColor   , d_BKColor; 			//������ɫ   ������ɫ
	// int d_onFColor , d_onFBKColor;  		//ѡ��ʱ��������ɫ  ѡ��ʱ�ı�����ɫ
	const GUI_FONT GUI_UNI_PTR* d_Font; 	//����
	vu16 d_x, d_y, d_x1, d_y1;  				//��������	
	
};

///��ǰҳ�潹�㲼��
typedef struct winIdList
{
	int winFlag;    /*  ҳ����  */
	int Lines;      /*  ��  */
	int Columns;    /*  ��  */
	
}winIdLists;

char *show_times_char( vu32 _r );

extern int get_nextindex( int id,const int idList[], int handle );

extern int *get_id_index( struct SGraphData temp_dList[], int dataLen);

//  WM����  С���� ----  �б�����
extern void bush_WinObj(vu8 langType ,WM_MESSAGE * pMsg , struct SListData sList[], int MAX_LEN /*, const GUI_FONT * pNewFont*/);

//  WM����  С���� ----  ��������
extern void bush_SingleWin(vu8 langType ,WM_MESSAGE * pMsg ,struct SListData sData ,const GUI_FONT * pNewFont );

//  �б����
extern void bush_Graph_StringList(vu8 langType , struct SGraphData SGData[], int MAX_LEN);

//  ��������
extern void bush_Graph_String(vu8 langType , struct SGraphData SGData/* , const GUI_FONT * pNewFont*/ );

//  ��ȡ��ǰ�����˳��ֵ   ---  �����б� 
extern int *focus_FunLocation( struct SGraphData datalist[], int dlen, const int focusList[], int flen );

extern int return_vlaueNum( char temp[], int dot_loc );

/* void __cbSetButton( WM_MESSAGE * pMsg); */

extern void Write_Flash_Bytes();  //д��

extern void Read_Flash_Bytes();	 //��ȡ

extern char *Read_Data_Parse(vu8 v_f);

extern int Box_Data_Location( int k_d, char k_v[], vu8 type);

extern void OnOff_GPOI_ResetSet( vu8 _type, vu8 _value );
extern void Off_GPOI_ResetSet();

//trig����
extern void trig_power_charge();
extern void trig_load_discharge();
extern void trig_Cdc_charge();
extern void trig_R_charge();

//ɾ�����д���
extern void delteWindow(void);	

//����У׼
extern void Power_value_correct();   
extern void R_value_correct();  
extern void Load_value_correct();
extern void Cdc_value_correct();
extern void Graph_value_correct();
extern void Sysset_value_correct();
extern void Sysinfo_value_correct();

//���ְ���
extern void key_input_window(char *strx);
extern void KEY_POWER_INPUT(char *strx);
extern void KEY_IRT_INPUT(char *strx);
extern void KEY_LOAD_INPUT(char *strx);	
extern void KEY_CDC_INPUT(char *strx);	
extern void KEY_GRAPH_INPUT(char *strx);	
extern void KEY_SYSSET_INPUT(char *strx);
extern void KEY_SYSINFO_INPUT(char *strx);

//ȷ�ϼ�
extern void ent_input_win();
extern void Power_ent();
extern void R_ent();
extern void Load_ent();
extern void Cdc_ent();
extern void Graph_ent();
extern void Sysset_ent();
extern void Sysinfo_ent();

//�������Ұ��� 
extern void key_down_window( vu8 direction );
extern void KEY_DOWN_IRT( int handle );
extern void KEY_DOWN_POWER( int handle );
extern void KEY_DOWN_LOAD( int handle );
extern void KEY_DOWN_CDC( int handle );
extern void KEY_DOWN_GRAPH( int handle );
extern void KEY_DOWN_SYSSET( int handle );
extern void KEY_DOWN_SYSINFO( int handle );

//ɾ�������ַ�
extern void key_delete_input();
extern void Power_delete_input(); 
extern void R_delete_input();		
extern void Load_delete_input();
extern void Cdc_delete_input();	
extern void Graph_delete_input();	
extern void Sysset_delete_input();
extern void Sysinfo_delete_input();





















				
