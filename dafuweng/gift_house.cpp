//
// Created by beta on 11/4/18.
//

#include "dafuweng.h"
#include <time.h>

extern int player_num;
extern struct player_struct player[max_player_num];
extern char name_ch[4];
extern char full_name[4][20];
extern struct map_struct map[max_node_num];
extern int init_money; //初始资金
extern char name_id[10];   //记录输入编号
extern int tmp_player_num;   //当前玩家序号
extern bool player_alive[max_player_num];
extern int alive_num;
int gift_money = 2000;
int gift_dots = 200;
int gift_god_of_wealth ;

void step_into_gift_house() {

    printf("欢迎光临礼品屋，请选择一件您喜欢的礼品:\n【1】获得奖金2000元；\n【2】获得点数200；\n【3】财神附体：五轮内免付租金；\n【F】直接退出；\n");
    int map_pos = player[tmp_player_num].pos;
    int gift;
    while(1) {
        gift = getchar();
        if (gift == '1')  {
            player[tmp_player_num].money += 2000;
            printf("你获得了2000元\n");
            printf("你现在有%d元\n", player[tmp_player_num].money);
            return;
        } else if (gift == '2') {
            player[tmp_player_num].dot += 200;
            printf("你获得了200点数\n");
            printf("你现在的点数是%d\n", player[tmp_player_num].dot);
            return;
        }else if (gift == '3') {
            player[tmp_player_num].remained_gods_of_wealth += 5;
            printf("财神附体，五轮内免收租金~\n");
            return;
        }else if (gift == 'F'){
            return;
        }else{
            printf("emmm没有这个礼品呢~\n请重新输入：");
        }
    }


}

