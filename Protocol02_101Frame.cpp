/////////////////////////////////////////////////////////////////////
// 文件名                  :    Protocol02_101Frame.cpp
//-------------------------------------------------------------------
// 目的及主要功能          :    02版101协议各报文类型的处理函数实现
//-------------------------------------------------------------------
// 创建日期                :    2009.9.3
/////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Protocol101.h"

CProtocol02_101::CProtocol02_101()
{

}
CProtocol02_101::CProtocol02_101(int COT_Num,int ASDUA_Num,int InfoOBJ_Num)
{
	COT_lenNum = COT_Num;
	ASDUA_lenNum = ASDUA_Num;
	InfoOBJ_lenNum = InfoOBJ_Num;
}
CProtocol02_101::~CProtocol02_101(void)
{

}
CString CProtocol02_101::Pmain02_101(unsigned char *pdata, int len)
{
	unsigned char ucData[1000],iSumcheck;
	memcpy(ucData,pdata,len);
	CString dirstr,fcstr,strASDU,StrASDUtype,StrOBJ,Strtemp,Str02_101,StrDatahead,StrData="",StrCOMAddr;

	strSumcheck.Empty();
	
	if (ucData[len-1]==0x16)
	{
		for (int k=0;k<6;k++)
		{
			Strtemp = StrData;
			StrData.Format("%s%02XH ",Strtemp,ucData[k]);
		}
		StrDatahead.Format("<%s>起始字符(%XH) 结束字符(%XH)",StrData,ucData[0],ucData[len-1]);
		
		if ((len<6)&&(ucData[0]==0x10))//短报文
		{
			iSumcheck=Add_Sum_Check(&ucData[1],2);
			if (ucData[3]!=iSumcheck)
			{
				strSumcheck.Format("(%XH)和校验错误",iSumcheck);
			}
			control_code = ucData[1];
			linkaddr= ucData[2];
			DIR = control_code>>7;
			fun_code = control_code&0x0f;
			if (DIR==0)
			{
				dirstr="发送";
			}
			else
			{
				dirstr="接收";
			}
			fcstr = ParsePLDU_FCCode(DIR,fun_code);
			
			Str02_101.Format("〖%s站(%d)%s〗",dirstr,linkaddr,fcstr);
		}
		if (len>6&&(ucData[0]==0x68)&&(ucData[3]==0x68))//长报文
		{
			if (ucData[1]==ucData[2])
			{
				Strtemp = StrDatahead;
				StrDatahead.Format("%s 数据单元长度(%d) ",Strtemp,ucData[1]);
			}
			iSumcheck=Add_Sum_Check(&ucData[4],len-6);
  			if (ucData[len-2]!=iSumcheck)
			{
				strSumcheck.Format("(%XH)和校验错误",iSumcheck);
			}
			control_code = ucData[4];
			linkaddr= ucData[5];
			DIR = control_code>>7;
			fun_code = control_code&0x0f;
			if (DIR==0)
			{
				dirstr="发送";
			}
			else
			{
				dirstr="接收";
			}
			fcstr = ParsePLDU_FCCode(DIR,fun_code);

			ASDU asdu;
			
			asdu.RES1	= ((ucData[4]&0x20)!=0) ? 1 : 0;
			asdu.RES2	= ucData[5];			
			asdu.TYPE_ID= ucData[6];			
			asdu.VSQ.SQ	= ((ucData[7]&0x80)!=0) ? 1 : 0;
			asdu.VSQ.N	= ucData[7]&0x7F;			
			asdu.COT.T	= ((ucData[8]&0x80)!=0) ? 1 : 0;
			asdu.COT.PN	= ((ucData[8]&0x40)!=0) ? 1 : 0;
			asdu.COT.Cot= ucData[8]&0x3F;
			if (COT_lenNum==2)
			{
				asdu.COT.C_ADDR = ucData[9];
			}
			asdu.ASDUAddr[1] =0;
			for (int i=0;i<ASDUA_lenNum;i++)
			{
				asdu.ASDUAddr[i] = ucData[8+COT_lenNum+i];
			}
			StrCOMAddr.Format("公共地址(%d)",(asdu.ASDUAddr[1]<<8)|asdu.ASDUAddr[0]);
			strCOT=ParseASDU_COT(asdu.COT);
			strVSQ=ParseASDU_VSQ(asdu.VSQ);

			StrData ="";
			for (k=0;k<2+COT_lenNum+ASDUA_lenNum;k++)
			{		
				Strtemp = StrData;
				StrData.Format("%s%02XH ",Strtemp,ucData[6+k]);
			}

			asdu.data.clear();
			for (i=8+COT_lenNum+ASDUA_lenNum;i<len-2;i++)
			{
				asdu.data.push_back(ucData[i]);
			}
			//if(len>11)		asdu.data.insert(asdu.data.end(),data->pData.begin()+10,data->pData.end()-2);//到校验和
			switch(asdu.TYPE_ID)
			{
			case _101_M_SP_NA_1:	//type=1 不带时标的单点信息---查询命令回送的单点信息
				StrASDUtype = "M_SP_NA_1 无时标单点遥信";
				StrOBJ = ParseASDU_IEC101_02_M_SP_NA_1(asdu);
				break;
			case _101_M_SP_TA_1:	//type=2 带时标的单点信息---查询命令回送的单点信息
				StrASDUtype = "M_SP_TA_1 带时标单点遥信";
				StrOBJ = ParseASDU_IEC101_02_M_SP_TA_1(asdu);
				break;
			case _101_M_DP_NA_1:	//type=3 不带时标的双点信息---查询命令回送的双点信息
				StrASDUtype = "M_DP_NA_1 无时标双点遥信";
				StrOBJ = ParseASDU_IEC101_02_M_DP_NA_1(asdu);
				break;
			case _101_M_DP_TA_1:	//type=4 带时标的双点信息---查询命令回送的双点信息
				StrASDUtype = "M_DP_TA_1 带时标双点遥信";
				StrOBJ = ParseASDU_IEC101_02_M_DP_TA_1(asdu);
				break;

			case _101_M_ME_NA_1:	//type = 9  --测量值--规一化
				StrASDUtype = "M_ME_NA_1 归一化遥测";
				StrOBJ = ParseASDU_IEC101_02_M_ME_NA_1(asdu);
				break;	
			case _101_M_ME_TA_1:	//type = 10 测量值--带时标的规一化值CP24Time2a
				StrASDUtype = "M_ME_TA_1 有时标归一化遥测";
				StrOBJ = ParseASDU_IEC101_02_M_ME_TA_1(asdu);
				break;
			case _101_M_ME_NB_1:	//type=11  测量值--标度化---查询命令回送的遥测信息
				StrASDUtype = "M_ME_NB_1 标度化遥测";
				StrOBJ = ParseASDU_IEC101_02_M_ME_NB_1(asdu);
				break;
			case _101_M_ME_TB_1:	//type=12  测量值--带时标的标度化值---遥测越限
				StrASDUtype = "M_ME_TB_1 有时标标度化值";
				StrOBJ = ParseASDU_IEC101_02_M_ME_TB_1(asdu);
				break;

			case _101_M_IT_NA_1:	//type=15 累计量
				StrASDUtype = "M_IT_NA_1 电度上送";
				StrOBJ = ParseASDU_IEC101_02_M_IT_NA_1(asdu);
				break;	
				
			case _101_M_ME_ND_1:	//type=21 不带品质描述词的归一化值
				StrASDUtype = "M_ME_ND_1 遥测上送";
				StrOBJ = ParseASDU_IEC101_02_M_ME_ND_1(asdu);
				break;
			case _101_M_SP_TB_1:	//type=30 带时标的单点信息---遥信变位的单点信息CP56Time2a
				StrASDUtype = "M_SP_TB_1 单点遥信变位上送";
				StrOBJ = ParseASDU_IEC101_02_M_SP_TB_1(asdu);
				break;
			case _101_M_DP_TB_1:	//type=31 带时标的双点信息---遥信变位的双点信息CP56Time2a
				StrASDUtype = "M_DP_TB_1 双点遥信变位上送";
				StrOBJ = ParseASDU_IEC101_02_M_DP_TB_1(asdu);
				break;


			case _101_M_ME_TD_1:	//type=34  测量值--带时标的归一化值---遥测越限CP56Time2a
				StrASDUtype = "M_ME_TD_1 有时标归一化遥测";
				StrOBJ = ParseASDU_IEC101_02_M_ME_TD_1(asdu);
				break;
			case _101_M_ME_TE_1:	//type=35  测量值--带时标的标度化值---遥测越限CP56Time2a
				StrASDUtype = "M_ME_TE_1 有时标标度化遥测";
				StrOBJ = ParseASDU_IEC101_02_M_ME_TE_1(asdu);
				break;
			case _101_M_IT_TB_1:	//type=37  带时标的累计量
				StrASDUtype = "M_IT_TB_1 电度上送";
				StrOBJ = ParseASDU_IEC101_02_M_IT_TB_1(asdu);
				break;

			case _101_C_SC_NA_1:	//type=45  单点遥控
				StrASDUtype = "C_SC_NA_1 单点遥控命令";
				StrOBJ = ParseASDU_IEC101_02_C_SC_NA_1(asdu);
				break;
			case _101_C_DC_NA_1:	//type=46  双点遥控命令
				StrASDUtype = "C_DC_NA_1 双点遥控命令";
				StrOBJ = ParseASDU_IEC101_02_C_DC_NA_1(asdu);
				break;
			case _101_C_RC_NA_1:	//type=47  遥调命令
				StrASDUtype = "C_RC_NA_1 遥调命令";
				StrOBJ = ParseASDU_IEC101_02_C_RC_NA_1(asdu);
				break;
				
			case _101_C_SE_NB_1:    //type=49  设定命令
				StrASDUtype = "C_SE_NB_1 设定标度化值";
				StrOBJ = ParseASDU_IEC101_02_C_SE_NB_1(asdu);
				break;

			case _101_M_EI_NA_1:	//type = 70 初始化结束
 				StrASDUtype = "M_EI_NA_1 初始化结束";
				StrOBJ = ParseASDU_IEC101_02_M_EI_NA_1(asdu);
				break;
			case _101_C_IC_NA_1:    //type = 100 总召命令
				StrASDUtype = "C_IC_NA_1 总召命令";
				StrOBJ = ParseASDU_IEC101_02_C_IC_NA_1(asdu);
				break;
			case _101_C_CI_NA_1:    //type = 101 总召命令
				StrASDUtype = "C_CI_NA_1 召电度";
				StrOBJ = ParseASDU_IEC101_02_C_CI_NA_1(asdu);
				break;
// 			case _101_C_RD_NA_1:    //type = 102 读命令
// 				StrASDUtype = "C_RD_NA_1 读命令";
// 				StrOBJ = ParseASDU_IEC101_02_C_CI_NA_1(asdu);
// 				break;
			case _101_C_CS_NA_1:    //type = 103 时钟同步
				StrASDUtype = "C_CS_NA_1 时钟同步";
				StrOBJ = ParseASDU_IEC101_02_C_CS_NA_1(asdu);
				break;

			case _101_P_ME_NB_1:    //type = 103 时钟同步
				StrASDUtype = "P_ME_NB_1 测量参数标度化";
				StrOBJ = ParseASDU_IEC101_02_P_ME_NB_1(asdu);
				break;

// 			case _101_C_CI_NA_1:
// 				ProcessFunction_IEC101_02_C_CI_NA_1(pStationObj,asdu),data->frameNo |= _RC_Mon_Package_DD;
// 				break;
// 			case _101_F_FR_NA_1://文件准备就绪
// 				ProcessFunction_IEC101_02_F_FR_NA_1(pStationObj,asdu),data->frameNo |= _RC_Mon_Package_FILE;
// 				break;
// 			case _101_F_SR_NA_1://节准备就绪
// 				ProcessFunction_IEC101_02_F_SR_NA_1(pStationObj,asdu),data->frameNo |= _RC_Mon_Package_FILE;
// 				break;
// 			case _101_F_LS_NA_1://最后的段、节
// 				ProcessFunction_IEC101_02_F_LS_NA_1(pStationObj,asdu),data->frameNo |= _RC_Mon_Package_FILE;
// 				break;
// 			case _101_F_SG_NA_1://段传输
// 				ProcessFunction_IEC101_02_F_SG_NA_1(pStationObj,asdu),data->frameNo |= _RC_Mon_Package_FILE;
// 				break;
// 			case _101_F_DR_TA_1://目录
// 				ProcessFunction_IEC101_02_F_DR_TA_1(pStationObj,asdu),data->frameNo |= _RC_Mon_Package_FILE;
// 				break;
// 			case _101_ASDU_129:	//自检报告
// 				ProcessFunction_IEC101_02_ASDU_129(pStationObj,asdu);
// 				break;
// 			case _101_ASDU_130:	//事件报告
// 				ProcessFunction_IEC101_02_ASDU_130(pStationObj,asdu),data->frameNo |= _RC_Mon_Package_EX;
// 				break;
// 			case _101_ASDU_131:	//故障报告
// 				ProcessFunction_IEC101_02_ASDU_131(pStationObj,asdu),data->frameNo |= _RC_Mon_Package_EX;
// 				break;
// 			case _101_ASDU_132:	//负荷录波
// 				ProcessFunction_IEC101_02_ASDU_132(pStationObj,asdu),data->frameNo |= _RC_Mon_Package_EX;
// 				break;
// 			case _101_ASDU_133:
// 				ProcessFunction_IEC101_02_ASDU_133(pStationObj,asdu),data->frameNo |= _RC_Mon_Package_EX;
// 				break;
// 			case _101_ASDU_134:
// 				ProcessFunction_IEC101_02_ASDU_134(pStationObj,asdu),data->frameNo |= _RC_Mon_Package_EX;
// 				break;
// 			case _101_ASDU_135:
// 				ProcessFunction_IEC101_02_ASDU_135(pStationObj,asdu),data->frameNo |= _RC_Mon_Package_EX;
// 				break;
// 			case _101_ASDU_136:
// 				ProcessFunction_IEC101_02_ASDU_136(pStationObj,asdu),data->frameNo |= _RC_Mon_Package_EX;
// 				break;
// 			case _101_ASDU_137:
// 				ProcessFunction_IEC101_02_ASDU_137(pStationObj,asdu),data->frameNo |= _RC_Mon_Package_EX;
// 				break;
// 			case _101_ASDU_138:
// 				ProcessFunction_IEC101_02_ASDU_138(pStationObj,asdu),data->frameNo |= _RC_Mon_Package_EX;
// 				break;
// 			case _101_ASDU_139:
// 				ProcessFunction_IEC101_02_ASDU_139(pStationObj,asdu),data->frameNo |= _RC_Mon_Package_EX;
// 				break;
// 			case _101_ASDU_140:
// 				ProcessFunction_IEC101_02_ASDU_140(pStationObj,asdu),data->frameNo |= _RC_Mon_Package_EX;
// 				break;
// 			case _101_ASDU_141:
// 				ProcessFunction_IEC101_02_ASDU_141(pStationObj,asdu),data->frameNo |= _RC_Mon_Package_EX;
// 				break;
// 			case _101_ASDU_142:
// 				ProcessFunction_IEC101_02_ASDU_142(pStationObj,asdu),data->frameNo |= _RC_Mon_Package_EX;
// 				break;
// 			case _101_ASDU_143:
// 				ProcessFunction_IEC101_02_ASDU_143(pStationObj,asdu),data->frameNo |= _RC_Mon_Package_EX;
// 				break;
// 			case _101_ASDU_144:
// 				ProcessFunction_IEC101_02_ASDU_144(pStationObj,asdu),data->frameNo |= _RC_Mon_Package_EX;
// 				break;
// 			case _101_ASDU_145://读整定值
// 				ProcessFunction_IEC101_02_ASDU_145(pStationObj,asdu),data->frameNo |= _RC_Mon_Package_EX;
// 				break;
// 			case _101_ASDU_146:
// 				ProcessFunction_IEC101_02_ASDU_146(pStationObj,asdu),data->frameNo |= _RC_Mon_Package_EX;
// 				break;
// 			case _101_ASDU_147://整定值切换
// 				ProcessFunction_IEC101_02_ASDU_147(pStationObj,asdu),data->frameNo |= _RC_Mon_Package_EX;
// 				break;
// 			case _101_ASDU_148:
// 				ProcessFunction_IEC101_02_ASDU_148(pStationObj,asdu),data->frameNo |= _RC_Mon_Package_EX;
// 				break;
// 			case _101_ASDU_149:
// 				ProcessFunction_IEC101_02_ASDU_149(pStationObj,asdu),data->frameNo |= _RC_Mon_Package_EX;
// 				break;
// 			case _101_ASDU_150:
// 				ProcessFunction_IEC101_02_ASDU_150(pStationObj,asdu),data->frameNo |= _RC_Mon_Package_EX;
// 				break;
			
			default:StrASDUtype.Format("(%d)报文类型未解析",asdu.TYPE_ID);
				break;
			}

			strASDU.Format("〖<%s>(%d):%s %s %s %s〗%s",StrData,asdu.TYPE_ID,StrASDUtype,strVSQ,strCOT,StrCOMAddr,StrOBJ);
			asdu.data.clear();
			Str02_101.Format("〖%s 控制域(%XH):%s%s 站(%d)〗\r\n%s%s",StrDatahead,control_code,dirstr,fcstr,linkaddr,strASDU,strSumcheck);	
		}			
	}
	else
	{
		Str02_101.Format("请重新选择协议类型或报文数据格式错误");
	}
	return Str02_101;
}

