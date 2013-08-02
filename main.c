#include "p.h"
#include "main.h"
#include <stdio.h>
#include <stdlib.h>
//#include <curses.h>
#include <unistd.h>
#define GREEN(x) "\e[32m"x"\e[0m"
#define RED(x) "\e[31m"x"\e[0m"
#define YELLOW(x) "\e[33m"x"\e[0m"
#define BLUE(x) "\e[34m"x"\e[0m"
#define PEPER(x) "\e[35m"x"\e[0m"
#define BG_DEEP_RED(x) "\e[41m"x"\e[0m"
#define N (20) //最大种群数量
#define X_A 4 //变异概率 百分之
#define X_B 5 //变异概率 百分之
#define X_bron 30 //出生率
static int isVariation = 0;     //基因变异
int main(void)
{
	struct stP ps[N];
	init(ps, N);
	int livetime = 0;
	int ret = 4;
	monitorAll(ps, N);
	int lastLive = liveNum(ps, N);
	//生存
	while (1) {
		usleep(1000*50);
		printf("\033[2J");
		printf("\033[%d;%dH", 0, 0);
		//clear();
		livetime++;
		info(ps, ret, livetime, lastLive, liveNum(ps, N));
		lastLive = liveNum(ps, N);
		life_go(ps, N);
		if (isAllDie(ps, N)==1) {
			info(ps, ret, livetime, lastLive, liveNum(ps, N));
			monitorAll(ps, N);
			break;
		}
		monitorAll(ps, N);
		//一定概率出生
		if (rand()%100<X_bron) {
			ret = bron(ps, N);
		} else {
			isVariation = 0;
			ret = 4;
		}
		//printf(YELLOW("After bron %d \n"),ret);
		//monitorAll(ps, N);
	}
	return 0;
}
//显示信息
int info(struct stP* ps, int ret, int livetime, int lastLive, int curlive)
{
	printf(RED(" ==="));
	printf(GREEN(" live time %5d"), livetime);
	printf(GREEN(" [%3d/%3d]"), lastLive, N);
	if (ret==1) {
		printf(YELLOW(" FULL"));
	} else {
		printf(YELLOW("     "));
	}
	if (ret==2) {
		printf(GREEN(" 无法繁衍"));
	} else {
		printf(GREEN("        "));
	}
	if (ret==4) {
		printf(GREEN(" 时光流逝"));
	} else {
		printf(GREEN(" 时光流逝"));
	}
	if (ret==0) {
		printf(YELLOW(" 生"));
	} else {
		printf(YELLOW("   "));
	}
	if (curlive<lastLive) {
		printf(YELLOW(" 死"));
	} else {
		printf(YELLOW("   "));
	}
	if (isVariation&0b1) {
		printf(RED(" [A]"));
	} else {
		printf(RED("    "));
	}
	if (isVariation&0b10) {
		printf(RED(" [B]"));
	} else {
		printf(RED("    "));
	}
	printf(RED(" ===\n"));
	return 0;
}
int bron(struct stP *ps, int n)
{
	int i, f, m, lived = 0;
	int this = 0;		//本次出生的序号
	for (i = 0; i<n; i++) {
		if (ps[i].leftlive==0) {
			this = i;
			break;
		}
	}
//没有出生的空间了
	if (i>=n) {
		return 1;
	}
	if (this>1000) {
		printf("here\n");
	}
//没有父母,至少需要2个个体才能繁衍
	for (i = 0; i<n; i++) {
		if (ps[i].leftlive>0) {
			lived++;
			if (lived>=2) {
				break;
			}
		}
	}
	if (lived<2) {
		return 2;
	}
//选择双亲
	do {
		f = rand()%N;
		m = rand()%N;
	} while (!(ps[f].leftlive>0&&ps[m].leftlive>0));
//交配出生
	makeOne(&ps[this], ps[f].a, ps[m].b);
	return 0;
}

//是否都死亡?游戏结束
int isAllDie(struct stP *ps, int n)
{
	int i;
	for (i = 0; i<n; i++) {
		if (ps[i].leftlive>0) {
			return 0;
		}
	}
	return 1;
}

//生命流逝
int life_go(struct stP *ps, int n)
{
	int i;
	for (i = 0; i<n; i++) {
		if (ps[i].leftlive>0) {
			ps[i].leftlive--;
		} else {
			ps[i].a = 0;
			ps[i].b = 0;
			ps[i].live = 0;
		}
	}
	return 0;
}
//初始化
int init(struct stP *ps, int n)
{
	int i;
	for (i = 0; i<n/10; i++) {
		makeOne(&ps[i], rand()%10+1, rand()%10+1);
	}
	for (i = n/10; i<n; i++) {
		makeOne(&ps[i], 0, 0);
	}
	return 0;
}

//查看个体
int monitorOne(struct stP *ps, int i)
{

	if (ps[i].live>0) {
		printf(GREEN("[%03d]"), i);
	} else {
		printf(BG_DEEP_RED("[%03d]"), i);
	}

	printf("%2d %2d\t%3d/%3d ",
	                ps[i].a, ps[i].b,
	                ps[i].leftlive, ps[i].live);
	int j;
	for (j = 0; j<ps[i].live; j += 1) {
		if (j<ps[i].leftlive) {
			printf(GREEN("="));
		} else {
			printf(GREEN("-"));
		}
	}
	printf("\n");
	return 0;
}
//查看所有个体
int monitorAll(struct stP *ps, int n)
{
	int i;
	int l = 0;
	for (i = 0; i<n; i++) {
		if (ps[i].leftlive>0) {
			l++;
		} else {
			//printf(RED("^"));
		}
		monitorOne(ps, i);
	}
//printf(RED("All:%d/%d\n"),l,N);
	return 0;
}
//制造一个个体,从f(父)m(母)中.
int makeOne(struct stP *ps, int f, int m)
{

	if (rand()%100<X_A) {
		ps->a = rand()%10+1;
		isVariation |= 0b1;
	} else {
		ps->a = f;
		isVariation &= ~0b1;
	}
	if (rand()%100<X_B) {
		ps->b = rand()%10+1;
		isVariation |= 0b10;
	} else {
		ps->b = m;
		isVariation &= ~0b10;
	}
	ps->live = ps->a*ps->b;
	ps->leftlive = ps->live;
	return 0;
}
//种群中生存的个数
int liveNum(struct stP *ps, int n)
{
	int i;
	int l = 0;
	for (i = 0; i<n; i++) {
		if (ps[i].leftlive>0) {
			l++;
		}
	}
	return l;
}
