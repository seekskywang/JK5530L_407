
#include "my_register.h" 
#include "key.h"
#include  "gui.h"
#include "MainTask.h"
#include "sysflag.h"

//CorrectList CLists; 

vu8 FLAG_FIRSTWIN[8];  //确认是否第一次加载当前页面，是则加载保存的值，不是则加载变量

extern GUI_CONST_STORAGE GUI_FONT GUI_FontHZ16;
extern GUI_CONST_STORAGE GUI_FONT GUI_FontSONG16;
extern GUI_CONST_STORAGE GUI_FONT GUI_FontSONG12;
extern GUI_CONST_STORAGE GUI_FONT GUI_FontSONGB12;
extern GUI_CONST_STORAGE GUI_FONT GUI_FontSONGB16;
extern GUI_CONST_STORAGE GUI_FONT GUI_FontHZ12;

char displayStr[20];  // 存储 输入的值

void Write_Flash_Bytes(){
	
	int return_flag;
	
	return_flag = WriteBytes( &CalPara, 0, sizeof(CalPara));
	
}

void Read_Flash_Bytes(){
	
	int return_flag;
	
	return_flag = ReadBytes( &CalPara, 0, sizeof(CalPara));

}

void Write_Boot_Flag(void)
{
	int return_flag;
	
	return_flag = WriteBytes( &bootflag, sizeof(CalPara), 1);
}

void Read_Boot_Flag(void)
{
	int return_flag;
	
	return_flag = ReadBytes( &bootflag, sizeof(CalPara), 1);
}
////   每个页面 焦点行数、列数
//static const winIdLists winList[] = {

//		{0, 2, 1},
//		{1, 4, 1},
//		{2, 2, 1},
//		{3, 7, 1},
//		{4, 0, 0},
//		{5, 7, 2},
//		{6, 2, 3},

//};

////  获取当前焦点的顺序值  
//int *focus_FunLocation( struct SGraphData dList[], int dlen, const int fList[], int flen ){

//	int i, j;
//	int focus_loc[] = { 1024, 1024 };   //焦点  1. 数据列表中的位置   2. 焦点列表中的位置
//	
//	for( i = 0; i < dlen; i++ )
//	{
//		if( dList[i].focusType == 1 && (dList[i].t_Type == T_THref || dList[i].t_Type == T_TLISTBOX) )
//		{	
//			for( j = 0; j < flen; j++)
//			{		
//				if( fList[j] == dList[i].d_ID )
//				{
//						
//					focus_loc[0] = i;  		//存在焦点  获取在数据列表中的排序
//						
//					focus_loc[1] = j;   	//          获取在焦点列表中的排序
//						
//					goto focus_break;
//					
//				}
//				else
//					
//					focus_loc[1] = 404; 	//存在焦点  焦点列表中不存在	
//			}
//		}
//		else
//				
//			focus_loc[0] = 404;   			//数据列表中无焦点
//	}
//	focus_break:
//	
//		return focus_loc;
//}

///** 
//*  注：索引id 不能为 0X194。
//*
//* @brief  获取当前id、索引
//* @param  struct SGraphData *temp_dList：  需要获取焦点的数据列表
//* @retval id_index:  当前移动后数据的id & 索引
//*
//* 创建日期： 2019-12-02
//* 修改日期：
//*
//*/
//int *get_id_index( struct SGraphData temp_dList[], int dataLen)
//{
//	int id_index[] = { 0X194 , 0 };
//	
//	int i = 0;

//	for( i = 0; i < dataLen; i++ )
//	{
//		if( ( temp_dList[i].focusType == 1 ) && ( temp_dList[i].t_Type == T_THref || temp_dList[i].t_Type == T_TLISTBOX ) )
//		{	
//			id_index[0] = temp_dList[i].d_ID;	
//			id_index[1] = i;
//		}		
//	}

//	return id_index;
//}

///** 
//*  注：
//*
//* @brief  给SGraphData对象设置值。
//* @param  _type:     传递的值类型。
//* @param  _old_Data: 传递的SGraphData对象。
//* @param  _t_data:   设置的值。
//* @retval _set_Data: 返回的SGraphData对象。
//*
//* 创建日期： 2019-12-10 10:07
//* 修改日期： 
//*
//*/