CString CProtocol02_101::Pmain97_101(unsigned char *pdata, int len)
{
	return "1";
}


CString CProtocol02_101::Pmain104(unsigned char *pdata, int len)
{
	unsigned char ucData[1000];
	memcpy(ucData,pdata,len);
	CString strASDU="",strAPCI,strData,StrASDUtype,StrOBJ,Strtemp,Str02_101,StrCOMAddr;
	int SendNo,RecvNo;

	strSumcheck.Empty();
	
	if (ucData[0]==0x68)
	{
		strAPCI.Format("起始字符(%XH) APDU长度(%d)",ucData[0],ucData[1]);
		for (int k=0;k<6;k++)
		{		
			Control_Data104[k] = ucData[k];
			Strtemp = strData;
			strData.Format("%s%02XH ",Strtemp,Control_Data104[k]);
		}
		Strtemp = strAPCI;
		SendNo = (Control_Data104[2]&0xFE)>>1;
		RecvNo = (Control_Data104[4]&0xFE)>>1;
		if (Control_Data104[2]&0x1)
		{
			if ((Control_Data104[2]&2)==2)//U格式帧
			{
				CString UFrame;
				int StartACT=(Control_Data104[2]&4)>>2;
				int StartCON=(Control_Data104[2]&8)>>3;
				int StopACT=(Control_Data104[2]&16)>>4;
				int StopCON=(Control_Data104[2]&32)>>5;
				int TestACT=(Control_Data104[2]&64)>>6;
				int TestCON=(Control_Data104[2]&128)>>7;
				UFrame.Format("STARTDT:ACT=(%d) CON=(%d) STOPDT:ACT(%d) CON(%d) TESTFR: ACT(%d) CON(%d)",StartACT,StartCON,StopACT,StopCON,TestACT,TestCON);
				strAPCI.Format("<%s> %s U格式帧 %s",strData,Strtemp,UFrame);
			}
			else//S格式帧
			{
				RecvNo = (Control_Data104[2]&0xFE)>>1;
				strAPCI.Format("<%s> %s S格式帧 接收序号RN(%d)",strData,Strtemp,RecvNo);
			}
		} 
		else//I格式帧
		{
			strAPCI.Format("<%s> %s I格式帧 发送序号SN(%d) 接收序号RN(%d)",strData,Strtemp,SendNo,RecvNo);
		}
		if (ucData[1]>4)//I格式帧
		{
			ASDU asdu;						
			asdu.TYPE_ID= ucData[6];			
			asdu.VSQ.SQ	= ((ucData[7]&0x80)!=0) ? 1 : 0;
			asdu.VSQ.N	= ucData[7]&0x7F;			
			asdu.COT.T	= ((ucData[8]&0x80)!=0) ? 1 : 0;
			asdu.COT.PN	= ((ucData[8]&0x40)!=0) ? 1 : 0;
			asdu.COT.Cot= ucData[8]&0x3F;
			if (COT_lenNum==2)
			{
				asdu.COT.C_ADDR = ucData[9];
			}
			asdu.ASDUAddr[1]=0;
			for (int i=0;i<ASDUA_lenNum;i++)
			{
				asdu.ASDUAddr[i] = ucData[8+COT_lenNum+i];
			}

			StrCOMAddr.Format("公共地址(%d)",(asdu.ASDUAddr[1]<<8)|asdu.ASDUAddr[0]);

			strCOT=ParseASDU_COT(asdu.COT);
			strVSQ=ParseASDU_VSQ(asdu.VSQ);

			strData ="";
			for (k=0;k<2+COT_lenNum+ASDUA_lenNum;k++)
			{		
				Strtemp = strData;
				strData.Format("%s%02XH ",Strtemp,ucData[6+k]);
			}

			asdu.data.clear();
			for (i=8+COT_lenNum+ASDUA_lenNum;i<len;i++)
			{
				asdu.data.push_back(ucData[i]);
			}
			//if(len>11)		asdu.data.insert(asdu.data.end(),data->pData.begin()+10,data->pData.end()-2);//到校验和
			switch(asdu.TYPE_ID)
			{
			case _101_M_SP_NA_1:	//type=1 不带时标的单点信息---查询命令回送的单点信息
				StrASDUtype = "M_SP_NA_1 无时标单点遥信";
				StrOBJ = ParseASDU_IEC101_02_M_SP_NA_1(asdu);
				break;
			case _101_M_SP_TA_1:	//type=2 带时标的单点信息---查询命令回送的单点信息
				StrASDUtype = "M_SP_TA_1 带时标单点遥信";
				StrOBJ = ParseASDU_IEC101_02_M_SP_TA_1(asdu);
				break;
			case _101_M_DP_NA_1:	//type=3 不带时标的双点信息---查询命令回送的双点信息
				StrASDUtype = "M_DP_NA_1 无时标双点遥信";
				StrOBJ = ParseASDU_IEC101_02_M_DP_NA_1(asdu);
				break;
			case _101_M_DP_TA_1:	//type=4 带时标的双点信息---查询命令回送的双点信息
				StrASDUtype = "M_DP_TA_1 带时标双点遥信";
				StrOBJ = ParseASDU_IEC101_02_M_DP_TA_1(asdu);
				break;

			case _101_M_ME_NA_1:	//type = 9  --测量值--规一化
				StrASDUtype = "M_ME_NA_1 归一化遥测";
				StrOBJ = ParseASDU_IEC101_02_M_ME_NA_1(asdu);
				break;	
			case _101_M_ME_TA_1:	//type = 10 测量值--带时标的规一化值CP24Time2a
				StrASDUtype = "M_ME_TA_1 有时标归一化遥测";
				StrOBJ = ParseASDU_IEC101_02_M_ME_TA_1(asdu);
				break;
			case _101_M_ME_NB_1:	//type=11  测量值--标度化---查询命令回送的遥测信息
				StrASDUtype = "M_ME_NB_1 标度化遥测";
				StrOBJ = ParseASDU_IEC101_02_M_ME_NB_1(asdu);
				break;
			case _101_M_ME_TB_1:	//type=12  测量值--带时标的标度化值---遥测越限
				StrASDUtype = "M_ME_TB_1 有时标标度化值";
				StrOBJ = ParseASDU_IEC101_02_M_ME_TB_1(asdu);
				break;

			case _101_M_IT_NA_1:	//type=15 累计量
				StrASDUtype = "M_IT_NA_1 电度上送";
				StrOBJ = ParseASDU_IEC101_02_M_IT_NA_1(asdu);
				break;	
				
			case _101_M_ME_ND_1:	//type=21 不带品质描述词的归一化值
				StrASDUtype = "M_ME_ND_1 遥测上送";
				StrOBJ = ParseASDU_IEC101_02_M_ME_ND_1(asdu);
				break;
			case _101_M_SP_TB_1:	//type=30 带时标的单点信息---遥信变位的单点信息CP56Time2a
				StrASDUtype = "M_SP_TB_1 单点遥信变位上送";
				StrOBJ = ParseASDU_IEC101_02_M_SP_TB_1(asdu);
				break;
			case _101_M_DP_TB_1:	//type=31 带时标的双点信息---遥信变位的双点信息CP56Time2a
				StrASDUtype = "M_DP_TB_1 双点遥信变位上送";
				StrOBJ = ParseASDU_IEC101_02_M_DP_TB_1(asdu);
				break;


			case _101_M_ME_TD_1:	//type=34  测量值--带时标的归一化值---遥测越限CP56Time2a
				StrASDUtype = "M_ME_TD_1 有时标归一化遥测";
				StrOBJ = ParseASDU_IEC101_02_M_ME_TD_1(asdu);
				break;
			case _101_M_ME_TE_1:	//type=35  测量值--带时标的标度化值---遥测越限CP56Time2a
				StrASDUtype = "M_ME_TE_1 有时标标度化遥测";
				StrOBJ = ParseASDU_IEC101_02_M_ME_TE_1(asdu);
				break;
			case _101_M_IT_TB_1:	//type=37  带时标的累计量
				StrASDUtype = "M_IT_TB_1 电度上送";
				StrOBJ = ParseASDU_IEC101_02_M_IT_TB_1(asdu);
				break;

			case _101_C_SC_NA_1:	//type=45  单点遥控
				StrASDUtype = "C_SC_NA_1 单点遥控命令";
				StrOBJ = ParseASDU_IEC101_02_C_SC_NA_1(asdu);
				break;
			case _101_C_DC_NA_1:	//type=46  双点遥控命令
				StrASDUtype = "C_DC_NA_1 双点遥控命令";
				StrOBJ = ParseASDU_IEC101_02_C_DC_NA_1(asdu);
				break;
			case _101_C_RC_NA_1:	//type=47  遥调命令
				StrASDUtype = "C_RC_NA_1 遥调命令";
				StrOBJ = ParseASDU_IEC101_02_C_RC_NA_1(asdu);
				break;
				
			case _101_C_SE_NB_1:    //type=49  设定命令
				StrASDUtype = "C_SE_NB_1 设定标度化值";
				StrOBJ = ParseASDU_IEC101_02_C_SE_NB_1(asdu);
				break;

			case _101_M_EI_NA_1:	//type = 70 初始化结束
 				StrASDUtype = "M_EI_NA_1 初始化结束";
				StrOBJ = ParseASDU_IEC101_02_M_EI_NA_1(asdu);
				break;
			case _101_C_IC_NA_1:    //type = 100 总召命令
				StrASDUtype = "C_IC_NA_1 总召命令";
				StrOBJ = ParseASDU_IEC101_02_C_IC_NA_1(asdu);
				break;
			case _101_C_CI_NA_1:    //type = 101 总召命令
				StrASDUtype = "C_CI_NA_1 召电度";
				StrOBJ = ParseASDU_IEC101_02_C_CI_NA_1(asdu);
				break;
// 			case _101_C_RD_NA_1:    //type = 102 读命令
// 				StrASDUtype = "C_RD_NA_1 读命令";
// 				StrOBJ = ParseASDU_IEC101_02_C_CI_NA_1(asdu);
// 				break;
			case _101_C_CS_NA_1:    //type = 103 时钟同步
				StrASDUtype = "C_CS_NA_1 时钟同步";
				StrOBJ = ParseASDU_IEC101_02_C_CS_NA_1(asdu);
				break;

			case _101_P_ME_NB_1:    //type = 103 时钟同步
				StrASDUtype = "P_ME_NB_1 测量参数标度化";
				StrOBJ = ParseASDU_IEC101_02_P_ME_NB_1(asdu);
				break;

// 			case _101_C_CI_NA_1:
// 				ProcessFunction_IEC101_02_C_CI_NA_1(pStationObj,asdu),data->frameNo |= _RC_Mon_Package_DD;
// 				break;
// 			case _101_F_FR_NA_1://文件准备就绪
// 				ProcessFunction_IEC101_02_F_FR_NA_1(pStationObj,asdu),data->frameNo |= _RC_Mon_Package_FILE;
// 				break;
// 			case _101_F_SR_NA_1://节准备就绪
// 				ProcessFunction_IEC101_02_F_SR_NA_1(pStationObj,asdu),data->frameNo |= _RC_Mon_Package_FILE;
// 				break;
// 			case _101_F_LS_NA_1://最后的段、节
// 				ProcessFunction_IEC101_02_F_LS_NA_1(pStationObj,asdu),data->frameNo |= _RC_Mon_Package_FILE;
// 				break;
// 			case _101_F_SG_NA_1://段传输
// 				ProcessFunction_IEC101_02_F_SG_NA_1(pStationObj,asdu),data->frameNo |= _RC_Mon_Package_FILE;
// 				break;
// 			case _101_F_DR_TA_1://目录
// 				ProcessFunction_IEC101_02_F_DR_TA_1(pStationObj,asdu),data->frameNo |= _RC_Mon_Package_FILE;
// 				break;
// 			case _101_ASDU_129:	//自检报告
// 				ProcessFunction_IEC101_02_ASDU_129(pStationObj,asdu);
// 				break;
// 			case _101_ASDU_130:	//事件报告
// 				ProcessFunction_IEC101_02_ASDU_130(pStationObj,asdu),data->frameNo |= _RC_Mon_Package_EX;
// 				break;
// 			case _101_ASDU_131:	//故障报告
// 				ProcessFunction_IEC101_02_ASDU_131(pStationObj,asdu),data->frameNo |= _RC_Mon_Package_EX;
// 				break;
// 			case _101_ASDU_132:	//负荷录波
// 				ProcessFunction_IEC101_02_ASDU_132(pStationObj,asdu),data->frameNo |= _RC_Mon_Package_EX;
// 				break;
// 			case _101_ASDU_133:
// 				ProcessFunction_IEC101_02_ASDU_133(pStationObj,asdu),data->frameNo |= _RC_Mon_Package_EX;
// 				break;
// 			case _101_ASDU_134:
// 				ProcessFunction_IEC101_02_ASDU_134(pStationObj,asdu),data->frameNo |= _RC_Mon_Package_EX;
// 				break;
// 			case _101_ASDU_135:
// 				ProcessFunction_IEC101_02_ASDU_135(pStationObj,asdu),data->frameNo |= _RC_Mon_Package_EX;
// 				break;
// 			case _101_ASDU_136:
// 				ProcessFunction_IEC101_02_ASDU_136(pStationObj,asdu),data->frameNo |= _RC_Mon_Package_EX;
// 				break;
// 			case _101_ASDU_137:
// 				ProcessFunction_IEC101_02_ASDU_137(pStationObj,asdu),data->frameNo |= _RC_Mon_Package_EX;
// 				break;
// 			case _101_ASDU_138:
// 				ProcessFunction_IEC101_02_ASDU_138(pStationObj,asdu),data->frameNo |= _RC_Mon_Package_EX;
// 				break;
// 			case _101_ASDU_139:
// 				ProcessFunction_IEC101_02_ASDU_139(pStationObj,asdu),data->frameNo |= _RC_Mon_Package_EX;
// 				break;
// 			case _101_ASDU_140:
// 				ProcessFunction_IEC101_02_ASDU_140(pStationObj,asdu),data->frameNo |= _RC_Mon_Package_EX;
// 				break;
// 			case _101_ASDU_141:
// 				ProcessFunction_IEC101_02_ASDU_141(pStationObj,asdu),data->frameNo |= _RC_Mon_Package_EX;
// 				break;
// 			case _101_ASDU_142:
// 				ProcessFunction_IEC101_02_ASDU_142(pStationObj,asdu),data->frameNo |= _RC_Mon_Package_EX;
// 				break;
// 			case _101_ASDU_143:
// 				ProcessFunction_IEC101_02_ASDU_143(pStationObj,asdu),data->frameNo |= _RC_Mon_Package_EX;
// 				break;
// 			case _101_ASDU_144:
// 				ProcessFunction_IEC101_02_ASDU_144(pStationObj,asdu),data->frameNo |= _RC_Mon_Package_EX;
// 				break;
// 			case _101_ASDU_145://读整定值
// 				ProcessFunction_IEC101_02_ASDU_145(pStationObj,asdu),data->frameNo |= _RC_Mon_Package_EX;
// 				break;
// 			case _101_ASDU_146:
// 				ProcessFunction_IEC101_02_ASDU_146(pStationObj,asdu),data->frameNo |= _RC_Mon_Package_EX;
// 				break;
// 			case _101_ASDU_147://整定值切换
// 				ProcessFunction_IEC101_02_ASDU_147(pStationObj,asdu),data->frameNo |= _RC_Mon_Package_EX;
// 				break;
// 			case _101_ASDU_148:
// 				ProcessFunction_IEC101_02_ASDU_148(pStationObj,asdu),data->frameNo |= _RC_Mon_Package_EX;
// 				break;
// 			case _101_ASDU_149:
// 				ProcessFunction_IEC101_02_ASDU_149(pStationObj,asdu),data->frameNo |= _RC_Mon_Package_EX;
// 				break;
// 			case _101_ASDU_150:
// 				ProcessFunction_IEC101_02_ASDU_150(pStationObj,asdu),data->frameNo |= _RC_Mon_Package_EX;
// 				break;
			
			default:StrASDUtype.Format("(%d)报文类型未解析",asdu.TYPE_ID);
				break;
			}

			strASDU.Format("〖<%s>(%d):%s %s %s%s〗%s",strData,asdu.TYPE_ID,StrASDUtype,strVSQ,strCOT,StrCOMAddr,StrOBJ);
			asdu.data.clear();
		}
		Str02_101.Format("〖%s〗\r\n%s",strAPCI,strASDU);
	}
	else
	{
		Str02_101.Format("请重新选择协议类型或报文数据格式错误");
	}
	return Str02_101;
}

