//
// Created by yc on 11/2/18.
//

//各种输入的判断及合法性分析
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

struct result analyse_cmd(char *cmd)  //分析命令行指令
{
    struct result res={-1,0};
//    if(strlen(cmd)==0+1) printf("please input an instruction\n");

    if((cmd[0] == 'R' || cmd[0] == 'r') && cmd[1] == 'o' && cmd[2] == 'l' && cmd[3] == 'l' && strlen(cmd) == 4+1){
        res.cmd_num = 0;
        res.n = 0;
    }
    else if((cmd[0] == 'S' || cmd[0] == 's') && cmd[1] == 'e' && cmd[2] == 'l' && cmd[3] == 'l' && cmd[4] == ' '){
//        printf("cmd.length = %d\n",strlen(cmd));
//        printf("cmd[5] = %d\n",cmd[5]-'0');
//        printf("cmd[6] = %d\n",cmd[6]-'0');
        if(strlen(cmd) > 7+1) res.cmd_num = -1;
        else if((cmd[5]-'0' < 0 || cmd[5]-'0' > 6) && cmd[6] != '\n') res.cmd_num = -1;
        else if((cmd[6]-'0' < 0 || cmd[6]-'0' > 9 ) && cmd[6] != '\n') res.cmd_num = -1;
        else {
//            printf("the input is right");
            if(cmd[6] == '\n') {
                res.cmd_num = 1;
                res.n = cmd[5]-'0';
            }
            else {
                res.cmd_num = 1;
                res.n = (cmd[5]-'0')*10+(cmd[6]-'0');
            }
        }
    }
    else if((cmd[0] == 'B' || cmd[0] == 'b') && cmd[1] == 'l' && cmd[2] == 'o' && cmd[3] == 'c' && cmd[4] == 'k' && cmd[5] == ' '){
        if(strlen(cmd) > 8+1) res.cmd_num = -1;
        else if((cmd[6]-'0' < 0 || cmd[6]-'0' > 6) && cmd[7] != '\n') res.cmd_num = -1;
        else if((cmd[7]-'0' < 0 || cmd[7]-'0' > 9 ) && cmd[7] != '\n') res.cmd_num = -1;
        else {
            if(cmd[7] == '\n') {
                res.cmd_num = 1;
                res.n = cmd[6]-'0';
            }
            else {
                res.cmd_num = 1;
                res.n = (cmd[6]-'0')*10+(cmd[7]-'0');
            }
        }
    }
    else if((cmd[0] == 'B' || cmd[0] == 'b') && cmd[1] == 'o' && cmd[2] == 'm' && cmd[3] == 'b' && cmd[4] == ' '){
        if(strlen(cmd) > 7+1) res.cmd_num = -1;
        else if((cmd[5]-'0' < 0 || cmd[5]-'0' > 6) && cmd[6] != '\n') res.cmd_num = -1;
        else if((cmd[6]-'0' < 0 || cmd[6]-'0' > 9 ) && cmd[6] != '\n') res.cmd_num = -1;
        else {
            if(cmd[6] == '\n') {
                res.cmd_num = 1;
                res.n = cmd[5]-'0';
            }
            else {
                res.cmd_num = 1;
                res.n = (cmd[5]-'0')*10+(cmd[6]-'0');
            }
        }
    }
    else if((cmd[0] == 'R' || cmd[0] == 'r') && cmd[1] == 'o' && cmd[2] == 'b' && cmd[3] == 'o' && cmd[4] == 't' && strlen(cmd) == 5+1){
        res.cmd_num = 4;
        res.n = 0;
    }
    else if((cmd[0] == 'Q' || cmd[0] == 'q') && cmd[1] == 'u' && cmd[2] == 'e' && cmd[3] == 'r' && cmd[4] == 'y' && strlen(cmd) == 5+1){
        res.cmd_num = 5;
        res.n = 0;
    }
    else if((cmd[0] == 'H' || cmd[0] == 'h') && cmd[1] == 'e' && cmd[2] == 'l' && cmd[3] == 'p' && strlen(cmd) == 4+1){
        res.cmd_num = 6;
        res.n = 0;
    }
    else if((cmd[0] == 'Q' || cmd[0] == 'q') && cmd[1] == 'u' && cmd[2] == 'i' && cmd[3] == 't' && strlen(cmd) == 4+1){
        res.cmd_num = 7;
        res.n = 0;
    }
    else if((cmd[0] == 'S' || cmd[0] == 's') && cmd[1] == 't' && cmd[2] == 'e' && cmd[3] == 'p' && cmd[4] == ' '){
        if(strlen(cmd) > 7+1) res.cmd_num = -1;
        else if((cmd[5]-'0' < 0 || cmd[5]-'0' > 6) && cmd[6] != '\n') res.cmd_num = -1;
        else if((cmd[6]-'0' < 0 || cmd[6]-'0' > 9 ) && cmd[6] != '\n') res.cmd_num = -1;
        else {
            if(cmd[6] == '\n') {
                res.cmd_num = 8;
                res.n = cmd[5]-'0';
            }
            else {
                res.cmd_num = 8;
                res.n = (cmd[5]-'0')*10+(cmd[6]-'0');
            }
        }
    }
    else if((cmd[0] == 'N' || cmd[0] == 'n') && cmd[1] == 'e' && cmd[2] == 'x' && cmd[3] == 't' && strlen(cmd) == 4+1){
        res.cmd_num = 9;
        res.n = 0;
    }
    else {
        res.cmd_num = -1;
    }

    return res;
}


bool judge_name_print(char *name)     //开始判断输入的名字是否合法，并对玩家进行初始化
{
    player_num=strlen(name);
    bool name_exit[4];  //标志这个名字是否已经被选择
    memset(name_exit, false, sizeof(name_exit));
    if(player_num<2||player_num>4)
        return false;
    alive_num=player_num;
    for(int i=0;i<player_num;i++)
    {
        int num=name[i]-'0';
        if(num<1||num>4)   //序号不在范围内
            return false;
        if(name_exit[num-1]== true)  //已存在
            return false;
        name_exit[num-1]= true;

        //add player chain
        struct player_num *new_player=(struct player_num*)malloc(sizeof(struct player_num));
        new_player->num=i;
        if(i==0)
            new_player->next=NULL;
        else
            new_player->next=map[0].p_num->next;
        map[0].p_num->next=new_player;

        player_alive[i]= true;
        player[i].name=name_ch[num-1];  //同时给player的name赋值
        strcpy(player[i].full_name,full_name[num-1]);   //给全名赋值
        player[i].money=init_money;
    }

//    struct player_num *p=map[0].p_num->next;
//    while(p)
//    {
//        printf("%c\n",player[p->num].name);
//        p=p->next;
//    }
    return true;
}

int judge_res_print(char * response)
{
    if(strcmp(response,"y\n")==0)
        return 1;
    else if(strcmp(response,"n\n")==0)
        return 0;
    else
        return -1;
}

