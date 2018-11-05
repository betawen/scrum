//
// Created by yc on 10/30/18.
//

//游戏初始化及地图显示

#include "dafuweng.h"

extern int player_num;
extern struct player_struct player[max_player_num];
extern char name_ch[4];
extern char full_name[4][20];
extern struct map_struct map[max_node_num];
struct result result_struct;
extern int init_money; //初始资金
extern char name_id[10];   //记录输入编号
extern int tmp_player_num;   //当前玩家序号
extern bool player_alive[max_player_num];
extern int alive_num;

char map_view[map_row_num][map_col_num] = {{'S','0','0','0','0','0','0','0','0','0','0','0','0','0','H','0','0','0','0','0','0','0','0','0','0','0','0','0','T'},
                                           { '$','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','0'},
                                           { '$','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','0'},
                                           { '$','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','0'},
                                           { '$','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','0'},
                                           { '$','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','0'},
                                           { '$','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','0'},
                                           { 'M','0','0','0','0','0','0','0','0','0','0','0','0','0','P','0','0','0','0','0','0','0','0','0','0','0','0','0','G'}};

void set_map_attribute () {
    int pos_now = 0;
    int map_now[map_row_num][map_col_num] = {{0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28},
                   {69,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,29},
                   {68,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,30},
                   {67,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,31},
                   {66,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,32},
                   {65,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,33},
                   {64,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,34},
                   {63,62,61,60,59,58,57,56,55,54,53,52,51,50,49,48,47,46,45,44,43,42,41,40,39,38,37,36,35}};
    for (int i =0; i< map_row_num; i++) {
        for( int j = 0; j < map_col_num; j++) {
            pos_now = map_now[i][j];
            if(pos_now >= 0) {
                map[pos_now].map_attribute = map_view[i][j];
            }
        }
    }
    return;
}


int get_last_player(struct map_struct here)
{
    if(NULL == here.p_num->next)
    {
        return -1;
    }
    //printf("%d ", here.p_num->next->num);
    return here.p_num->next->num;
}

int change_map(int map_row, int map_col)
{
    int map_change_list[map_row_num][map_col_num] = {{0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28},
                                                     {69,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,29},
                                                     {68,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,30},
                                                     {67,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,31},
                                                     {66,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,32},
                                                     {65,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,33},
                                                     {64,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,34},
                                                     {63,62,61,60,59,58,57,56,55,54,53,52,51,50,49,48,47,46,45,44,43,42,41,40,39,38,37,36,35}};
    return map_change_list[map_row][map_col];
}


void init_game()    //初始化地图及人物信息
{
    memset(player_alive, false, sizeof(player_alive));
    //初始化地图
    for (int i = 0; i < max_node_num; ++i)
    {
        map[i].p_num=(struct player_num*)malloc(sizeof(struct player_num));
        map[i].p_num->next = NULL;
        map[i].tool_id = 0;
        map[i].houser_id = -1;
        map[i].rank = 0;
        if(0 == i || map_col_num-1 == i || map_col_num+map_row_num-2 == i || 2*map_col_num+map_row_num-3 == i
           || (map_col_num-1)/2 == i || map_col_num+map_row_num+(map_col_num-1)/2-2 == i)
        {
            map[i].tmp_value = 0;
        }//特殊地点
        else if(i < map_col_num-1)
        {
            map[i].tmp_value = 200;
        }//地段1
        else if(i < map_col_num + map_row_num - 2)
        {
            map[i].tmp_value = 500;
        }//地段2
        else if(i< max_node_num - map_row_num + 1)
        {
            map[i].tmp_value = 300;
        }//地段3
        else
        {
            map[i].tmp_value = 0;
        }//矿地

    }
    //初始化地图每个节点
    for (int i = 0; i < max_player_num; ++i)
    {
        player[i].pos = 0;
        player[i].money = 0;
        player[i].dot = 0;
        player[i].p_house_head=(struct house_id_struct*)malloc(sizeof(struct house_id_struct));
        player[i].p_house_head->next = NULL;
    }
    //初始化每个玩家
    result_struct.cmd_num = 0;
    result_struct.n = 0;
    //初始化result结构体

    set_map_attribute();

    return ;
}

void print_map()    //打印地图
{
    int player_here = -1;
    int line_map_pos = -1;

    for (int i = 0; i < map_row_num; ++i)
    {
        for (int j = 0; j < map_col_num; ++j)
        {
            if('*' == map_view[i][j])
            {
                printf("  ");
                continue;
            }

            line_map_pos = change_map(i,j);//二维数组地图转一维数组
            player_here = get_last_player(map[line_map_pos]);//获取最后到该地的玩家

            if (-1 != player_here)
            {
                switch (player[player_here].name)
                {
                    case 'Q':
                        printf(COLOR_RED "Q " COLOR_RESET);
                        break;
                    case 'A':
                        printf(COLOR_GREEN "A " COLOR_RESET);
                        break;
                    case 'S':
                        printf(COLOR_BLUE "S " COLOR_RESET);
                        break;
                    case 'J':
                        printf(COLOR_YELLOW "J " COLOR_RESET);
                        break;
                    default: printf("%c ", map_view[i][j]);
                        break;
                }
            }//判断玩家是否在该地点
            else if(map[line_map_pos].houser_id != -1)
            {
                switch (player[map[line_map_pos].houser_id].name)
                {
                    case 'Q':
                        printf(COLOR_RED "%c " COLOR_RESET, map_view[i][j]);
                        break;
                    case 'A':
                        printf(COLOR_GREEN "%c " COLOR_RESET, map_view[i][j]);
                        break;
                    case 'S':
                        printf(COLOR_BLUE "%c " COLOR_RESET, map_view[i][j]);
                        break;
                    case 'J':
                        printf(COLOR_YELLOW "%c " COLOR_RESET, map_view[i][j]);
                        break;
                    default: printf("%c ", map_view[i][j]);
                        break;
                }
            }//判断该地是否有玩家买过房
            else
            {
                printf( "%c " ,map_view[i][j]);
            }
        }
        printf("\n\n");
    }

    return;
}
