#ifndef DEBUG_H
#define DEBUG_H
/*
void _OutputDebugString(LPCTSTR lpOutputString, ... )
{
   
    va_list argptr;   
   
    va_start( argptr, lpOutputString );           
   
	const int MAX_LENGTH = 80;
    TCHAR OutMsg[MAX_LENGTH];
    TCHAR format[MAX_LENGTH];
   
    for(int i=0,j=0;lpOutputString[i] != '\0';i++)
    {
        format[j++] = lpOutputString[i];
        // If escape character
        if(lpOutputString[i] == '\\')
        {
            i++;
            continue;
        }
        // if not a substitutal character
        if(lpOutputString[i] != '%')
            continue;
       
        format[j++] = lpOutputString[++i];
        format[j] = '\0';
        switch(lpOutputString[i])
        {
            // string
        case 's':
            {
                char* s = va_arg( argptr, char * );
                sprintf(OutMsg,format,s);
                strcpy(format,OutMsg);
                j = strlen(format);
                strcat(format," ");
                break;
            }
            // character
        case 'c':
            {
                char c = (char) va_arg( argptr, int );
                sprintf(OutMsg,format,c);
                strcpy(format,OutMsg);
                j = strlen(format);
                strcat(format," ");
                break;
            }
            // integer
        case 'd':
            {
                int d = va_arg( argptr, int );
                sprintf(OutMsg,format,d);
                strcpy(format,OutMsg);
                j = strlen(format);
                strcat(format," ");
                break;
            }
        }
    }           
    OutputDebugString(OutMsg);
    va_end( argptr );
}

*/
#endif