unsigned char CProtocol02_101::Add_Sum_Check(unsigned char *data, unsigned char data_len)
{
	unsigned char addcrc;
	for(addcrc=0;data_len>0;data_len--)addcrc+=*data++;
	return addcrc;
}

CString CProtocol02_101::ParsePLDU_FCCode(unsigned char dir, unsigned char fccode)
{
	CString fcstr;
	if (DIR==0)
	{
		switch(fun_code)
		{
		case 0:fcstr.Format(" FC%d:复位链路",fun_code);
			break;
		case 1:fcstr.Format(" FC%d:复位进程",fun_code);
			break;
		case 2:fcstr.Format(" FC%d:链路测试",fun_code);
			break;
		case 3:fcstr.Format(" FC%d:传送数据",fun_code);
			break;
		case 4:fcstr.Format(" FC%d:发送数据",fun_code);
			break;
		case 8:fcstr.Format(" FC%d:访问要求",fun_code);
			break;
		case 9:fcstr.Format(" FC%d:召链路状态",fun_code);
			break;
		case 10:fcstr.Format(" FC%d:召1级数据",fun_code);
			break;
		case 11:fcstr.Format(" FC%d:召2级数据",fun_code);
			break;
		default:fcstr.Format(" FC%d:其他类型",fun_code);
			break;
		}
	}
	else
	{
		switch(fun_code)
		{
		case 0:fcstr.Format(" FC%d:确认帧",fun_code);
			break;
		case 1:fcstr.Format(" FC%d:链路忙",fun_code);
			break;
		case 8:fcstr.Format(" FC%d:数据响应",fun_code);
			break;
		case 9:fcstr.Format(" FC%d:无召唤数据",fun_code);
			break;
		default:fcstr.Format(" FC%d:其他类型",fun_code);
			break;
		}
	}
	return fcstr;
}
CString CProtocol02_101::ParseASDU_COT(_COT cot)
{
	CString PNstr;
	CString addrstr,Strtemp;
	switch(cot.PN)
	{
	case 0:PNstr="肯定";
		break;
	case 1:PNstr="否定";
	    break;
	default:PNstr="错误";
	    break;
	}
	switch(cot.Cot)
	{
	case 0:Strtemp.Format("传送原因(%d):未用 %s",cot.Cot,PNstr);
		break;
	case 1:Strtemp.Format("传送原因(%d):周期or循环 %s",cot.Cot,PNstr);
		break;
	case 2:Strtemp.Format("传送原因(%d):背景扫描 %s",cot.Cot,PNstr);
		break;
	case 3:Strtemp.Format("传送原因(%d):突发 %s",cot.Cot,PNstr);
		break;
	case 4:Strtemp.Format("传送原因(%d):初始化 %s",cot.Cot,PNstr);
		break;
	case 5:Strtemp.Format("传送原因(%d):请求or被请求 %s",cot.Cot,PNstr);
		break;
	case 6:Strtemp.Format("传送原因(%d):激活 %s",cot.Cot,PNstr);
		break;
	case 7:Strtemp.Format("传送原因(%d):激活确认 %s",cot.Cot,PNstr);
		break;
	case 8:Strtemp.Format("传送原因(%d):停止激活 %s",cot.Cot,PNstr);
		break;
	case 9:Strtemp.Format("传送原因(%d):停止激活确认 %s",cot.Cot,PNstr);
		break;
	case 10:Strtemp.Format("传送原因(%d):激活终止 %s",cot.Cot,PNstr);
		break;
	case 11:Strtemp.Format("传送原因(%d):远方命令返送 %s",cot.Cot,PNstr);
		break;
	case 12:Strtemp.Format("传送原因(%d):当地命令返送 %s",cot.Cot,PNstr);
		break;
	case 13:Strtemp.Format("传送原因(%d):文件传输 %s",cot.Cot,PNstr);
		break;
	case 20:Strtemp.Format("传送原因(%d):响应站召唤 %s",cot.Cot,PNstr);
		break;
	case 21:;
	case 22:;
	case 23:;
	case 24:;
	case 25:;
	case 26:;
	case 27:;
	case 28:;
	case 29:;
	case 30:;
	case 31:;
	case 32:;
	case 33:;
	case 34:;
	case 35:;
	case 36:Strtemp.Format("传送原因(%d):响应第%d组召唤 %s",cot.Cot,cot.Cot-20,PNstr);
		break;
	case 37:Strtemp.Format("传送原因(%d):响应累计量站召唤 %s",cot.Cot,PNstr);
		break;
	case 38:;
	case 39:;
	case 40:;
	case 41:Strtemp.Format("传送原因(%d):响应累计量第%d组召唤 %s",cot.Cot,cot.Cot-37,PNstr);
		break;
	case 44:Strtemp.Format("传送原因(%d):未知类型标识 %s",cot.Cot,PNstr);
		break;
	case 45:Strtemp.Format("传送原因(%d):未知传送原因 %s",cot.Cot,PNstr);
		break;
	case 46:Strtemp.Format("传送原因(%d):未知装置地址 %s",cot.Cot,PNstr);
		break;
	case 47:Strtemp.Format("传送原因(%d):未知对象地址 %s",cot.Cot,PNstr);
		break;
	default:Strtemp.Format("传送原因(%d):兼容保留 %s",cot.Cot,PNstr);
		break;
	}
	if (COT_lenNum==2)
	{
		addrstr.Format("%s 源地址(%d)",Strtemp,cot.C_ADDR);
	}
	else
	{
		addrstr = Strtemp;
	}
	return addrstr;
}

