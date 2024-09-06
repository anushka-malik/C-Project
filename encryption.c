#include <string.h>
#define ENDEC_KEY 3;
void custom_encrypt(char *str)
{
   char ch;
   for(int i=0;str[i]!='\0';i++)
   {
      ch=str[i];
      if(ch>='a'  && ch<='z')
      {
          ch=ch+ENDEC_KEY;
          if(ch>'z')
          {
            ch=ch-'z'+'a'-1;
          }
          str[i]=ch;
      }
      else if(ch>='A' && ch<='Z')
      {
          ch=ch+ENDEC_KEY;
          if(ch>'Z')
          {
            ch=ch-'Z'+'A'-1;
          }
          str[i]=ch;
      }
      else
      {
        ch=ch+ENDEC_KEY;
        if(ch>'9')
        {
            ch=ch-'9'+'0'-1;
        }
        str[i]=ch;
      }
   }
}


