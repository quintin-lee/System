/* Copyright (C) 
 * 2018 - quintin
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 * 
 */
/**
 * @file log.c
 * @brief 
 * @author quintin
 * @version 1.0
 * @date 2018-08-04
 */
#include <stdio.h>
#include <stdlib.h>
#include "log.h"

#define MAXLEVELNUM (255)

LOGSET logsetting;
LOG loging;

const static char LogLevelText[4][10]={"INFO","DEBUG","WARN","ERROR"};

static char * getdate(char *date);

static unsigned char getcode(const char *path)
{
    unsigned char code=255;
    if(strcmp("INFO",path)==0)
        code=1;
    else if(strcmp("WARN",path)==0)
        code=3;
    else if(strcmp("ERROR",path)==0)
        code=4;
    else if(strcmp("NONE",path)==0)
        code=0;
    else if(strcmp("DEBUG",path)==0)
        code=2;
    return code;

}

static unsigned char ReadConfig(const char *path)
{
    char value[512]={0x0};
    char data[50]={0x0};

    FILE *fpath=fopen(path,"r");
    if(fpath==NULL)
        return -1;
    fscanf(fpath,"path=%s\n",value);
    getdate(data);
    strcat(data,".log");
    strcat(value,"/");
    strcat(value,data);
    if(strcmp(value,logsetting.filepath)!=0)
        memcpy(logsetting.filepath,value,strlen(value));
    memset(value,0,sizeof(value));

    fscanf(fpath,"level=%s\n",value);
    logsetting.loglevel=getcode(value);
    fclose(fpath);
    return 0;
}
/*
 *  *日志设置信息
 *   * */
static LOGSET *getlogset(const char *path)
{
    //char path[512]={0x0};
    //getcwd(path,sizeof(path));
    //strcat(path,"/log.conf");
    if(access(path,F_OK)==0){
        if(ReadConfig(path)!=0){
            logsetting.loglevel=INFO;
            logsetting.maxfilelen=4096;
        }
    }else{
        logsetting.loglevel=INFO;
        logsetting.maxfilelen=4096;
    }
    return &logsetting;
}

/*
 *  *获取日期
 *   * */
static char * getdate(char *date)
{
    time_t timer=time(NULL);
    strftime(date,11,"%Y-%m-%d",localtime(&timer));
    return date;
}

/*
 *  *获取时间
 *   * */
static void settime()
{
    time_t timer=time(NULL);
    strftime(loging.logtime,20,"%Y-%m-%d %H:%M:%S",localtime(&timer));
}

/*
 *  *不定参打印
 *   * */
static void PrintfLog(const char * fromat,va_list args)
{
    int d;
    char c,*s;
    while(*fromat)
    {
        switch(*fromat){
        case 's':{
                     s = va_arg(args, char *);
                     fprintf(loging.logfile,"%s",s);
                     break;
                 }
        case 'd':{
                     d = va_arg(args, int);
                     fprintf(loging.logfile,"%d",d);
                     break;
                 }
        case 'c':{
                     c = (char)va_arg(args, int);
                     fprintf(loging.logfile,"%c",c);
                     break;
                 }
        default:{
                    if(*fromat!='%'&&*fromat!='\n')
                        fprintf(loging.logfile,"%c",*fromat);
                    break;
                }
        }
        fromat++;
    }
    fprintf(loging.logfile,"%s","\n");
}

static int initlog(unsigned char loglevel, const char *conffilepath)
{
    char strdate[30]={0x0};
    LOGSET *logsetting;
    //获取日志配置信息
    if((logsetting=getlogset(conffilepath))==NULL){
        perror("Get Log Set Fail!");
        return -1;
    }
    if((loglevel&(logsetting->loglevel))!=loglevel)
        return -1;

    memset(&loging,0,sizeof(LOG));
    //获取日志时间
    settime();
    if(strlen(logsetting->filepath)==0){
        char *path=getenv("HOME");
        memcpy(logsetting->filepath,path,strlen(path));

        getdate(strdate);
        strcat(strdate,".log");
        strcat(logsetting->filepath,"/");
        strcat(logsetting->filepath,strdate);
    }
    memcpy(loging.filepath,logsetting->filepath,MAXFILEPATH);
    //打开日志文件
    if(loging.logfile==NULL)
        loging.logfile=fopen(loging.filepath,"a+");
    if(loging.logfile==NULL){
        perror("Open Log File Fail!");
        return -1;
    }
    //写入日志级别，日志时间
    fprintf(loging.logfile,"[%s] [%s] ",LogLevelText[loglevel-1],loging.logtime);
    return 0;
}

/*
 *日志写入
 * 
 */
int LogWrite(unsigned char loglevel,const char *fromat,...)
{
    int  rtv = -1;
    va_list args;

    //[为支持多线程需要加锁] pthread_mutex_lock(&mutex_log); //lock. 

    do{
        //初始化日志
        if(initlog(loglevel, "./log.conf") != 0)
        {
            rtv = -1;
            break;
        }
        //打印日志信息
        va_start(args,fromat);
        PrintfLog(fromat,args);
        va_end(args);
        //文件刷出
        fflush(loging.logfile);
        //日志关闭
        if(loging.logfile!=NULL)
            fclose(loging.logfile);
        loging.logfile=NULL;
        rtv = 0;
    }while(0);

    //[为支持多线程需要加锁] pthread_mutex_unlock(&mutex_log); //unlock. 

    return rtv;
}

