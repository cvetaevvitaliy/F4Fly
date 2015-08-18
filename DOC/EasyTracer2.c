#include "EasyTracer.h"
//从腐蚀中心向外腐蚀，得到新的腐蚀中心
static int Point_Corrode(unsigned int oldx, unsigned int oldy, const TARGET_CONDI *Condition, RESULT *Resu)
{
    unsigned int Xmin, Xmax, Ymin, Ymax, i, FailCount = 0;
    for (i = oldx; i > Condition->IMG_X; i--)
    {
        if (!Point_ColorMatch(i, oldy))
            FailCount++;
        if (FailCount > (((Condition->WIDTH_MIN + Condition->WIDTH_MAX) >> 2) >> ALLOW_FAIL_PER))
            break;
    }
    Xmin = i;
    FailCount = 0;

    for (i = oldx; i < Condition->IMG_X + Condition->IMG_W; i++)
    {
        if (!Point_ColorMatch(i, oldy))
            FailCount++;
        if (FailCount > (((Condition->WIDTH_MIN + Condition->WIDTH_MAX) >> 2) >> ALLOW_FAIL_PER))
            break;
    }
    Xmax = i;
    FailCount = 0;

    for (i = oldy; i > Condition->IMG_Y; i--)
    {
        if (!Point_ColorMatch(oldx, i))
            FailCount++;
        if (FailCount > (((Condition->HIGHT_MIN + Condition->HIGHT_MAX) >> 2) >> ALLOW_FAIL_PER))
            break;
    }
    Ymin = i;
    FailCount = 0;

    for (i = oldy; i < Condition->IMG_Y + Condition->IMG_H; i++)
    {
        if (!Point_ColorMatch(oldx, i))
            FailCount++;
        if (FailCount > (((Condition->HIGHT_MIN + Condition->HIGHT_MAX) >> 2) >> ALLOW_FAIL_PER))
            break;
    }
    Ymax = i;
    FailCount = 0;

    Resu->x = (Xmin + Xmax) / 2;
    Resu->y = (Ymin + Ymax) / 2;
    Resu->w = Xmax - Xmin;
    Resu->h = Ymax - Ymin;

    //如果 左右.上下尺寸 符合查找标准 返回 (1)
    if (((Xmax - Xmin) > (Condition->WIDTH_MIN)) && ((Ymax - Ymin) > (Condition->HIGHT_MIN)) && \
            ((Xmax - Xmin) < (Condition->WIDTH_MAX)) && ((Ymax - Ymin) < (Condition->HIGHT_MAX)) )
        return 1;
    else
        return 0;
}
//搜索腐蚀中心
static int Point_SearchCentre(unsigned int *x, unsigned int *y, const TARGET_CONDI *Condition, const SEARCH_AREA *Area)
{
		RESULT Result;
    unsigned int SpaceX, SpaceY, i, j, k, FailCount = 0;

    SpaceX = Condition->WIDTH_MIN / 2;//x步长
    SpaceY = Condition->HIGHT_MIN / 2;//y步长

    for (i = Area->Y_Start; i < Area->Y_End; i += SpaceY)//从 Y 起始位置到 终点位子 以 y 步长差
    {
        for (j = Area->X_Start; j < Area->X_End; j += SpaceX)//从 x 起始位置到 终点位子 以 x 步长差
        {
            FailCount = 0;
            for (k = 0; k < SpaceX + SpaceY; k++)
            {
                /*
                |****************
                |       |       |
                |       |       |
                |       |       |
                *****************
                |       |       |
                |       |       |
                |       |       |
                *****************/
                //以十字搜索
                if (k < SpaceX)
                {
                    if (!Point_ColorMatch(j + k, i + SpaceY / 2)) //如果匹配
                        FailCount++;//匹配次数
                }
                else
                {
                    if (!Point_ColorMatch(j + SpaceX / 2, i + (k - SpaceX))) //如果不匹配
                        FailCount++;//不匹配次数
                }
                if (FailCount > ((SpaceX + SpaceY) >> ALLOW_FAIL_PER))//容错率
                    break;
            }
            if (k == SpaceX + SpaceY)
            {
						if(!Point_Corrode(j + SpaceX / 2, i + SpaceY / 2, Condition, &Result))
								continue;
                *x = j + SpaceX / 2;
                *y = i + SpaceY / 2;
                return 1;
            }
        }
    }
    return 0;
}

