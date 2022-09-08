#include <iostream>
#include "Animator.h"
#include "VehicleBase.h"

int main()
{
    //Animator::MAX_VEHICLE_COUNT = 9999;  // vehicles will be displayed with four digits
    Animator::MAX_VEHICLE_COUNT = 999;  // vehicles will be displayed with three digits
    //Animator::MAX_VEHICLE_COUNT = 99;  // vehicles will be displayed with two digits

    int halfSize = 8;  // number of sections before intersection

    Animator anim(halfSize);

    // construct vectors of VehicleBase* of appropriate size, init to nullptr
    std::vector<VehicleBase*> westbound(halfSize * 2 + 2, nullptr);
    std::vector<VehicleBase*> eastbound(halfSize * 2 + 2, nullptr);
    std::vector<VehicleBase*> southbound(halfSize * 2 + 2, nullptr);
    std::vector<VehicleBase*> northbound(halfSize * 2 + 2, nullptr);

    char dummy;

    // test drawing vehicles moving eastbound and westbound
    VehicleBase vb1(VehicleType::car, Direction::east);
    VehicleBase vb2(VehicleType::suv, Direction::east);
    VehicleBase vb3(VehicleType::truck, Direction::east);
    VehicleBase vb4(VehicleType::car, Direction::west);
    VehicleBase vb5(VehicleType::suv, Direction::west);
    VehicleBase vb6(VehicleType::truck, Direction::west);

    VehicleBase vb7(VehicleType::car, Direction::south);
    VehicleBase vb8(VehicleType::suv, Direction::south);
    VehicleBase vb9(VehicleType::truck, Direction::south);
    VehicleBase vb10(VehicleType::car, Direction::north);
    VehicleBase vb11(VehicleType::suv, Direction::north);
    VehicleBase vb12(VehicleType::truck, Direction::north);
    VehicleBase vb13(vb12);  // just test the copy constructor -- should be #011

    int i = 0;
    int j = 0;

    anim.setLightNorthSouth(LightColor::red);
    anim.setLightEastWest(LightColor::green);

    for (; i < 4; i++)
    {
        eastbound[10+i] = eastbound[11+i] = &vb1;
        eastbound[6+i] = eastbound[7+i] = eastbound[8+i] = &vb2;
        eastbound[0+i] = eastbound[1+i] = eastbound[2+i] = eastbound[3+i] = &vb3;

        westbound[9+i] = westbound[10+i] = &vb4;
        westbound[6+i] = westbound[7+i] = westbound[8+i] = &vb5;
        westbound[0+i] = westbound[1+i] = westbound[2+i] = westbound[3+i] = &vb6;

        southbound[12+j] = southbound[13+j] = &vb7;
        southbound[5+j] = southbound[6+j] = southbound[7+j] = &vb8;
        southbound[0+j] = southbound[1+j] = southbound[2+j] = southbound[3+j] = &vb9;

        northbound[12+j] = northbound[13+j] = &vb10;
        northbound[5+j] = northbound[6+j] = northbound[7+j] = &vb11;
//        northbound[0+j] = northbound[1+j] = northbound[2+j] = northbound[3+j] = &vb12;
        northbound[0+j] = northbound[1+j] = northbound[2+j] = northbound[3+j] = &vb13;

        anim.setVehiclesNorthbound(northbound);
        anim.setVehiclesWestbound(westbound);
        anim.setVehiclesSouthbound(southbound);
        anim.setVehiclesEastbound(eastbound);

        anim.draw(i);
        std::cin.get(dummy);

        southbound.assign(halfSize * 2 + 2, nullptr); // reset vector
        northbound.assign(halfSize * 2 + 2, nullptr); // reset
        eastbound.assign(halfSize * 2 + 2, nullptr); // reset
        westbound.assign(halfSize * 2 + 2, nullptr); // reset

        if (i == 1) anim.setLightEastWest(LightColor::yellow);
    }

    anim.setLightNorthSouth(LightColor::green);
    anim.setLightEastWest(LightColor::red);

    for (; j < 5; j++)
    {
        eastbound[10+i] = eastbound[11+i] = &vb1;
        eastbound[6+i] = eastbound[7+i] = eastbound[8+i] = &vb2;
        eastbound[0+i] = eastbound[1+i] = eastbound[2+i] = eastbound[3+i] = &vb3;

        westbound[9+i] = westbound[10+i] = &vb4;
        westbound[6+i] = westbound[7+i] = westbound[8+i] = &vb5;
        westbound[0+i] = westbound[1+i] = westbound[2+i] = westbound[3+i] = &vb6;

        southbound[12+j] = southbound[13+j] = &vb7;
        southbound[5+j] = southbound[6+j] = southbound[7+j] = &vb8;
        southbound[0+j] = southbound[1+j] = southbound[2+j] = southbound[3+j] = &vb9;

        northbound[12+j] = northbound[13+j] = &vb10;
        northbound[5+j] = northbound[6+j] = northbound[7+j] = &vb11;
//        northbound[0+j] = northbound[1+j] = northbound[2+j] = northbound[3+j] = &vb12;
        northbound[0+j] = northbound[1+j] = northbound[2+j] = northbound[3+j] = &vb13;

        anim.setVehiclesNorthbound(northbound);
        anim.setVehiclesWestbound(westbound);
        anim.setVehiclesSouthbound(southbound);
        anim.setVehiclesEastbound(eastbound);

        anim.draw(i+j);
        std::cin.get(dummy);

        southbound.assign(halfSize * 2 + 2, nullptr); // reset
        northbound.assign(halfSize * 2 + 2, nullptr); // reset
        eastbound.assign(halfSize * 2 + 2, nullptr); // reset
        westbound.assign(halfSize * 2 + 2, nullptr); // reset

        if (j == 2) anim.setLightNorthSouth(LightColor::yellow);
    }

}
