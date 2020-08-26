#include "dbcreader.h"
//#include<iostream>
//#include<bitset>
//using namespace std;
//SINGLETON_STATIC_INITIALIZE(DBCReader) // [3]

DBCReader::DBCReader(){
    qDebug() << "DBCReader()";

}
DBCReader::~DBCReader(){
    qDebug() << "~DBCReader()";
    //释放所有地址
    //delete dbcMsg;
}

/********************************************
*
*********************************************/
int DBCReader::loadDBCFile(QString filePath){

//    dbcMsg->vSignals=new DBCSignal[256][256];
//    dbcMsg->vSignals=new DBCSignal[256*256];
    qDebug()<<"sizeof(DBCMessage)="<<sizeof(DBCMessage);
    //清除MAP
    DBCMap.clear();
    //打开文件
    QFile file;
//    QString f = QFileDialog::getOpenFileName(this, QString("选择文件"), QString("/"),QString("TEXT(*.txt)"));
    file.setFileName(filePath);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        qDebug()<<"DBC open failed;";
        return -1;
    }
    //只解DBC文件内BO_、SG
    while(!file.atEnd())
    {
        QByteArray line = file.readLine();
//        QString str(line);
        QString str=QString::fromLocal8Bit(line.data(),line.size()) ;
        //获取到MSG
        if(str.indexOf("BO_")==0){
            qDebug()<<"BO_"<<str;
            DBCMessage* dbcMsg = new DBCMessage;      //需要创建一个Qlist结构体类型的空间
            QStringList msgList=str.split(" ");
            if(msgList.length()<5) {
                qDebug()<<TR("msg异常:")<<str;
                continue;
            }
            //解析MSG

            quint32 key=msgList.at(1).toUInt();
            dbcMsg->nID=key;
            dbcMsg->nExtend=0;
            dbcMsg->nSize=8;
            dbcMsg->strName=msgList.at(2);
            dbcMsg->strComment=msgList.at(4);
            //解析MSG所属SG
            int sigCount[_DBC_SIGNAL_MULTI_GROUP_+1]={0};
            int offset=0;
            //
            while(1){//遍历所有信号

                QByteArray lineSignal =file.readLine();
//                QString strSignal(lineSignal);
                QString strSignal=QString::fromLocal8Bit(lineSignal.data(),lineSignal.size()) ;
                if(strSignal.indexOf(" SG_")==0){
                    qDebug()<<"  "<<strSignal;
                    //填充sig结构
                    QStringList sigList=strSignal.split(" ");
                    qDebug()<<"sigList:"<<sigList<<"sigList.length():"<<sigList.length();
                    if(sigList.length()<9){
                        break;
                    }else if(sigList.length()==9){//不带服用信号类型
                        offset=0;
                    }else if(sigList.length()==10){//带服用信号类型
                        offset=1;
                    }
                    //复用信号类型标识
                    int multitype=0;
                    int multivalue=0;
                    if(offset==0){
                        multitype=0;
                        multivalue=0;
                    }else if(offset==1){
                        if(sigList.at(3)=="M"){
                            multitype=1;
                            multivalue=0;
                        }else{
                            int value=sigList.at(3).trimmed().remove(0,1).toInt();
                            multitype=2;
                            multivalue=value;
                        }

                    }
                    fillDbcMsg(sigList,*dbcMsg,sigCount[multivalue],offset);
                    sigCount[multivalue]++;
                }else{//一个msg下sig获取完毕
                    //信号数量更新
                    for(int i=0;i<_DBC_SIGNAL_MULTI_GROUP_+1;i++){
                        dbcMsg->nSignalCount[i]=sigCount[i];
                    }

                    //dbcMsg 添加到map
                    DBCMap.insert(dbcMsg->nID,dbcMsg);
                    //
                    for(int i=0;i<_DBC_SIGNAL_MULTI_GROUP_+1;i++){
                        sigCount[i]=0;
                    }
                    qDebug()<<"msg end 结束...";
                    break;
                }
            }
        }
//        qDebug()<< str.simplified();;


    }
    file.close();

    return 0;

}
// SG_ Reserved_0 m0 : 15|10@0+ (1,0) [0|0] "N/A" Vector__XXX
// SG_ SRR_FRAME_INDEX_1 : 0|8@0+ (1,0) [0|255] "N/A" Vector__XXX
void DBCReader::fillDbcMsg(QStringList& sigList,DBCMessage& dbcMsg,int sigCount,int offset){
    //一组信号
//    /GroupSignals groupSignals;
    //单个信号
    DBCSignal dbcSig;
    //匹配起始位、长度、是否为motorola格式
    QString pattern("\\d+"); //Qt匹配正数、负数、浮点数
    QRegExp rx(pattern);
    int pos = 0;
//    qDebug()<<sigList.at(4+offset);
    int tempCount=0;
    while ((pos = rx.indexIn(sigList.at(4+offset), pos)) != -1){
//    qDebug()<<TR("起始位、长度、是否为motorola格式")<<rx.cap(0).toInt();
        if(tempCount==0)
            dbcSig.nStartBit=rx.cap(0).toInt();
        else if(tempCount==1)
            dbcSig.nLen=rx.cap(0).toInt();
        else if(tempCount==2)
            dbcSig.is_motorola=rx.cap(0).toInt();
        pos += rx.matchedLength();
        tempCount++;
    }
    tempCount=0;
    pos=0;
    //是否为无符号数
    if(sigList.at(4+offset).right(1)=="+"){
        dbcSig.is_signed=0;
    }else if(sigList.at(4+offset).right(1)=="-"){
        dbcSig.is_signed=1;
    }
     //因子和偏移
     QRegExp rx1("-?\\d+(\\.\\d+)?");    // primitive floating point matching
     while ((pos = rx1.indexIn(sigList.at(5+offset), pos)) != -1) {
         if(tempCount==0){
             dbcSig.nFactor=rx1.cap(0).toDouble();
         }else if(tempCount==1){
             dbcSig.nOffset=rx1.cap(0).toDouble();
         }
//                         qDebug()<<TR("因子和偏移")<<rx1.cap(0).toInt();
         pos += rx1.matchedLength();
         tempCount++;
     }
     tempCount=0;
     pos=0;

     //最大和最小值
//                     QRegExp rx1("-?\\d+(\\.\\d+)?");    // primitive floating point matching
     while ((pos = rx1.indexIn(sigList.at(6+offset), pos)) != -1) {
         if(tempCount==0){
             dbcSig.nMin=rx1.cap(0).toDouble();
         }else if(tempCount==1){
             dbcSig.nMax=rx1.cap(0).toDouble();
         }
//                         qDebug()<<TR("最大和最小值")<<rx1.cap(0).toInt();
         pos += rx1.matchedLength();
         tempCount++;
     }
     tempCount=0;
     pos=0;

    dbcSig.nValue=0;
    dbcSig.nRawValue=0;
    //复用信号类型标识
    if(offset==0){
        dbcSig.multiplexer_type=0;
        dbcSig.multiplexer_value=0;
    }else if(offset==1){
        if(sigList.at(3)=="M"){
            dbcSig.multiplexer_type=1;
            dbcSig.multiplexer_value=0;
        }else{
            int value=sigList.at(3).trimmed().remove(0,1).toInt();
            dbcSig.multiplexer_type=2;
            dbcSig.multiplexer_value=value;
        }

    }

    //名字
    dbcSig.strName=sigList.at(2);
    //注释
    dbcSig.strComment=sigList.at(7+offset);;
    //值描述
    dbcSig.strValDesc=sigList.at(8+offset);;
    //信号添加到信号组

    dbcMsg.vSignals[dbcSig.multiplexer_value][sigCount]=dbcSig;
    ///xxy addin
//    dbcMsg.signalToindex.insert(dbcSig.strName,sigCount);
    ///xxy addin
}


