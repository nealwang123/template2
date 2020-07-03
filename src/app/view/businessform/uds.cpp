#include "uds.h"

UDS::UDS(QObject *parent) : QObject(parent)
{

}
/// <summary>
/// 接收和发送函数
/// </summary>
/// <param name="data"></data可以是很多的数据不一定是单帧数据可以实多针数据/>
/// <param name="can_id"></param>
/// <returns></returns>
ECANStatus UDS::SendAndReceive(byte data[],uint can_id){
    ECANStatus result = _STATUS_ERR ;

    session=new Session();

    session->sendframe.byteAmount = sizeof(data);
    session->sendframe.array = data;
    session->sendframe.offset = 0;
    session->sendframe.SID = data[0];

    //表示相关数据的发送
    result = sendData(data, can_id);//发送数据，包括单帧和多帧数据

    //判断数据发送的结果
    if (result != _STATUS_OK)
    {
        return _STATUS_ERR;
    }
    else
    {
        return _STATUS_OK;
    }
}
/// <summary>
/// 发送数据，包括单帧和多帧数据
/// </summary>
/// <param name="data"></param>
/// <param name="canID"></param>
/// <returns></returns>
ECANStatus UDS::sendData(byte data[], uint canID){

    return _STATUS_OK;
}//包括发送数据和CAN地址信息
/// <summary>
/// 接收数据处理
/// </summary>
void UDS::ReceiveDataProc(){

}
