/////////////////////////////////////////////////////////////////////
// 文件名                  :    Protocol.h
//-------------------------------------------------------------------
// 目的及主要功能          :    定义协议层不同协议类及其类型处理函数
//                              包括02版101处理类，光芒网络规约处理类
//                              07版101规约处理类，104规约处理类
//-------------------------------------------------------------------
// 创建日期                :    2009.9.3
/////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <vector>

/////////////////////////遥信///////////////////////
const int _101_M_SP_NA_1 = 0x01;	//=1	单点遥信
const int _101_M_SP_TA_1 = 0x02;	//=2	带CP24Time的单点遥信
const int _101_M_DP_NA_1 = 0x03;	//=3	双点遥信
const int _101_M_DP_TA_1 = 0x04;	//=4	带CP24Time的双点遥信
const int _101_M_SP_TB_1 = 0x1E;	//=30	带CP56Time的单点遥信
const int _101_M_DP_TB_1 = 0x1F;	//=32	带CP56Time的双点遥信

/////////////////////////遥测///////////////////////
const int _101_M_ME_NA_1 = 0x09;	//=9	归一化遥测值
const int _101_M_ME_TA_1 = 0x0A;	//=10	带CP24Time归一化遥测值
const int _101_M_ME_NB_1 = 0x0B;	//=11	标度化遥测值
const int _101_M_ME_TB_1 = 0x0C;	//=12	带CP24Time标度化遥测值
const int _101_M_ME_ND_1 = 0x15;	//=21	不带品质描述词的归一化遥测值
const int _101_M_ME_TD_1 = 0x22;	//=34	带CP56Time的归一化遥测值
const int _101_M_ME_TE_1 = 0x23;	//=35	带CP56Time的标度化遥测值

/////////////////////////系统命令///////////////////////////////
const int _101_M_EI_NA_1 = 0x46;	//=70	初始化结束
const int _101_C_IC_NA_1 = 0x64;	//=100	全召
const int _101_C_CI_NA_1 = 0x65;	//=101	召电度
const int _101_C_CS_NA_1 = 0x67;	//=103	对时

/////////////////////////遥控////////////////////////////////
const int _101_C_SC_NA_1 = 0x2D;	//=45	遥控单命令下发
const int _101_C_DC_NA_1 = 0x2E;	//=46	遥控双命令下发
const int _101_C_RC_NA_1 = 0x2F;    //=47	遥调命令下发

/////////////////////////电度////////////////////////////////
const int _101_M_IT_NA_1 = 0x0F;	//=15	累计量
const int _101_M_IT_TB_1 = 0x25;	//=37	带CP56Time的累计量

/////////////////////////设定命令////////////////////////////////
//const int _101_C_SE_NA_1 = 0x30;	//=48	设定归一化值命令
const int _101_C_SE_NB_1 = 0x31;	//=49	设定标度化值命令
//const int _101_C_SE_NC_1 = 0x32;	//=50	设定短浮点值命令