CString CProtocol02_101::ParseASDU_VSQ(_VSQ vsq)
{
	CString vsqSQstr="",Strtemp;
	if (vsq.SQ)	
	{
		vsqSQstr="不";
	}
	Strtemp.Format("(%d)%s连续对象",vsq.N,vsqSQstr);
	return Strtemp;
}

CString CProtocol02_101::ParseASDU_IEC101_02_M_SP_NA_1(ASDU asdu)////Type = 1 不带时标单点遥信
{
	CString StrTemp,str,StrRe="",QDSstr;
	int OBJAddr,Value;
	unsigned char objTemp[10];
	if (asdu.VSQ.SQ)//连续
	{
		OBJAddr=0;
		for (int k=0;k<InfoOBJ_lenNum;k++)
		{
			OBJAddr += (asdu.data[k]<<(8*k));//
		}
		//OBJAddr = asdu.data[0]|(asdu.data[1]<<8);
		for (int i=0;i<asdu.VSQ.N;i++)
		{
			Value = asdu.data[i+InfoOBJ_lenNum];
			QDSstr = ParseASDU_QDS(1,Value);
			CString	strobjdata = "";
			for (int j=0;j<1;j++)
			{
				StrTemp = strobjdata;
				strobjdata.Format("%s%02XH ",StrTemp,asdu.data[i+j]);
			}
			StrTemp = StrRe;
			StrRe.Format("%s \r\n【<%s> 对象码:%5d %s】",StrTemp,strobjdata,OBJAddr+i,QDSstr);
		}
	} 
	else
	{
		for (int i=0;i<asdu.VSQ.N;i++)
		{
			OBJAddr = 0;
			for (int k=0;k<InfoOBJ_lenNum;k++)
			{
				OBJAddr += (asdu.data[i*(1+InfoOBJ_lenNum)+k]<<(8*k));
				objTemp[k] = asdu.data[i*(1+InfoOBJ_lenNum)+k]<<(8*k);
			}
			//OBJAddr = asdu.data[i*3]|(asdu.data[i*3+1]<<8);
			Value = asdu.data[i*(1+InfoOBJ_lenNum)+InfoOBJ_lenNum];
			objTemp[k] = Value;
			CString	strobjdata = "";
			for (int j=0;j<InfoOBJ_lenNum+1;j++)
			{
				StrTemp = strobjdata;
				strobjdata.Format("%s%02XH ",StrTemp,asdu.data[i*(1+InfoOBJ_lenNum)+j]);
			}
			QDSstr = ParseASDU_QDS(1,Value);
			StrTemp = StrRe;
			StrRe.Format("%s \r\n【<%s> 对象码:%4d %s】",StrTemp,strobjdata,OBJAddr,QDSstr);
		}
	}
	
	return StrRe;
}

CString CProtocol02_101::ParseASDU_IEC101_02_M_SP_TA_1(ASDU asdu)//type = 2 带CP24Time2a的单点信息
{
	CString StrTemp,str,StrRe="",QDSstr;
	int OBJAddr; 
	unsigned char Value;
	if (asdu.VSQ.SQ)//连续
	{
		OBJAddr = 0;
		for (int k=0;k<InfoOBJ_lenNum;k++)
		{
			OBJAddr += (asdu.data[k]<<(8*k));//
		}
		//OBJAddr = asdu.data[0]|(asdu.data[1]<<8);
		for (int i=0;i<asdu.VSQ.N;i++)
		{
			Value = asdu.data[i*4+InfoOBJ_lenNum];
			QDSstr = ParseASDU_QDS(1,Value);
			Time3.Min = asdu.data[i*4+InfoOBJ_lenNum+3]&0x3f;
			int msec = asdu.data[i*4+InfoOBJ_lenNum+1]|(asdu.data[i*4+InfoOBJ_lenNum+2]<<8);
			Time3.Sec = msec/1000;
			Time3.msec = msec%1000;
			
			CString	strobjdata = "";
			for (int j=0;j<4;j++)
			{
				StrTemp = strobjdata;
				strobjdata.Format("%s%02XH ",StrTemp,asdu.data[i*4+j]);
			}
			StrTemp = StrRe;		
			StrRe.Format("%s \r\n【<%s>对象码:%d %s CP24Time(%d:%d.%d)】",StrTemp,strobjdata,OBJAddr+i,QDSstr,Time3.Min,Time3.Sec,Time3.msec);
		}
	} 
	else
	{
		for (int i=0;i<asdu.VSQ.N;i++)
		{
			OBJAddr = 0;
			for (int k=0;k<InfoOBJ_lenNum;k++)
			{
				OBJAddr += (asdu.data[i*(4+InfoOBJ_lenNum)+k]<<(8*k));
			}
			//OBJAddr = asdu.data[i*3]|(asdu.data[i*3+1]<<8);
			Value = asdu.data[i*(4+InfoOBJ_lenNum)+InfoOBJ_lenNum];
			QDSstr = ParseASDU_QDS(1,Value);
			Time3.Min = asdu.data[i*(4+InfoOBJ_lenNum)+InfoOBJ_lenNum+3]&0x3f;
			int msec = asdu.data[i*(4+InfoOBJ_lenNum)+InfoOBJ_lenNum+1]|(asdu.data[i*(4+InfoOBJ_lenNum)+InfoOBJ_lenNum+2]<<8);
			Time3.Sec = msec/1000;
			Time3.msec = msec%1000;
			
			CString	strobjdata = "";
			for (int j=0;j<InfoOBJ_lenNum+4;j++)
			{
				StrTemp = strobjdata;
				strobjdata.Format("%s%02XH ",StrTemp,asdu.data[i*(4+InfoOBJ_lenNum)+j]);
			}
			StrTemp = StrRe;		
			StrRe.Format("%s \r\n【<%s>对象码:%d %s CP24Time(%d:%d.%d)】",StrTemp,strobjdata,OBJAddr,QDSstr,Time3.Min,Time3.Sec,Time3.msec);
		}
	}
	
	return StrRe;
}

CString CProtocol02_101::ParseASDU_IEC101_02_M_DP_NA_1(ASDU asdu)////Type = 3 不带时标双点遥信
{
	CString StrTemp,str,StrRe="",QDSstr;
	int OBJAddr,Value;
	if (asdu.VSQ.SQ)//连续
	{
		OBJAddr=0;
		for (int k=0;k<InfoOBJ_lenNum;k++)
		{
			OBJAddr += (asdu.data[k]<<(8*k));//
		}
		for (int i=0;i<asdu.VSQ.N;i++)
		{
			Value = asdu.data[i+InfoOBJ_lenNum];
			QDSstr = ParseASDU_QDS(2,Value);

			CString	strobjdata = "";
			for (int j=0;j<1;j++)
			{
				StrTemp = strobjdata;
				strobjdata.Format("%s%02XH ",StrTemp,asdu.data[i+j]);
			}
			StrTemp = StrRe;
			StrRe.Format("%s \r\n【<%s>对象码:%d %s】",StrTemp,strobjdata,OBJAddr+i,QDSstr);
		}
	} 
	else
	{
		for (int i=0;i<asdu.VSQ.N;i++)
		{
			OBJAddr = 0;
			for (int k=0;k<InfoOBJ_lenNum;k++)
			{
				OBJAddr += (asdu.data[i*(1+InfoOBJ_lenNum)+k]<<(8*k));
			}
			Value = asdu.data[i*(1+InfoOBJ_lenNum)+InfoOBJ_lenNum];
			QDSstr = ParseASDU_QDS(2,Value);
			
			CString	strobjdata = "";
			for (int j=0;j<InfoOBJ_lenNum+1;j++)
			{
				StrTemp = strobjdata;
				strobjdata.Format("%s%02XH ",StrTemp,asdu.data[i*(1+InfoOBJ_lenNum)+j]);
			}
			StrTemp = StrRe;			
			StrRe.Format("%s \r\n【<%s>对象码:%d %s】",StrTemp,strobjdata,OBJAddr,QDSstr);
		}
	}
	
	return StrRe;
}

CString CProtocol02_101::ParseASDU_IEC101_02_M_DP_TA_1(ASDU asdu)//type = 4 带CP24Time2a的双点信息
{
	CString StrTemp,str,StrRe="",QDSstr;
	int OBJAddr; 
	unsigned char Value;
	if (asdu.VSQ.SQ)//连续
	{
		OBJAddr = 0;
		for (int k=0;k<InfoOBJ_lenNum;k++)
		{
			OBJAddr += (asdu.data[k]<<(8*k));//
		}
		for (int i=0;i<asdu.VSQ.N;i++)
		{
			Value = asdu.data[i*4+InfoOBJ_lenNum];
			QDSstr = ParseASDU_QDS(2,Value);
			Time3.Min = asdu.data[i*4+InfoOBJ_lenNum+3]&0x3f;
			int msec = asdu.data[i*4+InfoOBJ_lenNum+1]|(asdu.data[i*4+InfoOBJ_lenNum+2]<<8);
			Time3.Sec = msec/1000;
			Time3.msec = msec%1000;
			
			CString	strobjdata = "";
			for (int j=0;j<4;j++)
			{
				StrTemp = strobjdata;
				strobjdata.Format("%s%02XH ",StrTemp,asdu.data[i*4+j]);
			}
			StrTemp = StrRe;			
			StrRe.Format("%s \r\n【<%s>对象码:%d %s CP24Time(%d:%d.%d)】",StrTemp,strobjdata,OBJAddr+i,QDSstr,Time3.Min,Time3.Sec,Time3.msec);
		}
	} 
	else
	{
		for (int i=0;i<asdu.VSQ.N;i++)
		{
			OBJAddr = 0;
			for (int k=0;k<InfoOBJ_lenNum;k++)
			{
				OBJAddr += (asdu.data[i*(4+InfoOBJ_lenNum)+k]<<(8*k));
			}
			Value = asdu.data[i*(4+InfoOBJ_lenNum)+InfoOBJ_lenNum];
			QDSstr = ParseASDU_QDS(2,Value);
			Time3.Min = asdu.data[i*(4+InfoOBJ_lenNum)+InfoOBJ_lenNum+3]&0x3f;
			int msec = asdu.data[i*(4+InfoOBJ_lenNum)+InfoOBJ_lenNum+1]|(asdu.data[i*(4+InfoOBJ_lenNum)+InfoOBJ_lenNum+2]<<8);
			Time3.Sec = msec/1000;
			Time3.msec = msec%1000;
			
			CString	strobjdata = "";
			for (int j=0;j<InfoOBJ_lenNum+4;j++)
			{
				StrTemp = strobjdata;
				strobjdata.Format("%s%02XH ",StrTemp,asdu.data[i*(4+InfoOBJ_lenNum)+j]);
			}
			StrTemp = StrRe;	
			StrRe.Format("%s \r\n【<%s>对象码:%d %s CP24Time(%d:%d.%d)】",StrTemp,strobjdata,OBJAddr,QDSstr,Time3.Min,Time3.Sec,Time3.msec);
		}
	}
	
	return StrRe;
}

