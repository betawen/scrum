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

#define START_POS      0
#define HOSPITAL_POS   14
#define TOOLHOUSE_POS  28
#define GIFTHOUSE_POS  35
#define PRISON_POS     49
#define MAGICHOUSE_POS 63
#define MINE_POS_LOW   64
#define MINE_POS_HIGH  69

#define HOUSE_VALUE    200

//list function
void addtohouselist(int house_id, player_struct* curr_player);
int  delethouselist(int house_id, player_struct* curr_player);
int  searchhouselist(int house_id, struct house_id_struct* phouse);
void showhouselist(struct house_id_struct* phouse);

//main test function
void test_buy();
void test_sell();
void test_levelup();

//test tools
//houser_id = -1，rank，tmp_value=200，money，p_house_head，pos，dot
void init_map(int test_money);
void show_map_result(struct map_struct curr_pos);
void show_people_result(struct player_struct curr_player);
//改变玩家位置
void set_people_position(struct player_struct * curr_player, int position);
//改变地图等级
void set_map_rank(struct map_struct *targ_pos,int rank);
//包含全局变量player,因为要删掉别人的房子以免逻辑混乱
void set_map_owner(int house_id, int player_id);

void erro_record(const char * error, int player_id, int place);


void buy(int house_id)
{
    
    if((house_id == START_POS)||(house_id == HOSPITAL_POS)||(house_id == TOOLHOUSE_POS)\
    ||(house_id == GIFTHOUSE_POS)||(house_id == MAGICHOUSE_POS)||(house_id == PRISON_POS)\
    ||((MINE_POS_LOW <= house_id)&&(house_id<= MINE_POS_HIGH)))
    {
        printf("此地不出售\n");
        return ;
    }
    if(map[house_id].houser_id != -1)
    {
        printf("此地已经出售\n");
        return ;
    }
    if(player[tmp_player_num].money < map[house_id].tmp_value)
    {
        printf("您的账户余额不足\n");
        return ;
    }
    player[tmp_player_num].money -= map[house_id].tmp_value;
    map[house_id].houser_id = tmp_player_num;
    addtohouselist(house_id,&(player[tmp_player_num]));
    printf("您现在还有：%d\n",player[tmp_player_num].money);
    printf("购买成功！！！\n");
    return ;
}

void levelup(int house_id)
{
    if(map[house_id].houser_id != tmp_player_num)
    {
        printf("这不是您的土地\n");
        return;
    }
    if(map[house_id].rank == 3)
    {
        printf("已经达到顶级\n");
        return;
    }
    if(player[tmp_player_num].money < map[house_id].tmp_value/(map[house_id].rank + 1))
    {
        printf("您的账户余额不足\n");
        return;
    }
    player[tmp_player_num].money -= map[house_id].tmp_value/(map[house_id].rank + 1);
    map[house_id].tmp_value += map[house_id].tmp_value/(map[house_id].rank + 1);
    map[house_id].rank ++;
    if(map[house_id].rank == 1)
    {
        printf("已经升级为茅屋\n");
    }
    else if(map[house_id].rank == 2)
    {
        printf("已经升级为平房\n");
    }
    else if(map[house_id].rank == 3)
    {
        printf("已经升级为摩天大楼\n");
    }
    printf("您现在还有：%d\n",player[tmp_player_num].money);
    return;
}

void sell(int house_id)    //卖房产
{
    showhouselist(player[tmp_player_num].p_house_head);
    int ret = 0;
    ret = delethouselist(house_id,&(player[tmp_player_num]));
    if(ret == 0)
    {
        printf("这不是您的资产\n");
        return;
    }
    player[tmp_player_num].money += map[house_id].tmp_value*2; //更新玩家所拥有的现金
    map[house_id].tmp_value = (map[house_id].tmp_value)/(map[house_id].rank + 1);  //将地产价值置为初始值
    map[house_id].rank = 0;
    map[house_id].houser_id = -1;
    printf("您现在还有：%d\n",player[tmp_player_num].money);
    return ;
}

void addtohouselist(int house_id, player_struct* curr_player)
{
    struct house_id_struct * pre_house = curr_player->p_house_head->next;
    struct house_id_struct * new_house = (struct house_id_struct *)malloc(sizeof(struct house_id_struct));
    new_house->house_id = house_id;
    new_house->next = pre_house;
    curr_player->p_house_head->next = new_house;
}