//raw data 6   "96"   "ac"   "86"   "80"   "f9"   "60"   "4"   "0"
QStringList DBCReader::parseMSGWithData(quint32 ID,BYTE Data[],quint8 m_id,bool m_isMulti){

    QStringList tmp;
        if(DBCMap.contains(ID)){
             tmp.clear();
             DBCMessage* temp=DBCMap.find(ID).value();

    //         qDebug()<<"MSG_:"<<QString("0x%1").arg(ID,8,16,QChar('0'))<<"下信号数量"<<temp->nSignalCount;
             for(int i=0;i<temp->nSignalCount[m_id];i++){
                 int startBit=temp->vSignals[m_id][i].nStartBit;
                 int len=temp->vSignals[m_id][i].nLen;
                 //motorola格式解析
                 if(temp->vSignals[m_id][i].is_motorola==0){
                     //nowByteLast,到字节尾的
                     //5
                     int start2end=startBit-((startBit/8)*8-1);
                     if(start2end>=len){//非跨字节信号
                         //解析当前信号
                        temp->vSignals[m_id][i].nRawValue= (Data[startBit/8]>>(start2end-len))&mybitset(len);
                        if(temp->vSignals[m_id][i].is_signed==0){
                            temp->vSignals[m_id][i].nValue=temp->vSignals[m_id][i].nRawValue*temp->vSignals[m_id][i].nFactor+temp->vSignals[m_id][i].nOffset;
                        }else  if(temp->vSignals[m_id][i].is_signed==1){
                            int flage=(temp->vSignals[m_id][i].nRawValue>>(temp->vSignals[m_id][i].nLen-1))&0x01;
                            if(flage==0){//无符号
                                temp->vSignals[m_id][i].nValue=temp->vSignals[m_id][i].nRawValue*temp->vSignals[m_id][i].nFactor+temp->vSignals[m_id][i].nOffset;
                            }else if(flage==1){//有符号
                                int bitlen=temp->vSignals[m_id][i].nLen;
                                quint16 bitset1=0x7FFF;
                                int tempvalue=(~((temp->vSignals[m_id][i].nRawValue&(bitset1>>(16-bitlen)))-1))&(bitset1>>(16-bitlen));
        //                        qDebug()<<"tempvalue========"<<tempvalue<<(0-tempvalue);
                                temp->vSignals[m_id][i].nValue=0-tempvalue*temp->vSignals[m_id][i].nFactor+temp->vSignals[m_id][i].nOffset;
                            }
                        }
                     }else{//跨字节序号
                     ////ff e5 fc bf 97 f2 fe 5f
                     //凑足信号长度
                     temp->vSignals[m_id][i].nRawValue = parseDataCrossByte(startBit, len, start2end, Data);
                         if(temp->vSignals[m_id][i].is_signed==0){
                            temp->vSignals[m_id][i].nValue=temp->vSignals[m_id][i].nRawValue*temp->vSignals[m_id][i].nFactor+temp->vSignals[m_id][i].nOffset;
                         }else  if(temp->vSignals[m_id][i].is_signed==1){
                            int flage=(temp->vSignals[m_id][i].nRawValue>>(temp->vSignals[m_id][i].nLen-1))&0x01;
        //                        qDebug()<<"0000===="<<temp->vSignals[m_id][i].is_signed;
                                int bitlen=temp->vSignals[m_id][i].nLen;
                                int tempvalue = parseSignedData(flage, bitlen, temp->vSignals[m_id][i].nRawValue);
                                if (flage == 0)
                                    temp->vSignals[m_id][i].nValue = tempvalue*temp->vSignals[m_id][i].nFactor + temp->vSignals[m_id][i].nOffset;
                                else if (flage == 1)
                                    temp->vSignals[m_id][i].nValue = 0 - tempvalue*temp->vSignals[m_id][i].nFactor + temp->vSignals[m_id][i].nOffset;
                         }
                    }
                 }else{//Intel格式信号
    //                 qDebug()<<"temp->vSignals[m_id][i].is_motorola===="<<temp->vSignals[m_id][i].is_motorola;
                    //所有数据存到uint64八字节数据中
                    quint64 allData=0;
                    allData=((((quint64)Data[7])&0xFF)<<(8*7))
                                    +((((quint64)Data[6])&0xFF)<<(8*6))
                                    +((((quint64)Data[5])&0xFF)<<(8*5))
                                    +((((quint64)Data[4])&0xFF)<<(8*4))
                                    +((((quint64)Data[3])&0xFF)<<(8*3))
                                    +((((quint64)Data[2])&0xFF)<<(8*2))
                                    +((((quint64)Data[1])&0xFF)<<(8*1))
                                    +(((quint64)Data[0])&0xFF);
                   startBit=startBit+len-1;
                   //解析当前信号，取出原始值，判断有符号无符号
                   temp->vSignals[m_id][i].nRawValue= (allData>>(startBit-len+1))&(0xFFFFFFFFFFFFFFFF>>(64-len));
                   if(temp->vSignals[m_id][i].is_signed==0){
                       temp->vSignals[m_id][i].nValue=temp->vSignals[m_id][i].nRawValue*temp->vSignals[m_id][i].nFactor+temp->vSignals[m_id][i].nOffset;
                   }else{
                       int flage=(temp->vSignals[m_id][i].nRawValue>>(temp->vSignals[m_id][i].nLen-1))&0x01;
                       if(flage==0){
                            temp->vSignals[m_id][i].nValue=temp->vSignals[m_id][i].nRawValue*temp->vSignals[m_id][i].nFactor+temp->vSignals[m_id][i].nOffset;
                       }else if(flage==1){
                            quint64 bitset1=0x7FFFFFFFFFFFFFFF;
                            quint64 tempvalue=(~((temp->vSignals[m_id][i].nRawValue&(bitset1>>(64-len)))-1))&(bitset1>>(64-len));
                            temp->vSignals[m_id][i].nValue=0-tempvalue*temp->vSignals[m_id][i].nFactor+temp->vSignals[m_id][i].nOffset;
                       }
                   }
                 }
                 tmp.append(temp->vSignals[m_id][i].strName
                            +"|"
                            +QString("%1").arg(temp->vSignals[m_id][i].nRawValue, 8, 16, QChar('0'))
                            +"|"
                            +QString::number(temp->vSignals[m_id][i].nValue, 'f',2)
                            );
             }
             QStringList inverseList;
             for(int i=0;i<tmp.length();i++){
                 inverseList<<tmp.at(i);
             }
             return inverseList;
        }else{
            tmp.append("未解析到数据，parse null!");
            return tmp;
        }

}