//struct SGraphData set_sgraphdata( char _type, struct SGraphData *_old_Data,void *_t_data )
//{
//	struct SGraphData _set_Data;
//	
//	switch(_type)
//	{
//		case 'A':
//			
//		break;
//		case 'B':
//			
//		
//		break;
//	}
//	
//	
//	return _set_Data;
//}

///** 
//*  注：id 不能为 0XFF 及 0。
//*
//* @brief  获取移动后的下一个焦点
//* @param  int Llen：几行（可略去）
//* @param  int Clen：几列（可略去）
//* @param  int handle：操作  	 8 - 上   2 - 下    4 - 左   6 - 右
//* @retval next_id:  当前移动后数据的id
//*
//* 创建日期： 2019-12-02
//* 修改日期： 2019/12/10 10:50
//*
//*/
//int get_nextindex( int id, const int idList[], int handle )
//{
//	int i = 0, x = 0 ,y = 0, old_x = 0;
//	
//	int next_id = 0XFF;    
//	
//	int isWin = 0, isNumNull = 0, winIndex = 0; 
//	
//	int _c = 0, _l = 0;

//	int dlen = sizeof(winList)/sizeof(winList[0]);
//	
//	for( i = 0; i < dlen; i++ )
//	{
//		if( FLAG_WIN == winList[i].winFlag)
//		{	
//			isWin = 1;	
//			
//			winIndex = i; 			
//		}	
//	}	
//	if( isWin )
//	{
//		i = 0;
//		
//		_c = winList[winIndex].Columns; //列数
//		
//		_l = winList[winIndex].Lines;  //行数
//		
//		while(1)
//		{
//			if( id == idList[i] )
//				
//				break;
//			
//			y++;
//			i++;
//			
//			if( y == _c )
//			{				
//				y = 0;
//				x++;
//			}	
//			if( x == _l )
//			{
//				x = 0XFF;
//				y = 0XFF;
//				break;
//			}	
//		}
//		if( x == 0XFF || y == 0XFF )
//			
//			return next_id;
//		
//		i = 0;
//		while(1)
//		{
//			if( ( i / _c ) < _l && ( handle == 8 || handle == 2 ) )
//			{	
//				if( i % _c == 0 )

//					isNumNull += ( idList[ ( (i / _c) * _c ) + y ] ) == 0 ? 1 : 0;
//				
//			}
//			if( handle == 4 || handle == 6 )
//			
//				isNumNull += idList[i] == 0 ? 1 : 0;
//			
//			i++;
//			
//			if( ( i > ( _l - 1 ) * _c + y ) && ( handle == 8 || handle == 2 )  )
//			{
//				if( isNumNull >= _l - 1 )
//					
//					return id;
//				else	
//					
//					break;
//			}
//			if( isNumNull >= ( _l - 1 ) * _c + _c - 1 )
//				
//				return id;
//			
//			else
//				
//				break;
//			
//		}
//		while(1)
//		{
//			if( handle == 8 )
//			{
//				while( 1 )
//				{
//					if( x == 0)
//					
//						x = _l - 1;
//							
//					else
//							
//						x--;
//					
//					if( idList[x * _c + y] != 0  )		break;		
//				}
//				
//			}
//			if( handle == 2 )
//			{	
//				while( 1 )
//				{
//					if( x == _l - 1 )
//						
//						x = 0;
//					
//					else
//							
//						x++;
//					
//					if( idList[x * _c + y] != 0  )		break;
//				}	
//			}	
//			if( handle == 4 )
//			{
//				old_x = x; //存储原始行数值
//				
//				while( 1 )
//				{	
//					if( y == 0 )
//					{		
//						x--;	
//							
//						y = _c - 1;
//					}		
//					else	
//							
//						y--;
//							
//					if( y == _c - 1 && idList[x * _c + y] != 0)  //换行后判断数值  
//									
//						break;
//							
//					else     //为零  则更改行值
//								
//						old_x = x;
//						
//					if( y != _c - 1 && x == old_x &&  idList[x * _c + y] != idList[x * _c + y + 1] && idList[x * _c + y] != 0  )   //同一行判断数值
//								
//						break;		
//				}
//			}	
//			if( handle == 6 )
//			{
//				old_x = x;