CString CProtocol02_101::ParseASDU_IEC101_02_M_ME_NA_1(ASDU asdu)//Type = 9 测量值，无时标的归一化值
{
	CString StrTemp,QZstr,StrRe="",QDSstr;
	int ycobj,ycValue;
	unsigned char ValueT;
	if (asdu.VSQ.SQ)//连续
	{
		//ycobj = asdu.data[0]|(asdu.data[1]<<8);
		ycobj = 0;
		for (int k=0;k<InfoOBJ_lenNum;k++)
		{
			ycobj += (asdu.data[k]<<(8*k));//
		}
		for (int i=0;i<asdu.VSQ.N;i++)
		{
			ycValue = asdu.data[i*3+InfoOBJ_lenNum]|(asdu.data[i*3+InfoOBJ_lenNum+1]<<8);
			ValueT = asdu.data[i*3+InfoOBJ_lenNum+2];
			QDSstr = ParseASDU_QDS(3,ValueT);
			
			CString	strobjdata = "";
			for (int j=0;j<3;j++)
			{
				StrTemp = strobjdata;
				strobjdata.Format("%s%02XH ",StrTemp,asdu.data[i*3+j]);
			}
			StrTemp = StrRe;			
			StrRe.Format("%s \r\n【<%s>对象码:%d 值%d %s】",StrTemp,strobjdata,ycobj+i,ycValue,QDSstr);
		}
	} 
	else
	{
		for (int i=0;i<asdu.VSQ.N;i++)
		{
			ycobj = 0;
			for (int k=0;k<InfoOBJ_lenNum;k++)
			{
				ycobj += (asdu.data[i*(3+InfoOBJ_lenNum)+k]<<(8*k));
			}		
			ycValue = asdu.data[i*(InfoOBJ_lenNum+3)+InfoOBJ_lenNum]|(asdu.data[i*(InfoOBJ_lenNum+3)+InfoOBJ_lenNum+1]<<8);
			ValueT = asdu.data[i*(InfoOBJ_lenNum+3)+InfoOBJ_lenNum+2];			
			QDSstr = ParseASDU_QDS(3,ValueT);
			
			CString	strobjdata = "";
			for (int j=0;j<InfoOBJ_lenNum+3;j++)
			{
				StrTemp = strobjdata;
				strobjdata.Format("%s%02XH ",StrTemp,asdu.data[i*(3+InfoOBJ_lenNum)+j]);
			}
			StrTemp = StrRe;			
			StrRe.Format("%s \r\n【<%s>对象码:%d 值%d %s】",StrTemp,strobjdata,ycobj,ycValue,QDSstr);
		}
	}
	
	return StrRe;
}

CString CProtocol02_101::ParseASDU_IEC101_02_M_ME_TA_1(ASDU asdu)//Type = 10 测量值，有时标的归一化值
{
	CString StrTemp,QZstr,StrRe="",QDSstr;
	int ycobj,ycValue;
	unsigned char ValueT;
	if (asdu.VSQ.SQ)//连续
	{
		//ycobj = asdu.data[0]|(asdu.data[1]<<8);
		ycobj = 0;
		for (int k=0;k<InfoOBJ_lenNum;k++)
		{
			ycobj += (asdu.data[k]<<(8*k));//
		}
		for (int i=0;i<asdu.VSQ.N;i++)
		{
			ycValue = asdu.data[i*6+InfoOBJ_lenNum]|(asdu.data[i*6+InfoOBJ_lenNum+1]<<8);
			ValueT = asdu.data[i*6+InfoOBJ_lenNum+2];
			QDSstr = ParseASDU_QDS(3,ValueT);

			Time3.Min = asdu.data[i*6+InfoOBJ_lenNum+3]&0x3f;
			int msec = asdu.data[i*6+InfoOBJ_lenNum+4]|(asdu.data[i*6+InfoOBJ_lenNum+5]<<8);
			Time3.Sec = msec/1000;
			Time3.msec = msec%1000;
			
			CString	strobjdata = "";
			for (int j=0;j<6;j++)
			{
				StrTemp = strobjdata;
				strobjdata.Format("%s%02XH ",StrTemp,asdu.data[i*6+j]);
			}
			StrTemp = StrRe;
			StrRe.Format("%s \r\n【<%s>对象码:%d 值%d %s CP24Time(%d:%d.%d)】",StrTemp,strobjdata,ycobj+i,ycValue,QDSstr,Time3.Min,Time3.Sec,Time3.msec);			
		}
	} 
	else
	{
		for (int i=0;i<asdu.VSQ.N;i++)
		{
			ycobj = 0;
			for (int k=0;k<InfoOBJ_lenNum;k++)
			{
				ycobj += (asdu.data[i*(6+InfoOBJ_lenNum)+k]<<(8*k));
			}		
			ycValue = asdu.data[i*(InfoOBJ_lenNum+6)+InfoOBJ_lenNum]|(asdu.data[i*(InfoOBJ_lenNum+6)+InfoOBJ_lenNum+1]<<8);
			ValueT = asdu.data[i*(InfoOBJ_lenNum+6)+InfoOBJ_lenNum+2];			
			QDSstr = ParseASDU_QDS(3,ValueT);
			Time3.Min = asdu.data[i*(InfoOBJ_lenNum+6)+InfoOBJ_lenNum+3]&0x3f;
			int msec = asdu.data[i*(InfoOBJ_lenNum+6)+InfoOBJ_lenNum+4]|(asdu.data[i*(InfoOBJ_lenNum+6)+InfoOBJ_lenNum+5]<<8);
			Time3.Sec = msec/1000;
			Time3.msec = msec%1000;
			
			CString	strobjdata = "";
			for (int j=0;j<InfoOBJ_lenNum+6;j++)
			{
				StrTemp = strobjdata;
				strobjdata.Format("%s%02XH ",StrTemp,asdu.data[i*(6+InfoOBJ_lenNum)+j]);
			}			
			StrTemp = StrRe;			
			StrRe.Format("%s \r\n【<%s>对象码:%d 值%d %s CP24Time(%d:%d.%d)】",StrTemp,strobjdata,ycobj,ycValue,QDSstr,Time3.Min,Time3.Sec,Time3.msec);
		}
	}
	
	return StrRe;
}

CString CProtocol02_101::ParseASDU_IEC101_02_M_ME_NB_1(ASDU asdu)//Type = 11 测量值，标度化值
{
	CString StrTemp,QZstr,StrRe="",QDSstr;
	int ycobj,ycValue;
	unsigned char ValueT;
	if (asdu.VSQ.SQ)//连续
	{
		//ycobj = asdu.data[0]|(asdu.data[1]<<8);
		ycobj = 0;
		for (int k=0;k<InfoOBJ_lenNum;k++)
		{
			ycobj += (asdu.data[k]<<(8*k));//
		}
		for (int i=0;i<asdu.VSQ.N;i++)
		{
			ycValue = asdu.data[i*3+InfoOBJ_lenNum]|(asdu.data[i*3+InfoOBJ_lenNum+1]<<8);
			ValueT = asdu.data[i*3+InfoOBJ_lenNum+2];
			QDSstr = ParseASDU_QDS(3,ValueT);
			
			CString	strobjdata = "";
			for (int j=0;j<3;j++)
			{
				StrTemp = strobjdata;
				strobjdata.Format("%s%02XH ",StrTemp,asdu.data[i*3+j]);
			}
			StrTemp = StrRe;
			
			StrRe.Format("%s \r\n【<%s>对象码:%d 值%d %s】",StrTemp,strobjdata,ycobj+i,ycValue,QDSstr);
		}
	} 
	else
	{
		for (int i=0;i<asdu.VSQ.N;i++)
		{
			ycobj = 0;
			for (int k=0;k<InfoOBJ_lenNum;k++)
			{
				ycobj += (asdu.data[i*(3+InfoOBJ_lenNum)+k]<<(8*k));
			}		
			ycValue = asdu.data[i*(InfoOBJ_lenNum+3)+InfoOBJ_lenNum]|(asdu.data[i*(InfoOBJ_lenNum+3)+InfoOBJ_lenNum+1]<<8);
			ValueT = asdu.data[i*(InfoOBJ_lenNum+3)+InfoOBJ_lenNum+2];			
			QDSstr = ParseASDU_QDS(3,ValueT);
			
			CString	strobjdata = "";
			for (int j=0;j<InfoOBJ_lenNum+3;j++)
			{
				StrTemp = strobjdata;
				strobjdata.Format("%s%02XH ",StrTemp,asdu.data[i*(3+InfoOBJ_lenNum)+j]);
			}
			StrTemp = StrRe;
			StrRe.Format("%s \r\n【<%s>对象码:%d 值%d %s】",StrTemp,strobjdata,ycobj,ycValue,QDSstr);
		}
	}
	
	return StrRe;
}

CString CProtocol02_101::ParseASDU_IEC101_02_M_ME_TB_1(ASDU asdu)//Type = 12 测量值，带时标的标度化值
{
	CString StrTemp,QZstr,StrRe="",QDSstr;
	int ycobj,ycValue;
	unsigned char ValueT;
	if (asdu.VSQ.SQ)//连续
	{
		//ycobj = asdu.data[0]|(asdu.data[1]<<8);
		ycobj = 0;
		for (int k=0;k<InfoOBJ_lenNum;k++)
		{
			ycobj += (asdu.data[k]<<(8*k));//
		}
		for (int i=0;i<asdu.VSQ.N;i++)
		{
			ycValue = asdu.data[i*6+InfoOBJ_lenNum]|(asdu.data[i*6+InfoOBJ_lenNum+1]<<8);
			ValueT = asdu.data[i*6+InfoOBJ_lenNum+2];
			QDSstr = ParseASDU_QDS(3,ValueT);
			
			Time3.Min = asdu.data[i*6+InfoOBJ_lenNum+3]&0x3f;
			int msec = asdu.data[i*6+InfoOBJ_lenNum+4]|(asdu.data[i*6+InfoOBJ_lenNum+5]<<8);
			Time3.Sec = msec/1000;
			Time3.msec = msec%1000;
			
			CString	strobjdata = "";
			for (int j=0;j<6;j++)
			{
				StrTemp = strobjdata;
				strobjdata.Format("%s%02XH ",StrTemp,asdu.data[i*6+j]);
			}
			StrTemp = StrRe;			
			StrRe.Format("%s \r\n【<%s>对象码:%d 值%d %s CP24Time(%d:%d.%d)】",StrTemp,strobjdata,ycobj+i,ycValue,QDSstr,Time3.Min,Time3.Sec,Time3.msec);			
		}
	} 
	else
	{
		for (int i=0;i<asdu.VSQ.N;i++)
		{
			ycobj = 0;
			for (int k=0;k<InfoOBJ_lenNum;k++)
			{
				ycobj += (asdu.data[i*(6+InfoOBJ_lenNum)+k]<<(8*k));
			}		
			ycValue = asdu.data[i*(InfoOBJ_lenNum+6)+InfoOBJ_lenNum]|(asdu.data[i*(InfoOBJ_lenNum+6)+InfoOBJ_lenNum+1]<<8);
			ValueT = asdu.data[i*(InfoOBJ_lenNum+6)+InfoOBJ_lenNum+2];			
			QDSstr = ParseASDU_QDS(3,ValueT);
			Time3.Min = asdu.data[i*(InfoOBJ_lenNum+6)+InfoOBJ_lenNum+3]&0x3f;
			int msec = asdu.data[i*(InfoOBJ_lenNum+6)+InfoOBJ_lenNum+4]|(asdu.data[i*(InfoOBJ_lenNum+6)+InfoOBJ_lenNum+5]<<8);
			Time3.Sec = msec/1000;
			Time3.msec = msec%1000;

			CString	strobjdata = "";
			for (int j=0;j<InfoOBJ_lenNum+6;j++)
			{
				StrTemp = strobjdata;
				strobjdata.Format("%s%02XH ",StrTemp,asdu.data[i*(6+InfoOBJ_lenNum)+j]);
			}
			StrTemp = StrRe;
			StrRe.Format("%s \r\n【<%s>对象码:%d 值%d %s CP24Time(%d:%d.%d)】",StrTemp,strobjdata,ycobj,ycValue,QDSstr,Time3.Min,Time3.Sec,Time3.msec);
		}
	}
	
	return StrRe;
}

CString CProtocol02_101::ParseASDU_IEC101_02_M_IT_NA_1(ASDU asdu)//Type = 15 累计量
{
	CString StrTemp,QZstr,StrRe="",QDSstr;
	int ycobj,ycValue;
	unsigned char ValueT;
	if (asdu.VSQ.SQ)//连续
	{
		//ycobj = asdu.data[0]|(asdu.data[1]<<8);
		ycobj = 0;
		for (int k=0;k<InfoOBJ_lenNum;k++)
		{
			ycobj += (asdu.data[k]<<(8*k));//
		}
		for (int i=0;i<asdu.VSQ.N;i++)
		{
			ycValue = asdu.data[i*5+InfoOBJ_lenNum]|(asdu.data[i*5+InfoOBJ_lenNum+1]<<8)|(asdu.data[i*5+InfoOBJ_lenNum+2]<<16)|(asdu.data[i*5+InfoOBJ_lenNum+3]<<24);
			ValueT = asdu.data[i*5+InfoOBJ_lenNum+4];
			QDSstr = ParseASDU_BCR(ValueT);

			CString	strobjdata = "";
			for (int j=0;j<5;j++)
			{
				StrTemp = strobjdata;
				strobjdata.Format("%s%02XH ",StrTemp,asdu.data[i*5+j]);
			}
			StrTemp = StrRe;
			StrRe.Format("%s \r\n【<%s>对象码:%d 值%d %s】",StrTemp,strobjdata,ycobj+i,ycValue,QDSstr);
		}
	} 
	else
	{
		for (int i=0;i<asdu.VSQ.N;i++)
		{
			ycobj = 0;
			for (int k=0;k<InfoOBJ_lenNum;k++)
			{
				ycobj += (asdu.data[i*(5+InfoOBJ_lenNum)+k]<<(8*k));
			}		
			ycValue = asdu.data[i*(5+InfoOBJ_lenNum)+InfoOBJ_lenNum]|(asdu.data[i*(5+InfoOBJ_lenNum)+InfoOBJ_lenNum+1]<<8)|(asdu.data[i*(5+InfoOBJ_lenNum)+InfoOBJ_lenNum+2]<<16)|(asdu.data[i*(5+InfoOBJ_lenNum)+InfoOBJ_lenNum+3]<<24);
			ValueT = asdu.data[i*(5+InfoOBJ_lenNum)+InfoOBJ_lenNum+4];
			QDSstr = ParseASDU_BCR(ValueT);
			CString	strobjdata = "";
			for (int j=0;j<InfoOBJ_lenNum+5;j++)
			{
				StrTemp = strobjdata;
				strobjdata.Format("%s%02XH ",StrTemp,asdu.data[i*(5+InfoOBJ_lenNum)+j]);
			}
			StrTemp = StrRe;			
			StrRe.Format("%s \r\n【<%s>对象码:%d 值%d %s】",StrTemp,strobjdata,ycobj,ycValue,QDSstr);
		}
	}	
	return StrRe;
}


