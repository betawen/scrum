//
// Created by yc on 10/28/18.
//

#ifndef DAFUWENG_DAFUWENG_H
#define DAFUWENG_DAFUWENG_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define max_node_num   70
#define max_player_num 4
#define map_row_num 8
#define map_col_num 29

#define ERROR   0
#define SUCCESS 1

//color
#define COLOR_RED "\x1b[31m"
#define COLOR_GREEN "\x1b[32m"
#define COLOR_YELLOW "\x1b[33m"
#define COLOR_BLUE "\x1b[34m"
#define COLOR_MAGENTA "\x1b[35m"
#define COLOR_CYAN "\x1b[36m"
#define COLOR_RESET "\x1b[0m"

struct player_num   //记录人的下标
{
    int num;
    struct player_num *next;
};
//map
struct map_struct
{
    struct player_num *p_num;  //只是这个地点的上的人
    int tool_id;
    int houser_id;    //房主的id , 默认-1
    int rank;    //the rank of the land
    int tmp_value;   //the total value of the land now
    char map_attribute;
    //the kind of land 'H'=hospital, 'T'=tool_house, 'M'=magic_house, 'P'=prison, '$'=mine, 'G'=gift_house, 'S'=start, '0'=empty_land
};

struct house_id_struct
{
    int house_id;
    struct house_id_struct *next;
};

struct player_struct
{
    char name;   //姓名首字母
    char full_name[20];
    int pos;   //所在地图上的位置
    int money;
    int dot;   //点数
    int remained_gods_of_wealth;      //是否拥有财神和财神的剩余作用次数
    struct house_id_struct *p_house_head;  //拥有的房子的id
};

struct result
{
    int cmd_num;   //指令代码
    int n;    //参数，若不需要，则置为0
};

bool judge_name_print(char *name);   //判断输入的编号是否合法
int judge_res_print(char * response);//if the response is right
void init_game();    //初始化地图及人物信息
void print_map();    //打印地图
struct result analyse_cmd(char *cmd);  //分析命令行指令
void roll();    //投色子
void step(int n);   //行走
int judge_position(int map_pos);   //判断当前位置，并执行相应的功能
void collect_rent(int map_pos);    //罚款
bool is_broken ();
void buy(int house_id);
void levelup(int house_id);
void sell(int house_id);
void buy(int house_id);     //买房产
void sell(int house_id);    //卖房产
void query(int player_num);   //查询资产
void help();   //帮助菜单

//gift house and mine
void step_into_mine();
void step_into_gift_house();


#endif //DAFUWENG_DAFUWENG_H