//				while( 1 )
//				{	
//					if( y == _c - 1)
//					{	
//						x++;
//						
//						y = 0;
//					}
//					else
//						y++;
//					
//					if( y == 0 && idList[x * _c + y] != 0)  //换行后判断数值  
//									
//						break;
//							
//					else     //为零  则更改行值
//								
//						old_x = x;
//						
//					if( y != 0 && x == old_x &&  idList[x * _c + y] != idList[x * _c + y - 1] && idList[x * _c + y] != 0  )   //同一行判断数值
//								
//						break;		
//				}
//			}	
//			if( idList[ x  * _c + y] != 0xFF || idList[x * _c + y] == id || idList[x * _c + y] != 0 )	
//				
//				return idList[ x * _c + y];
//		}
//	}
//	return next_id;
//}

////列表绘制小工具
//void bush_WinObj(vu8 langType ,WM_MESSAGE * pMsg ,struct SListData sList[], int MAX_LEN  ){
//	
//	int i ;
//	WM_HWIN hItem;
//	
//	for( i = 0 ; i < MAX_LEN ; i++ )
//		
//		bush_SingleWin( langType, pMsg , sList[i] , 0x0 );

//}

////单个绘制小工具
//void bush_SingleWin(vu8 langType ,WM_MESSAGE * pMsg ,struct SListData sData , const GUI_FONT * pNewFont){
//	
//	WM_HWIN hItem;
//	
//	char* W_FStr;
//	
//	hItem = WM_GetDialogItem ( pMsg->hWin , sData.d_ID ) ;
//	
//	GUI_UC_SetEncodeUTF8();
//	
//	W_FStr = ( langType == 0 ) ? sData.d_SName : sData.d_EName;
//	
//	switch( sData.d_Type ){
//		
//		case T_Button:
//		
//			BUTTON_SetBkColor(hItem ,0, 0x00f33333);
//			if( sData.d_BKColor != 0x0 ) 	  BUTTON_SetBkColor(hItem ,0, sData.d_BKColor);
//			
//			BUTTON_SetTextColor(hItem , 0,0x00B22222);
//			if( sData.d_Color   != 0x0 )  	BUTTON_SetTextColor(hItem, 0, sData.d_Color);
//			
//			BUTTON_SetFont(hItem, &GUI_FontSONG12);
//			if( pNewFont        != 0x0 )  	BUTTON_SetFont(hItem, pNewFont);
//			
//			BUTTON_SetText( hItem , W_FStr );
//			
//		break;
//		
//		case T_Text:
//		
//			TEXT_SetBkColor( hItem ,  0x00FFF8DC);
//			if( sData.d_BKColor != 0x0 ) 	TEXT_SetBkColor(hItem , sData.d_BKColor);
//			
//			TEXT_SetTextColor(hItem , 0x00ffffff);
//			if( sData.d_Color   != 0x0 )  	TEXT_SetTextColor(hItem, sData.d_Color);
//			
//			TEXT_SetFont(hItem, &GUI_FontSONG16);
//			if( pNewFont        != 0x0 )  	TEXT_SetFont(hItem, pNewFont);
//			
//			TEXT_SetTextAlign( hItem, TEXT_CF_LEFT | TEXT_CF_VCENTER );
//			
//			TEXT_SetText(hItem, W_FStr);
//			
//		break;
//	}
//}

////列表绘制矩形+文本
//void bush_Graph_StringList(vu8 langType ,struct SGraphData SGList[], int MAX_LEN ){
//	
//	int i ;
//	
//	WM_HWIN hItem;
//	
//	for( i = 0 ; i < MAX_LEN ; i++ )

//		bush_Graph_String( langType , SGList[i] );
//}

////绘制矩形+文本        当前语言     数据      字体
//void bush_Graph_String(vu8 langType , struct SGraphData SGData ){	

