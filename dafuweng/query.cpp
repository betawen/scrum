//
// Created by yc on 10/30/18.
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

void query(int player_num)   //查询资产
{
    int i=0;
    struct house_id_struct *phouse;
    phouse=player[player_num].p_house_head->next;
    printf("money:%d\n",player[player_num].money);
    printf("dot:  %d\n",player[player_num].dot);
    while(phouse)
    {
        printf("  house_id:%d   ",phouse->house_id);
        printf("  house_rank:%d\n",map[player_num].rank);
        phouse=phouse->next;
    }
    return;
}


void help()   //帮助菜单
{
    printf("----------------------------—-------------------------------\n");
    printf("                          游戏说明                           \n");
    printf("     1. 游戏顺序：按照进入游戏的先后确定玩家顺序，如输入‘123’   \n");
    printf(" 时的游戏顺序为钱夫人->阿土伯->孙小美。                        \n");
    printf("     2. 地产说明：房产一次只能升一级,需先购地再升级;            \n");
    printf("                 破产的判定条件为 金币值为0;                  \n");
    printf("                 破产时(之后)不可以卖地;                      \n");
    printf("                 只能站在自己的土地时才可以卖地。              \n");
    printf("     3. 道具说明：道具不能放在特殊地点,如道具屋;               \n");
    printf("                 玩家会出发自己放置的道具;                    \n");
    printf("                 炸弹在其他玩家的地盘爆炸时，玩家不需支付租金  \n ");
    printf("------------------------------------------------------------\n");
    return ;
}