//const int _101_P_ME_NA_1 = 0x6E;	//=110	设定归一化值命令
const int _101_P_ME_NB_1 = 0x6F;	//=111	设定标度化值命令
//const int _101_P_ME_NC_1 = 0x70;	//=112	设定短浮点值命令
//const int _101_P_AC_NA_1 = 0x71;	//=113	参数激活
/////////////////////////文件传输////////////////////////////////
const int _101_F_FR_NA_1 = 0x78;	//=
const int _101_F_SR_NA_1 = 0x79;	//=
const int _101_F_SC_NA_1 = 0x7A;	//=
const int _101_F_LS_NA_1 = 0x7B;	//=
const int _101_F_AF_NA_1 = 0x7C;	//=
const int _101_F_SG_NA_1 = 0x7D;	//=
const int _101_F_DR_TA_1 = 0x7E;	//=
////////////////自定义部分/////////////////////////////////////////////
const int _101_ASDU_129	= 0x81;		//=
const int _101_ASDU_130	= 0x82;		//=
const int _101_ASDU_131	= 0x83;		//=
const int _101_ASDU_132	= 0x84;		//=
const int _101_ASDU_133	= 0x85;		//=
const int _101_ASDU_134	= 0x86;		//=
const int _101_ASDU_135	= 0x87;		//=
const int _101_ASDU_136	= 0x88;		//=
const int _101_ASDU_137	= 0x89;		//=
const int _101_ASDU_138	= 0x8A;		//=
const int _101_ASDU_139	= 0x8B;		//=
const int _101_ASDU_140	= 0x8C;		//=
const int _101_ASDU_141	= 0x8D;		//=
const int _101_ASDU_142	= 0x8E;		//=
const int _101_ASDU_143	= 0x9F;		//=
const int _101_ASDU_144	= 0x90;		//=
const int _101_ASDU_145	= 0x91;		//=
const int _101_ASDU_146	= 0x92;		//=
const int _101_ASDU_147	= 0x93;		//=
const int _101_ASDU_148	= 0x94;		//=
const int _101_ASDU_149	= 0x95;		//=
const int _101_ASDU_150	= 0x96;		//=
/////////////////////////////////////////////////////////////

struct _VSQ
{
	unsigned char	SQ:1;
	unsigned char	N:7;
};
struct _COT
{
	unsigned char	T:1;
	unsigned char	PN:1;
	unsigned char	Cot:6;
	unsigned char	OriginAddr;
	unsigned short	C_ADDR;		//源地址
};

struct CP56Time2a//_TIME7
{
	unsigned char	Year;
	unsigned char	Month;
	unsigned char	Day;
	unsigned char	Hour;
	unsigned char	Min;
	unsigned char	Sec;
	unsigned short	msec;
};
struct CP32Time2a//_TIME4
{
	unsigned char	Hour;
	unsigned char	Min;
	unsigned char	Sec;
	unsigned short	msec;
};

struct CP24Time2a//_TIME4
{
	unsigned char	Min;
	unsigned char	Sec;
	unsigned short	msec;
};

class ASDU 
{
public:
	ASDU() {data.clear();}
	~ASDU(){data.clear();}
	unsigned char	RES1;			//ACD
	unsigned char	RES2;			//Addr
	unsigned char	RES3;
	unsigned char	TYPE_ID;		//asdu标识
	struct _VSQ		VSQ;			//可变结构限定词
	struct _COT		COT;			//传送原因
	unsigned char	ASDUAddr[2];	//应用服务数据公共地址
	std::vector<unsigned char> data;
};
class CProtocol02_101
{
public:
	unsigned char linkaddr;
	unsigned char sumcheck;
	unsigned char control_code;
	unsigned char fun_code;
	unsigned char DIR;
	CP56Time2a Time7;
	CP32Time2a Time4;
	CP24Time2a Time3;
	CString strLPDU;
	CString strVSQ;
	CString strCOT;
	CString strSumcheck;

public:
	unsigned char	Control_Data104[8];

private:
	int COT_lenNum;//传送原因长度参数 取值范围（1，2）
	int ASDUA_lenNum;//应用服务数据单元公共地址长度参数取值范围（1，2）
	int InfoOBJ_lenNum;//信息对象地址长度参数取值范围（1，2，3）

public:
	CProtocol02_101(int COT_Num,int ASDUA_Num,int InfoOBJ_Num);
	CProtocol02_101();
	virtual ~CProtocol02_101();
	
	CString ParsePLDU_FCCode(unsigned char dir, unsigned char fccode);
	CString ParseASDU_COT(_COT cot);//传送原因
	CString ParseASDU_VSQ(_VSQ vsq);//可变结构限定词
	
	CString Pmain02_101(unsigned char *pdata, int len);	//解析02版101
	CString Pmain97_101(unsigned char *pdata, int len);//解析97版101
	CString Pmain104(unsigned char *pdata, int len);//解析104


