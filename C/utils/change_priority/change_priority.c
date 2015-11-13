int set_priority(unsigned char priority)
{
	int policy = SCHED_FIFO;
	sched_param param;

	param.sched_priority = 99;
	if (0 == pthread_setschedparam(pthread_self(), policy, &param)) {
	
	return 1;
}

int get_priority()
{
	int oldPriority = 0;
	sched_param param;

	if(0 == pthread_getschedparam(pthread_self(), &policy, &param))
	{
		oldPriority = TASK_PRIORITY_CHANGE - param.sched_priority;
	}
	return oldPriority;
}
