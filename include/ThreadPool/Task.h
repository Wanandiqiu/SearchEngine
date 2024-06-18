#pragma once

/*
    任务类，需要执行的任务，要执行的任务写入一个函数中,
    子类实现这个虚函数，决定执行什么任务
*/

class Task{
public:
    Task(){}
    ~Task(){}

    virtual void execute() = 0;
};