#ifndef MAIN_H
#define MAIN_H
#include "p.h"
int init(struct stP *ps,int n);
int life_go(struct stP *ps, int n);
int isAllDie(struct stP *ps, int n);
int monitorAll(struct stP *ps, int n);
int monitorOne(struct stP *ps,int i);
int makeOne(struct stP *ps,int f,int m);
int bron(struct stP *ps, int n);
int liveNum(struct stP *ps, int n);
int info(struct stP* ps,int ret,int livetime,int lastLive,int curlive);
#endif
