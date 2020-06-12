#ifndef _MT_API_
#define _MT_API_
/*
C接口文件
V3.12.1  updated at 2016 5 9
*/
#pragma comment(lib,"MT_API.lib")
#ifdef __cplusplus
#define MT_API extern "C" __declspec( dllexport )
#else
#define MT_API __declspec( dllexport )
#endif

#define R_OK   0
#define RUN_OFF   0
#define RUN_ON   1
#define DIR_NEG   0
#define DIR_POS   1
#define LIMIT_ON   1
#define LIMIT_OFF   0
#define RESOURCE_MOTOR   0x00000001
#define RESOURCE_TTL_IN   0x00000002
#define RESOURCE_TTL_OUT   0x00000004
#define RESOURCE_OPTIC_IN   0x00000008
#define RESOURCE_OPTIC_OUT   0x00000010
#define RESOURCE_AD   0x00000020
#define RESOURCE_OC   0x00000040
#define RESOURCE_LINE   0x00000080
#define RESOURCE_CIRCLE   0x00000100
#define RESOURCE_PLC   0x00000200
#define RESOURCE_STREAM   0x00000400
#define RESOURCE_ENCODER   0x00000800

MT_API INT32 __stdcall MT_Init(void);

MT_API INT32 __stdcall MT_DeInit(void);

MT_API INT32 __stdcall MT_Get_Dll_Version(char** sVer);

MT_API INT32 __stdcall MT_Set_Thread_Timeout(UINT32 ms);

MT_API INT32 __stdcall MT_Open_UART(char* sCOM);

//MT_API INT32 __stdcall MT_Open_UART_Unicode(wchar* sCOM);

MT_API INT32 __stdcall MT_Close_UART(void);

MT_API INT32 __stdcall MT_Open_USB(void);

MT_API INT32 __stdcall MT_Close_USB(void);

MT_API INT32 __stdcall MT_Open_Net(BYTE IP0,BYTE IP1,BYTE IP2,BYTE IP3,WORD IPPort);

MT_API INT32 __stdcall MT_Close_Net(void);

MT_API INT32 __stdcall MT_Check(void);

MT_API INT32 __stdcall MT_Get_Product_Resource(INT32* pValue);

MT_API INT32 __stdcall MT_Get_Product_ID(char* sID);

MT_API INT32 __stdcall MT_Get_Product_SN(char* sSN);

MT_API INT32 __stdcall MT_Get_Product_SN2(BYTE* sSN);

MT_API INT32 __stdcall MT_Get_Product_SN3(char* sSN);

MT_API INT32 __stdcall MT_Get_Product_Version(INT32* pMajor,INT32* pMinor);

MT_API INT32 __stdcall MT_Get_Product_Version2(INT32* pMajor,INT32* pMinor,INT32* pRelease,INT32* pBuild);

MT_API INT32 __stdcall MT_Get_Axis_Num(INT32* pValue);

MT_API INT32 __stdcall MT_Get_Axis_Acc(WORD AObj,INT32* pValue);

MT_API INT32 __stdcall MT_Set_Axis_Acc(WORD AObj,INT32 Value);

MT_API INT32 __stdcall MT_Get_Axis_Dec(WORD AObj,INT32* pValue);

MT_API INT32 __stdcall MT_Set_Axis_Dec(WORD AObj,INT32 Value);

MT_API INT32 __stdcall MT_Get_Axis_Mode(WORD AObj,INT32* pValue);

MT_API INT32 __stdcall MT_Set_Axis_Mode_Velocity(WORD AObj);

MT_API INT32 __stdcall MT_Set_Axis_Mode_Position(WORD AObj);

MT_API INT32 __stdcall MT_Set_Axis_Mode_Position_Open(WORD AObj);

MT_API INT32 __stdcall MT_Set_Axis_Mode_Position_Close(WORD AObj);

MT_API INT32 __stdcall MT_Get_Axis_Velocity_V_Target(WORD AObj,INT32* pValue);

MT_API INT32 __stdcall MT_Set_Axis_Velocity_V_Target_Abs(WORD AObj,INT32 Value);