static SEARCH_AREA Area;// = {Condition->IMG_X, Condition->IMG_X + Condition->IMG_W, Condition->IMG_Y, Condition->IMG_Y + Condition->IMG_H};

void Point_Trace_Init(const TARGET_CONDI *Condition)
{
            Area.X_Start = Condition->IMG_X ;
            Area.X_End  = Condition->IMG_X + Condition->IMG_W;
            Area.Y_Start = Condition->IMG_Y ;
            Area.Y_End  = Condition->IMG_Y + Condition->IMG_H;
}
int Point_Trace(const TARGET_CONDI *Condition, RESULT *Resu)
{
    unsigned int i;
    static unsigned int x0, y0, flag = 0;
    //查找范围
    RESULT Result;
    if (flag == 0)//首次进入
    {
        //从上次迭代地点查起
        if (Point_SearchCentre(&x0, &y0, Condition, &Area))//寻找腐蚀中心
            flag = 9;//已找到
        else//没找着
        {
            //改变查找范围 从头找起
            Area.X_Start = Condition->IMG_X ;
            Area.X_End  = Condition->IMG_X + Condition->IMG_W;
            Area.Y_Start = Condition->IMG_Y ;
            Area.Y_End  = Condition->IMG_Y + Condition->IMG_H;

            if (Point_SearchCentre(&x0, &y0, Condition, &Area))//再次寻找
            {
                flag = 0;
                return 0;//退出
            }
        }
    }
//    else if (flag == 1)
//    {				
//        Area.X_Start =IMG_X;// Area.X_Start;// + Condition->WIDTH_MIN / 2;
//        Area.X_End  = IMG_X + IMG_W;
//        Area.Y_Start =IMG_Y;// Area.Y_Start + Condition->HIGHT_MIN / 2;
//        Area.Y_End  = IMG_Y + IMG_H;
//        if (Point_SearchCentre(&x0, &y0, Condition, &Area))//寻找腐蚀中心
//            flag=9;//已找到
//        else
//            flag = 0;
//    }
		else if (flag == 1)
    {
        if (Area.Y_Start > Area.Y_End)
        {
            flag = 0; return 0;
        }
			  //Area.X_Start = Area.X_Start+Condition->WIDTH_MIN / 2;;//Area.X_Start+((Result.w) >> 1);
        Area.Y_Start = Area.Y_Start+Condition->HIGHT_MIN / 2;
        Area.X_End  = Condition->IMG_X + Condition->IMG_W;
        Area.Y_End  = Condition->IMG_Y + Condition->IMG_H;
        if (Point_SearchCentre(&x0, &y0, Condition, &Area))//寻找腐蚀中心
            flag=9;//已找到
        else
				{
            flag = 0;
						return 0;
						}
    }
    Result.x = x0;
    Result.y = y0;

    //多次迭代
    for (i = 0; i < ITERATE_NUM; i++)
        Point_Corrode(Result.x, Result.y, Condition, &Result);//从腐蚀中心向外腐蚀，得到新的腐蚀中心

    //从腐蚀中心向外腐蚀，得到新的腐蚀中心
    if (Point_Corrode(Result.x, Result.y, Condition, &Result))
    {
        x0 = Result.x;
        y0 = Result.y;
        Resu->x = Result.x;
        Resu->y = Result.y;
        Resu->w = Result.w;
        Resu->h = Result.h;
        flag = 9;

        Area.X_Start = Result.x - ((Result.w));
        Area.X_End  = Result.x + ((Result.w));
        Area.Y_Start = Result.y - ((Result.h));
        Area.Y_End  = Result.y + ((Result.h));
        return 1;
    }
    else
    {
        flag=1;
        return 0;
    }
}
//输入RBG 返回灰度值
int Gray_Threshold_H;
int Gray_Threshold_L;
u16 RGB_To_Gray(u16 C16)
{
    u16 Gray;
    COLOR_RGB Rgb;
    Rgb.red   =     (unsigned char)((C16 & 0xf800) >> 8);
    Rgb.green =     (unsigned char)((C16 & 0x07e0) >> 3);
    Rgb.blue  =     (unsigned char)((C16 & 0x001f) << 3);
    Gray = (Rgb.red * 2449 + Rgb.green * 4809 + Rgb.blue * 934) >> 13;
    return Gray;
}
