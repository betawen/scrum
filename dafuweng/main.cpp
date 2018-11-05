#include "dafuweng.h"

struct player_struct player[max_player_num];
struct map_struct map[max_node_num];

char name_ch[4]={'Q','A','S','J'};
char full_name[4][20]={"钱夫人","阿土伯","孙小美","金贝贝"};
int init_money; //初始资金
char name_id[10];   //记录输入编号
int tmp_player_num;   //当前玩家序号
int player_num;   //这一局的玩家人数
bool player_alive[max_player_num];   //标记存活的玩家
int alive_num;    //存活的人数

int main() {

    init_game();   //初始化地图及人物信息

    init_money=10000;   //初始资金
    printf("请设置玩家初始资金(默认10000)：");
    scanf("%d",&init_money);
    printf("请选择2~4位不重复玩家，输入编号即可(如输入：12)\n");
    printf("1、\e[0;31m钱夫人\e[0m； 2、\e[0;32m阿土伯\e[0m； 3、\e[0;34m孙小美\e[0m； 4、\e[1;33m金贝贝\e[0m；\n");
    do {
        printf("请输入编号：");
        scanf("%s",name_id);
    }while(!judge_name_print(name_id));

    getchar();

    tmp_player_num=0;  //从第一个人开始

    int winner_num=-1;

    while(1)
    {
       // printf("按回车继续\n");
       // getchar();
       // system("clear");
        print_map();   //打印地图

        switch(player[tmp_player_num].name)   //实现颜色转变
        {
            case 'Q':
                printf(COLOR_RED "%s>" COLOR_RESET,player[tmp_player_num].full_name);
                break;
            case 'A':
                printf(COLOR_GREEN "%s>" COLOR_RESET,player[tmp_player_num].full_name);
                break;
            case 'S':
                printf(COLOR_BLUE "%s>" COLOR_RESET,player[tmp_player_num].full_name);
                break;
            case 'J':
                printf(COLOR_YELLOW "%s>" COLOR_RESET,player[tmp_player_num].full_name);
                break;
            default:
                break;
        }
        char cmd[20];
        //输入cmd并解析
        fgets(cmd,20,stdin);

        struct result res = analyse_cmd(cmd);
        switch (res.cmd_num)
        {
            //Roll
            case 0:
                roll();   //投色子,并行走
                winner_num=judge_position(player[tmp_player_num].pos);
                if(winner_num!=-1)
                {
                    printf("恭喜 %s 取得最后的胜利！！\n",player[winner_num].full_name);
                    exit(-1);
                }

                do {
                    tmp_player_num = (tmp_player_num + 1) % player_num;   //切换到下一个人
                }while(!player_alive[tmp_player_num]);
                break;

                //sell n
            case 1:     //卖房产
                sell(res.n);

                break;

                //block n
            case 2:    //使用路障

                break;

                //bomb n
            case 3:    //使用炸弹

                break;

                //robot
            case 4:    //使用机器娃娃

                break;

                //query
            case 5:    //查询资产
                query(tmp_player_num);

                break;

                //Help
            case 6:
                help();

                break;

                //Quit
            case 7:   //结束游戏
                exit(1);

//test部分
                //step n
            case 8:
                step(res.n);
                printf("你在这里：%d\n",player[tmp_player_num].pos);
                winner_num=judge_position(player[tmp_player_num].pos);
                if(winner_num!=-1)
                {
                    printf("恭喜 %s 取得最后的胜利！！\n",player[winner_num].full_name);
                    exit(-1);
                }
                do {
                    tmp_player_num = (tmp_player_num + 1) % player_num;   //切换到下一个人
                }while(!player_alive[tmp_player_num]);

                break;

                //next
            case 9:
                do {
                    tmp_player_num = (tmp_player_num + 1) % player_num;   //切换到下一个人
                }while(!player_alive[tmp_player_num]);
                break;

            default:
                printf("错误的命令！\n");
                break;
        }

    }

    return 0;
}