MT_API INT32 __stdcall MT_Set_Axis_Velocity_V_Target_Rel(WORD AObj,INT32 Value);

MT_API INT32 __stdcall MT_Set_Axis_Velocity_Stop(WORD AObj);

MT_API INT32 __stdcall MT_Get_Axis_Position_V_Max(WORD AObj,INT32* pValue);

MT_API INT32 __stdcall MT_Set_Axis_Position_V_Max(WORD AObj,INT32 Value);

MT_API INT32 __stdcall MT_Get_Axis_Position_P_Target(WORD AObj,INT32* pValue);

MT_API INT32 __stdcall MT_Set_Axis_Position_P_Target_Abs(WORD AObj,INT32 Value);

MT_API INT32 __stdcall MT_Set_Axis_Position_P_Target_Rel(WORD AObj,INT32 Value);

MT_API INT32 __stdcall MT_Set_Axis_Position_Stop(WORD AObj);

MT_API INT32 __stdcall MT_Set_Axis_Position_Close_Dec_Factor(WORD AObj,float AFactor);

MT_API INT32 __stdcall MT_Set_Axis_Halt(WORD AObj);

MT_API INT32 __stdcall MT_Set_Axis_Halt_All(void);

MT_API INT32 __stdcall MT_Get_Axis_V_Now(WORD AObj,INT32* pValue);

MT_API INT32 __stdcall MT_Get_Axis_Software_P_Now(WORD AObj,INT32* pValue);

MT_API INT32 __stdcall MT_Set_Axis_Software_P(WORD AObj,INT32 Value);

MT_API INT32 __stdcall MT_Get_Axis_Status(WORD AObj,BYTE* pRun,BYTE* pDir,BYTE* pNeg,BYTE* pPos,BYTE* pZero,BYTE* pMode);

MT_API INT32 __stdcall MT_Get_Axis_Status2(WORD AObj,INT32* pRun,INT32* pDir,INT32* pNeg,INT32* pPos,INT32* pZero,INT32* pMode);

MT_API INT32 __stdcall MT_Get_Axis_Status_Run(WORD AObj,INT32* pRun);

MT_API INT32 __stdcall MT_Get_Axis_Status_Dir(WORD AObj,INT32* pDir);

MT_API INT32 __stdcall MT_Get_Axis_Status_Neg(WORD AObj,INT32* pNeg);

MT_API INT32 __stdcall MT_Get_Axis_Status_Pos(WORD AObj,INT32* pPos);

MT_API INT32 __stdcall MT_Get_Axis_Status_Zero(WORD AObj,INT32* pZero);

MT_API INT32 __stdcall MT_Get_Axis_Status_Mode(WORD AObj,INT32* pMode);

MT_API INT32 __stdcall MT_Get_Param_Mem_Len(INT32* pValue);

MT_API INT32 __stdcall MT_Get_Param_Mem_Data(WORD AObj,BYTE* pValue);

MT_API INT32 __stdcall MT_Set_Param_Mem_Data(WORD AObj,BYTE Value);

MT_API INT32 __stdcall MT_Get_Optic_In_Num(INT32* pValue);

MT_API INT32 __stdcall MT_Get_Optic_In_Single(WORD AObj,INT32* pValue);

MT_API INT32 __stdcall MT_Get_Optic_In_All(INT32* pValue);

MT_API INT32 __stdcall MT_Get_Optic_Out_Num(INT32* pValue);

MT_API INT32 __stdcall MT_Set_Optic_Out_Single(WORD AObj,INT32 Value);

MT_API INT32 __stdcall MT_Set_Optic_Out_All(INT32 Value);

MT_API INT32 __stdcall MT_Get_OC_Out_Num(INT32* pValue);

MT_API INT32 __stdcall MT_Set_OC_Out_Single(WORD AObj,INT32 Value);

MT_API INT32 __stdcall MT_Set_OC_Out_All(INT32 Value);

MT_API INT32 __stdcall MT_Set_Axis_Mode_Home(WORD AObj);

MT_API INT32 __stdcall MT_Set_Axis_Mode_Home_Home_Switch(WORD AObj);