int delethouselist(int house_id, player_struct* curr_player)
{
    struct house_id_struct * curr_house = curr_player ->p_house_head->next;
    struct house_id_struct * pre_house = curr_player->p_house_head;
    if(curr_house == NULL)
    {
        return ERROR ;
    }
    while(curr_house -> house_id != house_id)
    {
        pre_house = curr_house;
        curr_house = curr_house->next;
        if(curr_house == NULL)
        {
            return ERROR ;
        }
    }
    pre_house->next = curr_house->next;
    free(curr_house);
    return SUCCESS;
}

int searchhouselist(int house_id, struct house_id_struct* phouse)
{
    phouse = phouse->next;
    while(phouse)
    {
        if(phouse->house_id == house_id)
        {
            return SUCCESS;
        }
        phouse = phouse->next;
    }
    return ERROR;
}
void showhouselist(struct house_id_struct* phouse)
{
    phouse = phouse->next;
    printf("房产：");
    while(phouse)
    {
        printf("%d\t",phouse->house_id);
        phouse = phouse->next;
    };
    printf("\n");
}

/*
1.买入房产后，观察：map.houser,map.tmp_value, people.money是否正常，
    即map.houser是否变成当前player,money是否减少了tmp_value
    测试样例：初始化一个无主地图，初始化金钱为10000,再买入1的房子，观察链表和其他变量
2.错误检测：
    1）只能买入无主的空地：
        测试样例：行走至别的player的房子上，不能买入，应当直接跳过该选项
                    初始化一为别的player的房子，观察结果
                 行走至已经是自己的房子上，不能买入，应当直接跳过该选项
    2）特殊地点不能买入：
        测试样例：起点，道具屋，魔法屋，矿地不能买入
                    初始化一为当前player的房子，观察结果
                 将玩家初始化至特殊地点，应当直接跳过该选项
    3）金钱不够不能买入：
        测试样例：将玩家的初始金钱置为10，观察结果，应当直接跳过该选项
*/
void test_buy()
{
    //设置玩家为第一位玩家
    tmp_player_num = 1;
    int player_id = 1;
    //设置其他玩家为第二位玩家
    int helper_id = 2;
    //设置初始金额为10000
    int test_money = 10000;
    //设置较少金额为10
    int test_low_money = 10;
    //设置将要到达的地点为1
    int temp_pos = 1;

    printf("TEST buy start\n");
    printf("step1 :normal test\n");
    printf("get the house under your feet\n");
    //test_buy_init
    init_map(test_money);
    //让玩家一走至第一块地
    set_people_position(&(player[player_id]), temp_pos);
    //buy the pos = 1;
    buy(temp_pos);
    //打印结果
    if(map[temp_pos].houser_id != player_id)
    {
        erro_record("normaltestfailed:houserid is wrong\n",player_id,temp_pos);
    }
    if(player[player_id].money != test_money - HOUSE_VALUE)
    {
        erro_record("normaltestfailed:money is wrong\n",player_id,temp_pos);
    }
    if(!searchhouselist(temp_pos, player[player_id].p_house_head))
    {
        erro_record("normaltestfailed:p_house_head is wrong\n",player_id,temp_pos);
    }

    printf("step2 ：ERRO test\n");
    printf("fist branch\n");
    printf("the house have an owner,and the owner is other\n");
    //test_buy_init_error1
    init_map(test_money);
    //使得第一块是第二个人的财产
    set_map_owner(temp_pos, helper_id);
    //让玩家一走至第一块地
    set_people_position(&(player[player_id]), temp_pos);
    //buy the pos = 1;

    buy(temp_pos);

    if(map[temp_pos].houser_id == player_id)
    {
        erro_record("error1testfailed(others house):houserid is wrong\n",player_id,temp_pos);
    }
    if(player[player_id].money != test_money)
    {
        erro_record("error1testfailed(others house):money is wrong\n",player_id,temp_pos);
    }
    if(searchhouselist(temp_pos, player[player_id].p_house_head))
    {
        erro_record("error1testfailed(others house):p_house_head is wrong\n",player_id,temp_pos);
    }

    printf("second branch\n");
    printf("the house have an owner,and the owner is herself\n");
    //test_buy_init_error1
    init_map(test_money);
    //使得第一块是第一个人的财产
    set_map_owner(temp_pos,player_id);
    //让玩家一走至第一块地
    set_people_position(&(player[player_id]), temp_pos);
    //buy the pos = 1;

    buy(temp_pos);

    if(map[temp_pos].houser_id != player_id)
    {
        erro_record("error2testfailed(yours house):houserid is wrong\n",player_id,temp_pos);
    }
    if(player[player_id].money != test_money)
    {
        erro_record("error2testfailed(yours house):money is wrong\n",player_id,temp_pos);
    }
    if(!searchhouselist(temp_pos, player[player_id].p_house_head))
    {
        erro_record("error2testfailed(yours house):p_house_head is wrong\n",player_id,temp_pos);
    }

    printf("third branch\n");
    printf("the house is a special place\n");
    //test_buy_init_error1
    init_map(test_money);
    //让玩家一走至一个特殊地点
    set_people_position(&(player[player_id]), HOSPITAL_POS);
    //buy the pos = 1;

    buy(HOSPITAL_POS);
    if(map[temp_pos].houser_id == player_id)
    {
        erro_record("error3testfailed(special place):houserid is wrong\n",player_id,HOSPITAL_POS);
    }
    if(player[player_id].money != test_money)
    {
        erro_record("error3testfailed(special place):money is wrong\n",player_id,HOSPITAL_POS);
    }
    if(searchhouselist(temp_pos, player[player_id].p_house_head))
    {
        erro_record("error3testfailed(special place):p_house_head is wrong\n",player_id,HOSPITAL_POS);
    }

    printf("fourth branch\n");
    printf("the house is too expensive for her\n");
    //test_buy_init_error1
    init_map(test_low_money);
    //让玩家一走至第一块地
    set_people_position(&(player[player_id]), temp_pos);
    //buy the pos = 1;

    buy(temp_pos);

    if(map[temp_pos].houser_id == player_id)
    {
        erro_record("error4testfailed(money is not enough):houserid is wrong\n",player_id,temp_pos);
    }
    if(player[player_id].money != test_low_money)
    {
        erro_record("error4testfailed(money is not enough):money is wrong\n",player_id,temp_pos);
    }
    if(searchhouselist(temp_pos, player[player_id].p_house_head))
    {
        erro_record("error4testfailed(money is not enough):p_house_head is wrong\n",player_id,temp_pos);
    }
}