CString CProtocol02_101::ParseASDU_IEC101_02_M_ME_ND_1(ASDU asdu)//Type = 21 测量值，不带品质描述词的的归一化值
{
	CString StrTemp,QZstr,StrRe="";
	int ycobj,ycValue;
	if (asdu.VSQ.SQ)//连续
	{
		//ycobj = asdu.data[0]|(asdu.data[1]<<8);
		ycobj = 0;
		for (int k=0;k<InfoOBJ_lenNum;k++)
		{
			ycobj += (asdu.data[k]<<(8*k));//
		}
		for (int i=0;i<asdu.VSQ.N;i++)
		{
			ycValue = asdu.data[i*2+InfoOBJ_lenNum]|(asdu.data[i*2+InfoOBJ_lenNum+1]<<8);
			
			CString	strobjdata = "";
			for (int j=0;j<2;j++)
			{
				StrTemp = strobjdata;
				strobjdata.Format("%s%02XH ",StrTemp,asdu.data[i*2+j]);
			}
			StrTemp = StrRe;			
			StrRe.Format("%s \r\n【<%s>对象码:%d 值%d】",StrTemp,strobjdata,ycobj+i,ycValue);
		}
	} 
	else
	{
		for (int i=0;i<asdu.VSQ.N;i++)
		{
			ycobj = 0;
			for (int k=0;k<InfoOBJ_lenNum;k++)
			{
				ycobj += (asdu.data[i*(2+InfoOBJ_lenNum)+k]<<(8*k));
			}
			//ycobj = asdu.data[i*4]|(asdu.data[i*4+1]<<8);			
			ycValue = asdu.data[i*(InfoOBJ_lenNum+2)+InfoOBJ_lenNum]|(asdu.data[i*(InfoOBJ_lenNum+2)+InfoOBJ_lenNum+1]<<8);
			CString	strobjdata = "";
			for (int j=0;j<InfoOBJ_lenNum+2;j++)
			{
				StrTemp = strobjdata;
				strobjdata.Format("%s%02XH ",StrTemp,asdu.data[i*(2+InfoOBJ_lenNum)+j]);
			}
			StrTemp = StrRe;
			
			StrRe.Format("%s \r\n【<%s>对象码:%d 值%d】",StrTemp,strobjdata,ycobj,ycValue);
		}
	}
	
	return StrRe;
}

CString CProtocol02_101::ParseASDU_IEC101_02_M_SP_TB_1(ASDU asdu)//type = 30 带CP56Time2a的单点信息变位上送
{
	CString StrTemp,str,StrRe="",QDSstr;
	int OBJAddr; 
	unsigned char Value;
	if (asdu.VSQ.SQ)//连续
	{
		OBJAddr = 0;
		for (int k=0;k<InfoOBJ_lenNum;k++)
		{
			OBJAddr += (asdu.data[k]<<(8*k));//
		}
		for (int i=0;i<asdu.VSQ.N;i++)
		{
			Value = asdu.data[i*8+InfoOBJ_lenNum];
			QDSstr = ParseASDU_QDS(1,Value);
			Time7.Year = asdu.data[i*8+InfoOBJ_lenNum+7]&0x7f;
			Time7.Month = asdu.data[i*8+InfoOBJ_lenNum+6]&0x0f;
			Time7.Day = asdu.data[i*8+InfoOBJ_lenNum+5]&0x1f;
			Time7.Hour = asdu.data[i*8+InfoOBJ_lenNum+4]&0x1f;
			Time7.Min = asdu.data[i*8+InfoOBJ_lenNum+3]&0x3f;
			int msec = asdu.data[i*8+InfoOBJ_lenNum+1]|(asdu.data[i*8+InfoOBJ_lenNum+2]<<8);
			Time7.Sec = msec/1000;
			Time7.msec = msec%1000;
			
			CString	strobjdata = "";
			for (int j=0;j<8;j++)
			{
				StrTemp = strobjdata;
				strobjdata.Format("%s%02XH ",StrTemp,asdu.data[i*8+j]);
			}
			StrTemp = StrRe;
			
			StrRe.Format("%s \r\n【<%S>对象码:%d %s CP24Time(%d.%d%d %d:%d:%d.%d)】",StrTemp,strobjdata,OBJAddr+i,QDSstr,Time7.Year,Time7.Month,Time7.Day,Time7.Hour,Time7.Min,Time7.Sec,Time7.msec);
		}
	} 
	else
	{
		for (int i=0;i<asdu.VSQ.N;i++)
		{
			OBJAddr = 0;
			for (int k=0;k<InfoOBJ_lenNum;k++)
			{
				OBJAddr += (asdu.data[i*(8+InfoOBJ_lenNum)+k]<<(8*k));
			}
			Value = asdu.data[i*(8+InfoOBJ_lenNum)+InfoOBJ_lenNum];
			QDSstr = ParseASDU_QDS(1,Value);
			Time7.Year = asdu.data[i*(8+InfoOBJ_lenNum)+InfoOBJ_lenNum+7]&0x7f;
			Time7.Month = asdu.data[i*(8+InfoOBJ_lenNum)+InfoOBJ_lenNum+6]&0x0f;
			Time7.Day = asdu.data[i*(8+InfoOBJ_lenNum)+InfoOBJ_lenNum+5]&0x1f;
			Time7.Hour = asdu.data[i*(8+InfoOBJ_lenNum)+InfoOBJ_lenNum+4]&0x1f;
			Time7.Min = asdu.data[i*(8+InfoOBJ_lenNum)+InfoOBJ_lenNum+3]&0x3f;
			int msec = asdu.data[i*(8+InfoOBJ_lenNum)+InfoOBJ_lenNum+1]|(asdu.data[i*(8+InfoOBJ_lenNum)+InfoOBJ_lenNum+2]<<8);
			Time7.Sec = msec/1000;
			Time7.msec = msec%1000;
			
			CString	strobjdata = "";
			for (int j=0;j<InfoOBJ_lenNum+8;j++)
			{
				StrTemp = strobjdata;
				strobjdata.Format("%s%02XH ",StrTemp,asdu.data[i*(8+InfoOBJ_lenNum)+j]);
			}
			StrTemp = StrRe;
			
			StrRe.Format("%s \r\n【<%s>对象码:%d %s CP56Time(%d年%d月%d日%d:%d:%d.%d)】",StrTemp,strobjdata,OBJAddr,QDSstr,Time7.Year,Time7.Month,Time7.Day,Time7.Hour,Time7.Min,Time7.Sec,Time7.msec);
		}
	}
	return StrRe;
}

CString CProtocol02_101::ParseASDU_IEC101_02_M_DP_TB_1(ASDU asdu)//type = 31 带CP56Time2a的双点信息变位上送
{
	CString StrTemp,str,StrRe="",QDSstr;
	int OBJAddr; 
	unsigned char Value;
	if (asdu.VSQ.SQ)//连续
	{
		OBJAddr = 0;
		for (int k=0;k<InfoOBJ_lenNum;k++)
		{
			OBJAddr += (asdu.data[k]<<(8*k));//
		}
		for (int i=0;i<asdu.VSQ.N;i++)
		{
			Value = asdu.data[i*8+InfoOBJ_lenNum];
			QDSstr = ParseASDU_QDS(2,Value);
			Time7.Year = asdu.data[i*8+InfoOBJ_lenNum+7]&0x7f;
			Time7.Month = asdu.data[i*8+InfoOBJ_lenNum+6]&0x0f;
			Time7.Day = asdu.data[i*8+InfoOBJ_lenNum+5]&0x1f;
			Time7.Hour = asdu.data[i*8+InfoOBJ_lenNum+4]&0x1f;
			Time7.Min = asdu.data[i*8+InfoOBJ_lenNum+3]&0x3f;
			int msec = asdu.data[i*8+InfoOBJ_lenNum+1]|(asdu.data[i*8+InfoOBJ_lenNum+2]<<8);
			Time7.Sec = msec/1000;
			Time7.msec = msec%1000;
			
			CString	strobjdata = "";
			for (int j=0;j<8;j++)
			{
				StrTemp = strobjdata;
				strobjdata.Format("%s%02XH ",StrTemp,asdu.data[i*8+j]);
			}
			StrTemp = StrRe;
			
			StrRe.Format("%s \r\n【<%s>对象码:%d %s CP56Time(%d年%d月%d日%d:%d:%d.%d)】",StrTemp,strobjdata,OBJAddr+i,QDSstr,Time7.Year,Time7.Month,Time7.Day,Time7.Hour,Time7.Min,Time7.Sec,Time7.msec);
		}
	} 
	else
	{
		for (int i=0;i<asdu.VSQ.N;i++)
		{
			OBJAddr = 0;
			for (int k=0;k<InfoOBJ_lenNum;k++)
			{
				OBJAddr += (asdu.data[i*(8+InfoOBJ_lenNum)+k]<<(8*k));
			}
			Value = asdu.data[i*(8+InfoOBJ_lenNum)+InfoOBJ_lenNum];
			QDSstr = ParseASDU_QDS(2,Value);
			Time7.Year = asdu.data[i*(8+InfoOBJ_lenNum)+InfoOBJ_lenNum+7]&0x7f;
			Time7.Month = asdu.data[i*(8+InfoOBJ_lenNum)+InfoOBJ_lenNum+6]&0x0f;
			Time7.Day = asdu.data[i*(8+InfoOBJ_lenNum)+InfoOBJ_lenNum+5]&0x1f;
			Time7.Hour = asdu.data[i*(8+InfoOBJ_lenNum)+InfoOBJ_lenNum+4]&0x1f;
			Time7.Min = asdu.data[i*(8+InfoOBJ_lenNum)+InfoOBJ_lenNum+3]&0x3f;
			int msec = asdu.data[i*(8+InfoOBJ_lenNum)+InfoOBJ_lenNum+1]|(asdu.data[i*(8+InfoOBJ_lenNum)+InfoOBJ_lenNum+2]<<8);
			Time7.Sec = msec/1000;
			Time7.msec = msec%1000;
			
			CString	strobjdata = "";
			for (int j=0;j<InfoOBJ_lenNum+8;j++)
			{
				StrTemp = strobjdata;
				strobjdata.Format("%s%02XH ",StrTemp,asdu.data[i*(8+InfoOBJ_lenNum)+j]);
			}
			StrTemp = StrRe;
			
			StrRe.Format("%s \r\n【<%s>对象码:%d %s CP56Time(%d年%d月%d日%d:%d:%d.%d)】",StrTemp,strobjdata,OBJAddr,QDSstr,Time7.Year,Time7.Month,Time7.Day,Time7.Hour,Time7.Min,Time7.Sec,Time7.msec);
		}
	}
	return StrRe;
}