MT_API INT32 __stdcall MT_Set_Axis_Mode_Home_Encoder_Index(WORD AObj);

MT_API INT32 __stdcall MT_Set_Axis_Mode_Home_Encoder_Home_Switch(WORD AObj);

MT_API INT32 __stdcall MT_Set_Axis_Home_V(WORD AObj,INT32 Value);

MT_API INT32 __stdcall MT_Set_Axis_Home_Stop(WORD AObj);

MT_API INT32 __stdcall MT_Set_Axis_Software_Limit_Neg_Value(WORD AObj,INT32 Value);

MT_API INT32 __stdcall MT_Set_Axis_Software_Limit_Pos_Value(WORD AObj,INT32 Value);

MT_API INT32 __stdcall MT_Set_Axis_Software_Limit_Enable(WORD AObj);

MT_API INT32 __stdcall MT_Set_Axis_Software_Limit_Disable(WORD AObj);

MT_API INT32 __stdcall MT_Set_Axis_Line_Axis(WORD AObj,INT32 Axis_ID0,INT32 Axis_ID1);

MT_API INT32 __stdcall MT_Set_Axis_Line_Acc(WORD AObj,INT32 Value);

MT_API INT32 __stdcall MT_Set_Axis_Line_Dec(WORD AObj,INT32 Value);

MT_API INT32 __stdcall MT_Set_Axis_Line_V(WORD AObj,INT32 Value);

MT_API INT32 __stdcall MT_Set_Axis_Line_Run(WORD AObj);

MT_API INT32 __stdcall MT_Set_Axis_Line_Stop(WORD AObj);

MT_API INT32 __stdcall MT_Set_Axis_Line_Halt(WORD AObj);

MT_API INT32 __stdcall MT_Set_Axis_Line_Rel(WORD AObj,INT32 Axis_Target0,INT32 Axis_Target1);

MT_API INT32 __stdcall MT_Set_Axis_Line_Abs(WORD AObj,INT32 Axis_Target0,INT32 Axis_Target1);

MT_API INT32 __stdcall MT_Set_Axis_Line_Run_Rel(WORD AObj,INT32 Axis_Target0,INT32 Axis_Target1);

MT_API INT32 __stdcall MT_Set_Axis_Line_Run_Abs(WORD AObj,INT32 Axis_Target0,INT32 Axis_Target1);

MT_API INT32 __stdcall MT_Get_Axis_Line_Num(INT32* pValue);

MT_API INT32 __stdcall MT_Get_Axis_Line_Status(WORD AObj,INT32* pValue);

MT_API INT32 __stdcall MT_Get_Axis_Line_Axis(WORD AObj,INT32* pID0,INT32* pID1);

MT_API INT32 __stdcall MT_Get_Axis_Line_Acc(WORD AObj,INT32* pValue);

MT_API INT32 __stdcall MT_Get_Axis_Line_Dec(WORD AObj,INT32* pValue);

MT_API INT32 __stdcall MT_Get_Axis_Line_V(WORD AObj,INT32* pValue);

MT_API INT32 __stdcall MT_Get_AD_Num(INT32* pValue);

MT_API INT32 __stdcall MT_Get_AD_Data(WORD AObj,INT32* pValue);

MT_API INT32 __stdcall MT_Get_Board_T(INT32* pValue);

MT_API INT32 __stdcall MT_Set_Axis_Circle_Axis(WORD AObj,INT32 Axis_ID0,INT32 Axis_ID1);

MT_API INT32 __stdcall MT_Set_Axis_Circle_Acc(WORD AObj,INT32 Value);

MT_API INT32 __stdcall MT_Set_Axis_Circle_Dec(WORD AObj,INT32 Value);

MT_API INT32 __stdcall MT_Set_Axis_Circle_V(WORD AObj,INT32 Value);

MT_API INT32 __stdcall MT_Set_Axis_Circle_R_CW_Run_Rel(WORD AObj,INT32 AR,INT32 Axis_Target0,INT32 Axis_Target1);

MT_API INT32 __stdcall MT_Set_Axis_Circle_R_CW_Run_Abs(WORD AObj,INT32 AR,INT32 Axis_Target0,INT32 Axis_Target1);