/*1.有一个完整的链表确保删除成功
2.显示一个完整的链表 确保people.phouse可以正常删除
3.观察map.houser,map.rank,map.tmp_value, people.money 是否正常，
    即map.houser是否变成-1，rank回退回0，tmp_value是否变为基础价格，money是否增加了tmp_value*2
    测试样例：初始化一个3,5,7 为自己的地图,3 is 3 level，卖掉观察结果
4.错误检测：
    1）不可以卖掉不属于自己的房子：
       初始化一个3,5,7格为自己的地图，第四个房子是别人的，卖掉4的房子观察结果
    2) 不可以卖掉空地：
       初始化一个3,5,7格为自己的地图，第四个房子是空地的，但是卖掉4的房子观察结果
*/
void test_sell()
{
    //设置玩家为第一位玩家
    tmp_player_num = 1;
    int player_id = 1;
    //设置初始金额为10000
    int test_money = 10000;
    //设置已经拥有的地块
    int house1 = 3;
    int house2 = 5;
    int house3 = 7;
    //设置同行玩家为第二位玩家
    int helper_id = 2;
    //设置一个其他人拥有的房子
    int house4 = 4;
    //rank =?
    int rank = 3;
    printf("TEST sell start\n");
    printf("step1 :normal test\n");
    printf("sell your house :pos 3\n");
    //test_sell_init
    init_map(test_money);
    //将house1,2,3的地设置为player1所拥有的
    set_map_owner(house1,player_id);
    set_map_owner(house2,player_id);
    set_map_owner(house3,player_id);
    set_map_rank(&(map[house1]),rank);

    sell(house1);

    if(map[house1].houser_id != -1)
    {
        erro_record("normaltestfailed:houserid is wrong\n",player_id,house1);
    }
    if(map[house1].rank != 0)
    {
        erro_record("normaltestfailed:rank is wrong\n",player_id,house1);
    }
    if(map[house1].tmp_value != HOUSE_VALUE)
    {
        erro_record("normaltestfailed:tmp_value is wrong\n", player_id, house1);
    }
    if(player[tmp_player_num].money != test_money + HOUSE_VALUE*(rank + 1)*2)
    {
        erro_record("normaltestfailed:person money is wrong\n",player_id,house1);
    }
    if(searchhouselist(house1,player[tmp_player_num].p_house_head))
    {
        erro_record("normaltestfailed:p_house_head is wrong\n",player_id,house1);
    }

    printf("step2 ：ERRO test\n");
    printf("fist branch\n");
    printf("the house have an owner,and the owner is other\n");

    //test_sell_init
    init_map(test_money);
    //将house1,2,3的地设置为player1所拥有的
    set_map_owner(house1,player_id);
    set_map_owner(house2,player_id);
    set_map_owner(house3,player_id);
    //设置house4为其他玩家拥有
    set_map_owner(house4,helper_id);

    sell(house4);

    if(map[house4].houser_id != helper_id)
    {
        printf("%d",helper_id);
        erro_record("error1testfailed(others house):houserid is wrong\n",player_id,house4);
    }
    if(player[tmp_player_num].money != test_money)
    {
        erro_record("error1testfailed(others house):person money is wrong\n",player_id,house4);
    }

    printf("second branch\n");
    printf("the house has freedom\n");

    //test_sell_init
    init_map(test_money);
    //将house1,2,3的地设置为player1所拥有的
    set_map_owner(house1,player_id);
    set_map_owner(house2,player_id);
    set_map_owner(house3,player_id);

    //卖掉空地
    sell(house4);

    if(map[house4].houser_id != -1)
    {
        printf("%d",helper_id);
        erro_record("error2testfailed(free house):houserid is wrong\n",player_id,house4);
    }
    if(player[tmp_player_num].money != test_money)
    {
        erro_record("error2testfailed(free house):person money is wrong\n",player_id,house4);
    }
}

