#include "candatamodelfactory.h"

CANDataModelFactory::CANDataModelFactory()
{

}
AbstractDataModel* CANDataModelFactory::createDeviceMethod(){
    return new CANDataModel();
}