quint64 DBCReader::parseDataCrossByte(int startBit, int len, int start2end, BYTE Data[])
{
    quint64 firstByteValue = (Data[startBit / 8] & mybitset(start2end)) << (len - start2end);

    quint64 midByteValue = 0;
    int completeByteNum = (len - start2end) / 8;
    for (int i = 0; i < completeByteNum; i++)
    {
        midByteValue += Data[startBit / 8 +1 + i] << (len - start2end - 8 * (i+1));
    }
    int end2start = (len - start2end) % 8;     //数据末尾所在字节据字节头的长度
    quint64 endByteValue = Data[startBit / 8 + 1 + completeByteNum] >> (8-end2start);

    quint64 result = firstByteValue + midByteValue + endByteValue;
    return result;
}

int DBCReader::parseSignedData(int flage, int bitlen, quint64 rawValue)
{
    int tempvalue = 0;
    if (bitlen <= 16)
    {
        quint16 bitset1 = 0x7FFF;
        if (flage == 0)
        {
            tempvalue = (((rawValue&(bitset1 >> (16 - bitlen)))));
        }
        else if (flage == 1)
        {
            tempvalue = (~((rawValue&(bitset1 >> (16 - bitlen))) - 1))&(bitset1 >> (16 - bitlen));
        }
    }
    else if (bitlen <= 32)
    {
        quint32 bitset1 = 0x7FFFFFFF;
        if (flage == 0)
        {
            tempvalue = (((rawValue&(bitset1 >> (32 - bitlen)))));
        }
        else if (flage == 1)
        {
            tempvalue = (~((rawValue&(bitset1 >> (32 - bitlen))) - 1))&(bitset1 >> (32 - bitlen));
        }
    }
    return tempvalue;
}
//添加到Model内
void DBCReader::debugDBCStruct(QString str){
////    qDebug()<<QString("------ %1 --------").arg(str);
        QMap <int,DBCMessage*>::iterator it;
        for( it=DBCMap.begin(); it!=DBCMap.end(); it++){
            qDebug()<<"++++++++++++++++MSG名字++++++++++++";

            qDebug() <<"key():"<< it.key() <<"value():"<<it.value()->strName<<"\t";
            DBCMessage* temp=it.value();
            for(int c=0;c<256;c++){
                if(temp->nSignalCount[c]>0){
                    qDebug()<<"temp->nSignalCount[c]:"<<temp->nSignalCount[c];

                    for(int i=0;i<temp->nSignalCount[c];i++){
                        qDebug()<<"-------->temp->vSignals["+QString::number(c,16)+"]["+QString::number(i,10)+"].strName "<<temp->vSignals[c][i].strName;
                        qDebug()<<"         temp->vSignals["+QString::number(c,16)+"]["+QString::number(i,10)+"].nStartBit "<<temp->vSignals[c][i].nStartBit;
                        qDebug()<<"         temp->vSignals["+QString::number(c,16)+"]["+QString::number(i,10)+"].nLen " <<temp->vSignals[c][i].nLen;
                        qDebug()<<"         temp->vSignals["+QString::number(c,16)+"]["+QString::number(i,10)+"].nMin " <<temp->vSignals[c][i].nMin;
                        qDebug()<<"         temp->vSignals["+QString::number(c,16)+"]["+QString::number(i,10)+"].nMax " <<temp->vSignals[c][i].nMax;
                        qDebug()<<"         temp->vSignals["+QString::number(c,16)+"]["+QString::number(i,10)+"].nFactor " <<temp->vSignals[c][i].nFactor;
                        qDebug()<<"         temp->vSignals["+QString::number(c,16)+"]["+QString::number(i,10)+"].nOffset " <<temp->vSignals[c][i].nOffset;
                        qDebug()<<"         temp->vSignals["+QString::number(c,16)+"]["+QString::number(i,10)+"].nRawValue "<<temp->vSignals[c][i].nRawValue;
                        qDebug()<<"         temp->vSignals["+QString::number(c,16)+"]["+QString::number(i,10)+"].nValue "<<temp->vSignals[c][i].nValue;
                    }
                }
            }

        }
}