//	int fx;
//	char* G_FStr;
//	
//	switch( SGData.t_Type ){
//		
//		case T_Button:  
//		
//		break;
//		case T_TLISTBOX:
//			//构图思路
//			//如果为下拉框  则需要下拉数据  设置ID与列表ID对应 
//		    //包含ID，是否得到焦点，中文显示，英文显示 ，
//			//列表数据所占的框
//			//列表数据获取焦点
//		case T_LBox_Text:
//			
//		case T_THref:
//		
//			if( SGData.focusType ){    //  1  ---  点击		
//			
//				GUI_SetColor( 0x00FDF3E6 );
//				
//				if( SGData.d_BKColor !=     0x0 ) 	    GUI_SetColor(SGData.d_BKColor);
//				
//				GUI_FillRect(SGData.d_x, SGData.d_y, SGData.d_x1, SGData.d_y1);   //绘制矩形
//				
//				GUI_SetColor( 0x00000000 );
//				 		
//			}else{
//				
//				GUI_SetColor( 0x004A4655 );
//				
//				if( SGData.d_BKColor !=      0x0 ) 		GUI_SetColor(SGData.d_BKColor);
//				
//				GUI_FillRect(SGData.d_x, SGData.d_y, SGData.d_x1, SGData.d_y1);   //绘制矩形
//				
//				GUI_SetColor( 0x00FFFFFF );  
//			}
//			if( SGData.d_FColor      !=      0x0 )      GUI_SetColor(SGData.d_FColor); 
//			
//			GUI_SetTextMode(GUI_TM_TRANS);  //透明   GUI_TEXTMODE_NORMAL   //正常
//			
//			GUI_UC_SetEncodeUTF8();
//			
//			GUI_SetFont(&GUI_FontSONG16); 
//			
//			if( SGData.d_Font        !=      0x0  )		GUI_SetFont( SGData.d_Font );
//			
//			fx = (SGData.d_y1 - SGData.d_y  - GUI_GetFontSizeY()) / 2 ;
//			
//			if( fx <= 0 )     fx = 0 ;
//			
//			G_FStr = ( langType == 0 ) ? SGData.f_SName : SGData.f_EName; 
//			
//			GUI_DispStringAt(G_FStr, SGData.d_x, SGData.d_y + fx);	
//			
//		break;

//		case T_Text:
//		
//			GUI_SetColor( 0x00ffffff ); 
//			
//			if( SGData.d_FColor      !=      0x0 )    GUI_SetColor( SGData.d_FColor );  
//			
//			GUI_SetTextMode(GUI_TM_TRANS); 
//			
//			GUI_UC_SetEncodeUTF8();
//			
//			GUI_SetFont(&GUI_FontSONG16); 
//			
//			if( SGData.d_Font        !=      0x0  )		GUI_SetFont( SGData.d_Font );
//			
//			G_FStr = ( langType == 0 ) ? SGData.f_SName : SGData.f_EName; 
//			
//			GUI_DispStringAtCEOL( G_FStr, SGData.d_x, SGData.d_y );  // 覆盖已经存在的

//		break;		
//			
//		
//		case T_TRect:
//		
//			GUI_SetColor(0x00ffffff);
//			
//			if( SGData.d_BKColor != 0x0 ) 	GUI_SetColor(SGData.d_BKColor);
//			
//			GUI_FillRect(SGData.d_x, SGData.d_y, SGData.d_x1, SGData.d_y1);   //绘制矩形
//		
//		break;
//	}	
//}



//char *Read_Data_Parse(vu8 v_f){
//	
//	char return_char[10], dTemp_Char[10], iTemp_Char[10];
//	
//	int  dot_loc = 1024, i = 0 ; 

//	double dTemp;
//	
//	memset( return_char, '\0', sizeof(return_char));
//	
//	memset( dTemp_Char, '\0', sizeof(dTemp_Char));
//	
//	memset( iTemp_Char, '\0', sizeof(iTemp_Char));
//	
//	switch( FLAG_WIN ){
//		
//		case 0:{
//			
//			switch( v_f ){
//				//====================================输出电压======================================
//				case 1:   
//				
//					dTemp = (double)CLists.POWER_OutPut_V / 1000;
//					
//					sprintf( iTemp_Char, "%f", dTemp );
//					
//					strncpy( dTemp_Char, iTemp_Char , 6  );  					//先截取五位

//					dot_loc = dot_Loc_isNull( dTemp_Char,strlen(dTemp_Char) );	//获取小数点位置
//					
//					if( dot_loc == 1024 ){
//										
//					}else{
//						
//						if( dot_loc < 2 ){		
//							
//							if( dTemp_Char[5] == '0' ){								
//								
//								 dTemp_Char[5] = '\0';								
//							}	 					
//						}	
//					}
//					
//				break;
//				//====================================限制电流======================================
//				case 2:
//				
//					dTemp = (double)CLists.POWER_Limit_C / 1000;
//					
//					sprintf( iTemp_Char, "%f", dTemp );
//					
//					strncpy( dTemp_Char, iTemp_Char, 6  );  					//先截取五位

