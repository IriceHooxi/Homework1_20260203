#ifndef __ROBOT_H__
#define __ROBOT_H__

#include <bits/stdc++.h>

enum class RobotType{
    Infantry,//0
    Engineer//1
};

const uint32_t EngineerMaxHp=300,
EngineerMaxHeat=0,
InfantryMaxHp[4]={0,100,150,250},//1,2,3
InfantryMaxHeat[4]={0,100,200,300};

//Base
class Robot{
protected:
    const uint32_t TeamID_;
    const uint32_t RobotID_;
    uint32_t MaxHp_;
    uint32_t MaxHeat_;
    uint32_t CurrentHp_;
    uint32_t CurrentHeat_;

public:
    Robot(uint32_t TeamID,uint32_t RobotID)
        :TeamID_(TeamID),RobotID_(RobotID),MaxHp_(0),MaxHeat_(0),CurrentHp_(0),CurrentHeat_(0){}

    virtual ~Robot()=default;
    virtual void Upgrade(uint32_t target_level)=0;
    virtual RobotType GetType() const=0;
    virtual uint32_t GetLevel() const=0;

    uint32_t get_team_id() const {return TeamID_;}
    uint32_t get_robot_id() const {return RobotID_;}
    uint32_t get_max_hp() const {return MaxHp_;}
    uint32_t get_max_heat() const {return MaxHeat_;}
    uint32_t get_current_hp() const {return CurrentHp_;}
    uint32_t get_current_heat() const {return CurrentHeat_;}

    void set_current_hp(uint32_t hp) {CurrentHp_=hp;}
    void set_current_heat(uint32_t heat) {CurrentHeat_=heat;}

protected:
    void set_max_hp(uint32_t max_hp) {MaxHp_=max_hp;}
    void set_max_heat(uint32_t max_heat) {MaxHeat_=max_heat;}
};

class InfantryRobot:public Robot{
private:
    uint32_t level_;
    
    void Update(){
        set_max_hp(InfantryMaxHp[level_]);
        set_max_heat(InfantryMaxHeat[level_]);
    }

public:
    InfantryRobot(uint32_t TeamID,uint32_t RobotID,uint32_t level=1)
        :Robot(TeamID,RobotID),level_(level){
            Update();
            set_current_hp(get_max_hp());
            set_current_heat(0);
        }

    void Upgrade(uint32_t target_level) override{
        if(target_level>level_&&target_level<=3){
            level_=target_level;
            Update();
            set_current_hp(get_max_hp());//reset
        }
    }

    RobotType GetType() const override {return RobotType::Infantry;}
    uint32_t GetLevel() const override {return level_;}
};

class EngineerRobot:public Robot{
// private:
//     uint32_t level_;
    
//     void Update(){
//         set_max_hp(InfantryMaxHp[level_]);
//         set_current_heat(InfantryMaxHeat[level_]);
//     }

public:
    EngineerRobot(uint32_t TeamID,uint32_t RobotID)
        :Robot(TeamID,RobotID){
            set_max_hp(EngineerMaxHp);
            set_max_heat(EngineerMaxHeat);
            set_current_hp(get_max_hp());
            set_current_heat(0);
        }

    void Upgrade(uint32_t target_level) override {}
    RobotType GetType() const override {return RobotType::Engineer;}
    uint32_t GetLevel() const override {return 0;}
};
#endif