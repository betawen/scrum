//
// Created by beta on 11/4/18.
//

#include "dafuweng.h"


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

void step_into_mine () {
    int map_pos = player[tmp_player_num].pos;
    if (map[map_pos].map_attribute == '$') {
        player[tmp_player_num].dot += 1;
        printf("你的点数增加了1哦~\n");
        printf("你现在的总点数是%d\n", player[tmp_player_num].dot);
        return;

    }
}