/////xxy addin
bool DBCReader::parseMSGWithValue(quint32 ID,QString strName,QString strValue,BYTE Data[],quint8 m_id,bool m_isMulti)
{
    QStringList tmp;
        if(DBCMap.contains(ID)){
             tmp.clear();
             DBCMessage *temp=DBCMap.find(ID).value();

             int index=-1;
             if(temp->signalToindex.contains(strName)){
                 index=temp->signalToindex.find(strName).value();
                 if(temp->vSignals[m_id][index].strName!=strName)
                 {
                     qDebug()<<"parseMSGWithValue   ERROR!";
                     return false;
                 }
                 double numValue=strValue.toDouble();
                 temp->vSignals[m_id][index].nValue=numValue;
                 float rawValue=(numValue-temp->vSignals[m_id][index].nOffset)/temp->vSignals[m_id][index].nFactor;
                 if(temp->vSignals[m_id][index].is_signed==0){//无符号数
                    temp->vSignals[m_id][index].nRawValue=(quint64)((float)rawValue+0.5f);
                 }
                 else  if(temp->vSignals[m_id][index].is_signed==1){//有符号数
                    if(-EPSINON >= numValue)//负数
                    {
                        qint64 numValue_qint64=(qint64)((float)rawValue+0.5f);
                        //取反+1
                        temp->vSignals[m_id][index].nRawValue=(~qAbs(numValue_qint64))+1;
                    }
                    else if(numValue >=EPSINON)//正数
                        temp->vSignals[m_id][index].nRawValue=(quint64)((float)rawValue+0.5f);
                    else
                        temp->vSignals[m_id][index].nRawValue=0;
                 }
                 DBCMap[ID]=temp;
             }
             if(index==-1)
                 return false;

             //组合Data
             quint64 numValue_quint64=0;//0x9988776655443322;
             memcpy((BYTE*)&numValue_quint64,Data,8);//低字节--低位

             if(temp->vSignals[m_id][index].is_motorola==0){
                 int startBit=temp->vSignals[m_id][index].nStartBit;
                 int len=temp->vSignals[m_id][index].nLen;
                 int start2end=startBit-((startBit/8)*8-1);
                 if(start2end>=len){//非跨字节信号
                     numValue_quint64=numValue_quint64|(temp->vSignals[m_id][index].nRawValue<<(start2end-len+startBit/8*8));
                 }
                 else{

                     //第一字节填充
                     int remainLen=len;
                     int removeLen=start2end;
                     int startBit1=startBit;
                     numValue_quint64=numValue_quint64|(temp->vSignals[m_id][index].nRawValue>>(len-start2end)<<(startBit-start2end+1));

                     //剩余字节填充
                     while(1)
                     {
                         remainLen=remainLen-removeLen;
                         startBit1=(startBit1/8+2)*8-1;
                         quint64 rawValue=temp->vSignals[m_id][index].nRawValue;
                         quint64 rawValue1=0;//(numValue_quint64_temp>>(8-start2end)<<(8-start2end))^(numValue_quint64_temp);

                        //是否够一字节
                         if(remainLen/8!=0)//够   取整字节
                         {
                             rawValue1=rawValue^(rawValue>>remainLen<<remainLen);
                             removeLen=8;
                             rawValue1=rawValue1>>(remainLen-removeLen);
                             numValue_quint64=numValue_quint64|(rawValue1<<(startBit1-removeLen+1));

                             //计算剩余字节
                             if(remainLen-removeLen<=0)
                             {
                                 break;
                             }

                         }
                         else//不够 移位
                         {
                             removeLen=remainLen%8;
                             rawValue1=rawValue^(rawValue>>removeLen<<removeLen);
                             numValue_quint64=numValue_quint64|(rawValue1<<(startBit1-removeLen+1));
                             break;
                         }
                     }

                }

             }
             else{//Intel格式信号
                 int len=temp->vSignals[m_id][index].nLen;
                 int startBit=temp->vSignals[m_id][index].nStartBit+len-1;
                 int start2end=startBit-((startBit/8)*8-1);
                 if(start2end>=len){//非跨字节信号
                     numValue_quint64=numValue_quint64|(temp->vSignals[m_id][index].nRawValue<<(startBit-len+1));
                 }
                 else{

                     //最高字节填充
                     int remainLen=len;
                     int removeLen=start2end;
                     int startBit1=startBit;

                     numValue_quint64=numValue_quint64|(temp->vSignals[m_id][index].nRawValue>>(len-start2end)<<(startBit-start2end+1));

                     while(1)//剩余字节填充
                     {
                         remainLen=remainLen-removeLen;
                         startBit1=startBit1/8*8-1;
                         quint64 rawValue=temp->vSignals[m_id][index].nRawValue;
                         quint64 rawValue1=0;//(numValue_quint64_temp>>(8-start2end)<<(8-start2end))^(numValue_quint64_temp);

                         //是否够一字节
                         if(remainLen/8!=0)//够   取整字节
                         {
                             removeLen=8;
                             rawValue1=0xFF&(rawValue>>(remainLen-removeLen));

                             numValue_quint64=numValue_quint64|(rawValue1<<(startBit1-removeLen+1));


                             //计算剩余字节
                             if(remainLen-removeLen<=0)
                             {
                                 break;
                             }

                         }
                         else//不够 移位
                         {
                            removeLen=remainLen%8;
                            //取低位
                            rawValue1=rawValue^(rawValue>>remainLen<<remainLen);
                            //rawValue1=((0xFF<<(8-removeLen))>>(8-removeLen))^rawValue;
                            numValue_quint64=numValue_quint64|(rawValue1<<(startBit1-removeLen+1));
                            break;
                         }
                     }

                 }
            }
             memcpy(Data,(BYTE*)&numValue_quint64,8);

             return true;
        }else{
            tmp.append("未解析到数据，parse null!");
            qDebug()<<tmp;
            return false;
        }









//////////
/*{
                     //nowByteLast,到字节尾的
                     //5
                     int start2end=startBit-((startBit/8)*8-1);
                     if(start2end>=len){//非跨字节信号
                     //解析当前信号
                        temp->vSignals[m_id][i].nRawValue= (Data[startBit/8]>>(start2end-len))&mybitset(len);
                        if(temp->vSignals[m_id][i].is_signed==0){//无符号数
                            temp->vSignals[m_id][i].nValue=temp->vSignals[m_id][i].nRawValue*temp->vSignals[m_id][i].nFactor+temp->vSignals[m_id][i].nOffset;
                        }else  if(temp->vSignals[m_id][i].is_signed==1){//有符号数
                            int flage=(temp->vSignals[m_id][i].nRawValue>>(temp->vSignals[m_id][i].nLen-1))&0x01;
                            if(flage==0)
                                temp->vSignals[m_id][i].nValue=temp->vSignals[m_id][i].nRawValue*temp->vSignals[m_id][i].nFactor+temp->vSignals[m_id][i].nOffset;
                            else if(flage==1){
                                int bitlen=temp->vSignals[m_id][i].nLen;
                                quint16 bitset1=0x7FFF;
                                int tempvalue=(~((temp->vSignals[m_id][i].nRawValue&(bitset1>>(16-bitlen)))-1))&(bitset1>>(16-bitlen));
        //                        qDebug()<<"tempvalue========"<<tempvalue<<(0-tempvalue);
                                temp->vSignals[m_id][i].nValue=0-tempvalue*temp->vSignals[m_id][i].nFactor+temp->vSignals[m_id][i].nOffset;
                            }
                        }
                     }else{//跨字节序号
                     ////ff e5 fc bf 97 f2 fe 5f
                     //凑足信号长度
                     temp->vSignals[m_id][i].nRawValue = parseDataCrossByte(startBit, len, start2end, Data);
    //                if(temp->vSignals[m_id][i].strName=="SRR_TRACK_POS_Y"){
    //                 qDebug()<<QString::number(Data[0],16)
    //                         <<QString::number(Data[1],16)
    //                         <<QString::number(Data[2],16)
    //                         <<QString::number(Data[3],16)
    //                         <<QString::number(Data[4],16)
    //                         <<QString::number(Data[5],16)
    //                         <<QString::number(Data[6],16)
    //                         <<QString::number(Data[7],16);
    //                 qDebug()<<"temp->vSignals[m_id][i].is_signed===="<<temp->vSignals[m_id][i].is_signed;
    //                 qDebug()<<"rawdata:"<<temp->vSignals[m_id][i].strName<<((8-(len-start2end))>=0)
    //                         <<((Data[(startBit/8)+1]<<((len-start2end)-8))+(Data[(startBit/8)+2]>>(8-((len-start2end)-8))))
    //                         <<startBit<<start2end<<temp->vSignals[m_id][i].nLen<<"len"<<QString::number(temp->vSignals[m_id][i].nRawValue,16);
    //                }
                     if(temp->vSignals[m_id][i].is_signed==0){
                        temp->vSignals[m_id][i].nValue=temp->vSignals[m_id][i].nRawValue*temp->vSignals[m_id][i].nFactor+temp->vSignals[m_id][i].nOffset;
                     }else  if(temp->vSignals[m_id][i].is_signed==1){
    //                     qDebug<<"raw value::::"+QString("%1").arg(temp->vSignals[m_id][i].nRawValue, 4, 16, QChar('0'));
    //                     qDebug()<<"rawdata:"<<QString::number(temp->vSignals[m_id][i].nRawValue,16)
    //                             <<"sign pos:"<<QString::number(temp->vSignals[m_id][i].nRawValue>>15,16)
    //                             <<"flage:"<<QString::number((temp->vSignals[m_id][i].nRawValue>>(temp->vSignals[m_id][i].nLen-1))&0x01,16);
                        int flage=(temp->vSignals[m_id][i].nRawValue>>(temp->vSignals[m_id][i].nLen-1))&0x01;
                        if(flage==0){
    //                        qDebug()<<"0000===="<<temp->vSignals[m_id][i].is_signed;
                            int bitlen=temp->vSignals[m_id][i].nLen;
                            int tempvalue = parseSignedData(flage, bitlen, temp->vSignals[m_id][i].nRawValue);
                            if (flage == 0)
                                temp->vSignals[m_id][i].nValue = tempvalue*temp->vSignals[m_id][i].nFactor + temp->vSignals[m_id][i].nOffset;
                            else if (flage == 1)
                                temp->vSignals[m_id][i].nValue = 0 - tempvalue*temp->vSignals[m_id][i].nFactor + temp->vSignals[m_id][i].nOffset;
                        }
                     }

                 }else{//Intel格式信号
//                     qDebug()<<"temp->vSignals[m_id][i].is_motorola===="<<temp->vSignals[m_id][i].is_motorola;
                    //所有数据存到uint64八字节数据中
                     quint64 allData=0;

    //                 qDebug()<<QString::number(Data[0],16)
    //                                          <<QString::number(Data[1],16)
    //                                          <<QString::number(Data[2],16)
    //                                          <<QString::number(Data[3],16)
    //                                          <<QString::number(Data[4],16)
    //                                          <<QString::number(Data[5],16)
    //                                          <<QString::number(Data[6],16)
    //                                          <<QString::number(Data[7],16);
    //                 qDebug()<<QString::number(allData,16);
    //                  qDebug()<<QString::number(allData,16);
    //                   qDebug()<<QString::number(allData,16);
    //                    qDebug()<<QString::number(allData,16);
    //                     qDebug()<<QString::number(allData,16);
    //                      qDebug()<<QString::number(allData,16);
    //                       qDebug()<<QString::number(allData,16);

                    allData=((((quint64)Data[7])&0xFF)<<(8*7))
                                    +((((quint64)Data[6])&0xFF)<<(8*6))
                                    +((((quint64)Data[5])&0xFF)<<(8*5))
                                    +((((quint64)Data[4])&0xFF)<<(8*4))
                                    +((((quint64)Data[3])&0xFF)<<(8*3))
                                    +((((quint64)Data[2])&0xFF)<<(8*2))
                                    +((((quint64)Data[1])&0xFF)<<(8*1))
                                    +(((quint64)Data[0])&0xFF);
    //                qDebug()<<ID<<"allData"<<QString::number(allData,16);


                   startBit=startBit+len-1;
                   //解析当前信号，取出原始值，判断有符号无符号
                   temp->vSignals[m_id][i].nRawValue= (allData>>(startBit-len+1))&(0xFFFFFFFF>>(32-len));
                   if(temp->vSignals[m_id][i].is_signed==0){
                       temp->vSignals[m_id][i].nValue=temp->vSignals[m_id][i].nRawValue*temp->vSignals[m_id][i].nFactor+temp->vSignals[m_id][i].nOffset;
                   }else{
                       int flage=(temp->vSignals[m_id][i].nRawValue>>(temp->vSignals[m_id][i].nLen-1))&0x01;
                       if(flage==0){
                            temp->vSignals[m_id][i].nValue=temp->vSignals[m_id][i].nRawValue*temp->vSignals[m_id][i].nFactor+temp->vSignals[m_id][i].nOffset;
                       }else if(flage==1){
                            quint32 bitset1=0x7FFFFFFF;
                            int tempvalue=(~((temp->vSignals[m_id][i].nRawValue&(bitset1>>(32-len)))-1))&(bitset1>>(32-len));
                            temp->vSignals[m_id][i].nValue=0-tempvalue*temp->vSignals[m_id][i].nFactor+temp->vSignals[m_id][i].nOffset;
                       }
                   }
                 }
                 tmp.append(temp->vSignals[m_id][i].strName
                            +"|"
                            +QString("%1").arg(temp->vSignals[m_id][i].nRawValue, 8, 16, QChar('0'))
                            +"|"
                            +QString::number(temp->vSignals[m_id][i].nValue, 'f',2)
                            );
    //             qDebug()<<temp->vSignals[m_id][i].strName<<"nRawValue==="<<temp->vSignals[m_id][i].nRawValue<<"nValue"<<temp->vSignals[m_id][i].nValue;
    //             lastLen=len-nowLen;
    //             if(lastLen)

             }
             QStringList inverseList;
             for(int i=tmp.length();i>0;i--){
                 inverseList<<tmp.at(i-1);
             }
    //         QStringList::const_iterator constIterator;
    //              for (constIterator = tmp.end(); constIterator != tmp.begin();
    //                     ++constIterator){
    //                  qDebug()<<"parse:" << *constIterator<< endl;

    //              }

    //         for (QStringList::iterator i = --tmp.end(); i != --tmp.begin();)
    //         {
    //             qDebug() <<"parse :"<< *(--i);
    //         }
          */
}

/////xxy addin
quint8 DBCReader::mybitset(quint8 count){
    switch (count) {
    case 1:
        return 0x01;
        break;
    case 2:
        return 0x03;
        break;
    case 3:
        return 0x07;
        break;
    case 4:
        return 0x0F;
        break;
    case 5:
        return 0x1F;
        break;
    case 6:
        return 0x3F;
        break;
    case 7:
        return 0x7F;
        break;
    case 8:
        return 0xFF;
        break;
    default:
        break;
    }
}
