#define ENDEC_KEY 3;
void custom_decrypt(char *str)
{
   char ch;
   for(int i=0;str[i]!='\0';i++)
   {
      ch=str[i];
      if(ch>='a'  && ch<='z')
      {
          ch=ch-ENDEC_KEY;
          if(ch<'a')
          {
            ch=ch-'a'+'z'+1;
          }
          str[i]=ch;
      }
      else if(ch>='A' && ch<='Z')
      {
          ch=ch-ENDEC_KEY;
          if(ch<'A')
          {
            ch=ch-'A'+'Z'+1;
          }
          str[i]=ch;
      }
      else
      {
        ch=ch-ENDEC_KEY;
        if(ch<'0')
        {
            ch=ch-'0'+'9'+1;
        }
        str[i]=ch;
      }
   }
}