/*
    1.行走到自己的地图上，升级自己的房产，观察map.rank,map.tmp_value, people.money 是否正常
        测试样例：1.初始化一个3,5,7是自己的房产的地图，行走到3，升级3times观察结果
    2.错误检测：
    1）不是自己的土地不能升级
        测试样例：初始化一个3,5,7是自己的房产的地图，行走到4，升级观察结果
    2）已经升到顶级的土地不可以升级
        测试样例：初始化一个3,5,7是自己的房产的地图，将三升级为摩天大楼，到达登记三，行走到3，升级观察结果
    3) 现金不够不能升级
        测试样例：初始化一个3,5,7是自己的房产的地图，初始化金额为10，行走到3，升级观察结果
*/
void test_levelup()
{
     //设置玩家为第一位玩家
    tmp_player_num = 1;
    int player_id = 1;
    //设置初始金额为10000
    int test_money = 10000;
    //设置低初始金额为10
    int test_low_money = 10;
    //设置已经拥有的地块
    int house1 = 3;
    int house2 = 5;
    int house3 = 7;
    //设置同行玩家为第二位玩家
    int helper_id = 2;
    //设置一个其他人拥有的房子
    int house4 = 4;
    int rank = 3;
    int i = 0;
    printf("TEST levelup start\n");
    printf("step1 :normal test\n");
    printf("levelup your house :pos 3\n");
    //test_sell_init
    init_map(test_money);
    //将house1,2,3的地设置为player1所拥有的
    set_map_owner(house1,player_id);
    set_map_owner(house2,player_id);
    set_map_owner(house3,player_id);
    //使玩家行走至house1
    set_people_position(&(player[player_id]), house1);

    for(i = 0;i < rank;i++)
    {
        levelup(house1);
    }

    if(map[house1].rank != rank)
    {
        erro_record("normaltestfailed:rank is wrong\n",player_id,house1);
    }
    if(map[house1].tmp_value != HOUSE_VALUE * (rank + 1))
    {
        erro_record("normaltestfailed:house tmp_value is wrong\n",player_id,house1);
    }
    if(player[tmp_player_num].money != test_money - HOUSE_VALUE)
    {
        erro_record("normaltestfailed:people money is wrong\n",player_id,house1);
    }

    printf("step2 ：ERRO test\n");
    printf("fist branch\n");
    printf("the house have an owner,and the owner is other\n");
    //test_sell_init
    init_map(test_money);
    //将house1,2,3的地设置为player1所拥有的
    set_map_owner(house1,player_id);
    set_map_owner(house2,player_id);
    set_map_owner(house3,player_id);
    //设置house4为其他玩家拥有
    set_map_owner(house4,helper_id);
    //使玩家行走至house4
    set_people_position(&(player[player_id]), house4);

    levelup(house4);

    if(map[house1].rank == rank)
    {
        erro_record("error1testfailed:rank is wrong\n",player_id,house1);
    }
    if(player[tmp_player_num].money != test_money)
    {
        erro_record("error1testfailed:people money is wrong\n",player_id,house1);
    }

    printf("second branch\n");
    printf("the house is a 3 level house\n");
    //test_sell_init
    init_map(test_money);
    //将house1,2,3的地设置为player1所拥有的
    set_map_owner(house1,player_id);
    set_map_owner(house2,player_id);
    set_map_owner(house3,player_id);
    //将房产1升级为rank3
    set_map_rank(&(map[house1]),3);
    //使玩家行走至house1
    set_people_position(&(player[player_id]), house1);

    levelup(house1);

    if(map[house1].rank > 3)
    {
        erro_record("error2testfailed:rank is wrong\n",player_id,house1);
    }
    if(player[tmp_player_num].money != test_money)
    {
        erro_record("error2testfailed:people money is wrong\n",player_id,house1);
    }

    printf("third branch\n");
    printf("the house is too expensive\n");
    //test_sell_init
    init_map(test_low_money);
    //将house1,2,3的地设置为player1所拥有的
    set_map_owner(house1,player_id);
    set_map_owner(house2,player_id);
    set_map_owner(house3,player_id);
  
    //使玩家行走至house1
    set_people_position(&(player[player_id]), house1);

    levelup(house1);

    if(map[house1].rank != 0)
    {
        erro_record("error3testfailed:rank is wrong\n",player_id,house1);
    }
    if(player[tmp_player_num].money != test_low_money)
    {
        erro_record("error3testfailed:people money is wrong\n",player_id,house1);
    }
}

