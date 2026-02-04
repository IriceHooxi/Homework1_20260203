#include <bits/stdc++.h>
#include "robot.h"

class RobotManager{
private:
    std::vector<std::shared_ptr<Robot>> normal_robots_;
    std::vector<std::shared_ptr<Robot>> destroyed_robots_;
    uint32_t last_time_=0;

    std::vector<std::shared_ptr<Robot>>::iterator
    FindRobot(std::vector<std::shared_ptr<Robot>>&container,uint32_t team_id,uint32_t robot_id){
        for(auto it=container.begin();it<container.end();++it){
            std::shared_ptr<Robot> robot=*it;
            if(robot->get_team_id()==team_id&&robot->get_robot_id()==robot_id){
                return it;
            }
        }
        return container.end();
    }

    void MoveToDestroyed(std::shared_ptr<Robot> robot){
        auto it=FindRobot(normal_robots_,robot->get_team_id(),robot->get_robot_id());
        if(it!=normal_robots_.end()){
            normal_robots_.erase(it);
        }
        destroyed_robots_.push_back(robot);
        std::cout<<"D "<<robot->get_team_id()<<" "<<robot->get_robot_id()<<std::endl;
    }

    void Calculate(uint32_t current_time){
        uint32_t time_d=current_time-last_time_;
        for(auto it=normal_robots_.begin();it<normal_robots_.end();){
            auto robot=*it;
            uint32_t current_heat=robot->get_current_heat();
            uint32_t max_heat=robot->get_max_heat();
            uint32_t current_hp=robot->get_current_hp();
            uint32_t excess_heat=std::max(int(current_heat-max_heat),0);
            uint32_t damage=std::min(excess_heat,time_d);
            uint32_t new_hp=current_hp>damage?current_hp-damage:0;
            robot->set_current_hp(new_hp);

            if(new_hp==0){
                MoveToDestroyed(robot);
                it=normal_robots_.erase(it);
                continue;
            }

            uint32_t new_heat=current_heat>time_d?current_heat-time_d:0;
            robot->set_current_heat(new_heat);

            ++it;
        }
        last_time_=current_time;
    }

    void HandleCmdA(const std::vector<uint32_t>&p){
        uint32_t team_id=p[0];
        uint32_t robot_id=p[1];
        uint32_t type=p[2];

        auto destroyed_it=FindRobot(destroyed_robots_,team_id,robot_id);
        if(destroyed_it!=destroyed_robots_.end()){
            auto robot=*destroyed_it;
            bool type_match;
            if((type==0&&robot->GetType()==RobotType::Infantry)||(type==1&&robot->GetType()==RobotType::Engineer)){type_match=true;}
            else{type_match=false;}
            if(type_match){
                destroyed_robots_.erase(destroyed_it);
                robot->set_current_hp(robot->get_max_hp());
                robot->set_current_heat(0);
                normal_robots_.push_back(robot);
            }
            return;
        }

        auto normal_it=FindRobot(normal_robots_,team_id,robot_id);//remove
        if(normal_it!=normal_robots_.end()){
            return;
        }

        std::shared_ptr<Robot>new_robot;//new
        if(type==0){
            new_robot=std::make_shared<InfantryRobot>(team_id,robot_id);
        } else if(type==1){
            new_robot=std::make_shared<EngineerRobot>(team_id,robot_id);
        } else{
            return;
        }
        normal_robots_.push_back(new_robot);
    }

    void HandleCmdF(const std::vector<uint32_t>&p){
        uint32_t team_id=p[0];
        uint32_t robot_id=p[1];
        uint32_t damage=p[2];

        auto normal_it=FindRobot(normal_robots_,team_id,robot_id);
        if(normal_it==normal_robots_.end()){
            return;
        }

        auto robot=*normal_it;
        uint32_t new_hp=robot->get_current_hp()>damage?robot->get_current_hp()-damage:0;
        robot->set_current_hp(new_hp);

        if(new_hp==0){
            MoveToDestroyed(robot);
            normal_robots_.erase(normal_it);
        }
    }

    void HandleCmdH(const std::vector<uint32_t>&p){
        uint32_t team_id=p[0];
        uint32_t robot_id=p[1];
        uint32_t heat_add=p[2];

        auto normal_it=FindRobot(normal_robots_,team_id,robot_id);
        if(normal_it==normal_robots_.end()||(*normal_it)->GetType()!=RobotType::Infantry){
            return;
        }

        uint32_t new_heat=(*normal_it)->get_current_heat() + heat_add;
        (*normal_it)->set_current_heat(new_heat);
    }

    void HandleCmdU(const std::vector<uint32_t>&p){
        uint32_t team_id=p[0];
        uint32_t robot_id=p[1];
        uint32_t target_level=p[2];

        auto normal_it=FindRobot(normal_robots_,team_id,robot_id);
        if(normal_it==normal_robots_.end()||(*normal_it)->GetType()!=RobotType::Infantry){
            return;
        }

        auto infantry=*normal_it;
        if(target_level>infantry->GetLevel()&&target_level <= 3){
            infantry->Upgrade(target_level);
        }
    }
    RobotManager()=default;

public:
    static RobotManager& GetInstance(){
        static RobotManager instance;
        return instance;
    }

    void ProcessCmd(uint32_t current_time,char cmd,const std::vector<uint32_t>&p){
        Calculate(current_time);
        switch (cmd){
            case 'A':{HandleCmdA(p);break;}
            case 'F':{HandleCmdF(p);break;}
            case 'H':{HandleCmdH(p);break;}
            case 'U':{HandleCmdU(p);break;}
            default:break;
        }
    }

    RobotManager(const RobotManager&)=delete;
    RobotManager& operator=(const RobotManager&)=delete;
};

int main(){
    uint32_t N;//10^9
    std::cin>>N;

    auto& manager=RobotManager::GetInstance();

    for(uint32_t i=0;i<N;++i){
        uint32_t current_time;
        char cmd;
        std::vector<uint32_t> p(3);
        std::cin>>current_time>>cmd>>p[0]>>p[1]>>p[2];
        manager.ProcessCmd(current_time,cmd,p);
    }
    return 0;
}