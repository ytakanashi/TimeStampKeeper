//TimeStampKeeper.h

//`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`
//          TimeStamp Keeper Ver.1.02 by x@rgs
//              under NYSL Version 0.9982
//
//`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`


#ifndef _TIMESTAMPKEEPER_H_9894B53E_DF70_490a_A360_48270CBF327E
#define _TIMESTAMPKEEPER_H_9894B53E_DF70_490a_A360_48270CBF327E


class TimeStampKeeper:public sslib::DialogApp{
public:
	TimeStampKeeper(){}
	~TimeStampKeeper(){}

public:
	bool init();
	bool run(sslib::CommandArgument& cmd_arg,int cmd_show);
	void cleanup();

};
#endif //_TIMESTAMPKEEPER_H_9894B53E_DF70_490a_A360_48270CBF327E