//					dot_loc = dot_Loc_isNull( dTemp_Char,strlen(dTemp_Char) );	//获取小数点位置
//					
//					if( dot_loc == 1024 ){
//						
//					}else{
//						
//						if( dot_loc < 2 ){		
//								
//							if( dTemp_Char[5] == '0' ){								
//								
//								 dTemp_Char[5] = '\0';								
//							}	 						
//						}	
//					}
//					
//				break;
//			}
//		
//		}
//			
//		break;
//		
//		case 1:{
//			
//			switch( v_f ){
//				//====================================起始电流======================================
//				case 1:   
//				
//					dTemp = (double)CLists.IR_Start_C / 1000;
//					
//					sprintf( iTemp_Char, "%f", dTemp );
//					
//					strncpy( dTemp_Char, iTemp_Char , 6  );  					//先截取五位

//					dot_loc = dot_Loc_isNull( dTemp_Char,strlen(dTemp_Char) );	//获取小数点位置
//					
//					if( dot_loc == 1024 ){
//								
//					}else{
//						
//						if( dot_loc < 2 ){		
//							
//							if( dTemp_Char[5] == '0' ){								
//								
//								 dTemp_Char[5] = '\0';								
//							}							
//						}	
//					}
//					
//				break;
//				//====================================步进电流======================================
//				case 2:
//				
//					dTemp = (double)CLists.IR_Step_C / 1000;
//					
//					sprintf( iTemp_Char, "%f", dTemp );
//					
//					strncpy( dTemp_Char, iTemp_Char, 6 );  					//先截取五位

//					dot_loc = dot_Loc_isNull( dTemp_Char,strlen(dTemp_Char) );	//获取小数点位置
//					
//					if( dot_loc == 1024 ){
//						
//					}else{
//						
//						if( dot_loc < 2 ){		
//								
//							if( dTemp_Char[5] == '0'  ){								
//								
//								dTemp_Char[5] = '\0';								
//							}							
//						}	
//					}
//					
//				break;
//				//====================================门槛电压======================================
//				case 3:
//				
//					dTemp =  (double)CLists.IR_Threshold_V / 1000 ;
//					
//					sprintf( iTemp_Char, "%f", dTemp );
//					
//					strncpy( dTemp_Char, iTemp_Char, 6  );  					//先截取五位

//					dot_loc = dot_Loc_isNull( dTemp_Char,strlen(dTemp_Char) );	//获取小数点位置
//					
//					if( dot_loc == 1024 ){
//						
//					}else{
//						
//						if( dot_loc < 2 ){		
//								
//							if( dTemp_Char[5] == '0'  ){								
//								
//								dTemp_Char[5] = '\0';						
//							}							
//						}	
//					}
//					
//				break;
//				//====================================步进时间======================================
//				case 4:			
//					
//					sprintf( iTemp_Char, "%d", CLists.IR_Step_Time );
//					
//					if( strlen(iTemp_Char) == 1 && iTemp_Char[0] == '0' ){
//						
//						strcpy( dTemp_Char, "0000" );
//						
//						break;			
//					}	
//					for( i = 0; i < 4 - strlen(iTemp_Char); i++ ){
//						
//						strcat( dTemp_Char, "0");	
//						
//					}			
//					strcat( dTemp_Char, iTemp_Char ); 
//				
//				break;	
//			}	
//		
//		}
//		
//		break;
//		
//		case 2:{
//			
//			switch( v_f ){
//				//====================================负载电流======================================
//				case 1:   
//				
//					dTemp = (double)CLists.LOAD_C / 1000;
//					
//					sprintf( iTemp_Char, "%f", dTemp );
//					
//					strncpy( dTemp_Char, iTemp_Char , 6  );  					//先截取五位

//					dot_loc = dot_Loc_isNull( dTemp_Char,strlen(dTemp_Char) );	//获取小数点位置
//					
//					if( dot_loc == 1024 ){
//									
//					}else{
//						
//						if( dot_loc < 2 ){		
//							
//							if( dTemp_Char[5] == '0' ){								
//								
//								 dTemp_Char[5] = '\0';								
//							}							
//						}	
//					}
//					
//				break;
//				
//				//====================================负载电流======================================
//				case 2:   
//				
//					dTemp = (double)CLists.LOAD_V / 1000;
//					
//					sprintf( iTemp_Char, "%f", dTemp );
//					
//					strncpy( dTemp_Char, iTemp_Char , 6  );  					//先截取五位