MT_API INT32 __stdcall MT_Set_Axis_Circle_R_CCW_Run_Rel(WORD AObj,INT32 AR,INT32 Axis_Target0,INT32 Axis_Target1);

MT_API INT32 __stdcall MT_Set_Axis_Circle_R_CCW_Run_Abs(WORD AObj,INT32 AR,INT32 Axis_Target0,INT32 Axis_Target1);

MT_API INT32 __stdcall MT_Set_Axis_Circle_Stop(WORD AObj);

MT_API INT32 __stdcall MT_Set_Axis_Circle_Halt(WORD AObj);

MT_API INT32 __stdcall MT_Get_Axis_Circle_Num(INT32* pValue);

MT_API INT32 __stdcall MT_Get_Axis_Circle_Status(WORD AObj,INT32* pValue);

MT_API INT32 __stdcall MT_Get_Axis_Circle_Axis(WORD AObj,INT32* pID0,INT32* pID1);

MT_API INT32 __stdcall MT_Get_Axis_Circle_Acc(WORD AObj,INT32* pValue);

MT_API INT32 __stdcall MT_Get_Axis_Circle_Dec(WORD AObj,INT32* pValue);

MT_API INT32 __stdcall MT_Get_Axis_Circle_V(WORD AObj,INT32* pValue);

MT_API INT32 __stdcall MT_Get_PLC_Var_Num(INT32* pValue);

MT_API INT32 __stdcall MT_Get_PLC_Var_Data(WORD AObj,INT32* pValue);

MT_API INT32 __stdcall MT_Set_PLC_Var_Data(WORD AObj,INT32 Value);

MT_API INT32 __stdcall MT_Set_PLC_Pause(void);

MT_API INT32 __stdcall MT_Set_PLC_Stop(void);

MT_API INT32 __stdcall MT_Set_PLC_Run(void);

MT_API INT32 __stdcall MT_Set_PLC_Data(WORD AObj,BYTE Value);

MT_API INT32 __stdcall MT_Get_PLC_Code_Size(INT32* pValue);

MT_API INT32 __stdcall MT_Set_Axis_Home_V_Start(WORD AObj,INT32 Value);

MT_API INT32 __stdcall MT_Set_Axis_Home_Acc(WORD AObj,INT32 Value);

MT_API INT32 __stdcall MT_Set_Axis_Home_Dec(WORD AObj,INT32 Value);

MT_API INT32 __stdcall MT_Get_Axis_Home_Acc(WORD AObj,INT32* pValue);

MT_API INT32 __stdcall MT_Get_Axis_Home_Dec(WORD AObj,INT32* pValue);

MT_API INT32 __stdcall MT_Set_Axis_Velocity_V_Start(WORD AObj,INT32 Value);

MT_API INT32 __stdcall MT_Set_Axis_Velocity_Acc(WORD AObj,INT32 Value);

MT_API INT32 __stdcall MT_Set_Axis_Velocity_Dec(WORD AObj,INT32 Value);

MT_API INT32 __stdcall MT_Get_Axis_Velocity_Acc(WORD AObj,INT32* pValue);

MT_API INT32 __stdcall MT_Get_Axis_Velocity_Dec(WORD AObj,INT32* pValue);

MT_API INT32 __stdcall MT_Set_Axis_Position_V_Start(WORD AObj,INT32 Value);

MT_API INT32 __stdcall MT_Set_Axis_Position_Acc(WORD AObj,INT32 Value);

MT_API INT32 __stdcall MT_Set_Axis_Position_Dec(WORD AObj,INT32 Value);

MT_API INT32 __stdcall MT_Get_Axis_Position_Acc(WORD AObj,INT32* pValue);

MT_API INT32 __stdcall MT_Get_Axis_Position_Dec(WORD AObj,INT32* pValue);

MT_API INT32 __stdcall MT_Set_Axis_Line_V_Start(WORD AObj,INT32 Value);

MT_API INT32 __stdcall MT_Set_Axis_Circle_V_Start(WORD AObj,INT32 Value);

MT_API INT32 __stdcall MT_Set_Axis_Line_X_Run_Rel(WORD AObj,INT32 AAxis_Num,INT32* pAxis,INT32* pTarget);