CString CProtocol02_101::ParseASDU_IEC101_02_M_ME_TD_1(ASDU asdu)//type = 34 带CP56Time2a的归一化遥测
{
	CString StrTemp,str,StrRe="",QDSstr;
	int OBJAddr,Value; 
	unsigned char QDS;
	if (asdu.VSQ.SQ==0)//连续
	{
		for (int i=0;i<asdu.VSQ.N;i++)
		{
			OBJAddr = 0;
			for (int k=0;k<InfoOBJ_lenNum;k++)
			{
				OBJAddr += (asdu.data[i*(10+InfoOBJ_lenNum)+k]<<(8*k));
			}
			Value = asdu.data[i*(10+InfoOBJ_lenNum)+InfoOBJ_lenNum]|(asdu.data[i*(10+InfoOBJ_lenNum)+InfoOBJ_lenNum+1]<<8);
			QDS = asdu.data[i*(10+InfoOBJ_lenNum)+InfoOBJ_lenNum+2];
			QDSstr = ParseASDU_QDS(3,QDS);
			Time7.Year = asdu.data[i*(10+InfoOBJ_lenNum)+InfoOBJ_lenNum+9]&0x7f;
			Time7.Month = asdu.data[i*(10+InfoOBJ_lenNum)+InfoOBJ_lenNum+8]&0x0f;
			Time7.Day = asdu.data[i*(10+InfoOBJ_lenNum)+InfoOBJ_lenNum+7]&0x1f;
			Time7.Hour = asdu.data[i*(10+InfoOBJ_lenNum)+InfoOBJ_lenNum+6]&0x1f;
			Time7.Min = asdu.data[i*(10+InfoOBJ_lenNum)+InfoOBJ_lenNum+5]&0x3f;
			int msec = asdu.data[i*(10+InfoOBJ_lenNum)+InfoOBJ_lenNum+3]|(asdu.data[i*(10+InfoOBJ_lenNum)+InfoOBJ_lenNum+4]<<8);
			Time7.Sec = msec/1000;
			Time7.msec = msec%1000;
			
			CString	strobjdata = "";
			for (int j=0;j<InfoOBJ_lenNum+10;j++)
			{
				StrTemp = strobjdata;
				strobjdata.Format("%s%02XH ",StrTemp,asdu.data[i*(10+InfoOBJ_lenNum)+j]);
			}
			StrTemp = StrRe;			
			StrRe.Format("%s \r\n【<%s>对象码:%d 值:%d %s CP56Time(%d年%d月%d日%d:%d:%d.%d)】",StrTemp,strobjdata,OBJAddr,Value,QDSstr,Time7.Year,Time7.Month,Time7.Day,Time7.Hour,Time7.Min,Time7.Sec,Time7.msec);
		}
	} 
	else
	{
		StrRe.Format("VSQ.SQ错误");
	}
	return StrRe;
}


CString CProtocol02_101::ParseASDU_IEC101_02_M_ME_TE_1(ASDU asdu)//type = 35 带CP56Time2a的标度化遥测
{
	CString StrTemp,str,StrRe="",QDSstr;
	int OBJAddr,Value; 
	unsigned char QDS;
	if (asdu.VSQ.SQ==0)//连续
	{
		for (int i=0;i<asdu.VSQ.N;i++)
		{
			OBJAddr = 0;
			for (int k=0;k<InfoOBJ_lenNum;k++)
			{
				OBJAddr += (asdu.data[i*(10+InfoOBJ_lenNum)+k]<<(8*k));
			}
			Value = asdu.data[i*(10+InfoOBJ_lenNum)+InfoOBJ_lenNum]|(asdu.data[i*(10+InfoOBJ_lenNum)+InfoOBJ_lenNum+1]<<8);
			QDS = asdu.data[i*(10+InfoOBJ_lenNum)+InfoOBJ_lenNum+2];
			QDSstr = ParseASDU_QDS(3,QDS);
			Time7.Year = asdu.data[i*(10+InfoOBJ_lenNum)+InfoOBJ_lenNum+9]&0x7f;
			Time7.Month = asdu.data[i*(10+InfoOBJ_lenNum)+InfoOBJ_lenNum+8]&0x0f;
			Time7.Day = asdu.data[i*(10+InfoOBJ_lenNum)+InfoOBJ_lenNum+7]&0x1f;
			Time7.Hour = asdu.data[i*(10+InfoOBJ_lenNum)+InfoOBJ_lenNum+6]&0x1f;
			Time7.Min = asdu.data[i*(10+InfoOBJ_lenNum)+InfoOBJ_lenNum+5]&0x3f;
			int msec = asdu.data[i*(10+InfoOBJ_lenNum)+InfoOBJ_lenNum+3]|(asdu.data[i*(10+InfoOBJ_lenNum)+InfoOBJ_lenNum+4]<<8);
			Time7.Sec = msec/1000;
			Time7.msec = msec%1000;
			
			CString	strobjdata = "";
			for (int j=0;j<InfoOBJ_lenNum+10;j++)
			{
				StrTemp = strobjdata;
				strobjdata.Format("%s%02XH ",StrTemp,asdu.data[i*(10+InfoOBJ_lenNum)+j]);
			}
			StrTemp = StrRe;			
			StrRe.Format("%s \r\n【<%s>对象码:%d 值:%d %s CP56Time(%d年%d月%d日%d:%d:%d.%d)】",StrTemp,strobjdata,OBJAddr,Value,QDSstr,Time7.Year,Time7.Month,Time7.Day,Time7.Hour,Time7.Min,Time7.Sec,Time7.msec);
		}
	} 
	else
	{
		StrRe.Format("VSQ.SQ错误");
	}
	return StrRe;
}

CString CProtocol02_101::ParseASDU_IEC101_02_M_IT_TB_1(ASDU asdu)//type = 37 带CP56Time2a的累加量上送
{
	CString StrTemp,str,StrRe="",QDSstr;
	int OBJAddr,Value; 
	unsigned char QDS;
	if (asdu.VSQ.SQ==0)//连续
	{
		for (int i=0;i<asdu.VSQ.N;i++)
		{
			OBJAddr = 0;
			for (int k=0;k<InfoOBJ_lenNum;k++)
			{
				OBJAddr += (asdu.data[i*(12+InfoOBJ_lenNum)+k]<<(8*k));
			}
			Value = asdu.data[i*(12+InfoOBJ_lenNum)+InfoOBJ_lenNum]|(asdu.data[i*(12+InfoOBJ_lenNum)+InfoOBJ_lenNum+1]<<8)|(asdu.data[i*(12+InfoOBJ_lenNum)+InfoOBJ_lenNum+2]<<16)|(asdu.data[i*(12+InfoOBJ_lenNum)+InfoOBJ_lenNum+3]<<24);
			QDS = asdu.data[i*(12+InfoOBJ_lenNum)+InfoOBJ_lenNum+4];
			QDSstr = ParseASDU_BCR(QDS);
			Time7.Year = asdu.data[i*(12+InfoOBJ_lenNum)+InfoOBJ_lenNum+11]&0x7f;
			Time7.Month = asdu.data[i*(12+InfoOBJ_lenNum)+InfoOBJ_lenNum+10]&0x0f;
			Time7.Day = asdu.data[i*(12+InfoOBJ_lenNum)+InfoOBJ_lenNum+9]&0x1f;
			Time7.Hour = asdu.data[i*(12+InfoOBJ_lenNum)+InfoOBJ_lenNum+8]&0x1f;
			Time7.Min = asdu.data[i*(12+InfoOBJ_lenNum)+InfoOBJ_lenNum+7]&0x3f;
			int msec = asdu.data[i*(12+InfoOBJ_lenNum)+InfoOBJ_lenNum+5]|(asdu.data[i*(12+InfoOBJ_lenNum)+InfoOBJ_lenNum+6]<<8);
			Time7.Sec = msec/1000;
			Time7.msec = msec%1000;
			
			CString	strobjdata = "";
			for (int j=0;j<InfoOBJ_lenNum+12;j++)
			{
				StrTemp = strobjdata;
				strobjdata.Format("%s%02XH ",StrTemp,asdu.data[i*(12+InfoOBJ_lenNum)+j]);
			}
			StrTemp = StrRe;			
			StrRe.Format("%s \r\n【<%s>对象码:%d 值:%d %s CP56Time(%d年%d月%d日%d:%d:%d.%d)】",StrTemp,strobjdata,OBJAddr,Value,QDSstr,Time7.Year,Time7.Month,Time7.Day,Time7.Hour,Time7.Min,Time7.Sec,Time7.msec);
		}
	} 
	else
	{
		StrRe.Format("VSQ.SQ错误");
	}
	return StrRe;
}

CString CProtocol02_101::ParseASDU_IEC101_02_C_SC_NA_1(ASDU asdu)//Type = 45 单点命令
{
	CString StrTemp,Statestr,StrRe="";
	int OBJAddr;
	unsigned char StateData=0;
	for (int i=0;i<asdu.VSQ.N;i++)
	{
		for (int k=0;k<InfoOBJ_lenNum;k++)
		{
			OBJAddr =+ (asdu.data[i*(1+InfoOBJ_lenNum)+k]<<(8*k));
		}
		StateData = asdu.data[i*(1+InfoOBJ_lenNum)+InfoOBJ_lenNum];
		Statestr = ParseASDU_DCO(1,StateData);
		
		CString	strobjdata = "";
		for (int j=0;j<InfoOBJ_lenNum+1;j++)
		{
			StrTemp = strobjdata;
			strobjdata.Format("%s%02XH ",StrTemp,asdu.data[i*(1+InfoOBJ_lenNum)+j]);
		}
		StrTemp = StrRe;		
		StrRe.Format("%s \r\n【<%s>对象码:%d 值:%XH(%s)】",StrTemp,strobjdata,OBJAddr,StateData,Statestr);
	}
	return StrRe;
}

CString CProtocol02_101::ParseASDU_IEC101_02_C_DC_NA_1(ASDU asdu)//Type = 46 双命令
{
	CString StrTemp,Statestr,StrRe="";
	int OBJAddr;
	unsigned char StateData=0;
	for (int i=0;i<asdu.VSQ.N;i++)
	{
		for (int k=0;k<InfoOBJ_lenNum;k++)
		{
			OBJAddr =+ (asdu.data[i*(1+InfoOBJ_lenNum)+k]<<(8*k));
		}
		StateData = asdu.data[i*(1+InfoOBJ_lenNum)+InfoOBJ_lenNum];
		Statestr = ParseASDU_DCO(2,StateData);

		CString	strobjdata = "";
		for (int j=0;j<InfoOBJ_lenNum+1;j++)
		{
			StrTemp = strobjdata;
			strobjdata.Format("%s%02XH ",StrTemp,asdu.data[i*(1+InfoOBJ_lenNum)+j]);
		}
		StrTemp = StrRe;		
		StrRe.Format("%s \r\n【<%s>对象码:%d 值:%XH(%s)】",StrTemp,strobjdata,OBJAddr,StateData,Statestr);
	}
	return StrRe;
}

CString CProtocol02_101::ParseASDU_IEC101_02_C_RC_NA_1(ASDU asdu)//Type = 步调 双命令
{
	CString StrTemp,Statestr,StrRe="";
	int OBJAddr;
	unsigned char StateData=0;
	for (int i=0;i<asdu.VSQ.N;i++)
	{
		for (int k=0;k<InfoOBJ_lenNum;k++)
		{
			OBJAddr =+ (asdu.data[i*(1+InfoOBJ_lenNum)+k]<<(8*k));
		}
		StateData = asdu.data[i*(1+InfoOBJ_lenNum)+InfoOBJ_lenNum];
		Statestr = ParseASDU_DCO(3,StateData);
		
		CString	strobjdata = "";
		for (int j=0;j<InfoOBJ_lenNum+1;j++)
		{
			StrTemp = strobjdata;
			strobjdata.Format("%s%02XH ",StrTemp,asdu.data[i*(1+InfoOBJ_lenNum)+j]);
		}
		StrTemp = StrRe;		
		StrRe.Format("%s \r\n【<%s>对象码:%d 值:%XH(%s)】",StrTemp,strobjdata,OBJAddr,StateData,Statestr);
	}
	return StrRe;
}

CString CProtocol02_101::ParseASDU_IEC101_02_C_SE_NB_1(ASDU asdu)//Type = 49 设定命令
{
	CString StrTemp,Statestr,StrRe="";
	int OBJAddr,Value;
	unsigned char StateData=0;
	for (int i=0;i<asdu.VSQ.N;i++)
	{
		for (int k=0;k<InfoOBJ_lenNum;k++)
		{
			OBJAddr =+ (asdu.data[i*(3+InfoOBJ_lenNum)+k]<<(8*k));
		}
		Value = asdu.data[i*(3+InfoOBJ_lenNum)+InfoOBJ_lenNum]|(asdu.data[i*(3+InfoOBJ_lenNum)+InfoOBJ_lenNum+1]<<8);
		StateData = asdu.data[i*(3+InfoOBJ_lenNum)+InfoOBJ_lenNum+2];
		
		if (StateData&0x80==0x80)
		{
			Statestr="选择";
		} 
		else
		{
			Statestr="执行";
		}

		CString	strobjdata = "";
		for (int j=0;j<InfoOBJ_lenNum+3;j++)
		{
			StrTemp = strobjdata;
			strobjdata.Format("%s%02XH ",StrTemp,asdu.data[i*(3+InfoOBJ_lenNum)+j]);
		}
		StrTemp = StrRe;		
		StrRe.Format("%s \r\n【<%s>对象码:%d 值:%d 状态:%s】",StrTemp,strobjdata,OBJAddr,Value,Statestr);
	}
	return StrRe;
}