//					dot_loc = dot_Loc_isNull( dTemp_Char,strlen(dTemp_Char) );	//获取小数点位置
//					
//					if( dot_loc == 1024 ){
//									
//					}else{
//						
//						if( dot_loc < 2 ){		
//							
//							if( dTemp_Char[5] == '0' ){								
//								
//								 dTemp_Char[5] = '\0';								
//							}							
//						}	
//					}
//					
//				break;
//			}	
//		}
//		
//		break;
//		
//		case 3:{
//			
//			switch( v_f ){
//				//====================================输出电压======================================
//				case 1:   
//				
//					dTemp = (double)CLists.CDC_OutPut_V / 1000;
//					
//					sprintf( iTemp_Char, "%f", dTemp );
//					
//					strncpy( dTemp_Char, iTemp_Char , 6  );  					//先截取五位

//					dot_loc = dot_Loc_isNull( dTemp_Char,strlen(dTemp_Char) );	//获取小数点位置
//					
//					if( dot_loc == 1024 ){
//									
//					}else{
//						
//						if( dot_loc < 2 ){		
//							
//							if( dTemp_Char[5] == '0' ){								
//								
//								 dTemp_Char[5] = '\0';								
//							}							
//						}	
//					}	
//					
//				break;
//				//====================================限制电流======================================
//				case 2:   
//				
//					dTemp = (double)CLists.CDC_Limit_C / 1000;
//					
//					sprintf( iTemp_Char, "%f", dTemp );
//					
//					strncpy( dTemp_Char, iTemp_Char , 6  );  					//先截取五位

//					dot_loc = dot_Loc_isNull( dTemp_Char,strlen(dTemp_Char) );	//获取小数点位置
//					
//					if( dot_loc == 1024 ){
//									
//					}else{
//						
//						if( dot_loc < 2 ){		
//							
//							if( dTemp_Char[5] == '0' ){								
//								
//								 dTemp_Char[5] = '\0';								
//							}							
//						}	
//					}	
//					
//				break;
//				//====================================截止电压======================================
//				case 3:   
//				
//					dTemp = (double)CLists.CDC_Ccutoff_V / 1000;
//					
//					sprintf( iTemp_Char, "%f", dTemp );
//					
//					strncpy( dTemp_Char, iTemp_Char , 6  );  					//先截取五位

//					dot_loc = dot_Loc_isNull( dTemp_Char,strlen(dTemp_Char) );	//获取小数点位置
//					
//					if( dot_loc == 1024 ){
//									
//					}else{
//						
//						if( dot_loc < 2 ){		
//							
//							if( dTemp_Char[5] == '0' ){								
//								
//								 dTemp_Char[5] = '\0';								
//							}							
//						}	
//					}	
//					
//				break;
//				//====================================截止电流======================================
//				case 4:   
//				
//					dTemp = (double)CLists.CDC_Ccutoff_C / 1000;
//					
//					sprintf( iTemp_Char, "%f", dTemp );
//					
//					strncpy( dTemp_Char, iTemp_Char , 6  );  					//先截取五位

//					dot_loc = dot_Loc_isNull( dTemp_Char,strlen(dTemp_Char) );	//获取小数点位置
//					
//					if( dot_loc == 1024 ){
//									
//					}else{
//						
//						if( dot_loc < 2 ){		
//							
//							if( dTemp_Char[5] == '0' ){								
//								
//								 dTemp_Char[5] = '\0';								
//							}							
//						}	
//					}	
//					
//				break;
//				//====================================负载电流======================================
//				case 5:   
//				
//					dTemp = (double)CLists.CDC_Load_C / 1000;
//					
//					sprintf( iTemp_Char, "%f", dTemp );
//					
//					strncpy( dTemp_Char, iTemp_Char , 6  );  					//先截取五位

//					dot_loc = dot_Loc_isNull( dTemp_Char,strlen(dTemp_Char) );	//获取小数点位置
//					
//					if( dot_loc == 1024 ){
//									
//					}else{
//						
//						if( dot_loc < 2 ){		
//							
//							if( dTemp_Char[5] == '0' ){								
//								
//								 dTemp_Char[5] = '\0';								
//							}							
//						}	
//					}	
//					
//				break;
//				//====================================截止电压======================================
//				case 6:   
//				
//					dTemp = (double)CLists.CDC_Dcutoff_V / 1000;
//					
//					sprintf( iTemp_Char, "%f", dTemp );
//					
//					strncpy( dTemp_Char, iTemp_Char , 6  );  					//先截取五位