MT_API INT32 __stdcall MT_Set_Axis_Line_X_Run_Abs(WORD AObj,INT32 AAxis_Num,INT32* pAxis,INT32* pTarget);

MT_API INT32 __stdcall MT_Set_Axis_Line_X_Target_Rel(WORD AObj,INT32 AAxisID,INT32 ATarget);

MT_API INT32 __stdcall MT_Set_Axis_Line_X_Target_Abs(WORD AObj,INT32 AAxisID,INT32 ATarget);

MT_API INT32 __stdcall MT_Set_Axis_Line_X_Count(WORD AObj,INT32 ANum);

MT_API INT32 __stdcall MT_Set_Axis_Line_X_Axis(WORD AObj,INT32 AAxisID,INT32 AAxis);

MT_API INT32 __stdcall MT_Set_Stream_Run(void);

MT_API INT32 __stdcall MT_Set_Stream_Stop(void);

MT_API INT32 __stdcall MT_Set_Stream_Pause(void);

MT_API INT32 __stdcall MT_Set_Stream_Clear(void);

MT_API INT32 __stdcall MT_Get_Stream_Space(INT32* pValue);

MT_API INT32 __stdcall MT_Get_Stream_Total(INT32* pValue);

MT_API INT32 __stdcall MT_Get_Stream_Count(INT32* pValue);

MT_API INT32 __stdcall MT_Set_Stream_Delay(INT32 Value);

MT_API INT32 __stdcall MT_Set_Stream_Line_Acc(WORD AObj,INT32 Value);

MT_API INT32 __stdcall MT_Set_Stream_Line_Dec(WORD AObj,INT32 Value);

MT_API INT32 __stdcall MT_Set_Stream_Line_V_Max(WORD AObj,INT32 Value);

MT_API INT32 __stdcall MT_Set_Stream_Line_V_Start(WORD AObj,INT32 Value);

MT_API INT32 __stdcall MT_Set_Stream_Line_Stop(WORD AObj);

MT_API INT32 __stdcall MT_Set_Stream_Line_Halt(WORD AObj);

MT_API INT32 __stdcall MT_Set_Stream_Line_X_Run_Rel(WORD AObj,INT32 AAxis_Num,INT32* pAxis,INT32* pTarget);

MT_API INT32 __stdcall MT_Set_Stream_Line_X_Run_Abs(WORD AObj,INT32 AAxis_Num,INT32* pAxis,INT32* pTarget);

MT_API INT32 __stdcall MT_Set_Stream_Circle_Axis(WORD AObj,INT32 Axis_ID0,INT32 Axis_ID1);

MT_API INT32 __stdcall MT_Set_Stream_Circle_Acc(WORD AObj,INT32 Value);

MT_API INT32 __stdcall MT_Set_Stream_Circle_Dec(WORD AObj,INT32 Value);

MT_API INT32 __stdcall MT_Set_Stream_Circle_V_Max(WORD AObj,INT32 Value);

MT_API INT32 __stdcall MT_Set_Stream_Circle_V_Start(WORD AObj,INT32 Value);

MT_API INT32 __stdcall MT_Set_Stream_Circle_R_CW_Run_Rel(WORD AObj,INT32 AR,INT32 Axis_Target0,INT32 Axis_Target1);

MT_API INT32 __stdcall MT_Set_Stream_Circle_R_CW_Run_Abs(WORD AObj,INT32 AR,INT32 Axis_Target0,INT32 Axis_Target1);

MT_API INT32 __stdcall MT_Set_Stream_Circle_R_CCW_Run_Rel(WORD AObj,INT32 AR,INT32 Axis_Target0,INT32 Axis_Target1);

MT_API INT32 __stdcall MT_Set_Stream_Circle_R_CCW_Run_Abs(WORD AObj,INT32 AR,INT32 Axis_Target0,INT32 Axis_Target1);

MT_API INT32 __stdcall MT_Set_Stream_Circle_Stop(WORD AObj);

MT_API INT32 __stdcall MT_Set_Stream_Circle_Halt(WORD AObj);