	CString ParseASDU_IEC101_02_M_SP_NA_1(ASDU asdu);//Type = 1   无时标单点遥信
	CString ParseASDU_IEC101_02_M_SP_TA_1(ASDU asdu);//Type = 2   带CP24Time单点遥信
	CString ParseASDU_IEC101_02_M_DP_NA_1(ASDU asdu);//Type = 3   无时标双点遥信
	CString ParseASDU_IEC101_02_M_DP_TA_1(ASDU asdu);//Type = 4   带CP24Time双点遥信

	CString ParseASDU_IEC101_02_M_ME_NA_1(ASDU asdu);//Type = 9   测量值，无时标的归一化值
	CString ParseASDU_IEC101_02_M_ME_TA_1(ASDU asdu);//Type = 10  测量值，有时标的归一化值
	CString ParseASDU_IEC101_02_M_ME_NB_1(ASDU asdu);//Type = 11  测量值，无时标的标度化值
	CString ParseASDU_IEC101_02_M_ME_TB_1(ASDU asdu);//Type = 12  测量值，有时标的标度化值

	CString ParseASDU_IEC101_02_M_IT_NA_1(ASDU asdu);//Type = 15  累计量

	CString ParseASDU_IEC101_02_M_ME_ND_1(ASDU asdu);//Type = 21  测量值，不带品质描述词的的归一化值

	CString ParseASDU_IEC101_02_M_SP_TB_1(ASDU asdu);//Type = 30  带CP56Time单点遥信变位上送
	CString ParseASDU_IEC101_02_M_DP_TB_1(ASDU asdu);//Type = 31  带CP56Time双点遥信变位上送

	CString ParseASDU_IEC101_02_M_ME_TD_1(ASDU asdu);//Type = 34  带CP56Time归一化值上送
	CString ParseASDU_IEC101_02_M_ME_TE_1(ASDU asdu);//Type = 35  带CP56Time标度化值上送

	CString ParseASDU_IEC101_02_M_IT_TB_1(ASDU asdu);//Type = 37  带CP56Time累加量上送

	CString ParseASDU_IEC101_02_C_SC_NA_1(ASDU asdu);//Type = 45  单命令
	CString ParseASDU_IEC101_02_C_DC_NA_1(ASDU asdu);//Type = 46  双命令
	CString ParseASDU_IEC101_02_C_RC_NA_1(ASDU asdu);//Type = 47  步调命令

	CString ParseASDU_IEC101_02_C_SE_NB_1(ASDU asdu);//Type = 49  设定标度化命令

	CString ParseASDU_IEC101_02_M_EI_NA_1(ASDU asdu);//Type = 70  初始化结束

	CString ParseASDU_IEC101_02_C_IC_NA_1(ASDU asdu);//Type = 100 站总召唤命令
	CString ParseASDU_IEC101_02_C_CI_NA_1(ASDU asdu);//Type = 101 召累计量

	CString ParseASDU_IEC101_02_C_CS_NA_1(ASDU asdu);//Type = 103 对时命令

	CString ParseASDU_IEC101_02_P_ME_NB_1(ASDU asdu);//Type = 111 测量参数标度化值

	CString ParseASDU_QDS(int type,unsigned char qds);//带品质描述词的单点信息type=1SIQ type=2DIQ type=3QDS type=4QDP 
	CString ParseASDU_DCO(int type,unsigned char StateData);//双命令 type=1DCO type=2RCO
	CString ParseASDU_BCR(unsigned char bcr);//累计量
	unsigned char Add_Sum_Check(unsigned char *data, unsigned char data_len);

};
class CProtocolGMWLGY
{
public:

	CProtocolGMWLGY();
	virtual ~CProtocolGMWLGY();

	CString PmainGMWLGY(unsigned char *pdata, int len);
	
public:
	CString FunStr;
	CString Strtemp;
};