//					dot_loc = dot_Loc_isNull( dTemp_Char,strlen(dTemp_Char) );	//获取小数点位置
//					
//					if( dot_loc == 1024 ){
//									
//					}else{
//						
//						if( dot_loc < 2 ){		
//							
//							if( dTemp_Char[5] == '0' ){								
//								
//								 dTemp_Char[5] = '\0';								
//							}							
//						}	
//					}	
//					
//				break;
//				//====================================循环次数======================================
//				case 7:   
//				
//					sprintf( dTemp_Char, "%d", CLists.CDC_Cycle_Time );
//					
//				break;
//			}	
//		
//		}
//		
//		break;
//		
//		case 4:{
//				
//		}
//		
//		break;
//		
//		case 5:{
//				
//			switch( v_f ){
//				//====================================分选开关======================================
//				case 1:   
//				
//					sprintf( dTemp_Char, "%d", CLists.SYS_Comparator );
//					
//				break;
//				//====================================过流测试======================================
//				case 2:   
//				
//					sprintf( dTemp_Char, "%d", CLists.SYS_Overflow );
//					
//				break;
//				//====================================报警声音======================================
//				case 3:   
//				
//					sprintf( dTemp_Char, "%d", CLists.SYS_Beep );
//					
//				break;
//				//====================================电压上限======================================
//				case 4:   
//				
//					dTemp = (double)CLists.SYS_UL_V / 1000;
//					
//					sprintf( iTemp_Char, "%f", dTemp );
//					
//					strncpy( dTemp_Char, iTemp_Char , 6  );  					//先截取五位

//					dot_loc = dot_Loc_isNull( dTemp_Char,strlen(dTemp_Char) );	//获取小数点位置
//					
//					if( dot_loc == 1024 ){
//									
//					}else{
//						
//						if( dot_loc < 2 ){		
//							
//							if( dTemp_Char[5] == '0' ){								
//								
//								 dTemp_Char[5] = '\0';								
//							}							
//						}	
//					}	
//					
//				break;
//				//====================================电压下限======================================
//				case 5:   
//				
//					dTemp = (double)CLists.SYS_LL_V / 1000;
//					
//					sprintf( iTemp_Char, "%f", dTemp );
//					
//					strncpy( dTemp_Char, iTemp_Char , 6  );  					//先截取五位

//					dot_loc = dot_Loc_isNull( dTemp_Char,strlen(dTemp_Char) );	//获取小数点位置
//					
//					if( dot_loc == 1024 ){
//									
//					}else{
//						
//						if( dot_loc < 2 ){		
//							
//							if( dTemp_Char[5] == '0' ){								
//								
//								 dTemp_Char[5] = '\0';								
//							}							
//						}	
//					}	
//					
//				break;
//				//====================================内阻上限======================================
//				case 6:   
//				
//					/* dTemp = (double)CLists.SYS_UL_II / 1000;
//					
//					sprintf( iTemp_Char, "%f", dTemp );
//					
//					strncpy( dTemp_Char, iTemp_Char , 6  );  					//先截取五位

//					dot_loc = dot_Loc_isNull( dTemp_Char,strlen(dTemp_Char) );	//获取小数点位置
//					
//					if( dot_loc == 1024 ){
//									
//					}else{
//						
//						if( dot_loc < 2 ){		
//							
//							if( dTemp_Char[5] == '0' ){								
//								
//								 dTemp_Char[5] = '\0';								
//							}							
//						}	
//					}	 */
//					
//					sprintf( dTemp_Char, "%d", CLists.SYS_UL_II );
//					
//				break;
//				//====================================内阻下限======================================
//				case 7:   

//					/* dTemp = (double)CLists.SYS_LL_IR / 1000;
//					
//					sprintf( iTemp_Char, "%f", dTemp );
//					
//					strncpy( dTemp_Char, iTemp_Char , 6  );  					//先截取五位

