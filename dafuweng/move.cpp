//
// Created by yc on 10/30/18.
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


int last_pos;  //

//roll and move
void roll() {
    srand(time(NULL));
    int n = (int)(rand()%6) + 1;    // random from 1~6
    printf("你摇了%d哦\n",n);
    step(n);
}

//collect and change player's money
void collect_rent(int map_pos) {
    if (player[tmp_player_num].remained_gods_of_wealth) {
        player[tmp_player_num].remained_gods_of_wealth -= 1;
        printf("财神效果，可免付租金，还剩%d次~\n",player[tmp_player_num].remained_gods_of_wealth);
        return;
    }
    int fine=map[map_pos].tmp_value/2;
    printf("你被罚款了%d元\n",fine);
    player[tmp_player_num].money -= fine;
    printf("你还有%d元哦！\n",player[tmp_player_num].money);
    player[map[map_pos].houser_id].money += fine;
    printf("%s钱数增加，有%d咯！\n",player[map[map_pos].houser_id].full_name,player[map[map_pos].houser_id].money);
}


//check if player is broken
bool is_broken () {
    if(player[tmp_player_num].money < 0)
        return true;
    else
        return false;
}

int judge_position(int map_pos)    //返回值标示谁取得胜利
{

    // gift house 'G'
    // get 200 dot, get 2000 money, get god of wealth
    if (map[map_pos].map_attribute == 'G') {
        step_into_gift_house();
        return -1;
    }

    // mine '$'
    // get 1 dot
    if(map[map_pos].map_attribute == '$') {
        step_into_mine();
        return -1;
    }

    // empty land '0'
    // pay for rent, buy land, or update own land
    if(map[map_pos].map_attribute == '0') {

        if(map[map_pos].houser_id!=-1 && tmp_player_num!=map[map_pos].houser_id)  //要被罚款
        {
            collect_rent(map_pos);
            if (is_broken())   //如果破产了
            {
                player_alive[tmp_player_num]=false;
                printf("%s被淘汰了，哈哈哈哈哈哈\n",player[tmp_player_num].full_name);

                alive_num--;
                if(alive_num==1)   //游戏结束
                {
                    for(int i=0;i<player_num;i++)
                        if(player_alive[i])
                            return i;
                }
                //删除房产
                struct house_id_struct *p_house=player[tmp_player_num].p_house_head->next;
                struct house_id_struct *q_house=p_house;

                while(p_house!=NULL)
                {
                    map[p_house->house_id].houser_id=-1;
                    p_house=p_house->next;
                    free(q_house);
                    q_house=p_house;
                }

                //删除地图上的这个人
                struct player_num *p_num=map[map_pos].p_num;
                struct player_num *q_num=p_num->next;
                while(p_num->next->num != tmp_player_num)
                {
                    p_num=p_num->next;
                }
                q_num=p_num->next;
                p_num->next=q_num->next;
                free(q_num);
            }
        }

        if(map[player[tmp_player_num].pos].houser_id == -1) //这块空地没有主人
        {
            printf("是否购买?（y/n）\n");
            char response[10];
            int ret=-1;
            do
            {
                fgets(response,10,stdin);
                ret=judge_res_print(response);  //-1->default ,0->n ,1->y
            }while(ret == -1);
            if(ret == 1)
            {
                buy(player[tmp_player_num].pos);
            }
        }
        else if(map[player[tmp_player_num].pos].houser_id == tmp_player_num)  //这块地是自己的
        {
            char response[10];
            int ret = -1;
            do
            {
                printf("是否升级?（y/n）\n");
                fgets(response,10,stdin);
                ret = judge_res_print(response);  //-1->default ,0->n ,1->y
            }while(ret == -1);
            if(ret == 1)
            {
                levelup(player[tmp_player_num].pos);
            }
        }
        return -1;
    }
}

void step(int n)   //行走n步
{
    int last_pos=player[tmp_player_num].pos;
    player[tmp_player_num].pos = (player[tmp_player_num].pos+n) % max_node_num;
    int map_pos = player[tmp_player_num].pos ;
    //add player chain
    struct player_num *new_player=(struct player_num*)malloc(sizeof(struct player_num));
    new_player->num=tmp_player_num;
    new_player->next=map[map_pos].p_num->next;
    map[map_pos].p_num->next=new_player;

//    struct player_num *p=map[0].p_num->next;
//    while(p)
//    {
//        printf("%c\n",player[p->num].name);
//        p=p->next;
//    }

    //delete player chain
    struct player_num* pre_player_num=map[last_pos].p_num;
    while(pre_player_num->next->next!=NULL)
        pre_player_num=pre_player_num->next;
    pre_player_num->next=NULL;
}