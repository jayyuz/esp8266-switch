///*************************************************************************************
// 手电程序
// 使用PIC单片机PIC16F1824,内部1024bit FLSAH  64bit SRAM 128bit EEPROM 6个I/O
// GP3脚用做复位，GP5控制侦测LED是否接入，GP2单击换挡，长按换组，GP0输出PWM波形
// GP4用于状态指示，GP1用于控制IC使能端
// 在开机状态下单击S1换档，档位为4个，在闪烁模式下单击，档位有3个
// 照明状态5档，分别是极亮（2500MA),节能档（3０ｍａ），低亮（150MA)，中亮(450MA)，高亮（1500MA),出厂默认为高
// 闪烁模式变频爆闪（15HZ/6HZ 2S,2.5A)
// 在任意档工作0.S后记忆该档位,只记忆照明模式的档位
// 电路输入电压范围3.0-4.2V,采用升压恒流
// *************************************************************************************/
///*************************************************************************************
// Project:                      pd40
// Chip type:                     PIC16F1824
// Clock frequency:               INTERNATE 4MHZ
// data;                          20160413
// Change data:                   20160413
// recode :      cr40.genggaiweiPD40YONG
// recode :      //更改好操作方式，双击电量显示，20150828，0429更新AD检测时间
// *************************************************************************************/
////引入头文件*************************************************************************
//#include <pic.h>
//
////定义芯片工作的配置位
//__CONFIG(0x2A04);//(0x2C04)
//__CONFIG(0x14FF);//(0x1EFF)内部RC振荡器，WDTE =0,上电延时定时器使能;MCLR做普通I/O;欠压检测使能
//
////脚位定义**********************************************************************
//
//#define	  		ADV					RA0				//AD采样口
//#define			AD_SW				RC4				//AD MOS 控制口
//#define 	    LED_ON1				RC3				//LED1使能控制端
//#define			SW_CH				RA2				//sw2
//#define			SW_OF				RA1				//SW1
//#define			OTP					RA3				//温度检测输入
//#define			DIS2				RC1				//电量显示一 rled
//#define			DIS1				RC2				//电量显示二 gled
//#define			PWM_OUT				RC5				//PWM输出端口
//#define       CHARGE_STATE 			RC0             //**充电状态??????????
//#define       CHARGE_INPUT			RA5             //××充电接入???????????????
//
//#define			Jie  	   2				//节能档30MA
//#define			Ji	      216		   //4.8A
//#define			Gao	   		66				//1500mA 69
//#define			Zhong	   16	         //420mA*************?
//#define			Di   	   6       		//150mA			低亮档
//#define       unchar           			 unsigned char
//#define       unint            			 unsigned int
//#define      key_mask           0b00000110			//Ra1 Ra2为按键输入口 屏蔽其他IO*****0b00011000
//#define      key_mask           0b00000110			//Ra1 Ra2为按键输入口 屏蔽其他IO*****0b00011000
//#define      key_state_0            0                        //按键初始态
//#define      key_state_1            1                        //按键第一次按下确认态
//#define      key_state_2            2                        //按键抬起确认态
//#define      key_state_3            3                        //按键抬起确认态
//#define      key_state_4            4                        //按键抬起确认态
//#define      key_state_5            5                        //按键抬起确认态
//#define      key_state_6            6                        //按键抬起确认态
//#define      key_state_7            7                        //按键抬起确认态
//#define      key_state_8            8                        //按键抬起确认态
//#define      key_state_9            9                        //按键抬起确认态
//static char key_state = 0;
//unchar stime = 0;                        //2ms定时次数存放寄存器
//unchar POWER_ON_FLAG = 0;                        //单片机工作状态寄存器
//unchar BAT = 0;				   						       //电量显示寄存器
//unint count_temp1 = 0;                        //  主按键时间计数寄存器
//unint count_temp2 = 0;                        //  副按键时间计算寄存器
//unint count_temp3 = 0;                        //  主副按键同时按下，时间计算寄存器
//unchar bright_time = 0;   					        //  爆闪记时
//unint address1 = 0x0010;                  //档位记忆地址
//unint address2 = 0x0020;                  //档位记忆地址
//unchar temp1 = 0;                        //常亮档位变量临时存放寄存器
//unchar temp2 = 0;                        //常亮档位变量临时存放寄存器
//unchar DELAY_02S = 0;							       //0.2ms delay
//unchar key_number = 0;                        //按键值存放寄存器
//unchar data_temp = 8;                        //PWM变量存储寄存器
//unint dalay_time_10ms = 0;                        //10mS完成次数寄存器
//unint lock_display_time = 0;							//开锁键提示
//unchar sleep_flag = 0;
//unchar data1 = 0;
//unchar data2 = 0;
//unint adresult = 0;
//unchar VOL_FLAG = 0;
//unchar Double_flag = 0;
//bit LOCK = 0;				//默认不锁键，每次重新上电表示解锁，档锁键时：LOCK==1
//unchar number1 = 0;		    //
//unchar PWM_DATA = 0;         //pwm占空比初值
//unint AUTO_TEMP_TIME = 0;   //智能降档时间
//unchar SOS_H[] = { 34, 34, 34, 100, 100, 100, 34, 34, 34 }; //SOS高电平时间(ms)   |--340-340-340-1000-1000-1000-340-340-340--|
//unchar SOS_L[] = { 33, 33, 200, 33, 33, 200, 33, 33, 233 }; //SOS低电平时间(ms)   |--330-330-2000-330-330-2000-330-330-2330--|
//unchar SOS1 = 0;
//unchar SOS2 = 0;
//unchar SOS_DATA = 0;                        //SOS内部次序寄存器
//unchar FC_FALSH_DATA = 0;
//unchar data_temp1 = 1;                        //闪烁档位PWM变量临时存储寄存器
//unchar data_temp2 = 0;                        //闪烁档位PWM变量临时存储寄存器
//unchar temp3 = 0;                        //双击完成标示临时存放寄存器
//unint dalay_time_1ms = 0;                        //1mS完成次数寄存器
//unchar BAT_VO_CH = 0;
//unchar FC_FALSH_FLAG = 0;                        //爆闪工作状态寄存器
//unchar Input_voltage_low = 0;
//unchar temp5 = 254;                        //档位复位临时存放寄存器
//unchar stop_run_flash = 0;				//关闭LED时，停止运行爆闪
//unchar MIRCOR = 0;                       //充电插入标记
//unchar STOP_RUN_FLASH = 0;			//关闭LED时，停止运行爆闪
//unchar CHARGE_PUTIN_FLAG = 0;			// //×××充电插入标记
//unchar CHARGE_DONE_FLAG = 0;			//××××充电状态标记
//unchar DISPLAY_ELE = 0;
//unint display_time = 0;
//unchar time_2s = 0;
//unint time_10s = 0;
//unint time_2min = 0;
//unint time_3min = 0;			//
//unchar time_2min_flag = 0;
//unchar time_3_min_flag = 0;
//
//unchar DOWN_FLAG = 0;				//各档位，降档标志位
//unchar DOWN_ELE = 0;				//降档电压位
//
//unchar REMENBER_data_temp1 = 0;		//
//unchar BAOSHAN_JIANDANG = 0;			//爆闪减档标志
//unchar add_flag = 0;					//如果加了，就标志
//unchar RESET_LOCK = 0;				//
//unint RESET_ELE = 0;				//据电压档位标志复位标记
//unchar CHAR_B = 0;
//unchar CHAR_C = 0;
//unint persent_10_time = 0;
//
//unchar input_flag1 = 0;
////	unint  persent_10_time=0;
//unchar Ji_down = 0;			//
//unchar AD_FLAG = 0;			//
//unchar AD_FLAG1 = 0;
//unchar mode_return_flag = 0;  //单击时：模式返回标志
//unchar DIS_FLAG = 0;			//
//unchar DIS_TIME = 0;			//
//unchar DIS_DELAY_500ms = 0;	//
//unchar delay1s = 0;			//
//unint SLEEP_TIME = 0;		//
//
////*******************************************************************
////函数名称：CUP_Init();
////输入参数：无
////输出参数：无
////功能描述：端口设置
//
////********************************************************************
//void CUP_Init(void) {
//	TRISA = 0b00101111;           //初始化GPIO  A1 A2按键输入口**0b00111010;
//	TRISC = 0b00000001; 				//****00000000.RC0为充电状态检测口
//
//	OPTION_REG = 0b00000011;           //弱上拉使能置高,T0内部时钟源，16分频
//
//	WPUA = 0b00000110;           //RA5 00000000**1.2上拉。４空
//	WPUC = 0b00000000;             //RC 不上拉
//
//	ANSELA = 0b00000001;             //除ra1设置全部为数字IO口*********
//	ANSELC = 0b00000000;             //设置RC1为模拟输入口,其他位EER**数字IO口	**********
//
//	IOCAP = 0b00000000;             //禁止RA0,RA2引脚正边沿电平变化中断
//	IOCAN = 0b00000000;             //禁止RA0,RA2引脚负边沿电平变化中断
//	IOCAF = 0b00000000;             //电平变化中断PORTA标志位
//
//	INTCON = 0b11101000;           //GIE = 1;INTE =0;PEIE=1;T0IE=1;GPIE=0;
//	OSCCON = 0b01101010;				//4MHZ,使用内部时钟0b01101011;
//	TMR0 = 0X7C;                 //
//
//	AD_SW = 1;
//
//	BORCON = 0B100000001;
//
//	PORTA = 0b00000000;				//RA 全部L
//	PORTC = 0b00000000;				//RC 全部L
//	LATA = 0b00000000;
//	LATC = 0b00000000;
//	CM1CON0 = 0b00000000;             //全部作为普通IO口，比较器关闭
//	CM2CON0 = 0b00000000;             //全部作为普通IO口，比较器关闭
//}
//
////*******************************************************************
////函数名称：data_write();
////输入参数：无
////输出参数：无
////功能描述：EEPROM写 子程序，将改变后的档位标示写入EEPROM中
////建造日期：20150112
////********************************************************************
//void data_write(void) {
//	if ((temp1 != data1))                      //常亮组是否有档位变换
//	{
//		eeprom_write(address1, data1);         //如果常量组有档位变换，则记忆
//		temp1 = data1;                       //更新档位存放寄存器
//
//	}
//}
////*******************************************************************
////函数名称：data_read();
////输入参数：无
////输出参数：无
////功能描述：读EEPROM子程序
////建造日期：20150112
////********************************************************************
//void data_read(void) {
//	data1 = EEPROM_READ(address1);    //读EEPROM
//	if (data1 >= 5)
//		data1 = 0;                       //常量档，首次为高亮
//	temp1 = data1;
//
//}
//
////*******************************************************************
////函数名称：key_scan_on
////输入参数：无
////输出参数：按键状态，0为无脉冲，3为70ms脉冲，4为150MS脉冲
////功能描述：按键扫描程序
////建造日期：2010.07.24
////修改日期：2010.12.30
////*******************************************************************
//
//unchar Key_scan_ON(void) {
//	unchar key_press = 0;                      //按键扫描存放寄存器
//	unchar key_return = 0;                   //按键返回值，0为无按键，1为S1单击，2为S2单击，3为S1双击
//	key_press = PORTA & key_mask;              //读GP1屏蔽其他IO口
//	switch (key_state) {
//
//	case key_state_0:
//		if ((key_press != key_mask) && (SW_OF == 0) && (SW_CH == 1))  //主按键按下  2
//			key_state = key_state_1;
//		if ((key_press != key_mask) && (SW_CH == 0) && (SW_OF == 1))  //副按键按下  1
//			key_state = key_state_4;
//		if ((PORTA & key_mask) == 0)    //主副按键同时按下，记时两秒
//				{
//			key_state = key_state_7; //跳转至确认状态，确认是否按下两秒
//		} else {
//			count_temp1 = 0;
//			count_temp2 = 0;
//			count_temp3 = 0;
//		}
//		break;
//	case key_state_1:
//		count_temp1++;	//按键判断
//		if ((PORTA & key_mask) == key_mask)  //如果两个按键都抬起则
//		{
//			key_return = 1;
//			key_state = key_state_0;
//			count_temp1 = 0;
//
//		}
//		if (((PORTA & key_mask) != 0) && (SW_OF == 0))  //表示主按键按下,不是两个按键
//				{
//			if (count_temp1 > 50)       //>0.5s
//				key_state = key_state_2;   //按下超过0.5s  跳转到按键释放状态判断
//		} else    //两个按键按下，则跳转到双按键模式
//		{
//			count_temp1 = 0;
//			count_temp3 = 0;
//			key_state = key_state_7;  //跳转至确认状态，确认是否按下两秒
//		}
//
//		break;
//	case key_state_2:
//		if (count_temp1 < 254)
//			count_temp1++;
//		if ((PORTA & key_mask) == key_mask)  //如果两个按键都抬起则
//		{
//			count_temp1 = 0;
//			key_state = key_state_0;
//		}
//		if (((PORTA & key_mask) != 0) && (SW_OF == 0))  //表示主按键按下  且没有第二个按键按下
//				{
//			if (count_temp1 > 50)       //>0.5s
//					{
//				count_temp1 = 0;
//				key_return = 2;
//				key_state = key_state_3; //按下超过0.5s  跳转
//			}
//		} else if (key_press == 0) //两个按键按下，则跳转到双按键模式
//				{
//			count_temp1 = 0;
//			count_temp3 = 0;
//			key_state = key_state_7; //跳转至确认状态，确认是否按下两秒
//		}
//		break;
//	case key_state_3:
//		if ((PORTA & key_mask) == key_mask)  //如果两个按键都抬起则
//		{
//			key_return = 4;  						 //长按主按键0.5s 关机，返回值为4，进入关机
//			key_state = key_state_0;
//			count_temp1 = 0;
//			count_temp3 = 0;
//		} else if ((PORTA & key_mask) == 0) //两个按键按下，则跳转到双按键模式
//				{
//			if (count_temp3 <= 299)
//				count_temp3++;
//			if (count_temp1 <= 299)
//				count_temp1++;
//			if (count_temp1 >= 289) {
//				count_temp1 = 0;
//				key_state = key_state_7; 					//跳转至确认状态，确认是否按下两秒
//			}
//		}
//		break;
//
//	case key_state_4:
//
//		if (count_temp2 <= 254)
//			count_temp2++;
//		if (((PORTA & key_mask) == key_mask) && (count_temp2 >= 2)) //如果两个按键都抬起则
//				{
//			key_return = 6;
//			key_state = key_state_0;
//			count_temp2 = 0;
//		}
//		if (((PORTA & key_mask) != 0) && (SW_CH == 0))  //表示fu按键按下,不是两个按键
//				{
//			if (count_temp2 > 50)       //>0.5s
//				key_state = key_state_5;   //按下超过0.5s  跳转到按键释放状态判断
//		} else if ((key_mask & PORTA) == 0)   //两个按键按下，则跳转到双按键模式
//				{
//			count_temp2 = 0;
//			count_temp3 = 0;
//			key_state = key_state_7;  //跳转至确认状态，确认是否按下两秒
//		}
//		break;
//
//	case key_state_5:												//副按键进入判断程序
//
//		if (count_temp2 <= 254)
//			count_temp2++;
//		if ((PORTA & key_mask) == key_mask) //如果两个按键松开 则
//		{
//			if (count_temp2 < 50)
//				key_return = 6;
//			else
//				key_return = 7;
//			key_state = key_state_0;
//			count_temp2 = 0;
//		}
//		if ((SW_CH == 0) && (count_temp2 >= 50)) {
//			key_return = 7;  									//长按副按键0.8秒，返回值7
//			key_state = key_state_6;
//			count_temp2 = 0;
//		} else if (key_press == 0)   //两个按键按下，则跳转到双按键模式
//				{
//			count_temp2 = 0;
//			count_temp3 = 0;
//			key_state = key_state_7; 						//跳转至确认状态，确认是否按下两秒
//		}
//		break;
//	case key_state_6:
//		if (key_press == key_mask) 							//两个键如果松开，则返回值为5，复位
//		{
//			key_return = 9;  						//长按主按键0.5s 关机，返回值为4，进入关机
//			key_state = key_state_0;
//			count_temp2 = 0;
//			count_temp3 = 0;
//		} else if (key_press == 0)   //两个按键按下，则跳转到双按键模式
//				{
//			if (count_temp3 <= 299)
//				count_temp3++;
//			if (count_temp2 <= 299)
//				count_temp2++;
//			if (count_temp2 >= 289) {
//				count_temp2 = 0;
//				key_state = key_state_7; 					//跳转至确认状态，确认是否按下两秒
//			}
//		}
//		break;           //×××××××判断如果松开，返回到指定等状态//////////////////////////////
//
//	case key_state_7:
//		if (key_press == ((PORTA & key_mask)) && (SW_OF == 0) && (SW_CH == 0)) //主按键处于稳定状态（0000 0000）（0001 1000）
//				{
//			key_state = key_state_8;								//稳定状态确认，计时
//			count_temp1 = 0;
//			count_temp2 = 0;
//		} else
//			key_state = key_state_0;
//		break;
//	case key_state_8:
//
//		if (count_temp3 <= 299)
//			count_temp3++;
//		if (((key_press == key_mask) && (SW_OF == 1) && (SW_CH == 1))
//				|| ((SW_OF == 0) && (SW_CH == 1))
//				|| ((SW_OF == 1) && (SW_CH == 0)))       				//主按键抬起
//				{
//
//			key_return = 0;         			 			           //主按键单击换档
//			key_state = key_state_9;
//			temp2 = 1;									//恢复标志，等待按键释放
//			count_temp3 = 0;
//		} else if ((count_temp3 >= 289) && ((key_mask & PORTA) == 0)) {
//			key_return = 8;  									//主 副 按键同时按下两秒
//			//count_temp3=0;
//			key_state = key_state_9;
//		}
//		break;
//
//	case key_state_9:
//		if ((PORTA & key_mask) == key_mask)  //如果两个按键都抬起则
//		{
//			count_temp2 = 0;
//			count_temp1 = 0;
//			count_temp3 = 0;
//			key_state = key_state_0;
//		}
//		break;
//		//-------------------------------------------------------------------------------------------------------------
//		//-------------------------------------------------------------------------------------------------------------
//	default:
//		key_state = key_state_0;
//		break;
//	}
//	return key_return;
//}
//
////**********************************************************************************************
////函数名称：INTERRUPT_Init ();
////输入参数：无
////输出参数：无
////功能描述：端口中断初始化子程序
////建造日期：2010.09.24
////**********************************************************************************************
//void INTERRUPT_Init(void) {
//	INTCON = 0b11100000;              //GIE = 1;INTE =0;PEIE=1;T0IE=1;GPIE=0;
//	TMR0 = 0X7C;             //GPPU=0,T0CS=0;PSA= 0,PS2:1=101 64分频，4M/64=62.5KHZ
//	TRISC5 = 0;
//	PR2 = 250;
//	CCPR1L = 0;
//	CCP1CON = 0b00001100;
//	PSTR1CON = 0X01;				//定义RC5
//	T2CON = 0b00000001;    //定时器2十六分频,预分频4，后分频1   980hz
//	TMR2ON = 1;
//}
////*******************************************************************
////*******************************************************************
////函数名称：interrupt key_int();
////输入参数：无
////输出参数：无
////功能描述：中断服务程序
////建造日期：2010.07.24
////*******************************************************************
//void interrupt key_int(void)
//{
//	if(TMR0IF)
//	{
//		TMR0IF = 0;                             //中断请求标志位清零
//		TMR0 = 0X7C;//(124+1)/62.5=2ms
//		stime++;//时间存放寄存器增一
//	}
//	if(stime>=5) {
//		//	BAT = 3;
//		if((BAT==3)&&(POWER_ON_FLAG!=0))//低于10%
//		{
//			persent_10_time++;
//			if(persent_10_time>=50)
//			{
//				persent_10_time=0;
//				DIS2=~DIS2;
//			}
//		}
//	}
//}
//
////*******************************************************************
////函数名称：AD_Init(void)
////输入参数：无
////输出参数：无
////功能描述：ＡＤＣ初始化
////建造日期：2010.09.09
////*******************************************************************
//
//void AD_Init(void) {
//	TRISA0 = 1;
//	ANSELA = 0b00000001;
//	ADCON0 = 0b00000001;       //转换结果格式右对齐，3.0V参考电压，模拟通道AN3(RA4)不启动AD转换，AD转换模块打开
//	ADCON1 = 0b10100011;         //
//	FVREN = 1;         //关闭内部参考电压
//	FVRRDY = 1;  //不使能参考电压
//	ADFVR0 = 0;
//	ADFVR1 = 1;  //固定参考电压guanbi
//	ADIE = 0;                  	//AD转换中断禁止
//	ADIF = 0;                  	//外部中断禁止
//	ADON = 0;                    //AD转换模块关闭
//	AD_SW = 1;
//}
////*******************************************************************
////函数名称：LED_ind_Init(void)
////输入参数：无
////输出参数：无
////功能描述：LED指示灯初始化
////建造日期：2010.09.09
////*******************************************************************
//
//void ADC_Read(void) {
//	AD_SW = 1;					//
//	unchar i, j;				//
//	ADON = 1;					//
//	ADIE = 0;					//
//	adresult = 0;				//ad值
//	for (i = 30; i > 0; i--)			//10
//			{
//		for (j = 0; j < 3; j++)
//			;		//
//		ADGO = 1;				//
//		while (ADGO)
//			;			//
//		adresult = adresult + ((ADRESH << 8) | ADRESL) / 30;    //
//	}
//	ADON = 0;			//AD转换模块关闭
//	//if(adresult<=875) BAT=1;		//70%
//	//if(adresult<=840) BAT=2;		//50%
//	//if(adresult<=790) BAT=3;		//10%
//	//如果电池电压小于12K 10K    70%;3.85V(875)   30:3.7V(840)    10%:3.5V(790)
//	if (adresult <= 716)         //3.2
//			{
//		BAT = 3;
//	} else if (adresult <= 791) {  //3.5v
//		BAT = 2;		//
//	} else if (adresult <= 817) {   //3.6显示电压
//		BAT = 1;		//
//	} else {
//		BAT = 0;		//
//	}
//	//   	if(adresult<=859) {data_temp1=2;DOWN_ELE=1;}  //3.85V 无负载电压
//	//		if(adresult<=772) {data_temp1=3;DOWN_ELE=2;}  //3.4V
//	//		if(adresult<=727) {data_temp1=4;DOWN_ELE=3;}  //3.2V
//	//		if(adresult<=682) {data_temp1=5;DOWN_ELE=4;}  //3.0V
//
//	if (CHARGE_INPUT == 1) {
//		if (adresult <= 909) {
//			RESET_ELE = 1;
//		}  //4.0V909
//		if (adresult <= 886) {
//			RESET_ELE = 2;
//		}  //3.9V886
//		if (adresult <= 840) {
//			RESET_ELE = 3;
//		}  //3.7V840
//		if (adresult <= 750) {
//			RESET_ELE = 4;
//		}  //3.3V750
//	}
//
//	if ((adresult <= 800) && (data1 == 0)) {
//		DOWN_ELE = 1;
//	} //3.5=794  3.55v       data1降档电压
//	if ((adresult <= 750) && (data1 == 4)) {
//		DOWN_ELE = 2;
//	} //3.35V757          	data1
//	if ((adresult <= 670) && (data1 == 3)) {
//		DOWN_ELE = 3;
//	}  //3.25V=735     		data1
//	if ((adresult <= 630) && (data1 == 2)) {
//		DOWN_ELE = 4;
//	}  //3.0V=678 2.9=635 	data1防跳
//	AD_SW = 1;					//
//	//--------------------------------------------------------------------------
//}
//
////*******************************************************************
////函数名称：void autotempr_control();
////输入参数：无
////输出参数：无
////功能描述：在极亮档时，智能温控，每30s检测一次，温度高于限制温度，则亮度减少10%
////如果温度低于限制温度，则亮度增加10% 直到最亮。
////建造日期：20150227
////********************************************************************
//
////
///*void autotempr_control(void)
// {
// if((OTP==0)&&(AUTO_TEMP_TIME>=2910))   //如果温度高于65摄氏度，而且30s钟时间到，则亮度减小
// {
// AUTO_TEMP_TIME=0;   //清时间计数器
// PWM_DATA++;
// if(PWM_DATA>=13) PWM_DATA=13;
// }
// if((OTP==1)&&(AUTO_TEMP_TIME>=2910))   //如果温度高于65摄氏度，而且30s钟时间到，则亮度增加
// {
// AUTO_TEMP_TIME=0;   //清时间计数器
// switch(data1)
// {
// case 0:
// PWM_DATA--;
// if(PWM_DATA<=0) PWM_DATA=1;
// break;
// case 4:
// PWM_DATA--;
// if(PWM_DATA<=8)PWM_DATA=8;
// break;
// default:break;
// }
// }
// switch(PWM_DATA)  //2.8A到1A  约百分之5的递增 递减
// {
// case 0:
// break;
// case 1:
// CCPR1L=Ji;   //175 2800mA
// break;
// case 2:
// CCPR1L=164;
// break;
// case 3:
// CCPR1L=153;
// break;
// case 4:
// CCPR1L=142;
// break;
// case 5:
// CCPR1L=131;
// break;
// case 6:
// CCPR1L=120;
// break;
// case 7:
// CCPR1L=109;
// break;
// case 8:
// CCPR1L=98;
// break;
// case 9:
// CCPR1L=87;
// break;
// case 10:
// CCPR1L=76;//1.3a 800 lux
// break;
// case 11:
// CCPR1L=65;
// break;
// case 12:
// CCPR1L=54;
// break;
// case 13:
// CCPR1L=43;//750ma
// break;
// default:
// break;
// }
// }*/
//void LCOK_prompt(void)   //锁定解锁提示
//{
//	switch (number1) {
//	case 0:
//		LED_ON1 = 0;
//		CCPR1L = 0;
//		if (lock_display_time >= 50)     //0.5秒
//				{
//			number1++;
//			lock_display_time = 0;
//		}
//		break;
//	case 1:
//		LED_ON1 = 1;
//		CCPR1L = Di;
//		if (lock_display_time >= 25)     //0.5秒
//				{
//			number1++;
//			lock_display_time = 0;
//		}
//		break;
//	case 2:
//		LED_ON1 = 0;
//		CCPR1L = 0;
//		if (lock_display_time >= 25)     //0.5秒
//				{
//			number1++;
//			lock_display_time = 0;
//		}
//		break;
//	case 3:
//		LED_ON1 = 1;
//		CCPR1L = Di;
//		if (lock_display_time >= 25)     //0.5秒
//				{
//			number1++;
//			lock_display_time = 0;
//		}
//		break;
//	case 4:
//		key_number = 0;
//		LED_ON1 = 0;
//		CCPR1L = 0;
//		number1 = 0;
//		POWER_ON_FLAG = 0;
//		sleep_flag = 0;
//		temp2 = 0;											 //temp2=0；清零
//		break;
//	default:
//		break;
//	}
//	//×××××××××××××××××××××××××××××××××××锁键状态（主副按下2秒响应）
//}
////*******************************************************************
////函数名称：lighting_MODE();
////输入参数：无
////输出参数：无
////功能描述：照明模式
////建造日期：20150227
////********************************************************************
//
//void lighting_MODE(void) {
//	switch (key_number) {
//	case 0:
//		break;
//
//	case 1:						//×××××××××××××××××××××××××××××××××××换档调光（主键单击）
//		key_number = 0;         	//按键返回值清零
//		data_temp = 255;       			//每次有按键改变，都要重新进入开关语句
//		dalay_time_10ms = 0;
//		data1++;
//		if (data1 >= 5)
//			data1 = 0;
//		if (((data1 == 0) && (DOWN_FLAG == 1))
//				|| (DOWN_ELE == 1) && ((data1 == 0)))
//			data1 = 1;   //如果条件，跳过极亮
//		if (((data1 == 4) && (DOWN_FLAG == 2))
//				|| (DOWN_ELE == 2) && ((data1 == 4)))
//			data1 = 1;   //如果条件，跳过高亮
//		if (((data1 == 3) && (DOWN_FLAG == 3))
//				|| (DOWN_ELE == 3) && ((data1 == 3)))
//			data1 = 1;   //如果条件，跳过中亮
//		if (((data1 == 2) && (DOWN_FLAG == 4))
//				|| (DOWN_ELE == 4) && ((data1 == 2)))
//			data1 = 1;   //如果条件，跳过低亮
//
//		/*	if(CHARGE_INPUT==1)			//充电强制转换成低亮档
//		 {
//		 data1=2;
//		 }  */
//		break;
//	case 6:   //×××××××××××××××××××××××××××××××××××副键单击
//		key_number = 0;
//		POWER_ON_FLAG = 2;
//		data_temp2 = 0;
//		dalay_time_1ms = 0;
//		dalay_time_10ms = 0;
//		FC_FALSH_DATA = 0;
//		bright_time = 0;
//		SOS_DATA = 0;
//		data2 = 0;
//		SOS1 = 0;
//		SOS2 = 0;
//		temp5 = 0;						//重新设置亮度
//		stop_run_flash = 0;   //照明模式进入爆闪模式，初始化一次设置
//		break;
//	case 7:   //×××××××××××××××××××××××××××××××××××关闭LED（主键0.5秒）
//		key_number = 0;         //按键返回值清零
//		CCPR1L = 0;
//		LED_ON1 = 0;
//		break;
//	case 9:         //×××××××××××××××××××××××××××××××××××关机（主键0.5秒）
//		key_number = 0;
//		if (temp2 == 1) //按键返回值清零				 //如果关机，temp2==1；则关机清零，否则temp2==0时 将temp2置1
//				{
//			data_temp = 255;       //每次有按键改变，都要重新进入开关语句
//			PWM_OUT = 0;                        //PWM输出低电平
//			LED_ON1 = 0;
//			POWER_ON_FLAG = 0;
//			stop_run_flash = 0;
//			data_temp = 255;
//			temp2 = 0;
//		}
//		if (temp2 == 0) 										//如果长按0.5s关机，tem
//				{
//			temp2 = 1;
//		}
//		break;
//	case 8: 				//×××××××××××××××××××××××××××××××××××锁键状态（主副按下2秒响应）
//		key_number = 0;
//		number1 = 0;
//		data_temp = 255;
//		LOCK = !LOCK;
//		lock_display_time = 0;
//		POWER_ON_FLAG = 3;
//		break;
//	case 2: 				//×××××××××××××××××××××××××××××××××××关闭LED（主键0.5秒）
//		key_number = 0;         //按键返回值清零
//		PWM_OUT = 0;                        //PWM输出低电平
//		LED_ON1 = 0;
//		break;
//	case 4:                      //×××××××××××××××××××××××××××××××××××关机（主键0.5秒）
//		key_number = 0;
//		if (temp2 == 1)   //按键返回值清零	//如果关机，temp2==1；则关机清零，否则temp2==0时 将temp2置1
//				{
//			data_temp = 255;       //每次有按键改变，都要重新进入开关语句
//			PWM_OUT = 0;                        //PWM输出低电平
//			LED_ON1 = 0;
//			POWER_ON_FLAG = 0;
//			data_temp = 255;
//			temp2 = 0;											 //temp2=0；清零
//		}
//		if (temp2 == 0) 										//如果长按0.5s关机，tem
//				{
//			temp2 = 1;
//		}
//		break;
//	default:
//		key_number = 0;
//		break;   //其他情况，跳出
//	}
//
//	if (CHARGE_INPUT == 1)	//******zzc充电介入
//			{
//		data1 = 2;
//		data_write();
//		if (CHARGE_STATE == 0) {
//			DIS2 = 1;
//
//			DIS1 = 0;
//		} else {
//			DIS2 = 0;
//
//			DIS1 = 1;
//		}
//	} else if ((BAT != 3) && (CHARGE_INPUT == 0))	//888888&&(BAT!=2)
//			{
//		DIS2 = 0;
//
//		DIS1 = 0;
//	}
//	//data1=0; 极亮
//	//data1=4; 高亮
//	//data1=3; 中亮
//	//data1=2; 低亮
//	//data1=1; 节能
//	switch (data1)	//××××××××调光选择×××××××××//
//	{
//	case 0:
//		if (data_temp != 0)          //如果是从其他档位切换过来
//				{
//			time_2min = 0;
//			time_2min_flag = 0;
//			time_2s = 0;
//
//			data_temp = 0;
//			CCPR1L = Ji;   					//设置亮度
//			LED_ON1 = 1;    		//使能输出
//			AUTO_TEMP_TIME = 0;			//重新赋值，智能温控时间
//			PWM_DATA = 1;           //每次换档重新回到极亮档时，智能温控重新开始检测
//		}
//		if (CCPR1L >= 80)  //
//				{
//			if (OTP == 0) {
//				if (AUTO_TEMP_TIME > 100)  			//每一次下降的时间，下降速度1S
//						{
//					AUTO_TEMP_TIME = 0;
//					CCPR1L = 70;   //
//				}
//			} else if ((data1 == 0) && (time_2min > 9000)) //限制在高亮***1200=2min 1.5MIN
//					{
//				if (AUTO_TEMP_TIME > 100)   			//每一次下降的时间，下降速度1S
//						{
//					AUTO_TEMP_TIME = 0;
//					CCPR1L--;   //智能温控做能降到底最低亮度
//					if (CCPR1L <= 70) {
//						CCPR1L = 70;
//					}
//
//				}
//			}
//		}
//		if ((VOL_FLAG == 1) && (dalay_time_10ms >= 150))   //如果电压
//				{
//			DOWN_FLAG = 1;
//			dalay_time_10ms = 0;
//			data1 = 4;
//			data_temp = 255;
//		}
//		break;                     //1200＊＊＊＊＊＊＊＊＊ｚｃ２５００mA
//	case 1:
//		if (data_temp != 1)          //如果是从其他档位切换过来
//				{
//			data_temp = 1;        //执行低亮PWM
//			CCPR1L = Jie;            //设置亮度
//			LED_ON1 = 1;						 //使能输出
//		}
//		break;                      //15mA
//	case 2:
//		if (data_temp != 2)          //如果是从其他档位切换过来
//				{
//			data_temp = 2;         //执行中亮PWM
//			CCPR1L = Di;              //设置亮度
//			LED_ON1 = 1;					  //使能输出
//		}
//		if ((VOL_FLAG == 1) && (dalay_time_10ms >= 150))				//如果电压
//				{
//			DOWN_FLAG = 4;
//			dalay_time_10ms = 0;
//			data1 = 1;
//			data_temp = 255;
//		}
//		break;                 //150mA
//	case 3:
//		if (data_temp != 3)         //如果是从其他档位切换过来
//				{
//			data_temp = 3;            //执行高亮PWM
//			CCPR1L = Zhong;					//设置亮度
//			LED_ON1 = 1;					//使能输出
//		}
//		if ((VOL_FLAG == 1) && (dalay_time_10ms >= 150))				//如果电压
//				{
//			DOWN_FLAG = 3;
//			dalay_time_10ms = 0;
//			data1 = 2;
//			data_temp = 255;
//		}
//		break;                    //400mA
//	case 4:
//		if (data_temp != 4)         //如果是从其他档位切换过来
//				{
//			data_temp = 4;        //执行高亮PWM
//			CCPR1L = Gao;						//设置亮度
//			LED_ON1 = 1;					//使能输出
//			PWM_DATA = 8;
//			AUTO_TEMP_TIME = 0;			//重新赋值，智能温控时间
//		}
//		if ((VOL_FLAG == 1) && (dalay_time_10ms >= 150))			//如果电压
//				{
//			DOWN_FLAG = 2;
//			dalay_time_10ms = 0;
//			data1 = 3;
//			data_temp = 255;
//		}
//		break;                    //400mA
//	default:
//		break;
//	}
//}
////*******************************************************************
////函数名称：LED_FC_FALSH();
////输入参数：无
////输出参数：无
////功能描述：主要程序，输出2.9A 变频爆闪
////建造日期：2011.12.06
////*******************************************************************
//void LED_FC_FALSH(void) {
//	switch (FC_FALSH_DATA) {
//	case 0:
//		FC_FALSH_FLAG = 0;
//		if (LED_ON1 == 0) {
//			if (dalay_time_10ms >= 3) {
//				dalay_time_10ms = 0;
//				LED_ON1 = 1;
//			}
//		} else if (LED_ON1 == 1) {
//			if (dalay_time_10ms >= 3) {
//				dalay_time_10ms = 0;
//				LED_ON1 = 0;
//				bright_time++;
//				if (bright_time >= 30) {
//					bright_time = 0;
//					FC_FALSH_DATA = 1;
//				}
//			}
//		}
//		break;
//	case 1:
//		if (LED_ON1 == 0) {
//			if (dalay_time_10ms >= 7) {
//				dalay_time_10ms = 0;
//				LED_ON1 = 1;
//			}
//		} else if (LED_ON1 == 1) {
//			if (dalay_time_10ms >= 7) {
//				dalay_time_10ms = 0;
//				LED_ON1 = 0;
//				bright_time++;
//				if (bright_time >= 12) {
//					bright_time = 0;
//					FC_FALSH_DATA = 0;
//					FC_FALSH_FLAG = 1;
//				}
//			}
//		}
//		break;
//	default:
//		break;
//	}
//}
////*******************************************************************
////函数名称：LED_SOS_FALSH();
////输入参数：无
////输出参数：无
////功能描述：主要程序，输出200mA SOS
////建造日期：2010.07.21
////*******************************************************************
//void LED_SOS_FALSH(void) {
//	switch (SOS_DATA) {
//	case 0:
//		LED_ON1 = 1;
//		if (dalay_time_10ms >= SOS_H[SOS1]) //unchar	SOS_H[]={34,34,34,100,100,100,34,34,34};
//				{
//			dalay_time_10ms = 0;
//			SOS_DATA++;
//			SOS1++;
//		}
//		break;
//	case 1:
//		LED_ON1 = 0;
//		if (dalay_time_10ms >= SOS_L[SOS2]) //unchar	SOS_L[] ={33,33,200,33,33,200,33,33,233};
//				{
//			dalay_time_10ms = 0;
//			SOS_DATA--;
//			SOS2++;
//		}
//		if (SOS2 == 9) {
//			SOS1 = 0;
//			SOS2 = 0;
//			dalay_time_10ms = 0;
//		}
//		break;
//	default:
//		break;
//	}
//}
////=================================================================================爆闪 -SOS
//void FLASH_BS_SOS_MODE(void) //爆闪/SOS
//{
//	if (CHARGE_INPUT == 1)	//******zzc充电介入
//			{
//		data1 = 2;
//		data_write();
//		if (CHARGE_STATE == 0) {
//			DIS1 = 0;
//			DIS2 = 1;
//		} else {
//			DIS1 = 1;
//			DIS2 = 0;
//		}
//	} else if ((BAT != 3) && (CHARGE_INPUT == 0))	//888888( BAT!=2)&&
//			{
//		DIS2 = 0;
//
//		DIS1 = 0;
//	}
//	switch (key_number)                   //在闪烁状态下，根据按键返回值
//	{
//	case 0:
//		break;  //无按键，继续向下执行
//	case 1:
//		POWER_ON_FLAG = 1;
//		LED_ON1 = 1;						//使能输出
//		data_temp = 255;				//进入记忆档位
//		key_number = 0;					//零按键值
//		data_temp2 = 0;
//		stop_run_flash = 0;     //恢复闪烁标志，为了一下次的进入。
//		break;
//	case 2:     //×××××××××××××××××××××××××××××××××××关闭LED（主键0.5秒）
//		key_number = 0;         //按键返回值清零
//		CCPR1L = 0;                  //关闭LED输出亮度
//		LED_ON1 = 0;  	     		 //关闭驱动输出
//		stop_run_flash = 1;					 //关闭闪烁功能
//		break;
//	case 4:					 //×××××××××××××××××××××××××××××××××××关机（主键0.5秒）
//		key_number = 0;
//		if (temp2 == 1)	//按键返回值清零			//如果关机，temp2==1；则关机清零，否则temp2==0时 将temp2置1
//				{
//			data_temp = 255;       //每次有按键改变，都要重新进入开关语句
//			CCPR1L = 0;            //PWM输出低电平
//			stop_run_flash = 0;     //恢复闪烁标志，为了一下次的进入。
//			LED_ON1 = 0; 			//关闭驱动芯片使能
//			POWER_ON_FLAG = 0;
//			temp2 = 0;											 //temp2=0；清零
//		}
//		if (temp2 == 0) 										//如果长按0.5s关机，tem
//				{
//			temp2 = 1;
//		}
//		break;
//	case 6:
//		key_number = 0;           //按键返回值清零
//		data_temp2 = 0;
//		data2++;                //闪烁档标志位清0
//		if (data2 >= 2)
//			data2 = 0;
//		dalay_time_1ms = 0;
//		dalay_time_10ms = 0;
//		FC_FALSH_DATA = 0;
//		bright_time = 0;
//		SOS_DATA = 0;
//		SOS1 = 0;
//		SOS2 = 0;
//		stop_run_flash = 0;     //恢复标志。
//		break;
//	case 7:     //×××××××××××××××××××××××××××××××××××关闭LED（主键0.5秒）
//		key_number = 0;         //按键返回值清零
//		CCPR1L = 0;
//		LED_ON1 = 0;
//		stop_run_flash = 1;     //关闭。
//		break;
//	case 8:     //×××××××××××××××××××××××××××××××××××锁键状态（主副按下2秒响应）
//
//		key_number = 0;
//		data_temp2 = 0;
//		data_temp = 255;
//		LOCK = !LOCK;
//		lock_display_time = 0;
//		number1 = 0;
//		POWER_ON_FLAG = 3;
//		stop_run_flash = 0;     //恢复闪烁标志，为了一下次的进入。
//		break;
//	case 9:     //×××××××××××××××××××××××××××××××××××关机（主键0.5秒）
//		key_number = 0;
//		if (temp2 == 1) //按键返回值清零				 //如果关机，temp2==1；则关机清零，否则temp2==0时 将temp2置1
//				{
//			data_temp = 255;       //每次有按键改变，都要重新进入开关语句
//			PWM_OUT = 0;                        //PWM输出低电平
//			LED_ON1 = 0;
//			POWER_ON_FLAG = 0;
//			temp2 = 0;											 //temp2=0；清零
//			stop_run_flash = 0;     //恢复闪烁标志，为了一下次的进入。
//		}
//		if (temp2 == 0) 										//如果长按0.5s关机，tem
//				{
//			temp2 = 1;
//		}
//		break;
//
//	default:
//		key_number = 0;
//		break;
//	}
//	if ((stop_run_flash != 1) && (CHARGE_INPUT != 1)) //CHARGE_STATE!=1*******ZC
//			{
//		switch (data2)                                //根据变量决定闪烁频率
//		{
//		case 0:
//			if (temp5 != 5)  		//如果TIMER1内不是1450ma的初值
//					{
//				temp5 = 5;                //执行低亮档位
//				TMR1ON = 1;                //关TIMER1中断
//				dalay_time_10ms = 0;
//				FC_FALSH_DATA = 0;
//				//	CCPR1L=Ji;
//				PWM_DATA = 1;   //初始化极亮，温控启动
//				//	if((BAOSHAN_JIANDANG==1)&&(data_temp1==1))data_temp1=2;
//				//	if((BAOSHAN_JIANDANG==2)&&(data_temp1==1))data_temp1=2;
//				//	if((BAOSHAN_JIANDANG==3)&&(data_temp1==1))data_temp1=2;
//				//	if((BAOSHAN_JIANDANG==4)&&(data_temp1==1))data_temp1=2;
//			}
//			switch (data_temp1) {
//
//			case 0:
//				break;
//			case 1:
//				if (BAOSHAN_JIANDANG == 1)
//					break;
//				if (BAOSHAN_JIANDANG == 2)
//					break;
//				if (BAOSHAN_JIANDANG == 3)
//					break;
//				if (BAOSHAN_JIANDANG == 4)
//					break;
//				CCPR1L = Ji;
//				break;
//			case 2:
//				if (BAOSHAN_JIANDANG == 2)
//					break;
//				if (BAOSHAN_JIANDANG == 3)
//					break;
//				if (BAOSHAN_JIANDANG == 4)
//					break;
//				CCPR1L = Gao;
//				break;
//			case 3:
//				if (BAOSHAN_JIANDANG == 3)
//					break;
//				if (BAOSHAN_JIANDANG == 4)
//					break;
//				CCPR1L = Zhong;
//				break;
//			case 4:
//				if (BAOSHAN_JIANDANG == 4)
//					break;
//				CCPR1L = Di;
//				break;
//			case 5:
//				CCPR1L = Jie;
//				break;
//			default:
//				break;
//			}
//			if ((VOL_FLAG == 1) && (dalay_time_1ms >= 150))				//如果电压
//					{
//				dalay_time_1ms = 0;
//				if (data_temp1 < 5) {
//					data_temp1++;
//				}
//			}
//			LED_FC_FALSH();					//变频
//
//			break;   //15HZ爆闪
//			//-------------------------------------------------------------------------------------
//		case 1:
//			if (temp5 != 6)                 //如果TIMER1内不是200ma的初值
//					{
//				temp5 = 6;                   //执行200mA亮档位
//				TMR1ON = 1;                   //开TIMER1中断
//				SOS_DATA = 0;
//				data_temp = 3;                    //设置初值
//				dalay_time_10ms = 0;
//				CCPR1L = Di;
//				SOS1 = 0;
//				SOS2 = 0;
//			} else {
//				switch (data_temp2) {
//				case 0:
//					break;
//
//				case 1:
//					CCPR1L = Jie;
//					break;
//				default:
//					break;
//				}
//			}
//
//			LED_SOS_FALSH();                   //以SOS使能LED
//			if (VOL_FLAG == 1) {
//				if (dalay_time_1ms >= 200)		//首次开机延时2s
//						{
//					dalay_time_1ms = 0;
//					Input_voltage_low = 1;
//					data_temp2++;
//					if (data_temp2 >= 1)
//						data_temp2 = 1;
//				}
//			} else {
//				dalay_time_1ms = 0;
//			}
//			break;   //SOS
//		default:
//			break;
//		}
//	}
//
//	/*if(BAT!=3){
//	 DIS1=DIS2=0;
//	 }*/
//}
////=========================================================================显示电量
//void DISPLAY_ELV() {
//	if (display_time >= 300)  //2秒时间到，退出
//			{
//		display_time = 0;
//		POWER_ON_FLAG = 0;
//		sleep_flag = 0;
//	}
//	//ADC_Read();
//	if ( CHARGE_INPUT != 1) {
//		switch (BAT) {
//		case 0:
//			DIS1 = 1;
//			DIS2 = 0;
//			break;
//		case 1:  //70%
//			DIS1 = 1;
//			DIS2 = 0;
//			break;
//		case 2:	//30%
//			DIS1 = 0;
//			DIS2 = 1;
//			break;
//		case 3:  //10%
//			if (stime >= 5) {
//				{
//					persent_10_time++;
//					if (persent_10_time >= 50) {
//						persent_10_time = 0;
//						DIS2 = ~DIS2;
//					}
//				}
//			}
//			//if(DIS_DELAY_500ms>=80)
//			//  {DIS_DELAY_500ms=0;
//			//		DIS1=0;
//			//   DIS2=~DIS2;}
//			//	DIS1=0;DIS2=1;
//			break;
//		default:
//			break;
//		}
//	}
//	if (CHARGE_INPUT == 1)	//
//			{
//		if (CHARGE_STATE == 0) {
//			DIS2 = 1;
//			DIS1 = 0;
//		} else {
//			DIS2 = 0;
//			DIS1 = 1;
//		}
//	}
//
//}
////=============================================================================================
//void LOW_VOLDOWN()   //低压降档子函数
//{
//	//	if(adresult<=795) {DOWN_ELE=1;}  //3.5V
//	//	if(adresult<=772) {DOWN_ELE=2;} //3.4V
//	//	if(adresult<=727) {DOWN_ELE=3;}  //3.2V
//	//	if(adresult<=682) {DOWN_ELE=4;}  //3.0V
//
//	//	if(adresult<=909) {RESET_ELE=1;}  //4.0V
//	//	if(adresult<=886) {RESET_ELE=2;}  //3.9V
//	//	if(adresult<=840) {RESET_ELE=3;}  //3.7V
//	//	if(adresult<=750) {RESET_ELE=4;}  //3.3
//
//	//	if(((data1==0)&&(DOWN_FLAG==1))||(DOWN_ELE==1)&&((data1==0)))data1=1;   //如果条件，跳过极亮
//	//	if(((data1==4)&&(DOWN_FLAG==2))||(DOWN_ELE==2)&&((data1==4)))data1=1;   //如果条件，跳过高亮
//	//	if(((data1==3)&&(DOWN_FLAG==3))||(DOWN_ELE==3)&&((data1==3)))data1=1;   //如果条件，跳过中亮
//	//	if(((data1==2)&&(DOWN_FLAG==4))||(DOWN_ELE==4)&&((data1==2)))data1=1;   //如果条件，跳过低亮
//
//	if (CHARGE_INPUT == 1)	//满足充电
//			{
//		switch (RESET_ELE)	//且电压恢复
//		{
//		case 1:					//4.0恢复点
//			DOWN_ELE = 0;
//			DOWN_FLAG = 0;
//			break;
//		case 2:
//			DOWN_ELE = 1;		//3.9恢复点
//			DOWN_FLAG = 1;
//			break;
//		case 3:					//3.7恢复点
//			DOWN_ELE = 2;
//			DOWN_FLAG = 2;
//			break;
//		case 4:					//3.3恢复点
//			DOWN_ELE = 3;
//			DOWN_FLAG = 3;
//			break;
//		default:
//			break;
//		}
//	}
//	switch (DOWN_ELE) {
//	case 0:
//		break;
//	case 1:				//如果是在极亮档，则下降一档
//		if (data1 == 0) {
//			DOWN_FLAG = 1;		//标志一
//			data1 = 4;
//		}
//		//--------------------------------------------------------------------------
//		if (data_temp1 == 1)		// 爆闪模式中 pwm占空比的标志位
//				{
//			BAOSHAN_JIANDANG = 1;
//			data_temp1 = 2;
//		}
//
//		break;
//	case 2:				//如果是在高亮档，则下降到中亮
//		if ((data1 == 4) || (data1 == 0)) {
//			DOWN_FLAG = 2;		//标志一
//			data1 = 3;
//		}
//		//--------------------------------------------------------------------------
//		if ((data_temp1 == 2) || (data_temp1 == 1))		// 爆闪模式中 pwm占空比的标志位
//				{
//			BAOSHAN_JIANDANG = 2;
//			data_temp1 = 3;
//		}
//		break;
//	case 3:				//如果是在中亮档，则下降到低亮
//		if ((data1 == 3) || (data1 == 4) || (data1 == 0)) {
//			DOWN_FLAG = 3;		//标志一
//			data1 = 2;
//		}
//		//--------------------------------------------------------------------------
//		if ((data_temp1 == 3) || (data_temp1 == 2) || (data_temp1 == 1))// 爆闪模式中 pwm占空比的标志位
//				{
//			data_temp1 = 4;
//			BAOSHAN_JIANDANG = 3;
//		}
//		break;
//	case 4:				//如果是在低亮档，则下降到节能
//		if ((data1 == 2) || (data1 == 3) || (data1 == 4) || (data1 == 0)) {
//			DOWN_FLAG = 4;		//标志一
//			data1 = 1;
//		}
//		//--------------------------------------------------------------------------
//		if ((data_temp1 == 4) || (data_temp1 == 2) || (data_temp1 == 1))// 爆闪模式中 pwm占空比的标志位
//				{
//			data_temp1 = 5;
//			BAOSHAN_JIANDANG = 4;
//		}
//		break;
//	default:
//		break;
//	}//=============================================================================================
//
//}
//
//void tsoftReset();
////*******************************************************************
////函数名称：main();
////输入参数：无
////输出参数：无
////功能描述：主程序
////建造日期：2010.09.14
////*******************************************************************
//
//void main(void) {
//
//	unint i;
//
//	CUP_Init();				    //脚位设置
//
//	INTERRUPT_Init();
//	AD_Init();
//	data_read();                         //读EEPROM中记忆的档位
//
//	softReset: while (1)                                      //死循环
//	{
//		if (stime >= 5)                                 //10MS到
//				{
//			stime = 0;                          //扫描时间到标示为清零
//			AUTO_TEMP_TIME++;
//			LOW_VOLDOWN();										//检测电压降档，标志
//			if ((POWER_ON_FLAG == 1) && (data1 == 0)) {
//				time_2min++;
//				if (time_2min_flag == 1)
//					time_2s++;
//				else
//					time_2s = 0;
//			} else   //清所以的计时参数
//			{
//				time_2min = 0;
//				time_2min_flag = 0;
//				time_2s = 0;
//			}
//			// OTP=1;
//			if (CCPR1L >= 80)  //
//					{
//				if (OTP == 0) {
//					if (AUTO_TEMP_TIME > 100)  			//每一次下降的时间，下降速度1S
//							{
//						AUTO_TEMP_TIME = 0;
//						CCPR1L = 70;   //
//					}
//				} else if ((data1 == 0) && (time_2min > 9000)) //限制在高亮***1200=2min 1.5MIN
//						{
//					if (AUTO_TEMP_TIME > 100)   			//每一次下降的时间，下降速度1S
//							{
//						AUTO_TEMP_TIME = 0;
//						CCPR1L--;   //智能温控做能降到底最低亮度
//						if (CCPR1L <= 70) {
//							CCPR1L = 70;
//						}
//
//					}
//				}
//			}
//
//			//*****************************************************************8888
//			if ((CHARGE_INPUT == 1) && (LOCK == 1) && (CHAR_C != 2))   //插上电
//					{
//				CHAR_B = 1;
//				CHAR_C = 1;
//				LOCK = 0;   //1
//			}
//			if ((CHARGE_INPUT == 1) && (LOCK == 0) && (CHAR_C != 2)) {
//				CHAR_B = 1;
//				CHAR_C = 1;
//				LOCK = 0;   //2
//			}
//			if ((CHARGE_INPUT == 1) && (CHAR_B == 1) && (LOCK == 1)) {
//				LOCK = 1;
//				CHAR_C = 2;   //3
//			}
//			if ((CHARGE_INPUT == 1) && (LOCK == 0) && (CHAR_B == 1))   //
//					{
//				LOCK = 0;
//				CHAR_C = 2;  //、、**************4
//			}
//			if ((CHARGE_INPUT == 1) && (LOCK == 1) && (CHAR_C == 2))  //插上电
//					{
//				LOCK = 1;  //5
//			}
//			if ((CHARGE_INPUT == 1) && (LOCK == 0) && (CHAR_C == 2)) {
//				LOCK = 0;  //6
//			}
//			if ((CHARGE_INPUT == 0) && (CHAR_B == 1) && (LOCK == 1)) {
//				LOCK = 1;
//				CHAR_B = 0;
//				CHAR_C = 0;  //7 JIA LOCK=1
//			}
//			if ((CHARGE_INPUT == 0) && (LOCK == 0) && (CHAR_B == 1))  //
//					{
//				LOCK = 0;
//				CHAR_B = 0;
//				CHAR_C = 0;  //、、**************8
//			}
//
//			DELAY_02S++;
//			DIS_DELAY_500ms++;
//			delay1s++;
//			lock_display_time++;
//			dalay_time_10ms++;
//			if (POWER_ON_FLAG == 4)
//				display_time++;
//			//else display_time=300;
//
//			if (VOL_FLAG == 1)
//				dalay_time_1ms++;
//			else {
//				dalay_time_1ms = 0;
//			}
//
//			/*	if((BAT==3)&&(POWER_ON_FLAG!=0)) //低于10%//////////**********************888
//			 {
//			 persent_10_time++;
//			 if(persent_10_time>=50)
//			 {
//			 persent_10_time=0;
//			 DIS2=~DIS2;
//			 }
//			 }*/
//
//			if (DELAY_02S >= 30)              //如果300ms到
//					{
//				DELAY_02S = 0;			//双击标志位
//				data_write();
//				if (POWER_ON_FLAG != 4)
//					ADC_Read();			//查看电量时不采样电
//			}
//
//			if ((POWER_ON_FLAG == 0) && (SW_OF == 1) && (SW_OF == 1)
//					&& (CHARGE_INPUT == 0))  //关机状态下，2秒不动作，则进入休眠状态。
//					{
//				SLEEP_TIME++;
//				if (SLEEP_TIME >= 300)
//					sleep_flag = 0;  //进入休眠
//			} else
//				SLEEP_TIME = 0;
//
//			key_number = Key_scan_ON();
//		}
//
//		switch (POWER_ON_FLAG)      //根据开机标示执行，上电进入休眠状态
//		{
//		case 0:
//			if (sleep_flag == 1) {
//
//				if ((key_number == 6) && (LOCK == 0))      //单击副开关
//						{
//					POWER_ON_FLAG = 4;     	//显示2秒电量
//					break;
//				}
//				if ((key_number == 2) && (LOCK == 0))	//S1按下 长按0.5S  开机，进入照明模式
//						{
//					POWER_ON_FLAG = 1;     	//转换到主灯
//					key_number = 0;
//					data_temp = 255;
//					temp2 = 0;
//					sleep_flag = 0;
//				} else if ((key_number == 7) && (LOCK == 0))    //关机-->爆闪
//						{
//					if (CHARGE_INPUT == 0) {
//						POWER_ON_FLAG = 1;     	//转换到主灯
//						key_number = 0;
//						data_temp = 255;
//						temp2 = 0;
//						sleep_flag = 0;
//					} else {
//						key_number = 0;
//						data_temp = 255;
//						POWER_ON_FLAG = 2;
//						temp2 = 0;
//						data2 = 0;
//						temp5 = 0;				//重新设置亮度
//						sleep_flag = 0;
//						stop_run_flash = 0;
//					}
//					if (CHARGE_INPUT == 0) {
//						key_number = 0;
//						data_temp = 255;
//						POWER_ON_FLAG = 2;
//						temp2 = 0;
//						data2 = 0;
//						temp5 = 0;				//重新设置亮度
//						sleep_flag = 0;
//						stop_run_flash = 0;
//					} else {
//						POWER_ON_FLAG = 1;     	//转换到主灯
//						key_number = 0;
//						data_temp = 255;
//						temp2 = 0;
//						sleep_flag = 0;
//					}
//				} else if (key_number == 8) {
//					key_number = 0;
//					lock_display_time = 0;
//					LOCK = !LOCK;		//锁定/解锁    LOCK==0;解锁××××××LOCK==1；锁键
//					if (LOCK == 1)
//						POWER_ON_FLAG = 3; //如果是在关机状态下进行锁键，则闪烁两次，然后熄灭，提示已经处于锁定状态。
//					else {
//						POWER_ON_FLAG = 1;		//解锁，进入照明模式
//						data_temp = 255;
//						temp2 = 1;  	 		//解锁后进入照明，防止需重复按两次才能关机
//						sleep_flag = 0;
//					}
//				} else if (((key_number == 2) && (LOCK == 1))
//						|| ((key_number == 1) && (LOCK == 1))
//						|| ((key_number == 6) && (LOCK == 1))
//						|| ((key_number == 7) && (LOCK == 1))) //如果锁定状态，单击任意键，提示一次锁定状态
//						{
//					lock_display_time = 0;
//					number1 = 1;
//					POWER_ON_FLAG = 3;    //如果是在关机状态下进行锁键，则闪烁两次，然后熄灭，提示已经处于锁定状态。
//				}
//
//				if (CHARGE_INPUT == 1) {   // POWER_ON_FLAG=1;
//
//					input_flag1 = 1;   //cuowu
//					if (CHARGE_STATE == 0) {
//						DIS1 = 0;
//						DIS2 = 1;
//					} else {
//						DIS1 = 1;
//						DIS2 = 0;
//					}
//				}  //充电显示
//				else {
//
//					if (input_flag1 == 1) {
//
//						input_flag1 = 0;
//						//RESET();
//						tsoftReset();
//						goto softReset;
//					}
//					if ((key_number == 2) && (LOCK == 0)) //S1按下 长按0.5S  开机，进入照明模
//
//							{
//						POWER_ON_FLAG = 1;     	//转换到主灯模式1
//						key_number = 0;          //按键返回值清
//						data_temp = 255;
//						temp2 = 0;  	              //副键状态为0；
//						sleep_flag = 0;             //解除睡眠
//					} else if ((key_number == 7) && (LOCK == 0))    //关机-->爆闪
//							{
//						if (CHARGE_INPUT != 1) {
//							key_number = 0;
//							data_temp = 255;
//							POWER_ON_FLAG = 2;
//							temp2 = 0;
//							data2 = 0;
//							temp5 = 0;		//重新设置亮度
//							sleep_flag = 0;
//							stop_run_flash = 0;
//						} else {
//							POWER_ON_FLAG = 1;     	//转换到主灯
//							key_number = 0;
//							data_temp = 255;
//							temp2 = 0;
//							sleep_flag = 0;
//						}
//					}
//					DIS1 = 0;
//					DIS2 = 0;
//				}
//			} else {
//				AD_FLAG = 0;
//				mode_return_flag = 0;			//模式返回标志位4：照明-->爆闪，
//				SLEEP_TIME = 0;
//
//				PWM_OUT = 0;            //PWM输出低电平
//				CCPR1L = 0;
//				LATA0 = 0;
//
//				LED_ON1 = 0;           //关闭驱动的使能端，使待机功耗最小
//				TMR1ON = 0;
//				temp2 = 0;
//				ADON = 0;           //关闭AD
//				FVREN = 0;           //关闭内部参考电压
//				FVRRDY = 0;  //不使能参考电压
//				ADFVR0 = 0;
//				ADFVR1 = 0;  //固定参考电压guanbi
//				TMR2ON = 0;
//				data_temp2 = 0;
//				WPUA = 0X06;
//				WPUC = 0X00;
//				TRISA = 0XFF;
//				TRISC = 0X00;  //FF*******
//				RA0 = 0;
//				CHARGE_PUTIN_FLAG = 0;				//清充电标志
//				CCPR1L = 0;
//				PORTA = 0x06;
//				PORTC = 0x00;
//				LATA = 0X06;
//				LATC = 0X00;
//				LATA1 = 1;
//				LATA2 = 1;
//
//				WPUA1 = 1;
//				WPUA2 = 1;
//				TRISA1 = 1;
//				TRISA2 = 1;
//
//				stop_run_flash = 0;
//				AD_SW = 0;
//
//				IOCAP = 0b00100000;     //充电GP上升沿唤醒
//				IOCAN = 0b00000110;     //GP1引脚电平变化唤醒引脚使能
//				//IOCAF        = 0b00100000;     //
//				//IOCAF        = 0b00000000;     //
//				INTCON = 0b00001000;     //开外部引脚电平变化唤醒，只唤醒，不中断
//				if (CHARGE_INPUT != 1) {     //充电时，不能休眠
//					SLEEP();                 		//休眠
//					;                             //空操作，睡眠后预取指令为空
//				}
//				IOCAF = 0b00000000;   //清唤醒标志位
//				//IOCAP  			=	0b00000000;
//				//IOCAN  			=	0b00000000;   	//清唤醒标志位
//				CUP_Init();							//唤醒后脚位重新设置
//				INTERRUPT_Init();
//				AD_Init();
//				data_temp = 255;
//				sleep_flag = 1;
//				DELAY_02S = 0;     // 清除双击标志位
//				for (i = 2000; i > 0; i--)
//					;     //必要延时
//				ADC_Read();
//				if ((data1 == 0) && (adresult <= 872))	//<3.85V
//						{
//					data1 = 4;
//					DOWN_ELE = 1;
//				}   	//休眠后空载电压检测
//				else if ((data1 == 4) && (adresult <= 845))	//<3.7
//						{
//					data1 = 3;
//					DOWN_ELE = 2;
//				}
//				/*		else if((data1 == 3)&&(adresult<=859))
//				 {
//				 data1 = 2;
//				 DOWN_ELE=3;
//				 }
//				 else if((data1 == 2)&&(adresult<=859))
//				 {
//				 data1 = 2;
//				 DOWN_ELE=4;
//				 }
//				 */
//				switch (DOWN_ELE)	// 判断电压
//				{
//				case 0:
//					break;
//				case 1:				//如果是在极亮档，则下降一档
//					if (data1 == 0) {
//						DOWN_FLAG = 1;		//标志一
//						data1 = 4;
//					}
//					if (data_temp1 == 1)		// 爆闪模式中 pwm占空比的标志位
//							{
//						data_temp1 = 2;
//						BAOSHAN_JIANDANG = 1;
//					}
//					break;
//				case 2:				//如果是在高亮档，则下降到中亮
//					if ((data1 == 4) || (data1 == 0)) {
//						DOWN_FLAG = 2;		//标志一
//						data1 = 3;
//					}
//					if ((data_temp1 == 2) || (data_temp1 == 1))	// 爆闪模式中 pwm占空比的标志位
//							{
//						data_temp1 = 3;
//						BAOSHAN_JIANDANG = 2;
//					}
//					break;
//				case 3:				//如果是在中亮档，则下降到低亮3.2
//					if ((data1 == 3) || (data1 == 4) || (data1 == 0)) {
//						DOWN_FLAG = 3;		//标志一
//						data1 = 2;
//					}
//					if ((data_temp1 == 3) || (data_temp1 == 2)
//							|| (data_temp1 == 1))		// 爆闪模式中 pwm占空比的标志位
//							{
//						data_temp1 = 4;
//						BAOSHAN_JIANDANG = 3;
//					}
//					break;
//				case 4:				//如果是在低亮档，则下降到节能
//					if ((data1 == 2) || (data1 == 3) || (data1 == 4)
//							|| (data1 == 0)) {
//						DOWN_FLAG = 4;		//标志一
//						data1 = 1;
//					}
//					if ((data_temp1 == 4) || (data_temp1 == 3)
//							|| (data_temp1 == 2) || (data_temp1 == 1))// 爆闪模式中 pwm占空比的标志位
//							{
//						data_temp1 = 5;
//						BAOSHAN_JIANDANG = 4;
//					}
//					break;
//				default:
//					break;
//				}
//			}
//			break;
//		case 1:   //模式1，照明档位
//			switch (key_number) {
//			case 0:
//				break;
//
//			case 1:   //×××××××××××××××××××××××××××××××××××换档调光（主键单击）
//				key_number = 0;         	//按键返回值清零
//				data_temp = 255;       			//每次有按键改变，都要重新进入开关语句
//				dalay_time_10ms = 0;
//				data1++;
//				if (data1 >= 5)
//					data1 = 0;
//				if (((data1 == 0) && (DOWN_FLAG == 1))
//						|| (DOWN_ELE == 1) && ((data1 == 0)))
//					data1 = 1;   //如果条件，跳过极亮
//				if (((data1 == 4) && (DOWN_FLAG == 2))
//						|| (DOWN_ELE == 2) && ((data1 == 4)))
//					data1 = 1;   //如果条件，跳过高亮
//				if (((data1 == 3) && (DOWN_FLAG == 3))
//						|| (DOWN_ELE == 3) && ((data1 == 3)))
//					data1 = 1;   //如果条件，跳过中亮
//				if (((data1 == 2) && (DOWN_FLAG == 4))
//						|| (DOWN_ELE == 4) && ((data1 == 2)))
//					data1 = 1;   //如果条件，跳过低亮
//				break;
//			case 6:   //×××××××××××××××××××××××××××××××极亮（副键单击）
//				key_number = 0;
//				POWER_ON_FLAG = 2;
//				data_temp2 = 0;
//				dalay_time_1ms = 0;
//				dalay_time_10ms = 0;
//				FC_FALSH_DATA = 0;
//				bright_time = 0;
//				SOS_DATA = 0;
//				data2 = 0;
//				SOS1 = 0;
//				SOS2 = 0;
//				temp5 = 0;						//重新设置亮度
//				stop_run_flash = 0;   //照明模式进入爆闪模式，初始化一次设置
//				break;
//			case 7:   //×××××××××××××××××××××××××××××××××××关闭LED（主键0.5秒）
//				key_number = 0;         //按键返回值清零
//				CCPR1L = 0;
//				LED_ON1 = 0;
//				break;
//			case 9:         //×××××××××××××××××××××××××××××××××××关机（主键0.5秒）
//				key_number = 0;
//				if (temp2 == 1) //按键返回值清零	//如果关机，temp2==1；则关机清零，否则temp2==0时 将temp2置1
//						{
//					data_temp = 255;       //每次有按键改变，都要重新进入开关语句
//					PWM_OUT = 0;                        //PWM输出低电平
//					LED_ON1 = 0;
//					POWER_ON_FLAG = 0;
//					stop_run_flash = 0;
//					data_temp = 255;
//					temp2 = 0;
//					if (input_flag1 == 1) {
//						input_flag1 = 0;
//						tsoftReset();                        //            8.4
//						goto softReset;
//					}
//				}
//				if (temp2 == 0) 			 //如果长按0.5s关机，
//						{
//					temp2 = 1;
//				}
//				break;
//			case 8: 		//×××××××××××××××××××××××××××××××××××锁键状态（主副按下2秒响应）
//				key_number = 0;
//				number1 = 0;
//				data_temp = 255;
//				LOCK = !LOCK;
//				lock_display_time = 0;
//				POWER_ON_FLAG = 3;
//				break;
//			case 2: 		//×××××××××××××××××××××××××××××××××××关闭LED（主键0.5秒）
//				key_number = 0;         //按键返回值清零
//				PWM_OUT = 0;                        //PWM输出低电平
//				LED_ON1 = 0;
//				break;
//			case 4:              //×××××××××××××××××××××××××××××××××××关机（主键0.5秒）
//				key_number = 0;
//				if (temp2 == 1) //按键返回值清零	//如果关机，temp2==1；则关机清零，否则temp2==0时 将temp2置1
//						{
//					data_temp = 255;       //每次有按键改变，都要重新进入开关语句
//					PWM_OUT = 0;                        //PWM输出低电平
//					LED_ON1 = 0;
//					POWER_ON_FLAG = 0;
//					data_temp = 255;
//					temp2 = 0;			 //temp2=0；清零
//					if (input_flag1 == 1) {
//						input_flag1 = 0;
//						tsoftReset();			 //         8.4
//						goto softReset;
//					}
//				}
//				if (temp2 == 0) 			//如果长按0.5s关机，tem
//						{
//					temp2 = 1;
//				}
//				break;
//			default:
//				key_number = 0;
//				break;   //其他情况，跳出
//			}
//			lighting_MODE();
//			break;
//		case 2:   //锁键开机提醒转接程序
//			FLASH_BS_SOS_MODE();
//			if (CHARGE_INPUT == 1)	//******zzc充电介入
//
//					{ // key_number=1;//
//				key_state = key_state_1;     //爆闪时充电介入!
//				//	POWER_ON_FLAG    =  1 ;     //强行转换为低档
//				data1 = 2;
//				//常亮模式
//				if (CHARGE_STATE == 0) {
//					DIS1 = 0;
//					DIS2 = 1;
//				} else {
//					DIS1 = 1;
//					DIS2 = 0;
//				}
//			}
//			break;
//		case 3:		//
//			LCOK_prompt();
//			break;
//		case 4:		//显示电量
//			//		if(CHARGE_INPUT!=1)//充电不显电
//			DISPLAY_ELV();
//			if ((key_number == 2) && (LOCK == 0))		//S1按下 长按0.5S  开机，进入照明模
//					{
//				POWER_ON_FLAG = 1;     	//转换到主灯模式1
//				key_number = 0;          //按键返回值清
//				data_temp = 255;
//				temp2 = 0;  	              //副键状态为0；
//				sleep_flag = 0;             //解除睡眠
//			} else if ((key_number == 7) && (LOCK == 0))    //关机-->爆闪
//					{
//				if (CHARGE_INPUT != 1) {
//					key_number = 0;
//					data_temp = 255;
//					POWER_ON_FLAG = 2;
//					temp2 = 0;
//					data2 = 0;
//					temp5 = 0;		//重新设置亮度
//					sleep_flag = 0;
//					stop_run_flash = 0;
//				} else {
//					POWER_ON_FLAG = 1;     	//转换到主灯
//					key_number = 0;
//					data_temp = 255;
//					temp2 = 0;
//					sleep_flag = 0;
//				}
//			}
//			break;
//		default:
//			break;
//		}
//	}
//}
//
//void tsoftReset() {
//	key_state = 0;
//	stime = 0;                        //2ms定时次数存放寄存器
//	POWER_ON_FLAG = 0;                        //单片机工作状态寄存器
//	BAT = 0;				   						//电量显示寄存器
//	count_temp1 = 0;                        //  主按键时间计数寄存器
//	count_temp2 = 0;                        //  副按键时间计算寄存器
//	count_temp3 = 0;                        //  主副按键同时按下，时间计算寄存器
//	bright_time = 0;   					//  爆闪记时
//	address1 = 0x0010;                  //档位记忆地址
//	address2 = 0x0020;                  //档位记忆地址
//	temp1 = 0;                        //常亮档位变量临时存放寄存器
//	temp2 = 0;                        //常亮档位变量临时存放寄存器
//	DELAY_02S = 0;							//0.2ms delay
//	key_number = 0;                        //按键值存放寄存器
//	data_temp = 8;                        //PWM变量存储寄存器
//	dalay_time_10ms = 0;                        //10mS完成次数寄存器
//	lock_display_time = 0;							//开锁键提示
//	sleep_flag = 0;
//	//	data1           =0; //此不能清
//	data2 = 0;
//	adresult = 0;
//	VOL_FLAG = 0;
//	Double_flag = 0;
//	//	LOCK=0;				//默认不锁键，每次重新上电表示解锁，档锁键时：LOCK==1
//	number1 = 0;		    //
//	PWM_DATA = 0;         //pwm占空比初值
//	AUTO_TEMP_TIME = 0;   //智能降档时间
//	SOS1 = 0;
//	SOS2 = 0;
//	SOS_DATA = 0;                        //SOS内部次序寄存器
//	FC_FALSH_DATA = 0;
//	data_temp1 = 1;                        //闪烁档位PWM变量临时存储寄存器
//	data_temp2 = 0;                        //闪烁档位PWM变量临时存储寄存器
//	temp3 = 0;                        //双击完成标示临时存放寄存器
//	dalay_time_1ms = 0;                        //1mS完成次数寄存器
//	BAT_VO_CH = 0;
//	FC_FALSH_FLAG = 0;                        //爆闪工作状态寄存器
//	Input_voltage_low = 0;
//	temp5 = 254;                        //档位复位临时存放寄存器
//	stop_run_flash = 0;				//关闭LED时，停止运行爆闪
//	MIRCOR = 0;                       //充电插入标记
//	STOP_RUN_FLASH = 0;			//关闭LED时，停止运行爆闪
//	CHARGE_PUTIN_FLAG = 0;			// //×××充电插入标记
//	CHARGE_DONE_FLAG = 0;			//××××充电状态标记
//	DISPLAY_ELE = 0;
//	display_time = 0;
//	time_2s = 0;
//	time_10s = 0;
//	time_2min = 0;
//	time_3min = 0;			//
//	time_2min_flag = 0;
//	time_3_min_flag = 0;
//
//	DOWN_FLAG = 0;				//各档位，降档标志位
//	DOWN_ELE = 0;				//降档电压位
//
//	REMENBER_data_temp1 = 0;		//
//	BAOSHAN_JIANDANG = 0;			//爆闪减档标志
//	add_flag = 0;					//如果加了，就标志
//	RESET_LOCK = 0;				//
//	RESET_ELE = 0;				//据电压档位标志复位标记
//	CHAR_B = 0;
//	CHAR_C = 0;
//	persent_10_time = 0;
//
//	input_flag1 = 0;
//	Ji_down = 0;			//
//	AD_FLAG = 0;			//
//	AD_FLAG1 = 0;
//	mode_return_flag = 0;  //单击时：模式返回标志
//	DIS_FLAG = 0;			//
//	DIS_TIME = 0;			//
//	DIS_DELAY_500ms = 0;	//
//	//	delay1s=0;			//
//	SLEEP_TIME = 0;		//
//}
////*******************************
////                              *
////  2016.8.18测试ok 没全程显示电量         *
////    充电没问题，                          *
////*******************************