void init_map(int test_money)
{
    int i = 0;
    for(i = 0;i < max_node_num; i++)
    {
        map[i].houser_id = -1;
        map[i].rank = 0;
        map[i].tmp_value = HOUSE_VALUE;
    }
    for(i = 0;i< max_player_num; i++)
    {
        player[i].money = test_money;
        player[i].p_house_head = (struct house_id_struct *)malloc(sizeof(struct house_id_struct));
        player[i].p_house_head->next = NULL;
        player[i].pos = 0;
        player[i].dot = 0;
    }
}
void set_map_owner(int house_id, int player_id)
{
    int i = 0;
    map[house_id].houser_id = player_id;
    for( i = 0 ; i < max_player_num ; i++)
    {
        delethouselist(house_id, &(player[i]));
    }
    addtohouselist(house_id, &(player[player_id]));
}
void set_map_rank(struct map_struct *targ_pos,int rank)
{
    targ_pos->rank = rank;
    targ_pos->tmp_value = targ_pos->tmp_value*(1 + rank);
}
void set_people_position(struct player_struct * curr_player, int position)
{
    curr_player->pos = position;
}
void show_people_result(struct player_struct curr_player)
{
    //printf("name:%c\n", curr_player.name);
    //printf("full_name:%s\n", curr_player.full_name);
    printf("pos:%d\n", curr_player.pos);
    printf("money:%d\n", curr_player.money);
    printf("dot:%d\n", curr_player.dot);
    showhouselist(curr_player.p_house_head);
}
void show_map_result(struct map_struct curr_pos)
{
    //printf("tool_id:%d\n",curr_pos.tool_id);
    printf("houser_id:%d\n",curr_pos.houser_id);
    printf("rank:%d\n",curr_pos.rank);
    printf("tmp_value:%d\n",curr_pos.tmp_value);
}

void erro_record(const char * error , int player_id, int place)
{
    show_people_result(player[player_id]);
    show_map_result(map[place]);
    printf("%s",error);
}

#if 0
//test main
struct player_struct player[max_player_num];
struct map_struct map[max_node_num];
int tmp_player_num;   //当前玩家序号
int main(void)
{
    test_buy();
    test_sell();
    test_levelup();
    return 1;
}
#endif