//					dot_loc = dot_Loc_isNull( dTemp_Char,strlen(dTemp_Char) );	//获取小数点位置
//					
//					if( dot_loc == 1024 ){
//									
//					}else{
//						
//						if( dot_loc < 2 ){		
//							
//							if( dTemp_Char[5] == '0' ){								
//								
//								 dTemp_Char[5] = '\0';								
//							}							
//						}	
//					} */
//					
//					sprintf( dTemp_Char, "%d", CLists.SYS_LL_IR );
//					
//				break;	
//				//====================================过流上限======================================
//				case 8:   
//				
//					dTemp = (double)CLists.SYS_UL_Overflow / 1000;
//					
//					sprintf( iTemp_Char, "%f", dTemp );
//					
//					strncpy( dTemp_Char, iTemp_Char , 6  );  					//先截取五位

//					dot_loc = dot_Loc_isNull( dTemp_Char,strlen(dTemp_Char) );	//获取小数点位置
//					
//					if( dot_loc == 1024 ){
//									
//					}else{
//						
//						if( dot_loc < 2 ){		
//							
//							if( dTemp_Char[5] == '0' ){								
//								
//								 dTemp_Char[5] = '\0';								
//							}							
//						}	
//					}	
//					
//				break;
//				//====================================过流下限======================================
//				case 9:   
//				
//					dTemp = (double)CLists.SYS_LL_Overflow / 1000;
//					
//					sprintf( iTemp_Char, "%f", dTemp );
//					
//					strncpy( dTemp_Char, iTemp_Char , 6  );  					//先截取五位

//					dot_loc = dot_Loc_isNull( dTemp_Char,strlen(dTemp_Char) );	//获取小数点位置
//					
//					if( dot_loc == 1024 ){
//									
//					}else{
//						
//						if( dot_loc < 2 ){		
//							
//							if( dTemp_Char[5] == '0' ){								
//								
//								 dTemp_Char[5] = '\0';								
//							}							
//						}	
//					}	
//					
//				break;
//				//====================================电池类型======================================
//				case 10:   
//				
//					sprintf( dTemp_Char, "%d", CLists.SYS_BatteryType );
//					
//				break;
//				//======================================语言=========================================
//				case 11:   
//					
//					sprintf( dTemp_Char, "%d", CLists.SYS_LANUGAGE );
//					
//				break;
//				
//				
//			}	
//			
//		}
//		
//		break;
//		
//		case 6:{
//			switch( v_f ){
//				
//				case 1:   
//					
//					sprintf( dTemp_Char, "%d", CLists.SYS_SYEARS );
//					
//				break;
//				
//				case 2:   
//					
//					sprintf( iTemp_Char, "%d", CLists.SYS_SMONTHS );
//					
//					if( strlen( iTemp_Char) < 2 ){    strcat( dTemp_Char, "0");    }
//					
//					strcat( dTemp_Char, iTemp_Char ); 
//					
//				break;
//				
//				case 3:   
//				
//					sprintf( iTemp_Char, "%d", CLists.SYS_SDAYS );
//					
//					if( strlen( iTemp_Char) < 2 ){    strcat( dTemp_Char, "0");    }
//					
//					strcat( dTemp_Char, iTemp_Char ); 
//					
//				break;
//				
//				case 4:   
//				
//					sprintf( iTemp_Char, "%d", CLists.SYS_SerialNO );
//					
//					if( strlen(iTemp_Char) == 1 && CLists.SYS_SerialNO == 0 ){
//						
//						strcpy( dTemp_Char, "00000000" );		
//					}
//					if( strlen(iTemp_Char) < 8 ){
//						
//						for( i = 0; i < 8 - strlen(iTemp_Char); i++ ){
//							
//							strcat( dTemp_Char, "0" ); 
//						}	
//					}
//					strcat( dTemp_Char, iTemp_Char);	
//				break;

//			}				
//		}
//		
//		break;
//		
//	}
//	strcpy( return_char, dTemp_Char );
//	
//	return return_char;
//} 

//int dot_Loc_isNull(char *_Char, int _Len);
//int dot_Loc_isNull(char *_Char, int _Len){
//	
//	int dot_loc = 1024;
//	
//	int i = 0;
//	
//	for( i = 0; i < _Len; i++ ){
//		
//		if( _Char[i] == 0x2e ){
//			
//			dot_loc = i;
//			
//			break;
//		}	
//	}
//	
//	return dot_loc;
//	
//}































