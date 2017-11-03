// 
// @filename:    lpublic.h
// @Author:      luhengsi
// @DateTime:    2017-11-03 20:27:23
// @Description: common define
// @Description: 


#define LU_FUNCTION __FUNCTION__

#define LU_USE_ARGUMENT(arg) (arg)

#define LU_PROCESS_ERROR(Condition) \
    do  \
    {   \
        if (!(Condition))   \
            goto Exit0;     \
    } while (false)


#define LU_PROCESS_SUCCESS(Condition) \
    do  \
    {   \
        if (Condition)      \
            goto Exit1;     \
    } while (false)