CString CProtocol02_101::ParseASDU_IEC101_02_M_EI_NA_1(ASDU asdu)//Type = 70 初始化结束
{
	CString StrTemp,BSstr,CIOstr,StrRe="";
	int OBJAddr;
	unsigned char qoi=0;
	for (int i=0;i<asdu.VSQ.N;i++)
	{
		for (int k=0;k<InfoOBJ_lenNum;k++)
		{
			OBJAddr =+ (asdu.data[i*(1+InfoOBJ_lenNum)+k]<<(8*k));
		}
		qoi = asdu.data[i*(1+InfoOBJ_lenNum)+InfoOBJ_lenNum];
		if (qoi&0x80 == 0x80)	
		{
			BSstr.Format("未改变当地参数的初始化");		
		}
		else
		{
			BSstr.Format("改变当地参数后的初始化");	
		}
		switch(qoi&0x7f)
		{
		case 0:CIOstr = "当地电源合上";
			break;
		case 1:CIOstr = "当地手动复位";
			break;
		case 2:CIOstr = "远方复位";
		    break;
		default:CIOstr = "使用保留";
		    break;
		}

		CString	strobjdata = "";
		for (int j=0;j<InfoOBJ_lenNum+1;j++)
		{
			StrTemp = strobjdata;
			strobjdata.Format("%s%02XH ",StrTemp,asdu.data[i*(1+InfoOBJ_lenNum)+j]);
		}
		StrTemp = StrRe;
		StrRe.Format("%s \r\n【<%s>对象码:%d %s %s】",StrTemp,strobjdata,OBJAddr,BSstr,CIOstr);
	}
	return StrRe;
}

CString CProtocol02_101::ParseASDU_IEC101_02_C_IC_NA_1(ASDU asdu)//Type = 100 召唤命令
{
	CString StrTemp,QZstr,StrRe="";
	int OBJAddr;
	unsigned char qoi=0;
	for (int i=0;i<asdu.VSQ.N;i++)
	{
		for (int k=0;k<InfoOBJ_lenNum;k++)
		{
			OBJAddr =+ (asdu.data[i*(1+InfoOBJ_lenNum)+k]<<(8*k));
		}
		qoi = asdu.data[i*(1+InfoOBJ_lenNum)+InfoOBJ_lenNum];
		if (qoi == 0)	
		{
			QZstr.Format("未用");		
		}
		else if (qoi == 20)
		{
			QZstr.Format("站召唤");	
		}
		else if((qoi>20)&&(qoi<37))
		{
			QZstr.Format("第%d组召唤",qoi-20);	
		}
		else
		{
			QZstr.Format("兼容保留");	
		}

		CString	strobjdata = "";
		for (int j=0;j<InfoOBJ_lenNum+1;j++)
		{
			StrTemp = strobjdata;
			strobjdata.Format("%s%02XH ",StrTemp,asdu.data[i*(1+InfoOBJ_lenNum)+j]);
		}
		StrTemp = StrRe;	
		StrRe.Format("%s \r\n【<%s>对象码:%d %s】",StrTemp,strobjdata,OBJAddr,QZstr);
	}
	return StrRe;
}

CString CProtocol02_101::ParseASDU_IEC101_02_C_CI_NA_1(ASDU asdu)//Type = 101 召累计量
{
	CString StrTemp,Statestr,StrRe="",RQTstr,FRZstr;
	int OBJAddr,iRAT,iFRZ;
	unsigned char StateData=0;
	for (int i=0;i<asdu.VSQ.N;i++)
	{
		for (int k=0;k<InfoOBJ_lenNum;k++)
		{
			OBJAddr =+ (asdu.data[i*(1+InfoOBJ_lenNum)+k]<<(8*k));
		}
		StateData = asdu.data[i*(1+InfoOBJ_lenNum)+InfoOBJ_lenNum];
		iRAT = StateData&0x3f;
		switch(iRAT)
		{
		case 0:RQTstr = "没请求计数量";
			break;
		case 1:
		case 2:
		case 3:
		case 4:RQTstr.Format("请求计数量第%组",iRAT);
			break;
		case 5:RQTstr = "总的请求计数量";
			break;
		default:RQTstr = "协议预留";
		    break;
		}
		iFRZ = (StateData>>6)&0x3;
		switch(iFRZ)
		{
		case 0:FRZstr = "无冻结";
			break;
		case 1:FRZstr = "冻结值为累加量";
			break;
		case 2:FRZstr = "冻结值为增量";
		    break;
		case 3:FRZstr = "计数量复位";
		    break;
		default:
		    break;
		}
		CString	strobjdata = "";
		for (int j=0;j<InfoOBJ_lenNum+1;j++)
		{
			StrTemp = strobjdata;
			strobjdata.Format("%s%02XH ",StrTemp,asdu.data[i*(1+InfoOBJ_lenNum)+j]);
		}
		StrTemp = StrRe;
		StrRe.Format("%s \r\n【<%s>对象码:%d 限定词:%d(%s)】",StrTemp,strobjdata,OBJAddr,StateData,RQTstr,FRZstr);
	}
	return StrRe;
}

CString CProtocol02_101::ParseASDU_IEC101_02_C_CS_NA_1(ASDU asdu)//Type = 103 时钟同步命令
{
	CString Statestr,StrRe="",StrTemp;
	int OBJAddr=0;
	unsigned char StateData=0;
	
	for (int k=0;k<InfoOBJ_lenNum;k++)
	{
		OBJAddr += (asdu.data[k]<<(8*k));
	}
	Time7.Year = asdu.data[InfoOBJ_lenNum+6]&0x7f;
	Time7.Month = asdu.data[InfoOBJ_lenNum+5]&0x0f;
	Time7.Day = asdu.data[InfoOBJ_lenNum+4]&0x1f;
	Time7.Hour = asdu.data[InfoOBJ_lenNum+3]&0x1f;
	Time7.Min = asdu.data[InfoOBJ_lenNum+2]&0x3f;
	int msec = asdu.data[InfoOBJ_lenNum]|(asdu.data[InfoOBJ_lenNum+1]<<8);
	Time7.Sec = msec/1000;
	Time7.msec = msec%1000;
	CString	strobjdata = "";
	for (int j=0;j<InfoOBJ_lenNum+6;j++)
	{
		StrTemp = strobjdata;
		strobjdata.Format("%s%02XH ",StrTemp,asdu.data[(6+InfoOBJ_lenNum)+j]);
	}
	StrRe.Format("\r\n【(%s)对象码:%d CP56Time(%d年%d月%d日%d:%d:%d.%d)】",strobjdata,OBJAddr,Time7.Year,Time7.Month,Time7.Day,Time7.Hour,Time7.Min,Time7.Sec,Time7.msec);	
	return StrRe;
}

CString CProtocol02_101::ParseASDU_IEC101_02_P_ME_NB_1(ASDU asdu)//Type = 111 测量参数标度化
{
	CString StrTemp,Statestr,StrRe="",KPAstr,LPCstr,POPstr;
	int OBJAddr,Value;
	unsigned char StateData=0;
	for (int i=0;i<asdu.VSQ.N;i++)
	{
		for (int k=0;k<InfoOBJ_lenNum;k++)
		{
			OBJAddr =+ (asdu.data[i*(3+InfoOBJ_lenNum)+k]<<(8*k));
		}
		Value = asdu.data[i*(3+InfoOBJ_lenNum)+InfoOBJ_lenNum]|(asdu.data[i*(3+InfoOBJ_lenNum)+InfoOBJ_lenNum+1]<<8);
		StateData = asdu.data[i*(3+InfoOBJ_lenNum)+InfoOBJ_lenNum+2];
		switch(StateData&0x3f)
		{
		case 0:KPAstr = "未用";
			break;
		case 1:KPAstr = "门限值";
			break;
		case 2:KPAstr = "平滑系数";
		    break;
		case 3:KPAstr = "测量值下限";
		    break;
		case 4:KPAstr = "测量值上";
		    break;
		default:
		    break;
		}
		if (StateData&0x40==0x40)
		{
			LPCstr="改变";
		} 
		else
		{
			LPCstr="未改变";
		}
		if (StateData&0x80==0x80)
		{
			POPstr="未运行";
		} 
		else
		{
			POPstr="运行";
		}
		CString	strobjdata = "";
		for (int j=0;j<InfoOBJ_lenNum+3;j++)
		{
			StrTemp = strobjdata;
			strobjdata.Format("%s%02XH ",StrTemp,asdu.data[i*(3+InfoOBJ_lenNum)+j]);
		}
		StrTemp = StrRe;
		StrRe.Format("%s \r\n【<%s>对象码:%d 值:%d 状态(%d KPA:%s LPC:%s POP:%s)】",StrTemp,strobjdata,OBJAddr,Value,StateData,KPAstr,LPCstr,POPstr);
	}
	return StrRe;
}

CString CProtocol02_101::ParseASDU_BCR(unsigned char bcr)
{
	CString StrRe,CYstr,CAstr,SBstr,IVstr;
	unsigned char sq = bcr&0x1f;
	if (bcr&0x20==0x20)
	{
		CYstr="在相应的累加周期中计数器溢出";
	} 
	else
	{
		CYstr="在相应的累加周期中计数器未溢出";
	}
	if (bcr&0x40==0x40)
	{
		CAstr = "计数器已调整";
	} 
	else
	{
		CAstr = "计数器未调整";
	}
	if (bcr&0x80==0x80)
	{
		IVstr = "无效";
	} 
	else
	{
		IVstr = "有效";
	}
	StrRe.Format("顺序号:%d CY:%s CA:%s IV:%s",sq,CYstr,CAstr,IVstr);
	return StrRe;
}
CString CProtocol02_101::ParseASDU_QDS(int type,unsigned char diq)
{
	CString StrRe,SPIstr,BLstr,SBstr,NTstr,IVstr;
	//type=1SIQ type=2DIQ type=3QDS type=4QDP
	switch(type)
	{
	case 1:
		{
			if (diq&0x01 == 0)
			{
				SPIstr="开";
			} 
			else
			{
				SPIstr="合";
			}
		}
		break;
	case 2:
		{
			switch(diq&0x3)
			{
			case 0:SPIstr ="中间态";
				break;
			case 1:SPIstr ="开";
				break;
			case 2:SPIstr ="合";
				break;
			case 3:SPIstr ="不确定";
				break;
			default:SPIstr ="错误";
				break;
			}
		}
		break;
	case 3:
		{
			if (diq&0x01 == 0)
			{
				SPIstr="未溢出";
			} 
			else
			{
				SPIstr="溢出";
			}
		}
	    break;
	case 4:
	    break;
	default:
	    break;
	}
	
	if (diq&0x10 == 0)
	{
		BLstr="未被闭锁";
	} 
	else
	{
		BLstr="被闭锁";
	}
	if (diq&0x20 == 0)
	{
		SBstr="未被取代";
	} 
	else
	{
		SBstr="被取代";
	}
	if (diq&0x40 == 0)
	{
		NTstr="当前值";
	} 
	else
	{
		NTstr="非当前值";
	}
	if (diq&0x80 == 0)
	{
		IVstr="有效";
	} 
	else
	{
		IVstr="无效";
	}
	StrRe.Format("状态:%s  BL:%s SB:%s NT:%s IV:%s",SPIstr,BLstr,SBstr,NTstr,IVstr);
	return StrRe;
}

CString CProtocol02_101::ParseASDU_DCO(int type,unsigned char StateData)
{
	CString StrRe,CSstr,QUstr,SEstr;
	unsigned char QOC;
	//type=1SIQ type=2DIQ type=3QDS type=4QDP
	switch(type)
	{
	case 1://双命令
		{
			switch(StateData&0x1)
			{
			case 0:CSstr ="开";
				break;
			case 1:CSstr ="合";
				break;
			default:CSstr ="错误";
				break;
			}
		}
		break;
	case 2://双命令
		{
			switch(StateData&0x3)
			{
			case 0:CSstr ="不允许";
				break;
			case 1:CSstr ="开";
				break;
			case 2:CSstr ="合";
				break;
			case 3:CSstr ="不允许";
				break;
			default:CSstr ="错误";
				break;
			}
		}
		break;
	case 3://步调命令
		{
			switch(StateData&0x3)
			{
			case 0:CSstr ="不允许";
				break;
			case 1:CSstr ="降一步";
				break;
			case 2:CSstr ="升一步";
				break;
			case 3:CSstr ="不允许";
				break;
			default:CSstr ="错误";
				break;
			}
		}
		break;
	default: CSstr="错误";
		break;
	}
	QOC = StateData&0xfc;
	if (QOC&0x80==0x80)
	{
		SEstr = "选择";
	} 
	else
	{
		SEstr = "执行";
	}
	switch((QOC&0x7c)>>2)
	{
	case 0:QUstr = "无另外的定义";
		break;
	case 1:QUstr = "短脉冲";
		break;
	case 2:QUstr = "长脉冲";
	    break;
	case 3:QUstr = "持续";
	    break;
	default:QUstr = "协议兼容";
	    break;
	}
	StrRe.Format("命令:%s QU:%s SE:%s",CSstr,QUstr,SEstr);
	return StrRe;
}