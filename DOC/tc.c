int task_control(void)
{
    _SS
    while (1)
    {
        WaitX(100);
        if (1 == RuiSaKey)
        {
            WaitX(1000);
            RuiSaThr = *RuiSaTh1;
            WaitX((int)(*RuiSaTime1));
            RuiSaPit = 5;
            WaitX((int)(*RuiSaTime2));
            RuiSaThr = 0;
        }
        if (2 == RuiSaKey)
        {
            WaitX(1000);
            RuiSaThr = *RuiSaTh1;
            WaitX((int)(*RuiSaTime1));
            run();
            WaitX((int)(*RuiSaTime2));
            RuiSaThr = 0;
        }
        if (3 == RuiSaKey)
        {
            WaitX(1000);
            RelayOn();
            WaitX(3000);
            RuiSaThr = *RuiSaTh1;
            WaitX((int)(*RuiSaTime1));
            RuiSaPit = 5;
            WaitX((int)(*RuiSaTime2));
            RelayOff();
            RuiSaPit = -5;
            WaitX((int)(*RuiSaTime3));
            RuiSaThr = 0;
        }
        if (4 == RuiSaKey)
        {
            WaitX(1000);
            RelayOn();
            RuiSaThr = *RuiSaTh1;
            WaitX((int)(*RuiSaTime1));
            RuiSaPit = 5;
            WaitX((int)(*RuiSaTime2));
            FindPoint();
            RuiSaThr = 0;
            WaitX((int)(*RuiSaTime3));
            RuiSaThr = *RuiSaTh2;
            WaitX((int)(*RuiSaTime4));
            RuiSaPit = -5;
            WaitX((int)(*RuiSaTime2));
            RuiSaThr = 0;
        }
    }
    _EE
}