MT_API INT32 __stdcall MT_Set_Stream_Wait_Line(WORD AObj);

MT_API INT32 __stdcall MT_Set_Stream_Wait_Circle(WORD AObj);

MT_API INT32 __stdcall MT_Set_Stream_OC_Out_Single(WORD AObj,INT32 Value);

MT_API INT32 __stdcall MT_Set_Stream_OC_Out_All(INT32 Value);

MT_API INT32 __stdcall MT_Set_Stream_Optic_Out_Single(WORD AObj,INT32 Value);

MT_API INT32 __stdcall MT_Set_Stream_Optic_Out_All(INT32 Value);

MT_API INT32 __stdcall MT_Set_Stream_Dec_Enable(void);

MT_API INT32 __stdcall MT_Set_Stream_Dec_Disable(void);

MT_API INT32 __stdcall MT_Get_Encoder_Num(INT32* pValue);

MT_API INT32 __stdcall MT_Get_Encoder_Pos(WORD AObj,INT32* pValue);

MT_API INT32 __stdcall MT_Set_Encoder_Pos(WORD AObj,INT32 Value);

MT_API INT32 __stdcall MT_Set_Encoder_Z_Polarity(WORD AObj,INT32 Value);

MT_API INT32 __stdcall MT_Set_Encoder_Dir_Polarity(WORD AObj,INT32 Value);

MT_API INT32 __stdcall MT_Get_Encoder_Index_Line_Count(WORD AObj,INT32* pValue);

MT_API INT32 __stdcall MT_Set_Encoder_Over_Enable(WORD AObj,INT32 Value);

MT_API INT32 __stdcall MT_Set_Encoder_Over_Max(WORD AObj,INT32 Value);

MT_API INT32 __stdcall MT_Set_Encoder_Over_Stable(WORD AObj,INT32 Value);

MT_API INT32 __stdcall MT_Help_Step_Line_Real_To_Steps(double AStepAngle,INT32 ADiv,double APitch,double ALineRatio,double AValue);

MT_API INT32 __stdcall MT_Help_Step_Circle_Real_To_Steps(double AStepAngle,INT32 ADiv,double ACircleRatio,double AValue);

MT_API double __stdcall MT_Help_Step_Line_Steps_To_Real(double AStepAngle,INT32 ADiv,double APitch,double ALineRatio,INT32 AValue);

MT_API double __stdcall MT_Help_Step_Circle_Steps_To_Real(double AStepAngle,INT32 ADiv,double ACircleRatio,INT32 AValue);

MT_API INT32 __stdcall MT_Help_Encoder_Line_Real_To_Steps(double APitch,double ALineRatio,INT32 ALineCount,double AValue);

MT_API INT32 __stdcall MT_Help_Encoder_Circle_Real_To_Steps(double ACircleRatio,INT32 ALineCount,double AValue);

MT_API double __stdcall MT_Help_Encoder_Line_Steps_To_Real(double APitch,double ALineRatio,INT32 ALineCount,INT32 AValue);

MT_API double __stdcall MT_Help_Encoder_Circle_Steps_To_Real(double ACircleRatio,INT32 ALineCount,INT32 AValue);

MT_API INT32 __stdcall MT_Help_Grating_Line_Real_To_Steps(double AUnit_um,double AValue);

MT_API INT32 __stdcall MT_Help_Grating_Circle_Real_To_Steps(INT32 ALineCount,double AValue);

MT_API double __stdcall MT_Help_Grating_Line_Steps_To_Real(double AUnit_um,INT32 AValue);

MT_API double __stdcall MT_Help_Grating_Circle_Steps_To_Real(INT32 ALineCount,INT32 AValue);

MT_API float __stdcall MT_Help_Encoder_Factor(double AStepAngle,INT32 ADiv,INT32 ALineCount);

MT_API float __stdcall MT_Help_Grating_Line_Factor(double AStepAngle,INT32 ADiv,double APitch,double ALineRatio,double AUnit_um);

MT_API float __stdcall MT_Help_Grating_Circle_Factor(double AStepAngle,INT32 ADiv,double ACircleRatio,INT32 ALineCount);

#endif

